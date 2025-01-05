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

void add_hand_to_game_state(GameState *gs, uint16_t *i_gs, Player *player) {
    Hand hand = player->get_hand();
    uint8_t hand_size = player->get_size_hand();
    std::set<uint8_t> hand_set;

    // Create ordered, unique set of cards from player's hand
    for (uint8_t i_hand = 0; i_hand < hand_size; i_hand++) {
        hand_set.insert(card_to_uint8_t(hand[i_hand]));
    }

    uint8_t hand_set_size = hand_set.size();
    std::set<uint8_t>::iterator hand_set_iter = hand_set.begin();

    // Add ordered, unique cards to game state, including spaces
    for (uint8_t i_set = 0; i_set < HAND_SIZE_MAX_START; i_set++) {
        if (i_set < hand_set_size) {
            (*gs)[(*i_gs)++] = *hand_set_iter;
            ++hand_set_iter;
        } else {
            (*gs)[(*i_gs)++] = 0;
        }
    }
}

void add_caravan_to_game_state(GameState *gs, uint16_t *i_gs, Caravan *caravan) {
    // Current highest numeral position along caravan track
    uint8_t max_track = caravan->get_size();

    // Add caravan suit
    (*gs)[(*i_gs)++] = suit_to_uint8_t(caravan->get_suit());

    for (uint8_t i_track = 0; i_track < TRACK_NUMERIC_MAX; i_track++) {
        // If numeral at track position, fetch slot state
        if (i_track < max_track) {
            Slot slot = caravan->get_slot(i_track + 1);

            // Add numeral
            (*gs)[(*i_gs)++] = rank_to_uint8_t(slot.card.rank);

            // Add face cards
            for (uint8_t i_face = 0; i_face < TRACK_FACE_MAX; i_face++) {
                if (i_face < slot.i_faces) {
                    (*gs)[(*i_gs)++] = rank_to_uint8_t(slot.faces[i_face].rank);
                } else {
                    (*gs)[(*i_gs)++] = 0;
                }
            }
        } else {
            // No populated slot at caravan position, leave blank spaces
            // for numeral and max face cards
            for (uint8_t _ = 0; _ < (1 + TRACK_FACE_MAX); _++) {
                (*gs)[(*i_gs)++] = 0;
            }
        }
    }
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
        add_caravan_to_game_state(gs, &i_gs, caravan);
    }
}

void populate_action_space(ActionSpace *as) {
    uint16_t i_as = 0;

    // Add DISCARD actions
    // 4 x 13 + 1 = 53
    for (uint8_t s = CLUBS; s <= SPADES; s++) {
        for (uint8_t r = ACE; r <= KING; r++) {
            (*as)[i_as++] = {ACTION_NAME_DISCARD, s, r, 0, 0};
        }
    }
    (*as)[i_as++] = {ACTION_NAME_DISCARD, NO_SUIT, JOKER, 0, 0};

    // Add CLEAR actions
    // 3 only (actual caravan determined based on player)
    for (uint8_t n = 1; n <= 3; n++) {
        (*as)[i_as++] = {ACTION_NAME_CLEAR, 0, 0, n, 0};
    }

    // Add PLAY for numeral actions
    // 4 x 10 x 6 = 240
    for (uint8_t s = CLUBS; s <= SPADES; s++) {
        for (uint8_t r = ACE; r <= TEN; r++) {
            for (uint8_t n = 1; n <= 6; n++) {
                (*as)[i_as++] = {ACTION_NAME_PLAY, s, r, n, 0};
            }
        }
    }

    // Add PLAY for face actions
    // (4 x 3 x 6 x 8) + (6 x 8) = 624
    for (uint8_t s = CLUBS; s <= SPADES; s++) {
        for (uint8_t r = JACK; r <= KING; r++) {
            for (uint8_t n = 1; n <= 6; n++) {
                for (uint8_t t = TRACK_NUMERIC_MIN; t <= TRACK_NUMERIC_MAX; t++) {
                    (*as)[i_as++] = {ACTION_NAME_PLAY, s, r, n, t};
                }
            }
        }
    }
    for (uint8_t n = 1; n <= 6; n++) {
        for (uint8_t t = TRACK_NUMERIC_MIN; t <= TRACK_NUMERIC_MAX; t++) {
            (*as)[i_as++] = {ACTION_NAME_PLAY, NO_SUIT, JOKER, n, t};
        }
    }

    // Total: 53 + 3 + 240 + 624 = 720
}

bool generate_input(std::string *input, Action *action, Game *game) {
    Player *player = game->get_player_turn();
    PlayerName pname = player->get_name();
    Hand hand = player->get_hand();

    if ((*action)[KEY_ACTION_NAME] == ACTION_NAME_DISCARD) {
        Card card = {
            static_cast<Suit>((*action)[KEY_ACTION_SUIT]),
            static_cast<Rank>((*action)[KEY_ACTION_RANK])
        };

        for (uint8_t i = 0; i < player->get_size_hand(); i++) {
            if (hand[i].suit == card.suit and hand[i].rank == card.rank) {
                *input = "D" + std::to_string(i+1);
                return true;
            }
        }

    } else if ((*action)[KEY_ACTION_NAME] == ACTION_NAME_CLEAR) {
        switch ((*action)[KEY_ACTION_CVN_NAME]) {
            case 1:
                *input = pname == PLAYER_ABC ? "CA" : "CD";
                return true;
            case 2:
                *input = pname == PLAYER_ABC ? "CB" : "CE";
                return true;
            case 3:
                *input = pname == PLAYER_ABC ? "CC" : "CF";
                return true;
            default:
                return false;
        }

    } else if ((*action)[KEY_ACTION_NAME] == ACTION_NAME_PLAY) {
        std::string ret = "P";

        Card card = {
            static_cast<Suit>((*action)[KEY_ACTION_SUIT]),
            static_cast<Rank>((*action)[KEY_ACTION_RANK])
        };

        for (uint8_t i = 0; i < player->get_size_hand(); i++) {
            if (hand[i].suit == card.suit and hand[i].rank == card.rank) {
                ret += std::to_string(i+1);
                break;
            }
        }

        // Unable to find card in hand
        if (ret.length() == 1) {
            return false;
        }

        if ((*action)[KEY_ACTION_CVN_NAME] > 0) {
            switch ((*action)[KEY_ACTION_CVN_NAME]) {
                case 1:
                    ret += pname == PLAYER_ABC ? "A" : "D";
                    break;
                case 2:
                    ret += pname == PLAYER_ABC ? "B" : "E";
                    break;
                case 3:
                    ret += pname == PLAYER_ABC ? "C" : "F";
                    break;
                case 4:
                    ret += pname == PLAYER_ABC ? "D" : "A";
                    break;
                case 5:
                    ret += pname == PLAYER_ABC ? "E" : "B";
                    break;
                case 6:
                    ret += pname == PLAYER_ABC ? "F" : "C";
                    break;
                default:
                    return false;
            }

            if ((*action)[KEY_ACTION_CVN_POS] > 0) {
                ret += std::to_string((*action)[KEY_ACTION_CVN_POS]);
            }
        }

        *input = ret;
        return true;
    }

    return false;
}

void train_on_game(Game *game, QTable &q_table, ActionSpace &action_space, TrainConfig &tc, std::mt19937 &gen) {
    GameState gs;
    GameCommand command;
    uint32_t num_moves = 0;

    Action action;
    int16_t action_index;
    float action_value;
    std::vector<Action> action_pool;
    std::string action_input;

    GameState last_gs_abc;
    Action last_action_abc;
    GameState last_gs_def;
    Action last_action_def;

    std::uniform_real_distribution<float> dist_explore(0, 1);

    // Play until winner
    while (game->get_winner_name() == NO_PLAYER) {
        // Determine player
        PlayerName pturn = game->get_player_turn()->get_name();
        PlayerName popp = pturn == PLAYER_ABC ? PLAYER_DEF : PLAYER_ABC;

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
            if (explore or (!q_table.contains(gs))) {
                // If exploring, fetch a random action from the action pool
                std::uniform_int_distribution<uint16_t> dist_pool(0, action_pool.size() - 1);
                action_index = dist_pool(gen);
                action = action_pool[action_index];
                action_value = q_table[gs][action];

            } else {
                action_index = -1;
                action_value = 0;

                // Try all known actions first to see if any are above 0
                for (auto it_q = q_table[gs].begin(); it_q != q_table[gs].end(); it_q++) {
                    Action a = it_q->first;

                    // Found an action explored in the past with a better-than-default value
                    if (q_table[gs][a] > action_value) {
                        // Find its index in action pool
                        auto it_ap = std::find(action_pool.begin(), action_pool.end(), a);

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

                printf("- %llu\n", q_table[gs].size());
            }

            // Generate input from action
            if (generate_input(&action_input, &action, game)) {
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

        if (action_value > 0)
            printf("[%s] %s (i=%hu, v=%.2f)\n",
                pturn == PLAYER_ABC ? "ABC" : "DEF",
                action_input.c_str(),
                action_index,
                action_value);

        // Perform action
        // (Exceptions intentionally not handled)
        game->play_option(&command);

        // Update q-table
        if (num_moves >= 2) {
            GameState last_gs = pturn == PLAYER_ABC ? last_gs_abc : last_gs_def;
            Action last_action = pturn == PLAYER_ABC ? last_action_abc : last_action_def;

            if (game->get_winner_name() != NO_PLAYER) {
                if (game->get_winner_name() == pturn) {
                    q_table[gs][action] = 1;
                } else {
                    q_table[gs][action] = -1;
                }
            }

            q_table[last_gs][last_action] = q_table[last_gs][last_action] + tc.learning * (tc.discount * q_table[gs][action] - q_table[last_gs][last_action]);
            /*
            if (game->get_winner_name() != NO_PLAYER) {
                //printf("%f\n", q_table[gs][action]);
                printf("%f\n", q_table[last_gs][last_action]);
            }
            */
        }

        // Log last move
        if (pturn == PLAYER_ABC) {
            last_gs_abc = gs;
            last_action_abc = action;
        } else {
            last_gs_def = gs;
            last_action_def = action;
        }

        num_moves += 1;
    }
}
