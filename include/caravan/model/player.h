// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_MODEL_PLAYER_H
#define CARAVAN_MODEL_PLAYER_H

#include <array>
#include <memory>

#include "caravan/model/deck.h"

namespace Caravan::Model {
    class Player {
        protected:
            PlayerName name;
            Hand hand;
            uint8_t i_hand;
            uint16_t moves;
            std::unique_ptr<Deck> deck;

        public:
            explicit Player(PlayerName pname, std::unique_ptr<Deck> deck);

            ~Player() = default;

            Card discard_from_hand_at(uint8_t pos);

            [[nodiscard]] Card get_from_hand_at(uint8_t pos) const;

            [[nodiscard]] const Hand &get_hand() const;

            [[nodiscard]] uint8_t get_size_deck() const;

            [[nodiscard]] uint8_t get_size_hand() const;

            [[nodiscard]] uint16_t get_moves_count() const;

            [[nodiscard]] PlayerName get_name() const;

            void increment_moves();

            void maybe_add_card_to_hand_from_deck();
    };
}

#endif //CARAVAN_MODEL_PLAYER_H
