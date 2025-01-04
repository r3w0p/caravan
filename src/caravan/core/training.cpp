// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <map>
#include <random>
#include <algorithm>
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

    // Total: 240 + 48 + 6 + 5 = 299
}

void train_on_game(Game *game, QTable &q_table, ActionSpace &action_space, TrainConfig &tc, std::mt19937 &gen) {
    GameState gs;
    GameCommand command;
    uint32_t num_moves = 0;

    std::string action;
    uint16_t action_index;
    float action_value;

    GameState last_gs_abc;
    std::string last_action_abc;

    GameState last_gs_def;
    std::string last_action_def;

    //std::uniform_int_distribution<uint16_t> dist_action(0, SIZE_ACTION_SPACE - 1);
    std::uniform_real_distribution<float> dist_explore(0, 1);

    // Play until winner
    while (game->get_winner() == NO_PLAYER) {
        // Determine player
        PlayerName pturn = game->get_player_turn();
        PlayerName popp = pturn == PLAYER_ABC ? PLAYER_DEF : PLAYER_ABC;

        // Get game state in relation to current player
        get_game_state(&gs, game, pturn);

        // Maybe add game state and actions if new state discovered
        if (!q_table.contains(gs)) {
            // TODO maybe is this needed: q_table[gs] = {};
            for (uint16_t i = 0; i < SIZE_ACTION_SPACE; i++) {
                q_table[gs][action_space[i]] = 0;
            }
        }

        // Use action pool that depletes as actions are found to be invalid
        std::vector<std::string> action_pool;
        for (int i = 0; i < SIZE_ACTION_SPACE; i++) {
            action_pool.push_back(action_space[i]);
        }

        // Determine whether to explore for next move
        bool explore = dist_explore(gen) < tc.explore;

        // Find a valid action
        while (true) {
            if (explore) {
                // If exploring, fetch a random action from the action pool
                std::uniform_int_distribution<uint16_t> dist_pool(0, action_pool.size() - 1);
                action_index = dist_pool(gen);
                action = action_pool[action_index];
                action_value = q_table[gs][action];

            } else {
                // Otherwise, pick the optimal action from the q-table
                action_index = 0;
                action_value = q_table[gs][action_pool[action_index]];

                for (uint16_t i_action = 1; i_action < action_pool.size(); i_action++) {
                    // Change pick if next action has greater value
                    if (q_table[gs][action_pool[i_action]] > action_value) {
                        action_index = i_action;
                        action_value = q_table[gs][action_pool[action_index]];
                    }
                }

                action = action_pool[action_index];
            }

            // Generate command for action
            command = generate_command(action, true);

            // TODO error if all actions are invalid? should not be possible!

            // Use action if valid
            if (game->check_option(&command)) break;

            // Remove action from pool and try again
            action_pool.erase(action_pool.begin() + action_index);
        }

        if (action_value > 0)
            printf("[%s] %s (i=%hu, v=%.2f)\n", pturn == PLAYER_ABC ? "ABC" : "DEF", action.c_str(), action_index, action_value);

        // Perform action
        // (Exceptions intentionally not handled)
        game->play_option(&command);

        // Update q-table
        if (num_moves >= 2) {
            GameState last_gs = pturn == PLAYER_ABC ? last_gs_abc : last_gs_def;
            std::string last_action = pturn == PLAYER_ABC ? last_action_abc : last_action_def;

            if (game->get_winner() != NO_PLAYER) {
                if (game->get_winner() == pturn) {
                    q_table[gs][action] = 1;
                } else {
                    q_table[gs][action] = -1;
                }
            }

            q_table[last_gs][last_action] = q_table[last_gs][last_action] + tc.learning * (tc.discount * q_table[gs][action] - q_table[last_gs][last_action]);
            /*
            if (game->get_winner() != NO_PLAYER) {
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
