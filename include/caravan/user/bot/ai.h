// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_USER_BOT_AI_H
#define CARAVAN_USER_BOT_AI_H

#include "caravan/user/user.h"

class UserBotAI : public UserBot {
protected:
    int policy;  // TODO typedef array

public:
    explicit UserBotAI(PlayerName pn);

    std::string request_move(Game *game) override;

    void get_policy(); // TODO return policy
};

#endif //CARAVAN_USER_BOT_AI_H
