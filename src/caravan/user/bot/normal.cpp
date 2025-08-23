// Copyright (c) 2022-2025 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <random>
#include "caravan/core/exceptions.h"
#include "caravan/user/bot/normal.h"
#include "caravan/user/constants.h"
#include "caravan/user/functions.h"

/*
 * PRIVATE
 */


uint8_t pos_card_numeral(Player *player) {
    uint8_t size_hand = player->get_size_hand();

    for (int pos = 1; pos <= size_hand; ++pos) {
        if (player->get_from_hand_at(pos).is_numeral_card()) {
            return pos;
        }
    }

    return 0;
}

uint8_t numeral_rank_value(Card card) {
    switch (card.rank) {
        case ACE:
            return 1;
        case TWO:
            return 2;
        case THREE:
            return 3;
        case FOUR:
            return 4;
        case FIVE:
            return 5;
        case SIX:
            return 6;
        case SEVEN:
            return 7;
        case EIGHT:
            return 8;
        case NINE:
            return 9;
        case TEN:
            return 10;
        default:
            throw CaravanFatalException("Card is not a numeral.");
    }
}

/*
 * PROTECTED
 */

std::string UserBotNormal::generate_move(
    Game *game,
    bool allow_numeral,
    bool allow_face,
    bool allow_clear) {

    Player *me = game->get_player(name);
    uint8_t my_hand_size = me->get_size_hand();

    if (my_hand_size == 0) {
        throw CaravanFatalException("Bot has an empty hand.");
    }

    PlayerCaravanNames my_cvns = game->get_player_caravan_names(name);
    PlayerCaravanNames opp_cvns = game->get_player_caravan_names(
        name == PLAYER_ABC ? PLAYER_DEF : PLAYER_ABC);

    uint16_t my_move_count = me->get_moves_count();

    if (my_move_count < MOVES_START_ROUND) {
        // Add numeral cards for start round
        const uint8_t pos_hand = pos_card_numeral(me);

        if (pos_hand == 0) {
            throw CaravanFatalException(
                "Bot does not have enough numeral cards "
                "to finish the start phase.");
        }

        return LETTER_PLAY +
               std::to_string(pos_hand) +
               caravan_letter(my_cvns[my_move_count]);
    }

    // After start round
    Table *table = game->get_table();

    // Clear any caravans that are bust or full of cards
    if(allow_clear) {
        for (uint8_t i_cvn = 0; i_cvn < PLAYER_CARAVANS_MAX; i_cvn++) {
            Caravan *cvn = table->get_caravan(my_cvns[i_cvn]);

            if (cvn->get_bid() > CARAVAN_SOLD_MAX ||
                cvn->get_size() == TRACK_NUMERIC_MAX) {
                return LETTER_CLEAR + caravan_letter(my_cvns[i_cvn]);
            }
        }
    }

    // Otherwise, cycle through cards in hand
    for (uint8_t pos_hand = 1; pos_hand <= my_hand_size; pos_hand++) {
        Card c_hand = me->get_from_hand_at(pos_hand);

        if (c_hand.is_numeral_card() && allow_numeral) {
            // If numeral, look through caravans
            for (uint8_t i = 0; i < PLAYER_CARAVANS_MAX; ++i) {
                Caravan *my_cvn = table->get_caravan(my_cvns[i]);
                Caravan *opp_cvn = table->get_caravan(opp_cvns[i]);

                std::string move_draft =
                    LETTER_PLAY +
                    std::to_string(pos_hand) +
                    caravan_letter(my_cvn->get_name());

                uint16_t my_cvn_bid = my_cvn->get_bid();
                uint16_t opp_cvn_bid = opp_cvn->get_bid();

                // Skip caravan if sold and winning
                if (my_cvn_bid >= CARAVAN_SOLD_MIN &&
                    my_cvn_bid <= CARAVAN_SOLD_MAX &&
                    (my_cvn_bid > opp_cvn_bid ||
                     opp_cvn_bid > CARAVAN_SOLD_MAX)) {
                    continue;
                }

                uint8_t my_cvn_size = my_cvn->get_size();

                // Skip caravan if full
                if (my_cvn_size == TRACK_NUMERIC_MAX) {
                    continue;
                }

                // Play numeral if caravan is empty
                if (my_cvn_size == 0) {
                    return move_draft;
                }

                // Not empty, so check the top slot in the caravan
                Slot slot_top = my_cvn->get_slot(my_cvn_size);

                // Ignore numeral if cards have same rank
                if (slot_top.card.rank == c_hand.rank) {
                    continue;
                }

                bool not_bust =
                    (my_cvn_bid +
                     numeral_rank_value(c_hand)) <= CARAVAN_SOLD_MAX;

                // Same suit as caravan and numeral would not cause bust
                if (my_cvn->get_suit() == c_hand.suit && not_bust) {
                    return move_draft;
                }

                // Not same suit, check direction
                Direction my_cvn_dir = my_cvn->get_direction();

                // Card ascending with caravan and would not bust
                if (my_cvn_dir == ASCENDING &&
                    c_hand.rank > slot_top.card.rank &&
                    not_bust) {
                    return move_draft;
                }

                // Card descending with caravan and would not bust
                if (my_cvn_dir == DESCENDING &&
                    c_hand.rank < slot_top.card.rank &&
                    not_bust) {
                    return move_draft;
                }
            }

        } else if(allow_face) {  // is face card
            uint8_t i_opp_cvn_most_cards = PLAYER_CARAVANS_MAX;
            uint8_t n_opp_cvn_most_cards = 0;

            // Put face card on opp caravan with the most cards
            for (uint8_t i = 0; i < PLAYER_CARAVANS_MAX; ++i) {
                uint8_t size_cvn =
                    table->get_caravan(opp_cvns[i])->get_size();

                if (size_cvn > n_opp_cvn_most_cards) {
                    i_opp_cvn_most_cards = i;
                    n_opp_cvn_most_cards = size_cvn;
                }
            }

            // If there exists an opp caravan with at least 1 card on it
            if (i_opp_cvn_most_cards < PLAYER_CARAVANS_MAX &&
                n_opp_cvn_most_cards > 0) {

                Caravan *opp_cvn = table->get_caravan(
                    opp_cvns[i_opp_cvn_most_cards]);

                uint8_t opp_cvn_size = opp_cvn->get_size();
                Slot opp_slot_top = opp_cvn->get_slot(opp_cvn_size);

                std::string move_draft =
                    LETTER_PLAY +
                    std::to_string(pos_hand) +
                    caravan_letter(opp_cvn->get_name()) +
                    std::to_string(opp_cvn->get_size());

                if (opp_slot_top.i_faces < TRACK_FACE_MAX) {
                    return move_draft;
                }
            }
        }
    }

    return "";
}

/*
 * PUBLIC
 */

std::string UserBotNormal::request_move(Game *game) {
    std::string move = generate_move(game, true, true, true);

    // Return move if able to generate one
    if(!move.empty()) { return move; }

    // If no useful move could be made, discard first card in hand
    return "D1";
}
