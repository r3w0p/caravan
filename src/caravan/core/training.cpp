// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <map>
#include <random>
#include <algorithm>
#include <set>
#include "caravan/model/game.h"
#include "caravan/core/training.h"


uint8_t card_to_uint8_t(Card c) {
    if (c.rank == JOKER) { return 1; }

    if (c.suit == NO_SUIT) {
        throw CaravanFatalException(
            "Cannot convert card to uint8_t: "
            "card has suit NO_SUIT with a rank that is not JOKER");
    }

    // 13 ranks (excluding JOKER)
    // suit - 1 because NO_SUIT is value 0
    // rank + 1 because ACE is value 0
    // + 1 at the end because JOKER is value 1 (see above)
    return
        static_cast<uint8_t>(c.suit - 1) * 13 +
        static_cast<uint8_t>(c.rank + 1) + 1;
}

uint8_t rank_to_uint8_t(Rank r) {
    // + 1 so ACE is 1
    return static_cast<uint8_t>(r) + 1;
}

uint8_t suit_to_uint8_t(Suit s) {
    // NO_SUIT is 0
    return static_cast<uint8_t>(s);
}

uint8_t direction_to_uint8_t(Direction d) {
    // ANY is 0
    return static_cast<uint8_t>(d);
}

void add_hand_to_game_state(GameState *gs, uint16_t *i_gs, Player *player) {
    Hand hand = player->get_hand();
    uint8_t hand_size = player->get_size_hand();
    std::set<uint8_t> hand_set;

    bool is_numeral = false;
    bool is_jack = false;
    bool is_queen = false;
    bool is_king = false;
    bool is_joker = false;

    // Check for rank types
    for (uint8_t i_hand = 0; i_hand < hand_size; i_hand++) {
        Card card = hand[i_hand];

        if (is_numeral_card(card)) {
            is_numeral = true;

        } else if (card.rank == JACK) {
            is_jack = true;

        } else if (card.rank == QUEEN) {
            is_queen = true;

        } else if (card.rank == KING) {
            is_king = true;

        } else if (card.rank == JOKER) {
            is_joker = true;
        }
    }

    (*gs)[(*i_gs)++] = is_numeral;
    (*gs)[(*i_gs)++] = is_jack;
    (*gs)[(*i_gs)++] = is_queen;
    (*gs)[(*i_gs)++] = is_king;
    (*gs)[(*i_gs)++] = is_joker;
}

void add_caravan_to_game_state(GameState *gs, uint16_t *i_gs, Game *game, Caravan *caravan) {
    uint8_t caravan_size = caravan->get_size();
    uint8_t caravan_bid = caravan->get_bid();

    // Add bid state
    if (caravan_bid < CARAVAN_SOLD_MIN) {  // light
        (*gs)[(*i_gs)++] = 0;

    } else if (caravan_bid > CARAVAN_SOLD_MAX) {  // bust
        (*gs)[(*i_gs)++] = 2;

    } else {  // sold (but not necessarily winning...)
        (*gs)[(*i_gs)++] = 1;
    }

    // Add whether caravan is winning
    (*gs)[(*i_gs)++] = game->is_caravan_winning(caravan->get_name());

    // Add whether numeral track is full
    (*gs)[(*i_gs)++] = caravan_size == TRACK_NUMERIC_MAX;

    // Add caravan direction
    (*gs)[(*i_gs)++] = direction_to_uint8_t(caravan->get_direction());

    // Add caravan suit
    (*gs)[(*i_gs)++] = suit_to_uint8_t(caravan->get_suit());
}

void get_game_state(GameState *gs, Game *game, PlayerName pname) {
    uint16_t i_gs = 0;
    uint8_t cvn_names_size = 6;
    CaravanName cvn_names[cvn_names_size];

    Player *player = game->get_player(pname);
    Table *table = game->get_table();

    // Add player's hand to state
    add_hand_to_game_state(gs, &i_gs, game->get_player(pname));

    // Put player's caravans first in the game state
    if (player->get_name() == PLAYER_ABC) {
        cvn_names[0] = CARAVAN_A;
        cvn_names[1] = CARAVAN_B;
        cvn_names[2] = CARAVAN_C;
        cvn_names[3] = CARAVAN_D;
        cvn_names[4] = CARAVAN_E;
        cvn_names[5] = CARAVAN_F;
    } else { // PLAYER_DEF
        cvn_names[0] = CARAVAN_D;
        cvn_names[1] = CARAVAN_E;
        cvn_names[2] = CARAVAN_F;
        cvn_names[3] = CARAVAN_A;
        cvn_names[4] = CARAVAN_B;
        cvn_names[5] = CARAVAN_C;
    }

    // Add state of each caravan, player's caravans first
    for (uint8_t i_cvn = 0; i_cvn < cvn_names_size; i_cvn++) {
        Caravan *caravan = table->get_caravan(cvn_names[i_cvn]);
        add_caravan_to_game_state(gs, &i_gs, game, caravan);
    }
}

void populate_action_space(ActionSpace *as) {
    uint16_t i_as = 0;

    (*as)[i_as++] = ACTION_DISCARD_NUMERAL;
    (*as)[i_as++] = ACTION_DISCARD_JACK;
    (*as)[i_as++] = ACTION_DISCARD_QUEEN;
    (*as)[i_as++] = ACTION_DISCARD_KING;
    (*as)[i_as++] = ACTION_DISCARD_JOKER;

    (*as)[i_as++] = ACTION_CLEAR_BUST;

    (*as)[i_as++] = ACTION_PLAY_NUMERAL_1;
    (*as)[i_as++] = ACTION_PLAY_NUMERAL_2;
    (*as)[i_as++] = ACTION_PLAY_NUMERAL_3;

    (*as)[i_as++] = ACTION_PLAY_JACK_SELF;
    (*as)[i_as++] = ACTION_PLAY_JACK_OPP;

    (*as)[i_as++] = ACTION_PLAY_QUEEN_SELF;
    (*as)[i_as++] = ACTION_PLAY_QUEEN_OPP;

    (*as)[i_as++] = ACTION_PLAY_KING_SELF;
    (*as)[i_as++] = ACTION_PLAY_KING_OPP;

    (*as)[i_as++] = ACTION_PLAY_JOKER;
}

bool generate_discard_numeral(std::string *input, Player *player) {
    Hand hand = player->get_hand();
    uint8_t hand_size = player->get_size_hand();
    std::string ret = "D";

    // Discard first numeral in hand
    for (uint8_t i = 0; i < hand_size; i++) {
        if (is_numeral_card(hand[i])) {
            ret += std::to_string(i+1);
            break;
        }
    }

    // Could not find a numeral
    if (ret.length() == 1) {
        return false;
    }

    *input = ret;
    return true;
}

bool generate_discard_rank(std::string *input, Player *player, Rank rank) {
    Hand hand = player->get_hand();
    uint8_t hand_size = player->get_size_hand();
    std::string ret = "D";

    // Discard first card with specified rank in hand
    for (uint8_t i = 0; i < hand_size; i++) {
        if (hand[i].rank == rank) {
            ret += std::to_string(i + 1);
            break;
        }
    }

    // Could not find card with specified rank
    if (ret.length() == 1) {
        return false;
    }

    *input = ret;
    return true;
}

bool generate_clear_bust(std::string *input, Game *game, Player *player) {
    // Clear busted caravan with the highest bid, if any

    PlayerCaravanNames cvn_names = game->get_player_caravan_names(
        player->get_name());

    uint8_t to_clear = 0;
    uint16_t bid_clear = 0;

    for (uint8_t i_cvn = 0; i_cvn < cvn_names.size(); i_cvn++) {
        Caravan *cvn = game->get_table()->get_caravan(cvn_names[i_cvn]);
        uint16_t bid = cvn->get_bid();

        // Clear caravan that has bust the most, if any
        if (bid > CARAVAN_SOLD_MAX and bid > bid_clear) {
            to_clear = i_cvn + 1;
            bid_clear = bid;
        }
    }

    // No bust caravans to clear
    if (to_clear == 0) return false;

    *input = "C" + caravan_letter(cvn_names[to_clear - 1]);
    return true;
}

bool generate_play_numeral(std::string *input, Game *game, Player *player, uint8_t cvn_num) {
    PlayerCaravanNames cvn_names = game->get_player_caravan_names(
        player->get_name());

    Caravan *cvn = game->get_table()->get_caravan(cvn_names[cvn_num-1]);

    uint16_t cvn_size = cvn->get_size();

    // Caravan is full
    if (cvn_size == TRACK_NUMERIC_MAX) return false;

    uint16_t cvn_bid = cvn->get_bid();

    // Caravan has the best possible bid or is bust
    if (cvn_bid >= CARAVAN_SOLD_MAX) return false;

    Hand hand = player->get_hand();
    uint8_t hand_size = player->get_size_hand();

    uint8_t pos_best = 0;
    uint16_t value_best = 0;

    // Discard first numeral in hand
    for (uint8_t i = 0; i < hand_size; i++) {
        Card hcard = hand[i];
        Direction cvn_dir = cvn->get_direction();
        Suit cvn_suit = cvn->get_suit();

        if (is_numeral_card(hcard)) {
            uint8_t hcard_value = numeral_rank_value(hcard);

            if (cvn_size > 0) {
                Card top = cvn->get_slot(cvn_size).card;

                // Skip if ranks match
                if (hcard.rank == top.rank)
                    continue;

                // Skip if direction does not match and card has different suit to caravan
                if (hcard.suit != cvn_suit and cvn_dir == ASCENDING and hcard.rank < top.rank)
                    continue;

                if (hcard.suit != cvn_suit and cvn_dir == DESCENDING && hcard.rank > top.rank)
                    continue;

                // Skip if adding card would bust the caravan
                if (cvn_bid + hcard_value > CARAVAN_SOLD_MAX)
                    continue;

                // Use numeral if it is closest to top card's rank
                int diff = std::abs(numeral_rank_value(top) - hcard_value);
                if (pos_best == 0 or diff < value_best) {
                    pos_best = i+1;
                    value_best = diff;
                }
            } else {
                // Use card with the largest distance from 5 (middle rank)
                int diff = std::abs(5 - hcard_value);
                if (pos_best == 0 or diff > value_best) {
                    pos_best = i+1;
                    value_best = diff;
                }
            }
        }
    }

    // No (worthwhile) numeral card to play
    if (pos_best == 0) return false;

    *input = "P" + std::to_string(pos_best) + caravan_letter(cvn_names[cvn_num-1]);
    return true;
}

bool generate_input(std::string *input, Action action, Game *game) {
    Player *player = game->get_player_turn();

    if (action >= ACTION_DISCARD_NUMERAL and action <= ACTION_DISCARD_JOKER) {
        switch (action) {
            case ACTION_DISCARD_NUMERAL:
                return generate_discard_numeral(input, player);
            case ACTION_DISCARD_JACK:
                return generate_discard_rank(input, player, JACK);
            case ACTION_DISCARD_QUEEN:
                return generate_discard_rank(input, player, QUEEN);
            case ACTION_DISCARD_KING:
                return generate_discard_rank(input, player, KING);
            case ACTION_DISCARD_JOKER:
                return generate_discard_rank(input, player, JOKER);
            default:
                return false;
        }
    }

    if (action == ACTION_CLEAR_BUST) {
        return generate_clear_bust(input, game, player);

    } else if (action >= ACTION_PLAY_NUMERAL_1 and action <= ACTION_PLAY_NUMERAL_3) {
        switch (action) {
            case ACTION_PLAY_NUMERAL_1:
                return generate_play_numeral(input, game, player, 1);
            case ACTION_PLAY_NUMERAL_2:
                return generate_play_numeral(input, game, player, 2);
            case ACTION_PLAY_NUMERAL_3:
                return generate_play_numeral(input, game, player, 3);
            default:
                return false;
        }
    }  // TODO face cards

    return false;
}


bool train_on_game(Game *game, QTable &q_table, ActionSpace &action_space,
                   GameConfig &gc, TrainConfig &tc, std::mt19937 &gen) {
    GameState gs;
    GameCommand command;
    uint32_t num_moves = 0;

    Action action;
    int16_t action_index;
    float action_value;
    std::vector<Action> action_pool;
    std::string action_input;

    GameState last_gs;
    Action last_action;

    std::uniform_real_distribution<float> dist_explore(0, 1);
    bool winner = false;

    // Play until winner
    while (game->get_winner_name() == NO_PLAYER) {
        // Determine player
        PlayerName pturn = game->get_player_turn()->get_name();

        // Only first player is learning
        // Opp always makes random moves and does not influence learning
        bool learning = pturn == gc.player_first;

        // Get game state in relation to current player
        get_game_state(&gs, game, pturn);

        // Maybe add game state and actions if new state discovered
        /*
        if (!q_table.contains(gs)) {
            for (uint16_t i = 0; i < SIZE_ACTION_SPACE; i++) {
                q_table[gs][action_space[i]] = 0;
            }
        }
        */

        // Use action pool that depletes as actions are found to be invalid
        for (int i = 0; i < SIZE_ACTION_SPACE; i++) {
            action_pool.push_back(action_space[i]);
        }

        // Determine whether to explore for next move
        bool explore = dist_explore(gen) < tc.explore;

        // Find a valid action
        while (true) {
            if (!learning or explore or (!q_table.contains(gs))) {
                // If exploring, fetch a random action from the action pool
                std::uniform_int_distribution<uint16_t> dist_pool(
                    0, action_pool.size() - 1);
                action_index = dist_pool(gen);
                action = action_pool[action_index];
                action_value = q_table[gs][action];

            } else {
                action_index = -1;
                action_value = 0;

                // Try all known actions first to see if any are above 0
                for (auto it_q = q_table[gs].begin(); it_q != q_table[gs].end();
                     it_q++) {
                    Action a = it_q->first;

                    // Found an action explored in the past with a better-than-default value
                    if (q_table[gs][a] > action_value) {
                        // Find its index in action pool
                        auto it_ap = std::find(action_pool.begin(),
                                               action_pool.end(), a);

                        // Ignore if already removed from pool
                        if (it_ap == action_pool.end()) continue;

                        action_index = std::distance(action_pool.begin(), it_ap);
                        action_value = q_table[gs][action_pool[action_index]];
                    }
                }

                // If not, try the rest via explore
                if (action_index == -1) {
                    explore = true;
                    continue;
                }

                // Otherwise, pick the optimal action from the q-table
                action = action_pool[action_index];
            }

            // Generate input from action
            if (generate_input(&action_input, action, game)) {
                //printf("%s (%llu)\n", action_input.c_str(), action_pool.size());

                // Generate command for action
                command = generate_command(action_input, true);

                // TODO error if all actions are invalid? should not be possible!

                // Use action if valid
                if (game->check_option(&command)) break;
            }

            // Remove action from pool and try again
            action_pool.erase(action_pool.begin() + action_index);
        }

        // Clear pool for next time around
        action_pool.clear();


        //if (action_value > 0)
            printf("[%s] %s (i=%hu, v=%.2f)\n",
                   pturn == PLAYER_ABC ? "ABC" : "DEF",
                   action_input.c_str(),
                   action_index,
                   action_value);


        // Perform action
        // (Exceptions intentionally not handled)
        game->play_option(&command);

        // Update q-table
        if (learning and num_moves >= 2) {
            PlayerName winner_name = game->get_winner_name();

            if (winner_name != NO_PLAYER) {
                if (winner_name == pturn) {
                    q_table[gs][action] = 1;
                    winner = true;
                } else {
                    q_table[gs][action] = -1;
                }
            }

            q_table[last_gs][last_action] =
                q_table[last_gs][last_action] + tc.learning * (
                    tc.discount * q_table[gs][action] -
                    q_table[last_gs][last_action]);
        }

        if (learning) {
            last_gs = gs;
            last_action = action;
        }

        num_moves += 1;
    }

    return winner;
}
