// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_CARAVAN_H
#define CARAVAN_CARAVAN_H

#include <cstdint>
#include <array>
#include "deck.h"
#include "common.h"


class Caravan {
private:
    CaravanName name;
    Track track;
    uint8_t i_track;

    static uint8_t numeric_rank_to_int_value(Rank r);

    void remove_numeric_card(uint8_t i);

public:
    explicit Caravan(CaravanName pn) {
        name = pn;
        track = {};
        i_track = 0;
    }

    void clear();

    uint16_t get_bid();

    TrackSlot get_cards_at(uint8_t pos);

    Direction get_direction();

    CaravanName get_name();

    Suit get_suit();

    void put_numeric_card(Card c);

    Card put_face_card(Card c, uint8_t pos);

    void remove_rank(Rank r, uint8_t exclude);

    void remove_suit(Suit s, uint8_t exclude);

    uint8_t size();
};

#endif //CARAVAN_CARAVAN_H
