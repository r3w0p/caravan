// Copyright (c) 2022-2025 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_MODEL_TYPES_H
#define CARAVAN_MODEL_TYPES_H

#include "caravan/model/constants.h"
#include <array>
#include <vector>

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

enum Suit {
    NO_SUIT,
    CLUBS,
    DIAMONDS,
    HEARTS,
    SPADES
};

using Card = struct Card {
    Suit suit{NO_SUIT};
    Rank rank{NO_RANK};

    bool is_face_card() const {
        return (this->rank >= JACK and this->rank <= JOKER);
    }

    bool is_numeral_card() const {
        return (this->rank >= ACE and this->rank <= TEN);
    }
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

using GameMove = struct GameMove {
    OptionType option{NO_OPTION};
    uint8_t pos_hand{0};
    CaravanName caravan_name{NO_CARAVAN};
    uint8_t pos_caravan{0};
    Card hand{};
    Card board{};
};

#endif //CARAVAN_MODEL_TYPES_H
