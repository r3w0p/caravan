// Copyright (c) 2022-2023 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_DECK_H
#define CARAVAN_DECK_H

#include "common.h"

class DeckBuilder {
protected:
    static Deck build_standard_deck(bool shuffle);

    static Deck shuffle_deck(Deck d);

public:
    DeckBuilder() = delete;

    static Deck *build_caravan_deck(
            uint8_t num_cards,
            uint8_t num_sample_decks,
            bool balanced_sample);
};

#endif //CARAVAN_DECK_H
