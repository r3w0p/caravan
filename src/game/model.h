// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_MODEL_H
#define CARAVAN_MODEL_H

#include <cstdint>
#include <utility>
#include <vector>
#include <array>
#include <memory>
#include <stack>
#include <exception>

const uint8_t DECK_STANDARD_MAX = 52;
const uint8_t DECK_CARAVAN_MIN = 30;
const uint8_t DECK_CARAVAN_MAX = 156;
const uint8_t NUM_SAMPLE_DECKS_MIN = 1;
const uint8_t NUM_SAMPLE_DECKS_MAX = 3;
const uint8_t NUM_PILES_MAX = 6;
const uint8_t TRACK_NUMERIC_MIN = 1;
const uint8_t TRACK_NUMERIC_MAX = 10;
const uint8_t TRACK_FACE_MAX = 5;
const uint8_t HAND_SIZE_MIN = 0;
const uint8_t HAND_SIZE_MAX = 8;
const uint8_t HAND_POS_MIN = 1;
const uint8_t HAND_POS_MAX = HAND_SIZE_MAX;

enum Suit { NO_SUIT, CLUBS, DIAMONDS, HEARTS, SPADES };
enum Rank { ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, JOKER };
enum Direction { NO_DIRECTION, ASCENDING, DESCENDING };
enum PileName { PILE_A, PILE_B, PILE_C, PILE_D, PILE_E, PILE_F };
enum PlayerName { PLAYER_YOU, PLAYER_OPP };
enum OptionType { OPTION_PLAY, OPTION_DROP, OPTION_CLEAR };

typedef struct Card {
    Suit suit {};
    Rank rank {};
} Card;

typedef std::vector<Card> Deck;

class DeckBuilder {
private:
    static Deck shuffle_deck(Deck d);
    static Deck build_standard_deck(bool shuffle);

public:
    DeckBuilder() = delete;

    static Deck* build_caravan_deck(uint8_t num_cards, uint8_t num_sample_decks, bool balanced_sample);
};

typedef std::array<Card, HAND_SIZE_MAX> Hand;

class Player {
private:
    PlayerName name;
    Deck* deck;
    Hand hand;
    uint8_t i_hand;

public:
    explicit Player(PlayerName pn, Deck* d) {
        name = pn;
        deck = d;
        hand = {};
        i_hand = 0;
    }

    void populate_hand();
    Card take_from_hand_at(uint8_t pos);
    Hand get_hand();
    uint8_t size_deck();
    uint8_t size_hand();
};

typedef std::array<Card, TRACK_FACE_MAX> Faces;

typedef struct TrackSlot {
    Card card {};
    Faces faces {};
    uint8_t i_faces = 0;
} TrackSlot;

typedef std::array<TrackSlot, TRACK_NUMERIC_MAX> Track;

class Pile {
private:
    PileName name;
    Track track;
    uint8_t i_track;

    static bool is_numeric_card(Card c);
    static bool is_face_card(Card c);
    static uint8_t numeric_rank_to_int_value(Rank r);

    void remove_numeric_card(uint8_t i);

public:
    explicit Pile(PileName pn) {
        name = pn;
        track = {};
        i_track = 0;
    }

    uint16_t get_bid();
    TrackSlot get_cards_at(uint8_t pos);
    Direction get_direction();
    PileName get_name();
    Suit get_suit();

    void put_numeric_card(Card c);
    Card put_face_card(Card c, uint8_t pos);

    void remove_all_cards();
    void remove_rank(Rank r, uint8_t exclude);
    void remove_suit(Suit s, uint8_t exclude);

    uint8_t size();
};

class Table {
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

public:
    void clear_pile(PileName pn);
    void play_face_card(PileName pn, Card c, uint8_t pos);
    void play_numeric_card(PileName pn, Card c);

    uint16_t get_pile_bid(PileName pn);
    TrackSlot get_pile_cards_at(PileName pn, uint8_t pos);
    Direction get_pile_direction(PileName pn);
    uint8_t get_pile_size(PileName pn);
    Suit get_pile_suit(PileName pn);
};

typedef struct GameConfig {
    uint8_t you_num_cards;
    uint8_t you_num_sample_decks;
    bool you_balanced_sample;

    uint8_t opp_num_cards;
    uint8_t opp_num_sample_decks;
    bool opp_balanced_sample;
} GameConfig;

typedef struct GameOption {
    OptionType type;
    uint8_t pos_hand;
    PileName pile_name;
    uint8_t pos_pile;
} GameOption;

class Game {
private:
    GameConfig config {};
    bool started;
    bool closed;

    Table* table_ptr {};
    Deck* deck_you_ptr {};
    Deck* deck_opp_ptr {};
    Player* player_you_ptr {};
    Player* player_opp_ptr {};

public:
    explicit Game(GameConfig gc) {
        config = gc;
        started = false;
        closed = false;
    }

    void start();
    void close();
    void option(PlayerName pn, GameOption go);
};

#endif //CARAVAN_MODEL_H
