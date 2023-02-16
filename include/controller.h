// Copyright (c) 2022-2023 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_CONTROLLER_H
#define CARAVAN_CONTROLLER_H

#include <array>
#include "common.h"
#include "engine.h"
#include "view.h"
#include "user.h"


class Controller {
protected:
    Engine *engine_ptr;
    View *view_ptr;
    User *user_a_ptr;
    User *user_b_ptr;
public:
    explicit Controller(Engine *e, View *v, User *ua, User *ub) :
            engine_ptr(e), view_ptr(v), user_a_ptr(ua), user_b_ptr(ub) {};

    void run();
};

#endif //CARAVAN_CONTROLLER_H
