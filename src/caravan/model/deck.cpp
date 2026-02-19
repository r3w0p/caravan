// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <chrono>
#include <random>
#include <vector>
#include <algorithm>
#include "caravan/model/deck.h"
#include "caravan/model/types.h"
#include "caravan/core/exceptions.h"
#include "caravan/core/functions.h"

namespace Caravan::Model {
    /**
     * @param num_cards The number of cards to have in the caravan deck,
     *        must be between 30 and 162 cards (inclusive).
     * @param num_sample_decks The number of standard card decks
     *        (52 cards + 2 Jokers) from which to sample cards for the caravan deck,
     *        must be between 1 and 3 (inclusive).
     * @param balanced_sample If true, standard decks are sampled in a round-robin
     *        fashion e.g. a random card from deck 1, then deck 2, then 3, then
     *        1, 2, 3, and so on.
     *        If false, then standard decks are sampled randomly.
     * @param shuffle If true, the deck is shuffled.
     *
     * @return A caravan deck.
     *
     * @throws CaravanFatalModelException Requested number of cards outside of the acceptable range.
     * @throws CaravanFatalModelException Requested number of sample decks outside of the acceptable range.
     * @throws CaravanFatalModelException Insufficient cards to sample in order to build a deck.
     */
    Deck *DeckBuilder::build_caravan_deck(
        const uint8_t num_cards,
        const uint8_t num_sample_decks,
        const bool balanced_sample,
        const bool shuffle
    ) {
        Deck sample_decks[3];
        Card c_next;
        uint8_t first_hand_num_cards;

        if (num_cards < DECK_CARAVAN_MIN or
            num_cards > DECK_CARAVAN_MAX
        ) {
            throw CaravanFatalModelException(
                "A caravan deck must have between "
                "30 and 162 cards (inclusive)."
            );
        }

        if (num_sample_decks < SAMPLE_DECKS_MIN or
            num_sample_decks > SAMPLE_DECKS_MAX
        ) {
            throw CaravanFatalModelException(
                "A caravan deck must sample from between "
                "1 and 3 standard card decks (inclusive)."
            );
        }

        uint8_t total_sample_cards = num_sample_decks * DECK_TRADITIONAL_MAX;

        if (total_sample_cards < num_cards) {
            throw CaravanFatalModelException(
                "There are insufficient cards to sample for the "
                "caravan deck."
            );
        }

        const auto d = new Deck();

        do {
            d->clear();
            uint8_t i_next = 0;
            first_hand_num_cards = 0;

            for (int i = 0; i < num_sample_decks; ++i) {
                sample_decks[i] = build_traditional_deck(shuffle);
            }

            if (balanced_sample) {
                // Sample decks in a round-robin fashion
                while (d->size() < num_cards) {
                    c_next = sample_decks[i_next].back();
                    d->push_back(c_next);
                    sample_decks[i_next].pop_back();

                    i_next = (i_next + 1) % num_sample_decks;

                    if (num_cards - d->size() < HAND_SIZE_MAX_START and
                        c_next.is_numeral_card()
                    ) {
                        first_hand_num_cards += 1;
                    }
                }
            } else {
                // Sample decks randomly
                std::mt19937 gen(generate_seed());
                std::uniform_int_distribution<> distr(0, num_sample_decks - 1);

                while (d->size() < num_cards) {
                    i_next = distr(gen);
                    if (!sample_decks[i_next].empty()) {
                        c_next = sample_decks[i_next].back();
                        d->push_back(c_next);
                        sample_decks[i_next].pop_back();

                        if (num_cards - d->size() < HAND_SIZE_MAX_START and
                            c_next.is_numeral_card()
                        ) {
                            first_hand_num_cards += 1;
                        }
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
     * @return A traditional deck: standard 52 cards + 2 JOKERs.
     */
    Deck DeckBuilder::build_traditional_deck(const bool shuffle) {
        Deck d;

        for (int i = CLUBS; i <= SPADES; ++i) {
            for (int j = ACE; j <= KING; ++j) {
                d.push_back(
                    {
                        static_cast<Suit>(i),
                        static_cast<Rank>(j)
                    }
                );
            }
        }

        d.push_back({NO_SUIT, JOKER});
        d.push_back({NO_SUIT, JOKER});

        if (shuffle) {
            return shuffle_deck(d);
        }

        return d;
    }

    /**
     * @param deck The deck to shuffle.
     * @return A deck with shuffled cards.
     */
    Deck DeckBuilder::shuffle_deck(Deck deck) {
        std::mt19937 gen(generate_seed());
        std::ranges::shuffle(deck, gen);
        return deck;
    }
}
