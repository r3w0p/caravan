// Copyright (c) 2022-2023 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_TABLE_H
#define CARAVAN_TABLE_H

#include <array>
#include <cstdint>
#include "caravan.h"


class Table {
protected:
    Caravan* d = new Caravan(CARAVAN_D);
    Caravan* e = new Caravan(CARAVAN_E);
    Caravan* f = new Caravan(CARAVAN_F);
    Caravan* a = new Caravan(CARAVAN_A);
    Caravan* b = new Caravan(CARAVAN_B);
    Caravan* c = new Caravan(CARAVAN_C);

    std::array<Caravan*, TABLE_CARAVANS_MAX> caravans = { d, e, f, a, b, c };

    uint8_t caravan_name_to_index_value(CaravanName cn);

public:
    void clear_caravan(CaravanName cn);

    uint16_t get_caravan_bid(CaravanName cn);

    Slot get_slot_at(CaravanName cn, uint8_t pos);

    Direction get_caravan_direction(CaravanName cn);

    uint8_t get_caravan_size(CaravanName cn);

    Suit get_caravan_suit(CaravanName cn);

    void play_face_card(CaravanName cn, Card c, uint8_t pos);

    void play_numeral_card(CaravanName cn, Card c);
};

#endif //CARAVAN_TABLE_H
