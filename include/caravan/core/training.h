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

const uint16_t SIZE_GAME_STATE = 35;

const uint8_t NUM_PLAYER_ABC = 1;
const uint8_t NUM_PLAYER_DEF = 2;

// ACTIONS

const uint16_t SIZE_ACTION_SPACE = 16;
typedef uint8_t Action;

const uint8_t ACTION_DISCARD_NUMERAL = 10;
const uint8_t ACTION_DISCARD_JACK = 11;
const uint8_t ACTION_DISCARD_QUEEN = 12;
const uint8_t ACTION_DISCARD_KING = 13;
const uint8_t ACTION_DISCARD_JOKER = 14;

const uint8_t ACTION_CLEAR_BUST = 20;

const uint8_t ACTION_PLAY_NUMERAL_1 = 30;
const uint8_t ACTION_PLAY_NUMERAL_2 = 31;
const uint8_t ACTION_PLAY_NUMERAL_3 = 32;

const uint8_t ACTION_PLAY_JACK_SELF = 40;
const uint8_t ACTION_PLAY_JACK_OPP = 41;  // on highest non-bust

const uint8_t ACTION_PLAY_QUEEN_SELF = 50;  // on LOW+DES, HIGH+ASC, light
const uint8_t ACTION_PLAY_QUEEN_OPP = 51;  // on LOW+ASC, HIGH+DES, light

const uint8_t ACTION_PLAY_KING_SELF = 60; // lowest non-bust, on card that would not cause bust
const uint8_t ACTION_PLAY_KING_OPP = 61;  // highest non-bust, on card that would preferably cause bust

const uint8_t ACTION_PLAY_JOKER = 70;  // most frequent opp card, play on self where possible

/*
 * TYPES
 */

typedef std::array<Action, SIZE_ACTION_SPACE> ActionSpace;
typedef std::array<uint16_t, SIZE_GAME_STATE> GameState;
typedef std::map<GameState, std::map<Action, float>> QTable;

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

bool train_on_game(Game *game, QTable &q_table, ActionSpace &action_space, GameConfig &gc, TrainConfig &tc, std::mt19937 &gen);

#endif //CARAVAN_CORE_TRAINING_H
