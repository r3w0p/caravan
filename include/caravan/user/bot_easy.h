// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_USER_BOT_EASY_H
#define CARAVAN_USER_BOT_EASY_H


#include "caravan/user/user.h"

class UserBotEasy : public UserBot {
public:
    explicit UserBotEasy(PlayerName pn) : UserBot(pn) {};

    GameOption generate_option(Game *g) override;
};

#endif //CARAVAN_USER_BOT_EASY_H
