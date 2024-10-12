// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/user/bot/ai_train.h"

UserBotAITrain::UserBotAITrain(PlayerName pn) : UserBotAI(pn) {
    // TODO blank policy
}

std::string UserBotAITrain::make_move_train(Game *game, TrainConfig *tc) {
    if (closed) { throw CaravanFatalException("Bot is closed."); }

    return "D1";  // TODO
}
