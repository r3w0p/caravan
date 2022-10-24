// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_ENGINE_H
#define CARAVAN_ENGINE_H

#include <cstdint>
#include "table.h"
#include "player.h"

class Engine {
private:
    Table *table_ptr{};
    Deck *deck_p1_ptr{};
    Deck *deck_p2_ptr{};
    Player *p1_ptr{};
    Player *p2_ptr{};
    bool closed;
    PlayerName p_turn;

    void option_play(Player *p_ptr, GameOption go);

    void option_drop(Player *p_ptr, GameOption go);

    void option_clear(Player *p_ptr, GameOption go);

    bool has_sold(CaravanName cn);

    int8_t compare_bids(CaravanName cn1, CaravanName cn2);

public:
    explicit Engine(GameConfig gc) {
        deck_p1_ptr = DeckBuilder::build_caravan_deck(
                gc.p1_num_cards,
                gc.p1_num_sample_decks,
                gc.p1_balanced_sample);

        deck_p2_ptr = DeckBuilder::build_caravan_deck(
                gc.p2_num_cards,
                gc.p2_num_sample_decks,
                gc.p2_balanced_sample);

        table_ptr = new Table();
        p1_ptr = new Player(PLAYER_1, deck_p1_ptr);
        p2_ptr = new Player(PLAYER_2, deck_p2_ptr);

        closed = false;
        p_turn = gc.p_first != NO_PLAYER ? gc.p_first : PLAYER_1;
    }

    void close();

    void play_option(GameOption go);

    PlayerName get_winner();

    PlayerName get_player_turn();

    Table *get_table();

    Player *get_player(PlayerName pn);
};

#endif //CARAVAN_ENGINE_H
