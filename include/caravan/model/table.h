// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_MODEL_TABLE_H
#define CARAVAN_MODEL_TABLE_H

#include <array>
#include <memory>

#include "caravan/model/caravan.h"

namespace Caravan::Model {
    class Table {
        protected:
            std::array<Caravan, TABLE_CARAVANS_MAX> caravans{
                Caravan(CARAVAN_A),
                Caravan(CARAVAN_B),
                Caravan(CARAVAN_C),
                Caravan(CARAVAN_D),
                Caravan(CARAVAN_E),
                Caravan(CARAVAN_F)
            };

        public:
            explicit Table() = default;

            ~Table() = default;

            void clear_caravan(CaravanName cvname);

            [[nodiscard]] Caravan &get_caravan(CaravanName cvname);

            void play_face_card(
                CaravanName cvname,
                Card card,
                uint8_t pos
            );

            void play_numeral_card(CaravanName cvname, Card card);
    };
}

#endif //CARAVAN_MODEL_TABLE_H
