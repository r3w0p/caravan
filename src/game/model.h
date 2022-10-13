// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_MODEL_H
#define CARAVAN_MODEL_H

#include <cstdint>
#include <utility>
#include <vector>

const uint8_t DECK_STANDARD_MAX = 52;
const uint8_t DECK_CARAVAN_MIN = 30;
const uint8_t DECK_CARAVAN_MAX = 156;
const uint8_t NUM_SAMPLE_DECKS_MIN = 1;
const uint8_t NUM_SAMPLE_DECKS_MAX = 3;

const uint8_t TRACK_NUMERIC_MAX = 10;
const uint8_t TRACK_FACE_MAX = 5;

enum Suit { NONE, CLUBS, DIAMONDS, HEARTS, SPADES };
enum Rank { ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, JOKER };
enum Direction { DESCENDING, ASCENDING, BIDIRECTIONAL };

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

typedef std::array<Card, TRACK_FACE_MAX> SlotFace;

typedef struct SlotNumeric {
    Card card;
    SlotFace faces;
    uint8_t i_faces;
} SlotNumeric;

typedef std::array<SlotNumeric, TRACK_NUMERIC_MAX> Track;

class Pile {
public:
    uint16_t get_bid();
    Suit get_suit();
    Direction get_direction();
    void play_numeric_card(Card c);
    void clear();
    uint8_t size();
private:
    Track track {};
    uint8_t i_track = 0;
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

#endif //CARAVAN_MODEL_H
