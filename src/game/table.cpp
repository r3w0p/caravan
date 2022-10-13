// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <chrono>
#include <random>
#include <stdexcept>
#include <functional>
#include <iostream>
#include "table.h"

Deck DeckBuilder::build_standard_deck(bool shuffle) {

    Deck d;

    for(int i = CLUBS; i <= SPADES; ++i) {
        for(int j = ACE; j <= KING; ++j) {
            Card c;
            c.suit = static_cast<Suit>(i);
            c.rank = static_cast<Rank>(j);

            d.push_back(c);
        }
    }

    Card j1;
    j1.suit = NONE;
    j1.rank = JOKER;

    Card j2;
    j2.suit = NONE;
    j2.rank = JOKER;

    d.push_back(j1);
    d.push_back(j2);

    if(shuffle) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(d.begin(), d.end(), std::default_random_engine(seed));
    }

    return d;

}

Deck DeckBuilder::build_caravan_deck(uint8_t num_cards, uint8_t num_sample_decks, bool balanced_sample) {
    if(num_cards < 30 or num_cards > 156)
        throw std::out_of_range("Can only build deck with between 30 and 156 cards (inclusive).");

    if(num_sample_decks < 1 or num_sample_decks > 3)
        throw std::out_of_range("Can only sample between 1 and 3 standard decks (inclusive).");

    uint8_t total_sample_cards = num_sample_decks * 52;

    if(total_sample_cards < num_cards)
        throw std::out_of_range("Not enough sample cards to cover required number of cards.");

    Deck sample_decks[num_sample_decks];

    for(int i = 0; i < num_sample_decks; ++i) {
        sample_decks[i] = DeckBuilder::build_standard_deck(true);
        std::cout << "total: " << sample_decks[i].size() << std::endl;
    }

    Deck d;

    // balanced_sample = true
    uint8_t next = 0;
    while(d.size() < num_cards) {
        Deck dnext = sample_decks[next];

        d.push_back(dnext.back());
        dnext.pop_back();

        next = (next + 1) % num_sample_decks;
    }

    // TODO balanced_sample = false, random selection...

    return d;
}
