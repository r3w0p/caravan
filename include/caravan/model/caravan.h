// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_MODEL_CARAVAN_H
#define CARAVAN_MODEL_CARAVAN_H

#include <cstdint>
#include <array>
#include "caravan/model/deck.h"

namespace Caravan::Model {
    class Caravan {
        protected:
            CaravanName name;
            Track track;
            uint8_t i_track;

            static uint8_t numeral_rank_to_uint8_t(Rank rank);

            void remove_numeral_card(uint8_t index);

        public:
            /**
         * A caravan that contains all information for a given track of numeral
         * cards and any face cards attached to them, including
         * the total caravan bid, its direction, and its suit.
         *
         * @param cvname The caravan name.
         */
            explicit Caravan(const CaravanName cvname) : name(cvname),
                track({}),
                i_track(0) {
            }

            ~Caravan() = default;

            void clear();

            [[nodiscard]] uint16_t get_bid() const;

            [[nodiscard]] Direction get_direction() const;

            [[nodiscard]] CaravanName get_name() const;

            [[nodiscard]] uint8_t get_size() const;

            [[nodiscard]] Slot get_slot(uint8_t pos) const;

            [[nodiscard]] Suit get_suit() const;

            Card put_face_card(Card card, uint8_t pos);

            void put_numeral_card(Card card);

            void remove_rank(Rank rank, uint8_t pos_exclude);

            void remove_suit(Suit suit, uint8_t pos_exclude);
    };
}
#endif //CARAVAN_MODEL_CARAVAN_H
