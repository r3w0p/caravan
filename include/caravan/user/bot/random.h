// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_USER_BOT_RANDOM_H
#define CARAVAN_USER_BOT_RANDOM_H

#include <random>
#include "caravan/user/user.h"

constexpr uint16_t ALL_MOVES_MAX = 272;

using AllMoves = std::array<std::string, ALL_MOVES_MAX>;
using CacheMoves = std::array<uint16_t, ALL_MOVES_MAX>;


class UserBotRandom : public UserBot {
protected:
    AllMoves all_moves{};
    CacheMoves cache_moves{};
    uint16_t i_cache{0};
    uint16_t last_move_count{0};
    std::mt19937 gen;
    std::uniform_int_distribution<> distr;

    uint16_t check_index(uint16_t &index);
public:
    explicit UserBotRandom(PlayerName pname);

    std::string request_move(Game *game) override;

    void populate_moves(PlayerCaravanNames pcvnames);
};

#endif //CARAVAN_USER_BOT_RANDOM_H
