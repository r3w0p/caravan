// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_USER_BOT_AI_H
#define CARAVAN_USER_BOT_AI_H

#include "caravan/user/user.h"

class UserBotAI : public UserBot {
protected:
    bool train;
public:
    explicit UserBotAI(PlayerName pn, bool train);

    std::string request_move(Game *game) override;
    std::string request_move_train(Game *game, TrainConfig *tc);
};

#endif //CARAVAN_USER_BOT_AI_H
