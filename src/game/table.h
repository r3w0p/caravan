// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_TABLE_H
#define CARAVAN_TABLE_H

#include <cstdint>
#include <vector>

enum Suit { NONE, CLUBS, DIAMONDS, HEARTS, SPADES };
enum Rank { ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, JOKER };

typedef struct Card {
    Suit suit;
    Rank rank;
} Card;

typedef std::vector<Card> Deck;

class DeckBuilder {
public:
    DeckBuilder() = delete;
    static Deck build_standard_deck(bool shuffle);
    static Deck build_caravan_deck(uint8_t num_cards, uint8_t num_sample_decks, bool balanced_sample);
};

class Pile {
public:
    Pile();
};

typedef struct Caravan {
    Pile you;
    Pile opp;
} Caravan;

typedef struct Table {
    Caravan ab;
    Caravan cd;
    Caravan ef;
} Table;

#endif //CARAVAN_TABLE_H
