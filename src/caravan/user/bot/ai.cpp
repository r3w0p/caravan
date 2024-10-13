// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/user/bot/ai.h"

UserBotAI::UserBotAI(PlayerName pn) : UserBot(pn) {
    // TODO load trained q_table
}

std::string UserBotAI::request_move(Game *game) {
    if (closed) { throw CaravanFatalException("Bot is closed."); }

    return "D1";  // TODO use q_table
}
