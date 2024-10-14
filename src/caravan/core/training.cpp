// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/core/exceptions.h"
#include "caravan/core/common.h"
#include "caravan/core/training.h"
#include "caravan/model/game.h"
#include <string>

uint8_t card_to_uint8_t(Card c) {
    if (c.rank == JOKER) { return 1; }

    if (c.suit == NO_SUIT) {
        throw CaravanFatalException(
            "Cannot convert card to uint8_t: "
            "card has suit NO_SUIT with a rank that is not JOKER");
    }

    // 13 ranks excluding JOKER
    // suit - 1 because CLUBS is enum value 1
    // rank + 1 because ACE is enum value 0
    // + 1 at the end because JOKER returns value 1
    return ((uint8_t) (c.suit - 1) * 13) + ((uint8_t) c.rank + 1) + 1;
}

void get_game_state(GameState *gs, Game *game, PlayerName pname) {
    uint16_t i = 0;

    Player *player = game->get_player(pname);
    Table *table = game->get_table();

    Hand hand = player->get_hand();
    uint8_t size_hand = player->get_size_hand();

    // Get player indicator
    if (pname == PLAYER_ABC) {
        (*gs)[i++] = 1;
    } else if (pname == PLAYER_DEF) {
        (*gs)[i++] = 2;
    } else {
        throw CaravanFatalException(
            "Invalid player name when getting game state: "
            "must be either ABC or DEF.");
    }

    // Get state from player's hand
    // {AC, 2D, 8S, 8S, JO, 0, 0, 0} (all ints, with 0 for empty slot)
    for (uint8_t i_hand = 0; i_hand < HAND_SIZE_MAX_START; i_hand++) {
        if (i_hand < size_hand) {
            (*gs)[i++] = card_to_uint8_t(hand[i_hand]);
        } else {
            (*gs)[i++] = 0;
        }
    }

    // Get state from each caravan
    // {AH, KC, 0, 0} x 8 (all ints, with 0 for any empty slot)
    for (uint8_t i_cname = CARAVAN_A; i_cname <= CARAVAN_F; i_cname++) {
        Caravan *caravan = table->get_caravan(
            static_cast<CaravanName>(i_cname));

        // Current highest numeral position along caravan track
        uint8_t max_track = caravan->get_size();

        for (uint8_t i_track = 0; i_track < TRACK_NUMERIC_MAX; i_track++) {
            // If numeral at track position, fetch slot state
            if (i_track < max_track) {
                Slot slot = caravan->get_slot(i_track + 1);

                // Add numeral
                (*gs)[i++] = card_to_uint8_t(slot.card);

                // Add face cards, if any
                for (uint8_t i_face = 0; i_face < TRACK_FACE_MAX; i_face++) {
                    if (i_face < slot.i_faces) {
                        (*gs)[i++] = card_to_uint8_t(slot.faces[i_face]);
                    } else {
                        (*gs)[i++] = 0;
                    }
                }

            } else {
                // No populated slot at caravan position, leave 4 blank spaces
                // 1 for numeral, 3 for face cards
                for (uint8_t _ = 0; _ < 4; _++) {
                    (*gs)[i++] = 0;
                }
            }
        }
    }
}

void populate_action_space(ActionSpace *as) {
    uint16_t i = 0;

    // Discard actions (5)
    for(uint8_t d = 1; d <= HAND_SIZE_MAX_POST_START; d++) {
        (*as)[i++] = "D" + std::to_string(d);
    }

    // Clear actions (6)
    for (uint8_t c = CARAVAN_A; c <= CARAVAN_F; c++) {
        (*as)[i++] = "C" + caravan_letter(
            static_cast<CaravanName>(c));
    }

    // Play numeral actions (8 x 6 = 48)
    for (uint8_t p = 1; p <= HAND_SIZE_MAX_START; p++) {
        for (uint8_t c = CARAVAN_A; c <= CARAVAN_F; c++) {
            (*as)[i++] =
                "P" +
                std::to_string(p) +
                caravan_letter(static_cast<CaravanName>(c));
        }
    }

    // Play face actions (5 x 6 x 8 = 240)
    for (uint8_t p = 1; p <= HAND_SIZE_MAX_POST_START; p++) {
        for (uint8_t c = CARAVAN_A; c <= CARAVAN_F; c++) {
            for (uint8_t t = 1; t <= TRACK_NUMERIC_MAX; t++) {
                (*as)[i++] =
                    "P" +
                    std::to_string(p) +
                    caravan_letter(static_cast<CaravanName>(c)) +
                    std::to_string(t);
            }
        }
    }
}
