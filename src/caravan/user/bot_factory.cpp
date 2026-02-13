// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <algorithm>
#include <cctype>
#include <string>
#include "caravan/core/exceptions.h"
#include "caravan/user/bot_factory.h"
#include "caravan/user/user_bot_random.h"

namespace Caravan::User {
    const std::string NAME_RANDOM = "random";

    BaseUserBot<std::string> *BotFactory::get(
        std::string name,
        Model::PlayerName pname
    ) {
        // Set name to lowercase
        std::ranges::transform(
            name,
            name.begin(),
            [](unsigned char c) {
                return std::tolower(c);
            }
        );

        // Return bot that matches name, or fail
        if (name == NAME_RANDOM) {
            return new UserBotRandom(pname);
        }

        throw CaravanFatalException("Unknown bot name '" + name + "'.");
    }
}
