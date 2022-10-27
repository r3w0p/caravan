// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_PLAYER_H
#define CARAVAN_PLAYER_H

#include <array>
#include "deck.h"


class Player {
protected:
    PlayerName name;
    Deck *deck;
    Hand hand;
    uint8_t i_hand;
    uint16_t moves;

public:
    explicit Player(PlayerName pn, Deck *d) {
        name = pn;
        deck = d;
        hand = {};
        i_hand = 0;
        moves = 0;

        while (i_hand < HAND_SIZE_MAX_START) {
            hand[i_hand] = deck->back();
            deck->pop_back();
            i_hand += 1;
        }
    }

    Card get_from_hand_at(uint8_t pos);

    Hand get_hand();

    uint16_t get_moves_count();

    PlayerName get_name();

    uint8_t get_size_deck();

    uint8_t get_size_hand();

    void increment_moves();

    void maybe_add_card_to_hand();

    Card remove_from_hand_at(uint8_t pos);
};

#endif //CARAVAN_PLAYER_H
