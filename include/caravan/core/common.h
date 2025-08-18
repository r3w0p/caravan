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

/*
 * ENUMS
 */

enum CaravanName {
    NO_CARAVAN,
    CARAVAN_A,
    CARAVAN_B,
    CARAVAN_C,
    CARAVAN_D,
    CARAVAN_E,
    CARAVAN_F
};

enum Direction {
    ANY,
    ASCENDING,
    DESCENDING
};

enum OptionType {
    NO_OPTION,
    OPTION_PLAY,
    OPTION_DISCARD,
    OPTION_CLEAR
};

enum PlayerName {
    NO_PLAYER,
    PLAYER_ABC,
    PLAYER_DEF
};

enum Rank {
    // TODO NO_RANK ?
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

enum Suit {
    NO_SUIT,
    CLUBS,
    DIAMONDS,
    HEARTS,
    SPADES
};

/*
 * TYPES
 */

using Card = struct Card {
    Suit suit{};
    Rank rank{};
};

using Hand = std::array<Card, HAND_SIZE_MAX_START>;
using Deck = std::vector<Card>;
using Faces = std::array<Card, TRACK_FACE_MAX>;
using PlayerCaravanNames = std::array<CaravanName, 3>;

using Slot = struct Slot {
    Card card{};
    Faces faces{};
    uint8_t i_faces = 0;
};

using Track = std::array<Slot, TRACK_NUMERIC_MAX>;

using GameConfig = struct GameConfig {
    uint8_t player_abc_cards{0};
    uint8_t player_abc_samples{0};
    bool player_abc_balanced{false};

    uint8_t player_def_cards{0};
    uint8_t player_def_samples{0};
    bool player_def_balanced{false};

    PlayerName player_first{NO_PLAYER};
};

using GameCommand = struct GameCommand {
    OptionType option{NO_OPTION};
    uint8_t pos_hand{0};
    CaravanName caravan_name{NO_CARAVAN};
    uint8_t pos_caravan{0};
    Card hand{};
    Card board{};
};

/*
 * FUNCTIONS
 */

bool is_numeral_card(Card c);

bool is_face_card(Card c);

std::string caravan_letter(CaravanName caravan_name);

uint8_t numeral_rank_value(Card c);

#endif //CARAVAN_CORE_COMMON_H
