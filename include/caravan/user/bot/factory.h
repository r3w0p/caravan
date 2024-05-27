// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_USER_BOT_FACTORY_H
#define CARAVAN_USER_BOT_FACTORY_H

#include "caravan/user/user.h"

class BotFactory {
public:
    BotFactory() = delete;
    static UserBot *get(std::string name, PlayerName player_name);
};

#endif //CARAVAN_USER_BOT_FACTORY_H
