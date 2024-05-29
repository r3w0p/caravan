// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <algorithm>
#include <cctype>
#include <string>
#include "caravan/user/user.h"
#include "caravan/user/bot/factory.h"
#include "caravan/user/bot/normal.h"
#include "caravan/user/bot/friendly.h"

const std::string NAME_NORMAL = "normal";
const std::string NAME_FRIENDLY = "friendly";

UserBot* BotFactory::get(std::string name, PlayerName player_name) {
    // Set name to lowercase
    std::transform(
        name.begin(), name.end(), name.begin(),
        [](unsigned char c) { return std::tolower(c); });

    // Return bot that matches name, or fail
    if(name == NAME_NORMAL) { return new UserBotNormal(player_name); }
    if(name == NAME_FRIENDLY) { return new UserBotFriendly(player_name); }
    else {
        throw CaravanFatalException("Unknown bot name '" + name + "'.");
    }
}