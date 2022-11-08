// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_COMMON_H
#define CARAVAN_COMMON_H

#include <cstdint>
#include <array>
#include <vector>
#include <string>

/*
 * CONSTANTS
 */

const uint8_t TRACK_NUMERIC_MIN = 1;
const uint8_t TRACK_NUMERIC_MAX = 10;
const uint8_t TRACK_FACE_MAX = 5;
const uint8_t CARAVAN_SOLD_MIN = 21;
const uint8_t CARAVAN_SOLD_MAX = 26;
const uint8_t DECK_STANDARD_MAX = 52;
const uint8_t DECK_CARAVAN_MIN = 30;
const uint8_t DECK_CARAVAN_MAX = 156;
const uint8_t SAMPLE_DECKS_MIN = 1;
const uint8_t SAMPLE_DECKS_MAX = 3;
const uint8_t MOVES_START_ROUND = 3;
const uint8_t HAND_SIZE_MAX_START = 8;
const uint8_t HAND_SIZE_MAX_POST_START = 5;
const uint8_t HAND_POS_MIN = 1;
const uint8_t TABLE_CARAVANS_MAX = 6;
const uint8_t PLAYER_CARAVANS_MAX = 3;
const uint8_t VIEW_DISPLAY_ROW_MAX = 51;
const uint8_t VIEW_DISPLAY_COL_MAX = 72;
const uint8_t VIEW_DISPLAY_ROW_MIDDLE = 25;

/*
 * ENUMS
 */

enum PlayerName {
    NO_PLAYER, PLAYER_A, PLAYER_B
};
enum Direction {
    ANY, ASCENDING, DESCENDING
};
enum CaravanName {
    NO_CARAVAN, CARAVAN_A, CARAVAN_B, CARAVAN_C, CARAVAN_D, CARAVAN_E, CARAVAN_F
};
enum OptionType {
    NO_OPTION, OPTION_PLAY, OPTION_DISCARD, OPTION_CLEAR
};
enum Suit {
    NO_SUIT, CLUBS, DIAMONDS, HEARTS, SPADES
};
enum Rank {
    NO_RANK,
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


typedef std::array<std::array<wchar_t, VIEW_DISPLAY_COL_MAX>, VIEW_DISPLAY_ROW_MAX> ViewDisplayArray;

typedef struct Slot {
    Card card{};
    Faces faces{};
    uint8_t i_faces = 0;
} Slot;

typedef std::array<Slot, TRACK_NUMERIC_MAX> Track;

typedef struct GameConfig {
    uint8_t pa_num_cards;
    uint8_t pa_num_sample_decks;
    bool pa_balanced_sample;

    uint8_t pb_num_cards;
    uint8_t pb_num_sample_decks;
    bool pb_balanced_sample;

    PlayerName pn_first;
} GameConfig;

typedef struct GameOption {
    OptionType type;
    uint8_t pos_hand;
    CaravanName caravan_name;
    uint8_t pos_caravan;
} GameOption;

/*
 * FUNCTIONS
 */

std::string caravan_name_to_str(CaravanName cn);

std::string direction_to_str(Direction dir);

bool is_numeric_card(Card c);

bool is_face_card(Card c);

std::string player_name_to_str(PlayerName pn);

std::string rank_to_str(Rank r);

std::string suit_to_str(Suit s);

#endif //CARAVAN_COMMON_H
