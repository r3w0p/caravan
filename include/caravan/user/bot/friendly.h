// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_USER_BOT_FRIENDLY_H
#define CARAVAN_USER_BOT_FRIENDLY_H

#include "caravan/user/bot/normal.h"

class UserBotFriendly : public UserBotNormal {
public:
    explicit UserBotFriendly(PlayerName pn) : UserBotNormal(pn){};

    std::string request_move(Game *game) override;
};

#endif //CARAVAN_USER_BOT_FRIENDLY_H
