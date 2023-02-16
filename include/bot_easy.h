// Copyright (c) 2022-2023 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_BOT_EASY_H
#define CARAVAN_BOT_EASY_H


#include "user.h"

class UserBotEasy : public UserBot {
public:
    explicit UserBotEasy(PlayerName pn) : UserBot(pn) {};

    GameOption generate_option(Engine *e) override;
};

#endif //CARAVAN_BOT_EASY_H
