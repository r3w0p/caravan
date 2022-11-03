// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_BOT_SIMPLE_H
#define CARAVAN_BOT_SIMPLE_H

#include "../game/controller.h"


class UserBotSimple : public UserBot {
public:
    explicit UserBotSimple(PlayerName pn) : UserBot(pn) {};

    GameOption request_option(Engine *e, View *v) override;
};

#endif //CARAVAN_BOT_SIMPLE_H
