// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <string>
#include <array>
#include <map>
#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <algorithm>
#include "caravan/core/exceptions.h"
#include "caravan/core/common.h"
#include "caravan/model/game.h"
#include "caravan/core/training.h"


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

void train_on_game(Game *game, QTable &q_table, ActionSpace &action_space, TrainConfig &tc, std::mt19937 &gen) {
    // Get player names
    PlayerName pturn = game->get_player_turn();
    PlayerName popp = pturn == PLAYER_ABC ? PLAYER_DEF : PLAYER_ABC;

    // Read game state and maybe add to the q-table if state not present
    GameState gs;
    get_game_state(&gs, game, pturn);

    if (!q_table.contains(gs)) {
        // TODO maybe is this needed: q_table[gs] = {};
        for (uint16_t i = 0; i < SIZE_ACTION_SPACE; i++) {
            q_table[gs][action_space[i]] = 0;
        }
    }

    // Choose an action
    std::string action;
    GameCommand command;
    std::vector<std::string> invalid;
    bool explore = dist_explore(gen) < tc.explore;

    while (true) {
        if (explore) {
            // If exploring, fetch a random action from the action space
            action = action_space[dist_action(gen)];

        } else {
            // Otherwise, pick the optimal action from the q-table
            uint16_t pick_index = 0;
            float pick_value = q_table[gs][action_space[pick_index]];

            for (uint16_t i_action = 1; i_action < SIZE_ACTION_SPACE; i_action++) {
                // Ignore if already found to be invalid
                if (std::count(invalid.begin(), invalid.end(), action_space[i_action]) > 0) {
                    continue;
                }

                if (q_table[gs][action_space[i_action]] > pick_value) {
                    pick_index = i_action;
                    pick_value = q_table[gs][action_space[pick_index]];
                }
            }

            action = action_space[pick_index];
        }

        // If action in invalid list, ignore it and try another
        if (std::count(invalid.begin(), invalid.end(), action) > 0) {
            continue;
        }

        // Generate command for action
        command = generate_command(action, true);

        // Check action is valid
        if (!game->check_option(&command)) {
            // If invalid action for state, add to invalid list and try again
            invalid.push_back(action);
            continue;

        } else {
            // Pick action
            break;
        }
    }

    // Perform action
    // (Exceptions intentionally not handled)
    game->play_option(&command);

    // Measure reward (1 = win, -1 = loss, 0 = neither)
    uint16_t reward;

    if (game->get_winner() == pturn) {
        reward = 1;
    } else if (game->get_winner() == popp) {
        reward = -1;
    } else {
        reward = 0;
    }

    // TODO update q_table
    float q_value_former = q_table[gs][action];
    GameState gs_new;
    get_game_state(&gs_new, game, pturn);
    //  if a winner: +1 for winning player, -1 for losing player
}
