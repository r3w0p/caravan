// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_MODEL_H
#define CARAVAN_MODEL_H

#include <cstdint>
#include <utility>
#include <vector>
#include <array>

const uint8_t DECK_STANDARD_MAX = 52;
const uint8_t DECK_CARAVAN_MIN = 30;
const uint8_t DECK_CARAVAN_MAX = 156;
const uint8_t NUM_SAMPLE_DECKS_MIN = 1;
const uint8_t NUM_SAMPLE_DECKS_MAX = 3;
const uint8_t NUM_PILES_MAX = 6;
const uint8_t TRACK_NUMERIC_MAX = 10;
const uint8_t TRACK_FACE_MAX = 5;

enum Suit { NO_SUIT, CLUBS, DIAMONDS, HEARTS, SPADES };
enum Rank { ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, JOKER };
enum Direction { NO_DIRECTION, DESCENDING, ASCENDING };
enum PileName { PILE_A, PILE_B, PILE_C, PILE_D, PILE_E, PILE_F };

typedef struct Card {
    Suit suit {};
    Rank rank {};
} Card;

typedef std::vector<Card> Deck;

class DeckBuilder {
public:
    DeckBuilder() = delete;
    static Deck build_standard_deck(bool shuffle);
    static Deck build_caravan_deck(uint8_t num_cards, uint8_t num_sample_decks, bool balanced_sample);
private:
    static Deck shuffle_deck(Deck d);
};

typedef std::array<Card, TRACK_FACE_MAX> SlotFace;

typedef struct SlotNumeric {
    Card card {};
    SlotFace faces {};
    uint8_t i_faces = 0;
} SlotNumeric;

typedef std::array<SlotNumeric, TRACK_NUMERIC_MAX> Track;

class Pile {
public:
public:
    explicit Pile(PileName pn) : name(pn) {}
    uint16_t get_bid();
    Suit get_suit();
    Direction get_direction();
    void put_numeric_card(Card c);
    Card put_face_card(Card c, uint8_t i);
    void remove_suit(Suit s, int8_t exclude);
    void remove_rank(Rank r, int8_t exclude);
    void remove_all_cards();
    PileName get_name();
private:
    PileName name;
    Track track {};
    uint8_t i_track = 0;
    static uint8_t numeric_rank_to_int_value(Rank r);
    static bool is_numeric_card(Card c);
    static bool is_face_card(Card c);
    void remove_numeric_card(uint8_t i);
};

class Table {
public:
    void play_numeric_card(PileName pn, Card c);
    void play_face_card(PileName pn, Card c, uint8_t i);
    void clear_pile(PileName pn);
    uint16_t get_pile_bid(PileName pn);
    Suit get_pile_suit(PileName pn);
    Direction get_pile_direction(PileName pn);
private:
    std::array<Pile, NUM_PILES_MAX> piles = {
        Pile(PILE_A),
        Pile(PILE_B),
        Pile(PILE_C),
        Pile(PILE_D),
        Pile(PILE_E),
        Pile(PILE_F)
    };
    uint8_t pile_name_to_index_value(PileName pn);
};

#endif //CARAVAN_MODEL_H
