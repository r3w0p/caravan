// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <random>
#include "caravan/user/bot/friendly.h"

/*
 * PUBLIC
 */

std::string UserBotFriendly::request_move(Game *game) {
    if (closed) { throw CaravanFatalException("Bot is closed."); }

    std::string move = generate_move(game, true, false, true);

    // Return move if able to generate one
    if (!move.empty()) { return move; }

    // If no useful move could be made, discard first card in hand
    return "D1";
}
