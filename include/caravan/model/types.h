// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_MODEL_TYPES_H
#define CARAVAN_MODEL_TYPES_H

#include "caravan/model/constants.h"
#include <array>
#include <vector>

namespace Caravan::Model {
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
        NO_DIRECTION,
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
        NO_PLAYER, PLAYER_ABC, PLAYER_DEF
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

        [[nodiscard]] bool is_face_card() const {
            return this->rank >= JACK and this->rank <= JOKER;
        }

        [[nodiscard]] bool is_numeral_card() const {
            return this->rank >= ACE and this->rank <= TEN;
        }
    };

    using Hand = std::array<Card, HAND_SIZE_MAX_START>;
    using Deck = std::vector<Card>;
    using Faces = std::array<Card, TRACK_FACE_MAX>;
    using PlayerCaravanNames = std::array<CaravanName, PLAYER_CARAVANS_MAX>;

    using Slot = struct Slot {
        Card card{};
        Faces faces{};
        uint8_t n_faces = 0;
    };

    using Track = std::array<Slot, TRACK_NUMERIC_MAX>;

    // TODO change so that pos_hand is card_hand?
    //  can avoid ActionMove this way
    using GameMove = struct GameMove {
        OptionType option{NO_OPTION};
        uint8_t pos_hand{};
        CaravanName caravan_name{NO_CARAVAN};
        uint8_t pos_caravan{};
        Card hand{}; // card played from hand
        Card board{}; // card that face card was played on
    };
}

#endif //CARAVAN_MODEL_TYPES_H
