// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_MODEL_CARAVAN_H
#define CARAVAN_MODEL_CARAVAN_H

#include <cstdint>
#include <array>
#include "caravan/model/deck.h"
#include "caravan/core/common.h"


class Caravan {

protected:
    CaravanName name;
    Track track;
    uint8_t i_track;

    static uint8_t numeral_rank_to_uint8_t(Rank rank);

    void remove_numeral_card(uint8_t index);

public:
    explicit Caravan(CaravanName cvname);

    void clear();

    uint16_t get_bid();

    Slot get_slot(uint8_t pos);

    Direction get_direction();

    CaravanName get_name();

    uint8_t get_size();

    Suit get_suit();

    void put_numeral_card(Card card);

    Card put_face_card(Card card, uint8_t pos);

    void remove_rank(Rank rank, uint8_t pos_exclude);

    void remove_suit(Suit suit, uint8_t pos_exclude);
};

#endif //CARAVAN_MODEL_CARAVAN_H
