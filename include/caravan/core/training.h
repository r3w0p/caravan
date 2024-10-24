// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_CORE_TRAINING_H
#define CARAVAN_CORE_TRAINING_H

#include <cstdint>
#include <array>
#include <vector>
#include <string>

/*
 * CONSTANTS
 */

const uint16_t SIZE_ACTION_SPACE = 299;
const uint16_t SIZE_GAME_STATE = 200;

const uint8_t NUM_PLAYER_ABC = 1;
const uint8_t NUM_PLAYER_DEF = 2;

/*
 * TYPES
 */

typedef std::array<std::string, SIZE_ACTION_SPACE> ActionSpace;
typedef std::array<uint16_t, SIZE_GAME_STATE> GameState;
typedef std::map<GameState, std::map<std::string, float>> QTable;

typedef struct TrainConfig {
    float discount{0.0};
    float explore{0.0};
    float learning{0.0};
    uint32_t episode_max{0};
    uint32_t episode{0};
} TrainConfig;

std::uniform_int_distribution<uint8_t> dist_first_player(NUM_PLAYER_ABC, NUM_PLAYER_DEF);
std::uniform_int_distribution<uint16_t> dist_action(0, SIZE_ACTION_SPACE - 1);
std::uniform_real_distribution<float> dist_explore(0, 1);

/*
 * FUNCTIONS
 */

uint8_t card_to_uint8_t(Card c);
void get_game_state(GameState *gs, Game *game, PlayerName pname);
void populate_action_space(ActionSpace *as);

void train_on_game(Game *game, QTable &q_table, ActionSpace &action_space, TrainConfig &tc, std::mt19937 &gen);

#endif //CARAVAN_CORE_TRAINING_H
