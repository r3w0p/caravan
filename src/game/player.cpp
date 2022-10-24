// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "player.h"
#include "common.h"
#include "exceptions.h"


uint8_t Player::get_size_deck() {
    return deck->size();
}

uint8_t Player::get_size_hand() {
    return i_hand;
}

Card Player::remove_from_hand_at(uint8_t pos) {
    if (pos < HAND_POS_MIN or pos > HAND_POS_MAX)
        throw CaravanGameException("The chosen position is out of range.");

    if (i_hand == 0)
        throw CaravanGameException("The Player's hand is empty.");

    uint8_t i = pos - 1;

    if (i >= i_hand)
        throw CaravanGameException("There is no card at the chosen position.");

    // Get card at index
    Card c_ret = hand[i];

    // Move the cards above it down
    for (i; (i + 1) < i_hand; ++i)
        hand[i] = hand[i + 1];

    // If more cards in the deck
    if (!deck->empty()) {
        // Add new card from deck into top of hand
        hand[i_hand - 1] = deck->back();
        deck->pop_back();

    } else {
        // Reduce hand size by 1
        i_hand -= 1;
    }

    return c_ret;
}

Card Player::get_from_hand_at(uint8_t pos) {
    if (pos < HAND_POS_MIN or pos > HAND_POS_MAX)
        throw CaravanGameException("The chosen position is out of range.");

    if (i_hand == 0)
        throw CaravanGameException("The Player's hand is empty.");

    uint8_t i = pos - 1;

    if (i >= i_hand)
        throw CaravanGameException("There is no card at the chosen position.");

    return hand[i];
}

Hand Player::get_hand() {
    return hand;
}

PlayerName Player::get_name() {
    return name;
}

uint16_t Player::get_moves_count() {
    return moves;
}

void Player::increment_moves_count() {
    moves += 1;
}
