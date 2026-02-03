// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_USER_BOT_FRIENDLY_H
#define CARAVAN_USER_BOT_FRIENDLY_H

#include "caravan/user/bot/normal.h"

namespace Caravan::User {

    class UserBotFriendly : public UserBotNormal {
    public:
        explicit UserBotFriendly(const Model::PlayerName pname) : UserBotNormal(pname){};

        std::string request_move(Model::Game *game) override;
    };

}

#endif //CARAVAN_USER_BOT_FRIENDLY_H
