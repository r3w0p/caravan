// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_VIEW_H
#define CARAVAN_VIEW_H

#include <string>
#include "caravan/model/game.h"
#include "caravan/user/user.h"
#include "caravan/core/common.h"

class View {
protected:
    User *user_abc;
    User *user_def;
    Game *game;
    bool closed;
public:
    virtual ~View() = default;
    explicit View(User *user_abc, User *user_def, Game *game) :
            user_abc(user_abc),
            user_def(user_def),
            game(game),
            closed(false) {};

    virtual void run() = 0;
    virtual void close() = 0;
};

#endif //CARAVAN_VIEW_H
