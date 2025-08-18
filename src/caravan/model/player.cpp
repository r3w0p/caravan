// Copyright (c) 2022-2025 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/model/player.h"
#include "caravan/core/exceptions.h"

Player::Player(const PlayerName pn, std::unique_ptr<Deck> d) {
    name = pn;
    deck = std::move(d);
    hand = {};
    i_hand = 0;
    moves = 0;

    // Populate hand for game start
    for (; i_hand < HAND_SIZE_MAX_START; i_hand++) {
        hand[i_hand] = deck->back();
        deck->pop_back();
    }
}

Card Player::discard_from_hand_at(const uint8_t pos) {
    // Player hand should never be empty unless game has already ended
    if (i_hand == 0) {
        throw CaravanFatalModelException(
            "Player's hand is empty.");
    }

    // Illegal request to discard card outside of hand range
    if (pos < HAND_POS_MIN or pos > i_hand) {
        throw CaravanIllegalModelException(
            "The chosen hand position is out of range.");
    }

    // Get card to discard
    uint8_t i = pos - 1;
    const Card c_ret = hand[i];

    // Cards up the hand are moved downwards
    for (; i + 1 < i_hand; ++i) {
        hand[i] = hand[i + 1];
    }

    // Hand size is decreased
    i_hand -= 1;

    return c_ret;
}

Card Player::get_from_hand_at(const uint8_t pos) const {
    // Player hand should never be empty unless game has already ended
    if (i_hand == 0) {
        throw CaravanFatalModelException(
            "Player's hand is empty.");
    }

    // Illegal request to discard card outside of hand range
    if (pos < HAND_POS_MIN or pos > i_hand) {
        throw CaravanIllegalModelException(
            "The chosen hand position is out of range.");
    }

    return hand[pos - 1];
}

uint8_t Player::get_size_deck() const {
    return deck->size();
}

uint8_t Player::get_size_hand() const {
    return i_hand;
}

uint16_t Player::get_moves_count() const {
    return moves;
}

PlayerName Player::get_name() const {
    return name;
}

void Player::increment_moves() {
    moves += 1;
}

void Player::maybe_add_card_to_hand_from_deck() {
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
