// Copyright (c) 2022-2025 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_MODEL_CONSTANTS_H
#define CARAVAN_MODEL_CONSTANTS_H

#include <cstdint>

constexpr uint8_t TRACK_NUMERIC_MIN = 1;
constexpr uint8_t TRACK_NUMERIC_MAX = 8;
constexpr uint8_t TRACK_FACE_MAX = 3;

constexpr uint8_t CARAVAN_SOLD_MIN = 21;
constexpr uint8_t CARAVAN_SOLD_MAX = 26;

constexpr uint8_t DECK_TRADITIONAL_MAX = 54;
constexpr uint8_t DECK_CARAVAN_MIN = 30;
constexpr uint8_t DECK_CARAVAN_MAX = 162;

constexpr uint8_t SAMPLE_DECKS_MIN = 1;
constexpr uint8_t SAMPLE_DECKS_MAX = 3;

constexpr uint8_t MOVES_START_ROUND = 3;

constexpr uint8_t HAND_SIZE_MAX_START = 8;
constexpr uint8_t HAND_SIZE_MAX_POST_START = 5;
constexpr uint8_t HAND_POS_MIN = 1;

constexpr uint8_t TABLE_CARAVANS_MAX = 6;

constexpr uint8_t PLAYER_CARAVANS_MAX = 3;

#endif //CARAVAN_MODEL_CONSTANTS_H
