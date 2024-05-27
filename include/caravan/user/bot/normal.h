// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_USER_BOT_NORMAL_H
#define CARAVAN_USER_BOT_NORMAL_H

#include "caravan/user/user.h"

class UserBotNormal : public UserBot {
public:
    explicit UserBotNormal(PlayerName pn) : UserBot(pn){};

    void close() override;
    std::string request_move(Game *game) override;
};

#endif //CARAVAN_USER_BOT_NORMAL_H
