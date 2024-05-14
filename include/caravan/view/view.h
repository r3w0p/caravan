// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_VIEW_H
#define CARAVAN_VIEW_H

#include <string>
#include "caravan/model/game.h"
#include "caravan/user/user.h"

class View {
public:
    virtual void update(Game *g, User *ubottom, User *utop, GameOption* go_bottom, GameOption* go_top) = 0;
    virtual GameOption option(Game *g, User *u) = 0;
    virtual void close() = 0;
    virtual void error_message(std::string msg) = 0;
};

#endif //CARAVAN_VIEW_H
