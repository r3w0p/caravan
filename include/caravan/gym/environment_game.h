// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_GYM_ENVIRONMENT_GAME_H
#define CARAVAN_GYM_ENVIRONMENT_GAME_H

#include "base_environment.h"
#include "types.h"
#include "caravan/model/game.h"
#include "caravan/user/base_user_bot.h"
#include <set>

namespace Caravan::User {
    class UserBotRandom;
    class UserBotAgent;
}

namespace Caravan::Gym {
    // Observation:
    // Hand = 8
    // Caravans = 6
    // Slots per caravan = 8
    // Data per slot = 5 (numeral rank + # queens + # kings + # jokers + last queen)
    // 5 x 8 = 40 + cvn dir + cvn suit = 42
    // 42 x 6 = 252 + hand = 260
    constexpr uint16_t SIZE_OBSERVATION = 260;

    class EnvironmentGame : public BaseEnvironment<
            Observation,
            Action,
            Reward,
            Info
        > {
        protected:
            bool reset_never_called{true};
            bool environment_is_done{false};

            User::BaseUser<std::string> &user_agent;
            User::BaseUser<std::string> &user_random;

            Model::PlayerName first{};
            std::unique_ptr<Model::Game> game{};

            static uint8_t card_to_uint8_t(Model::Card card);

            static Model::Card uint8_t_to_card(uint8_t card_num);

            std::array<uint8_t, SIZE_OBSERVATION> make_observation();

        public:
            /**
             * @param user_agent User that is learning how to play.
             *        The environment operates from the perspective of
             *        this learning agent.
             * @param user_random User that makes random moves only.
             */
            explicit EnvironmentGame(
                User::BaseUser<std::string> &user_agent,
                User::BaseUser<std::string> &user_random
            );

            ~EnvironmentGame() override = default;

            std::tuple<Observation, Info> reset() override = 0;

            std::tuple<Action, Info> sample() override = 0;

            std::tuple<Observation, Reward, bool, Info> step(
                Action &action
            ) override = 0;
    };
}

#endif //CARAVAN_GYM_ENVIRONMENT_GAME_H
