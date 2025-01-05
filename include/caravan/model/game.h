// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_MODEL_GAME_H
#define CARAVAN_MODEL_GAME_H

#include <cstdint>
#include <memory>

#include "caravan/model/table.h"
#include "caravan/model/player.h"
#include "caravan/core/exceptions.h"

class Game {
protected:
    std::unique_ptr<Table> table;
    std::unique_ptr<Player> player_a;
    std::unique_ptr<Player> player_b;
    Player *player_turn;

    int8_t compare_bids(CaravanName cvname1, CaravanName cvname2);

    CaravanName winning_bid(CaravanName cvname1, CaravanName cvname2);

    bool has_sold(CaravanName cvname);

    bool option_clear(GameCommand *command, bool check_only);

    bool option_discard(GameCommand *command, bool check_only);

    bool option_play(GameCommand *command, bool check_only);

public:
    /**
     * @param config Game configuration.
     *
     * @throws CaravanFatalException Invalid first player in game configuration.
     */
    explicit Game(GameConfig *gc);

    static CaravanName get_opposite_caravan_name(CaravanName cvname);

    Player *get_player(PlayerName pname);

    PlayerCaravanNames get_player_caravan_names(PlayerName pname);

    Player *get_player_turn();

    Table *get_table();

    PlayerName get_winner_name();

    bool is_caravan_bust(CaravanName cvname);

    bool is_caravan_winning(CaravanName cvname);

    void play_option(GameCommand *command);

    bool check_option(GameCommand *command);
};

#endif //CARAVAN_MODEL_GAME_H
