// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/model/player.h"
#include "caravan/core/exceptions.h"

const std::string EXC_CLOSED = "Player is closed.";

Player::Player(PlayerName pn, Deck *d) {
    name = pn;
    deck = d;
    hand = {};
    i_hand = 0;
    moves = 0;
    closed = false;

    for(; i_hand < HAND_SIZE_MAX_START; i_hand++) {
        hand[i_hand] = deck->back();
        deck->pop_back();
    }
}

void Player::close() {
    if(!closed) {
        delete deck;

        closed = true;
    }
}

Card Player::get_from_hand_at(uint8_t pos) {
    if (closed) throw CaravanFatalException(EXC_CLOSED);

    if (i_hand == 0)
        throw CaravanFatalException("Player's hand is empty.");

    if (pos < HAND_POS_MIN or pos > i_hand)
        throw CaravanGameException("The chosen hand position is out of range.");

    return hand[pos - 1];
}

Hand Player::get_hand() {
    if (closed) throw CaravanFatalException(EXC_CLOSED);
    return hand;
}

uint16_t Player::get_moves_count() {
    if (closed) throw CaravanFatalException(EXC_CLOSED);
    return moves;
}

PlayerName Player::get_name() {
    if (closed) throw CaravanFatalException(EXC_CLOSED);
    return name;
}

uint8_t Player::get_size_deck() {
    if (closed) throw CaravanFatalException(EXC_CLOSED);
    return deck->size();
}

uint8_t Player::get_size_hand() {
    if (closed) throw CaravanFatalException(EXC_CLOSED);
    return i_hand;
}

void Player::increment_moves() {
    if (closed) throw CaravanFatalException(EXC_CLOSED);
    moves += 1;
}

void Player::maybe_add_card_to_hand() {
    if (closed) throw CaravanFatalException(EXC_CLOSED);
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
    if (closed) throw CaravanFatalException(EXC_CLOSED);
    uint8_t i;
    Card c_ret;

    if (i_hand == 0)
        throw CaravanFatalException(
                "Player's hand is empty.");

    if (pos < HAND_POS_MIN or pos > i_hand)
        throw CaravanGameException(
                "The chosen hand position is out of range.");

    i = pos - 1;
    c_ret = hand[i];

    // Move the cards above it downwards
    for (i; (i + 1) < i_hand; ++i)
        hand[i] = hand[i + 1];

    i_hand -= 1;

    return c_ret;
}
