// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <chrono>
#include <random>
#include "deck.h"
#include "exceptions.h"


Deck DeckBuilder::shuffle_deck(Deck d) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(d.begin(), d.end(), std::default_random_engine(seed));
    return d;
}

Deck DeckBuilder::build_standard_deck(bool shuffle) {
    Deck d;

    for (int i = CLUBS; i <= SPADES; ++i)
        for (int j = ACE; j <= KING; ++j)
            d.push_back({static_cast<Suit>(i), static_cast<Rank>(j)});

    d.push_back({NO_SUIT, JOKER});
    d.push_back({NO_SUIT, JOKER});

    if (shuffle)
        return shuffle_deck(d);
    else
        return d;
}

Deck *DeckBuilder::build_caravan_deck(uint8_t num_cards, uint8_t num_sample_decks, bool balanced_sample) {
    uint8_t total_sample_cards;
    Deck sample_decks[num_sample_decks];
    Deck *d;
    uint8_t i_next;
    Card c_next;
    uint8_t first_hand_num_cards;

    if (num_cards < DECK_CARAVAN_MIN or num_cards > DECK_CARAVAN_MAX)
        throw CaravanFatalException("A Caravan deck can only build deck with between 30 and 156 cards (inclusive).");

    if (num_sample_decks < SAMPLE_DECKS_MIN or num_sample_decks > SAMPLE_DECKS_MAX)
        throw CaravanFatalException("A Caravan deck can only sample between 1 and 3 standard decks (inclusive).");

    total_sample_cards = num_sample_decks * DECK_STANDARD_MAX;

    if (total_sample_cards < num_cards)
        throw CaravanFatalException("There are not enough sample cards to cover the required number of cards.");

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

                if ((num_cards - d->size()) < HAND_SIZE_MAX and is_numeric_card(c_next))
                    first_hand_num_cards += 1;
            }

        } else {
            // Sample decks randomly
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distr(0, num_sample_decks - 1);

            while (d->size() < num_cards) {
                i_next = distr(gen);
                if (!sample_decks[i_next].empty()) {
                    c_next = sample_decks[i_next].back();
                    d->push_back(c_next);
                    sample_decks[i_next].pop_back();

                    if ((num_cards - d->size()) < HAND_SIZE_MAX and is_numeric_card(c_next))
                        first_hand_num_cards += 1;
                }
            }
        }

    } while (first_hand_num_cards < MOVES_START_ROUND);

    return d;
}