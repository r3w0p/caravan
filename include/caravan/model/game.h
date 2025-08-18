// Copyright (c) 2022-2025 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_MODEL_GAME_H
#define CARAVAN_MODEL_GAME_H

#include <cstdint>
#include "caravan/model/table.h"
#include "caravan/model/player.h"

class Game {
protected:
    std::unique_ptr<Table> table;
    std::unique_ptr<Player> player_abc;
    std::unique_ptr<Player> player_def;
    Player* player_turn;

    int8_t compare_bids(CaravanName cvname1, CaravanName cvname2);

    CaravanName winning_bid(CaravanName cvname1, CaravanName cvname2);

    bool has_sold(CaravanName cvname);

    void option_clear(const Player *pptr, GameCommand *command);

    static void option_discard(Player *pptr, GameCommand *command);

    void option_play(Player *pptr, GameCommand *command);

public:
    explicit Game(const GameConfig *gc);

    static CaravanName get_opposite_caravan_name(CaravanName cvname);

    Player *get_player(PlayerName pname) const;

    PlayerCaravanNames get_player_caravan_names(PlayerName pname) const;

    bool is_caravan_winning(CaravanName cvname);

    bool is_caravan_bust(CaravanName cvname);

    PlayerName get_player_turn() const;

    Table *get_table() const;

    PlayerName get_winner();

    void play_option(GameCommand *command);
};

#endif //CARAVAN_MODEL_GAME_H
