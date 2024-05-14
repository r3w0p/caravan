// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_MODEL_TABLE_H
#define CARAVAN_MODEL_TABLE_H

#include <array>
#include <cstdint>
#include "caravan/model/caravan.h"


class Table {
protected:
    Caravan* a = new Caravan(CARAVAN_A);
    Caravan* b = new Caravan(CARAVAN_B);
    Caravan* c = new Caravan(CARAVAN_C);
    Caravan* d = new Caravan(CARAVAN_D);
    Caravan* e = new Caravan(CARAVAN_E);
    Caravan* f = new Caravan(CARAVAN_F);

    std::array<Caravan*, TABLE_CARAVANS_MAX> caravans = { a, b, c, d, e, f };

    uint8_t caravan_name_to_uint8_t_index(CaravanName cvname);

public:
    explicit Table();

    void clear_caravan(CaravanName cvname);

    uint16_t get_caravan_bid(CaravanName cvname);

    Slot get_slot_at(CaravanName cvname, uint8_t pos);

    Direction get_caravan_direction(CaravanName cvname);

    uint8_t get_caravan_size(CaravanName cvname);

    Suit get_caravan_suit(CaravanName cvname);

    void play_face_card(CaravanName cvname, Card card, uint8_t pos);

    void play_numeral_card(CaravanName cvname, Card card);
};

#endif //CARAVAN_MODEL_TABLE_H
