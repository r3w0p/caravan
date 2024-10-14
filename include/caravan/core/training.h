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
const uint16_t SIZE_GAME_STATE = 201;

/*
 * TYPES
 */

typedef std::array<std::string, SIZE_ACTION_SPACE> ActionSpace;
typedef std::array<uint16_t, SIZE_GAME_STATE> GameState;
typedef std::map<GameState, std::map<std::string, uint16_t>> QTable;

typedef struct TrainConfig {
    float discount{0.0};
    float explore{0.0};
    float learning{0.0};
    uint32_t episode_max{0};
    uint32_t episode{0};
} TrainConfig;

/*
 * FUNCTIONS
 */

uint8_t card_to_uint8_t(Card c);
void get_game_state(GameState *gs, Game *game, PlayerName pname);
void populate_action_space(ActionSpace *as);

#endif //CARAVAN_CORE_TRAINING_H
