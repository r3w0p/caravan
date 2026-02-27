// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_GYM_ENVIRONMENT_GAME_H
#define CARAVAN_GYM_ENVIRONMENT_GAME_H

#include <set>

#include "base_environment.h"
#include "types.h"
#include "caravan/model/game.h"
#include "caravan/user/base_user_bot.h"

namespace Caravan::User {
    class UserBotRandom;
    class UserBotAgent;
}

namespace Caravan::Gym {

    class EnvironmentGame : public BaseEnvironment<
            Observation,
            Action,
            Reward,
            Info
        > {
        protected:
            bool reset_never_called{true};
            bool environment_is_done{false};

            User::BaseUser<std::string> &user_random;

            Model::PlayerName pname_agent;
            Model::PlayerName pname_random;

            Model::PlayerCaravanNames cvnames_agent;
            Model::PlayerCaravanNames cvnames_random;
            std::array<Model::CaravanName, Model::TABLE_CARAVANS_MAX> cvnames_all_ordered;

            Model::PlayerName pname_first;
            std::unique_ptr<Model::Game> game;

            static uint8_t card_to_uint8_t(Model::Card card);

            static Model::Card uint8_t_to_card(uint8_t card_num);

            Observation make_observation();

            std::string game_move_to_action_key(Model::GameMove move);
            std::vector<ActionMove> get_valid_moves();

        public:
            /**
             * @param user_random User that makes random moves only.
             */
            explicit EnvironmentGame(
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
