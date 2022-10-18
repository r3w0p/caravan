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
const uint8_t NUM_CARAVANS_MAX = 6;
const uint8_t TRACK_NUMERIC_MIN = 1;
const uint8_t TRACK_NUMERIC_MAX = 10;
const uint8_t TRACK_FACE_MAX = 5;
const uint8_t HAND_SIZE_MIN = 0;
const uint8_t HAND_SIZE_MAX = 8;
const uint8_t HAND_POS_MIN = 1;
const uint8_t HAND_POS_MAX = HAND_SIZE_MAX;
const uint8_t NUM_MOVES_START_ROUND = 3;

enum Suit { NO_SUIT, CLUBS, DIAMONDS, HEARTS, SPADES };
enum Rank { ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, JOKER };
enum Direction { NO_DIRECTION, ASCENDING, DESCENDING };
enum CaravanName { CARAVAN_A, CARAVAN_B, CARAVAN_C, CARAVAN_D, CARAVAN_E, CARAVAN_F };
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

static bool is_numeric_card(Card c);
static bool is_face_card(Card c);

class Player {
private:
    PlayerName name;
    Deck* deck;
    Hand hand;
    uint8_t i_hand;
    uint16_t moves;

public:
    explicit Player(PlayerName pn, Deck* d) {
        name = pn;
        deck = d;
        hand = {};
        i_hand = 0;
        moves = 0;

        Card next;
        while(i_hand < HAND_SIZE_MAX and !deck->empty()) {
            next = deck->back();
            hand[i_hand] = next;

            deck->pop_back();
            i_hand += 1;
        }
    }

    Card remove_from_hand_at(uint8_t pos);
    Card get_from_hand_at(uint8_t pos);
    Hand get_hand();
    PlayerName get_name();
    uint8_t size_deck();
    uint8_t size_hand();
    uint16_t get_moves_count();
    void increment_moves_count();
};

typedef std::array<Card, TRACK_FACE_MAX> Faces;

typedef struct TrackSlot {
    Card card {};
    Faces faces {};
    uint8_t i_faces = 0;
} TrackSlot;

typedef std::array<TrackSlot, TRACK_NUMERIC_MAX> Track;

class Caravan {
private:
    CaravanName name;
    Track track;
    uint8_t i_track;

    static uint8_t numeric_rank_to_int_value(Rank r);

    void remove_numeric_card(uint8_t i);

public:
    explicit Caravan(CaravanName pn) {
        name = pn;
        track = {};
        i_track = 0;
    }

    void clear();

    uint16_t get_bid();
    TrackSlot get_cards_at(uint8_t pos);
    Direction get_direction();
    CaravanName get_name();
    Suit get_suit();

    void put_numeric_card(Card c);
    Card put_face_card(Card c, uint8_t pos);

    void remove_rank(Rank r, uint8_t exclude);
    void remove_suit(Suit s, uint8_t exclude);

    uint8_t size();
};

class Table {
private:
    std::array<Caravan, NUM_CARAVANS_MAX> caravans = {
            Caravan(CARAVAN_A),
            Caravan(CARAVAN_B),
            Caravan(CARAVAN_C),
            Caravan(CARAVAN_D),
            Caravan(CARAVAN_E),
            Caravan(CARAVAN_F)
    };
    uint8_t caravan_name_to_index_value(CaravanName cn);

public:
    void clear_caravan(CaravanName cn);

    void play_face_card(CaravanName cn, Card c, uint8_t pos);
    void play_numeric_card(CaravanName cn, Card c);

    uint16_t get_caravan_bid(CaravanName cn);
    TrackSlot get_caravan_cards_at(CaravanName cn, uint8_t pos);
    Direction get_caravan_direction(CaravanName cn);
    uint8_t get_caravan_size(CaravanName cn);
    Suit get_caravan_suit(CaravanName cn);
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
    CaravanName caravan_name;
    uint8_t pos_caravan;
} GameOption;

class Game {
private:
    Table* table_ptr {};
    Deck* deck_you_ptr {};
    Deck* deck_opp_ptr {};
    Player* player_you_ptr {};
    Player* player_opp_ptr {};
    bool closed;
    bool ended;

    void option_play(Player* p_ptr, GameOption go);
    void option_drop(Player* p_ptr, GameOption go);
    void option_clear(Player* p_ptr, GameOption go);
    void check_ended();

public:
    explicit Game(GameConfig gc) {
        deck_you_ptr = DeckBuilder::build_caravan_deck(
                gc.you_num_cards,
                gc.you_num_sample_decks,
                gc.you_balanced_sample);

        deck_opp_ptr = DeckBuilder::build_caravan_deck(
                gc.opp_num_cards,
                gc.opp_num_sample_decks,
                gc.opp_balanced_sample);

        table_ptr = new Table();
        player_you_ptr = new Player(PLAYER_YOU, deck_you_ptr);
        player_opp_ptr = new Player(PLAYER_OPP, deck_opp_ptr);

        closed = false;
        ended = false;
    }

    void close();
    bool has_ended();
    void option(PlayerName pn, GameOption go);
};

#endif //CARAVAN_MODEL_H
