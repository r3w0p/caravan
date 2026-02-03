// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_USER_BOT_NORMAL_H
#define CARAVAN_USER_BOT_NORMAL_H

#include "caravan/user/user.h"

namespace Caravan::User {

    class UserBotNormal : public UserBot {
    protected:
        std::string generate_move(Model::Game *game, bool allow_numeral, bool allow_face, bool allow_clear);
    public:
        explicit UserBotNormal(Model::PlayerName pname) : UserBot(pname){};

        std::string request_move(Model::Game *game) override;
    };

}

#endif //CARAVAN_USER_BOT_NORMAL_H
