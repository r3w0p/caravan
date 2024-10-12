// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_USER_BOT_AI_TRAIN_H
#define CARAVAN_USER_BOT_AI_TRAIN_H

#include "caravan/user/bot/ai.h"
#include "caravan/core/training.h"

class UserBotAITrain : public UserBotAI {
public:
    explicit UserBotAITrain(PlayerName pn);

    std::string make_move_train(Game *game, TrainConfig *tc);
};

#endif //CARAVAN_USER_BOT_AI_TRAIN_H
