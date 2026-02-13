// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_USER_USER_HUMAN_FTXUI_H
#define CARAVAN_USER_USER_HUMAN_FTXUI_H

#include "caravan/model/types.h"
#include "caravan/model/game.h"
#include "caravan/user/base_user_human.h"

namespace Caravan::User {
    class UserHumanFTXUI : public BaseUserHuman<std::string> {
        public:
            explicit UserHumanFTXUI(Model::PlayerName pname) : BaseUserHuman(
                pname
            ) {
            }

            std::string request_input(Model::Game *game) override {
                return {}; // not used due to how FTXUI works
            }
    };
}

#endif //CARAVAN_USER_USER_HUMAN_FTXUI_H
