// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_CONTROLLER_H
#define CARAVAN_CONTROLLER_H

#include <array>
#include "caravan/core/common.h"
#include "caravan/model/game.h"
#include "caravan/user/user.h"
#include "caravan/view/view.h"


class Controller {
protected:
    Game *game_ptr;
    View *view_ptr;
    User *user_a_ptr;
    User *user_b_ptr;
public:
    explicit Controller(Game *g, View *v, User *ua, User *ub) :
        game_ptr(g), view_ptr(v), user_a_ptr(ua), user_b_ptr(ub) {};

    void run();
};

#endif //CARAVAN_CONTROLLER_H
