// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_MODEL_DECK_H
#define CARAVAN_MODEL_DECK_H

#include "caravan/core/common.h"

class DeckBuilder {
protected:
    /**
     * @param shuffle If true, the deck is shuffled. If false, the deck is in
     *        numeral order.
     * @return A traditional deck: standard 52 cards + 2 JOKERs.
     */
    static Deck build_traditional_deck(bool shuffle);

    /**
     * @param d The deck to shuffle.
     * @return A deck with shuffled cards.
     */
    static Deck shuffle_deck(Deck d);

public:
    DeckBuilder() = delete;

    /**
     * @param num_cards The number of cards to have in the caravan deck,
     *        must be between 30 and 162 cards (inclusive).
     * @param num_sample_decks The number of standard card decks
     *        (52 cards + 2 Jokers) from which to sample cards for the caravan
     *        deck, must be between 1 and 3 (inclusive).
     * @param balanced_sample If true, standard decks are sampled in a
     *        round-robin fashion e.g. a random card from deck 1, then deck 2,
     *        then 3, then 1, 2, 3, and so on. If false, then standard decks are
     *        sampled randomly.
     *
     * @return A caravan deck.
     *
     * @throws CaravanFatalException Requested number of cards outside of
     *         acceptable range.
     * @throws CaravanFatalException Requested number of sample decks outside of
     *         acceptable range.
     * @throws CaravanFatalException Insufficient cards to sample in order to
     *         build deck.
     */
    static Deck *build_caravan_deck(
        uint8_t num_cards,
        uint8_t num_sample_decks,
        bool balanced_sample);
};

#endif //CARAVAN_MODEL_DECK_H
