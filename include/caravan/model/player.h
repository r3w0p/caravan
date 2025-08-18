// Copyright (c) 2022-2025 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_MODEL_PLAYER_H
#define CARAVAN_MODEL_PLAYER_H

#include <array>
#include <memory>

#include "caravan/model/deck.h"


class Player {
protected:
    PlayerName name;
    std::unique_ptr<Deck> deck;
    Hand hand;
    uint8_t i_hand;
    uint16_t moves;

public:
    explicit Player(PlayerName pn, std::unique_ptr<Deck> d);

    ~Player() = default;

    Card discard_from_hand_at(uint8_t pos);

    Card get_from_hand_at(uint8_t pos) const;

    uint8_t get_size_deck() const;

    uint8_t get_size_hand() const;

    uint16_t get_moves_count() const;

    PlayerName get_name() const;

    void increment_moves();

    void maybe_add_card_to_hand_from_deck();
};

#endif //CARAVAN_MODEL_PLAYER_H
