// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_CORE_TRAINING_H
#define CARAVAN_CORE_TRAINING_H

#include <cstdint>
#include <array>
#include <vector>
#include <string>

const uint16_t SIZE_GAME_STATE = 1;

typedef std::array<uint8_t, SIZE_GAME_STATE> GameState;

typedef std::map<GameState, std::map<std::string, uint8_t>> QTable;

typedef struct TrainConfig {
    float discount{0.0};
    float explore{0.0};
    float learning{0.0};
    uint32_t episode_max{0};
    uint32_t episode{0};
} TrainConfig;

#endif //CARAVAN_CORE_TRAINING_H
