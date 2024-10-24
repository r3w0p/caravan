// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/model/player.h"
#include "caravan/core/exceptions.h"

Player::Player(PlayerName pn, Deck *d) {
    name = pn;
    deck = d;
    hand = {};
    i_hand = 0;
    moves = 0;

    for (; i_hand < HAND_SIZE_MAX_START; i_hand++) {
        hand[i_hand] = deck->back();
        deck->pop_back();
    }
}

Player::~Player() {
    delete deck;
}

Card Player::get_from_hand_at(uint8_t pos) {
    if (i_hand == 0) {
        throw CaravanFatalException("Player's hand is empty.");
    }

    if (pos < HAND_POS_MIN or pos > i_hand) {
        throw CaravanGameException("The chosen hand position is out of range.");
    }

    return hand[pos - 1];
}

Hand Player::get_hand() {
    return hand;
}

uint16_t Player::get_moves_count() {
    return moves;
}

PlayerName Player::get_name() {
    return name;
}

uint8_t Player::get_size_deck() {
    return deck->size();
}

uint8_t Player::get_size_hand() {
    return i_hand;
}

void Player::increment_moves() {
    moves += 1;
}

void Player::maybe_add_card_to_hand() {
    // If more cards in the deck
    if (!deck->empty()) {
        // If post-Start and hand not at post-Start max (5 cards)
        if (moves > MOVES_START_ROUND and i_hand < HAND_SIZE_MAX_POST_START) {
            // Add new card from deck to top of hand
            hand[i_hand] = deck->back();
            deck->pop_back();
            i_hand += 1;
        }
    }
}

Card Player::discard_from_hand_at(uint8_t pos) {
    uint8_t i;
    Card c_ret;

    if (i_hand == 0) {
        throw CaravanFatalException(
            "Player's hand is empty.");
    }

    if (pos < HAND_POS_MIN or pos > i_hand) {
        throw CaravanGameException(
            "The chosen hand position is out of range.");
    }

    i = pos - 1;
    c_ret = hand[i];

    // Move the cards above it downwards
    for (i; (i + 1) < i_hand; ++i) {
        hand[i] = hand[i + 1];
    }

    i_hand -= 1;

    return c_ret;
}
