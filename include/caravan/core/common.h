// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_CORE_COMMON_H
#define CARAVAN_CORE_COMMON_H

#include <cstdint>
#include <array>
#include <vector>
#include <string>

/*
 * CONSTANTS
 */

const uint8_t TRACK_NUMERIC_MIN = 1;
const uint8_t TRACK_NUMERIC_MAX = 8;
const uint8_t TRACK_FACE_MAX = 3;
const uint8_t CARAVAN_SOLD_MIN = 21;
const uint8_t CARAVAN_SOLD_MAX = 26;
const uint8_t DECK_TRADITIONAL_MAX = 54;
const uint8_t DECK_CARAVAN_MIN = 30;
const uint8_t DECK_CARAVAN_MAX = 162;
const uint8_t SAMPLE_DECKS_MIN = 1;
const uint8_t SAMPLE_DECKS_MAX = 3;
const uint8_t MOVES_START_ROUND = 3;
const uint8_t HAND_SIZE_MAX_START = 8;
const uint8_t HAND_SIZE_MAX_POST_START = 5;
const uint8_t HAND_POS_MIN = 1;
const uint8_t TABLE_CARAVANS_MAX = 6;
const uint8_t PLAYER_CARAVANS_MAX = 3;

/*
 * ENUMS
 */

enum PlayerName {
    NO_PLAYER,
    PLAYER_ABC,
    PLAYER_DEF
};
enum Direction {
    ANY,
    ASCENDING,
    DESCENDING
};
enum CaravanName {
    NO_CARAVAN,
    CARAVAN_A,
    CARAVAN_B,
    CARAVAN_C,
    CARAVAN_D,
    CARAVAN_E,
    CARAVAN_F
};
enum OptionType {
    NO_OPTION,
    OPTION_PLAY,
    OPTION_DISCARD,
    OPTION_CLEAR
};
enum Suit {
    NO_SUIT,
    CLUBS,
    DIAMONDS,
    HEARTS,
    SPADES
};
enum Rank {
    ACE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING,
    JOKER
};

/*
 * TYPES
 */

typedef struct Card {
    Suit suit{};
    Rank rank{};
} Card;

typedef std::array<Card, HAND_SIZE_MAX_START> Hand;
typedef std::vector<Card> Deck;
typedef std::array<Card, TRACK_FACE_MAX> Faces;
typedef std::array<CaravanName, 3> PlayerCaravanNames;

typedef struct Slot {
    Card card{};
    Faces faces{};
    uint8_t i_faces = 0;
} Slot;

typedef std::array<Slot, TRACK_NUMERIC_MAX> Track;

typedef struct GameConfig {
    uint8_t player_abc_cards{0};
    uint8_t player_abc_samples{0};
    bool player_abc_balanced{false};

    uint8_t player_def_cards{0};
    uint8_t player_def_samples{0};
    bool player_def_balanced{false};

    PlayerName player_first{NO_PLAYER};
} GameConfig;

typedef struct GameCommand {
    OptionType option{NO_OPTION};
    uint8_t pos_hand{0};
    CaravanName caravan_name{NO_CARAVAN};
    uint8_t pos_caravan{0};
    Card hand{};
    Card board{};
} GameCommand;

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

bool is_numeral_card(Card c);

bool is_face_card(Card c);

std::string caravan_letter(CaravanName caravan_name);

uint8_t numeral_rank_value(Card c);

#endif //CARAVAN_CORE_COMMON_H
