// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_BOT_RANDOM_H
#define CARAVAN_BOT_RANDOM_H

#include "../game/controller.h"


class UserBotRandom : public UserBot {
public:
    explicit UserBotRandom(PlayerName pn) : UserBot(pn) {};

    GameOption request_option(Engine *e) override;
};

#endif //CARAVAN_BOT_RANDOM_H
