// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_MODEL_GAME_H
#define CARAVAN_MODEL_GAME_H

#include <cstdint>
#include "caravan/model/table.h"
#include "caravan/model/player.h"
#include "caravan/core/exceptions.h"

class Game {
protected:
    Table *table_ptr{};
    Player *pa_ptr{};
    Player *pb_ptr{};
    Player *p_turn;
    bool closed;

    int8_t compare_bids(CaravanName cvname1, CaravanName cvname2);

    CaravanName winning_bid(CaravanName cvname1, CaravanName cvname2);

    bool has_sold(CaravanName cvname);

    void option_clear(Player *pptr, GameCommand *command);

    void option_discard(Player *pptr, GameCommand *command);

    void option_play(Player *pptr, GameCommand *command);

public:
    explicit Game(GameConfig *gc);

    static CaravanName get_opposite_caravan_name(CaravanName cvname);

    void close();

    Player *get_player(PlayerName pname);

    PlayerCaravanNames get_player_caravan_names(PlayerName pname);

    bool is_caravan_winning(CaravanName cvname);

    bool is_caravan_bust(CaravanName cvname);

    PlayerName get_player_turn();

    Table *get_table();

    PlayerName get_winner();

    bool is_closed();

    void play_option(GameCommand *command);
};

#endif //CARAVAN_MODEL_GAME_H
