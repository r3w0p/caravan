// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_MODEL_GAME_H
#define CARAVAN_MODEL_GAME_H

#include <list>
#include "caravan/model/table.h"
#include "caravan/model/player.h"

namespace Caravan::Model {

    class GameSubscriber;

    class Game {
    protected:
        std::unique_ptr<Table> table;
        std::unique_ptr<Player> player_abc;
        std::unique_ptr<Player> player_def;
        Player* player_turn;

        std::list<GameSubscriber *> subscribers;

        int8_t compare_bids(CaravanName cvname1, CaravanName cvname2);

        CaravanName winning_bid(CaravanName cvname1, CaravanName cvname2);

        bool has_sold(CaravanName cvname);

        void option_clear(const Player *player, GameMove *move);

        static void option_discard(Player *player, GameMove *move);

        void option_play(Player *player, GameMove *move);

    public:
        explicit Game(const GameConfig &config);

        ~Game() = default;

        static CaravanName get_opposite_caravan_name(CaravanName cvname);

        [[nodiscard]] Player *get_player(PlayerName pname) const;

        [[nodiscard]] PlayerCaravanNames get_player_caravan_names(PlayerName pname) const;

        [[nodiscard]] PlayerName get_player_turn() const;

        [[nodiscard]] Table *get_table() const;

        PlayerName get_winner();

        bool is_caravan_bust(CaravanName cvname);

        bool is_caravan_winning(CaravanName cvname);

        void make_move(GameMove *move);
    };

}

#endif //CARAVAN_MODEL_GAME_H
