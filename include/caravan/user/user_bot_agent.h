// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_USER_USER_BOT_AGENT_H
#define CARAVAN_USER_USER_BOT_AGENT_H

#include <random>
#include "caravan/user/base_user_bot.h"
#include "caravan/core/functions.h"

namespace Caravan::User {
    class UserBotAgent : public BaseUserBot<std::string> {
        public:
            explicit
            UserBotAgent(Model::PlayerName pname) : BaseUserBot(pname) {
            }

            ~UserBotAgent() override = default;

            std::string request_input(Model::Game &game) override;
    };
}

#endif //CARAVAN_USER_USER_BOT_AGENT_H
