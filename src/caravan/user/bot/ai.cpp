// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/user/bot/ai.h"

UserBotAI::UserBotAI(PlayerName pn, bool train) : UserBot(pn), train(train) {
    // TODO setup for train vs not train
}

std::string UserBotAI::request_move(Game *game) {
    if (closed) { throw CaravanFatalException("Bot is closed."); }
    if (train) { throw CaravanFatalException("Bot is in training mode."); }

    return "D1";  // TODO
}

std::string UserBotAI::request_move_train(Game *game, TrainConfig *tc) {
    if (closed) { throw CaravanFatalException("Bot is closed."); }
    if (!train) { throw CaravanFatalException("Bot is not in training mode."); }

    return "D1";  // TODO
}
