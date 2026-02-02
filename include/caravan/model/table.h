// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_MODEL_TABLE_H
#define CARAVAN_MODEL_TABLE_H

#include <array>
#include <cstdint>
#include <memory>

#include "caravan/model/caravan.h"


class Table {
protected:
    std::unique_ptr<Caravan> cvn_a;
    std::unique_ptr<Caravan> cvn_b;
    std::unique_ptr<Caravan> cvn_c;
    std::unique_ptr<Caravan> cvn_d;
    std::unique_ptr<Caravan> cvn_e;
    std::unique_ptr<Caravan> cvn_f;

    std::array<Caravan *, TABLE_CARAVANS_MAX> caravans{};

public:
    explicit Table();

    ~Table() = default;

    void clear_caravan(CaravanName cvname) const;

    Caravan *get_caravan(CaravanName cvname) const;

    void play_face_card(CaravanName cvname, Card card, uint8_t pos) const;

    void play_numeral_card(CaravanName cvname, Card card) const;
};

#endif //CARAVAN_MODEL_TABLE_H
