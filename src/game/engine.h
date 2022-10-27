// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_ENGINE_H
#define CARAVAN_ENGINE_H

#include <cstdint>
#include "table.h"
#include "player.h"
#include "exceptions.h"

class Engine {
protected:
    Table *table_ptr{};
    Deck *deck_pa_ptr{};
    Deck *deck_pb_ptr{};
    Player *pa_ptr{};
    Player *pb_ptr{};
    bool closed;
    Player *p_turn;

    int8_t compare_bids(CaravanName cn1, CaravanName cn2);

    bool has_sold(CaravanName cn);

    void option_clear(Player *p_ptr, GameOption go);

    void option_drop(Player *p_ptr, GameOption go);

    void option_play(Player *p_ptr, GameOption go);

public:
    explicit Engine(GameConfig gc, PlayerName pna, PlayerName pnb) {
        if (pna == NO_PLAYER or pnb == NO_PLAYER)
            throw CaravanFatalException(
                    "Players must have valid names.");

        if (pna == pnb)
            throw CaravanFatalException(
                    "Players cannot have the same name.");

        if (gc.pn_first != pna and gc.pn_first != pnb)
            throw CaravanFatalException(
                    "The name of the player who goes first does not "
                    "match either of the names chosen for players.");

        deck_pa_ptr = DeckBuilder::build_caravan_deck(
                gc.pa_num_cards,
                gc.pa_num_sample_decks,
                gc.pa_balanced_sample);

        deck_pb_ptr = DeckBuilder::build_caravan_deck(
                gc.pb_num_cards,
                gc.pb_num_sample_decks,
                gc.pb_balanced_sample);

        table_ptr = new Table();
        pa_ptr = new Player(pna, deck_pa_ptr);
        pb_ptr = new Player(pnb, deck_pb_ptr);

        closed = false;
        p_turn = gc.pn_first == pa_ptr->get_name() ? pa_ptr : pb_ptr;
    }

    void close();

    Player *get_player(PlayerName pn);

    PlayerCaravanNames get_player_caravan_names(PlayerName pn);

    PlayerName get_player_turn();

    Table *get_table();

    PlayerName get_winner();

    bool is_closed();

    void play_option(GameOption go);
};

#endif //CARAVAN_ENGINE_H
