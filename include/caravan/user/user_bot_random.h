// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_USER_USER_BOT_RANDOM_H
#define CARAVAN_USER_USER_BOT_RANDOM_H

#include <random>
#include "caravan/user/base_user_bot.h"
#include "caravan/core/functions.h"

namespace Caravan::User {
    constexpr uint16_t ALL_MOVES_MAX = 272;

    using AllMoves = std::array<std::string, ALL_MOVES_MAX>;
    using CacheMoves = std::array<uint16_t, ALL_MOVES_MAX>;

    const std::string LETTER_CLEAR = "C";
    const std::string LETTER_DISCARD = "D";
    const std::string LETTER_PLAY = "P";

    class UserBotRandom : public BaseUserBot<std::string> {
        protected:
            AllMoves all_moves{};
            CacheMoves cache_moves{};
            uint16_t i_cache{0};
            uint16_t last_move_count{0};
            std::mt19937 gen;
            std::uniform_int_distribution<> distr;

            [[nodiscard]] uint16_t check_index(const uint16_t &index) const;

            void populate_moves(Model::PlayerCaravanNames pcvnames);

        public:
            explicit
            UserBotRandom(Model::PlayerName pname) : BaseUserBot(pname),
                gen(generate_seed()),
                distr(0, ALL_MOVES_MAX - 1) {
            }

            ~UserBotRandom() override = default;

            std::string request_input(Model::Game &game) override;
    };
}

#endif //CARAVAN_USER_USER_BOT_RANDOM_H
