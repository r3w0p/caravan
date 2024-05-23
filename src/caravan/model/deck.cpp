// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <chrono>
#include <random>
#include <vector>
#include <algorithm>
#include "caravan/model/deck.h"
#include "caravan/core/common.h"
#include "caravan/core/exceptions.h"

/**
 * @param num_cards The number of cards to have in the caravan deck,
 *        must be between 30 and 156 cards (inclusive).
 * @param num_sample_decks The number of standard card decks
 *        (52 cards + 2 Jokers) from which to sample cards for the caravan deck,
 *        must be between 1 and 3 (inclusive).
 * @param balanced_sample If true, standard decks are sampled in a round-robin
 *        fashion e.g. a random card from deck 1, then deck 2, then 3, then
 *        1, 2, 3, and so on.
 *        If false, then standard decks are sampled randomly.
 *
 * @return A caravan deck.
 *
 * @throws CaravanFatalException Requested number of cards outside of acceptable range.
 * @throws CaravanFatalException Requested number of sample decks outside of acceptable range.
 * @throws CaravanFatalException Insufficient cards to sample in order to build deck.
 */
Deck *DeckBuilder::build_caravan_deck(
        uint8_t num_cards,
        uint8_t num_sample_decks,
        bool balanced_sample) {

    uint8_t total_sample_cards;
    Deck sample_decks[3];
    Deck *d;
    uint8_t i_next;
    Card c_next;
    uint8_t first_hand_num_cards;

    if (num_cards < DECK_CARAVAN_MIN or
        num_cards > DECK_CARAVAN_MAX)
        throw CaravanFatalException(
                "A caravan deck must have between "
                "30 and 156 cards (inclusive).");

    if (num_sample_decks < SAMPLE_DECKS_MIN or
        num_sample_decks > SAMPLE_DECKS_MAX)
        throw CaravanFatalException(
                "A caravan deck must sample from between "
                "1 and 3 standard card decks (inclusive).");

    total_sample_cards = num_sample_decks * DECK_STANDARD_MAX;

    if (total_sample_cards < num_cards)
        throw CaravanFatalException(
                "There are insufficient cards to sample for the "
                "caravan deck.");

    d = new Deck();

    do {
        d->clear();
        i_next = 0;
        first_hand_num_cards = 0;

        for (int i = 0; i < num_sample_decks; ++i)
            sample_decks[i] = DeckBuilder::build_standard_deck(true);

        if (balanced_sample) {
            // Sample decks in a round-robin fashion
            while (d->size() < num_cards) {
                c_next = sample_decks[i_next].back();
                d->push_back(c_next);
                sample_decks[i_next].pop_back();

                i_next = (i_next + 1) % num_sample_decks;

                if ((num_cards - d->size()) < HAND_SIZE_MAX_START and
                    is_numeral_card(c_next))
                    first_hand_num_cards += 1;
            }

        } else {
            // Sample decks randomly
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distr(
                    0, num_sample_decks - 1);

            while (d->size() < num_cards) {
                i_next = distr(gen);
                if (!sample_decks[i_next].empty()) {
                    c_next = sample_decks[i_next].back();
                    d->push_back(c_next);
                    sample_decks[i_next].pop_back();

                    if ((num_cards - d->size()) < HAND_SIZE_MAX_START and
                        is_numeral_card(c_next))
                        first_hand_num_cards += 1;
                }
            }
        }

    } while (first_hand_num_cards < MOVES_START_ROUND);

    return d;
}

/*
 * PROTECTED
 */

/**
 * @param shuffle If true, deck is shuffled. If false, it is in numeral order.
 * @return A standard card deck (52 cards + 2 Jokers).
 */
Deck DeckBuilder::build_standard_deck(bool shuffle) {
    Deck d;

    for (int i = CLUBS; i <= SPADES; ++i)
        for (int j = ACE; j <= KING; ++j)
            d.push_back({
                static_cast<Suit>(i),
                static_cast<Rank>(j)
            });

    d.push_back({NO_SUIT, JOKER});
    d.push_back({NO_SUIT, JOKER});

    if (shuffle)
        return shuffle_deck(d);
    else
        return d;
}

/**
 * @param d The deck to shuffle.
 * @return A deck with shuffled cards.
 */
Deck DeckBuilder::shuffle_deck(Deck d) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(d.begin(), d.end(), std::default_random_engine(seed));
    return d;
}