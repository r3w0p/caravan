// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_COMMON_H
#define CARAVAN_COMMON_H

#include <cstdint>
#include <array>
#include <vector>

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
const uint8_t HAND_SIZE_MAX = 8;
const uint8_t HAND_POS_MIN = 1;
const uint8_t HAND_POS_MAX = HAND_SIZE_MAX;
const uint8_t TABLE_CARAVANS_MAX = 6;

/*
 * ENUMS
 */

enum PlayerName {
    NO_PLAYER, PLAYER_1, PLAYER_2, COMPUTER
};
enum Direction {
    NO_DIRECTION, ASCENDING, DESCENDING
};
enum CaravanName {
    NO_CARAVAN, CARAVAN_A, CARAVAN_B, CARAVAN_C, CARAVAN_D, CARAVAN_E, CARAVAN_F
};
enum OptionType {
    NO_OPTION, OPTION_PLAY, OPTION_REMOVE, OPTION_CLEAR
};
enum Suit {
    NO_SUIT, CLUBS, DIAMONDS, HEARTS, SPADES
};
enum Rank {
    NO_RANK, ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, JOKER
};

/*
 * TYPES
 */

typedef struct Card {
    Suit suit{};
    Rank rank{};
} Card;

typedef std::array<Card, HAND_SIZE_MAX> Hand;
typedef std::vector<Card> Deck;
typedef std::array<Card, TRACK_FACE_MAX> Faces;

typedef struct TrackSlot {
    Card card{};
    Faces faces{};
    uint8_t i_faces = 0;
} TrackSlot;

typedef std::array<TrackSlot, TRACK_NUMERIC_MAX> Track;

typedef struct GameConfig {
    uint8_t p1_num_cards;
    uint8_t p1_num_sample_decks;
    bool p1_balanced_sample;

    uint8_t p2_num_cards;
    uint8_t p2_num_sample_decks;
    bool p2_balanced_sample;

    PlayerName p_first;
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

bool is_numeric_card(Card c);

bool is_face_card(Card c);

#endif //CARAVAN_COMMON_H
