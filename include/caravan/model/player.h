// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_MODEL_PLAYER_H
#define CARAVAN_MODEL_PLAYER_H

#include <array>
#include "caravan/model/deck.h"


class Player {
protected:
    PlayerName name;
    Deck *deck;
    Hand hand;
    uint8_t i_hand;
    uint16_t moves;

public:
    explicit Player(PlayerName pn, Deck *d);

    ~Player();

    Card get_from_hand_at(uint8_t pos);

    Hand get_hand();

    uint16_t get_moves_count();

    PlayerName get_name();

    uint8_t get_size_deck();

    uint8_t get_size_hand();

    void increment_moves();

    void maybe_add_card_to_hand();

    Card discard_from_hand_at(uint8_t pos);
};

#endif //CARAVAN_MODEL_PLAYER_H
