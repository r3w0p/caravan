// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_CONTROLLER_H
#define CARAVAN_CONTROLLER_H

#include <array>
#include "view.h"
#include "common.h"
#include "engine.h"


class User {
protected:
    PlayerName name;
public:
    explicit User(PlayerName pn) : name(pn) {};

    PlayerName get_name() { return name; }

    virtual GameOption request_option(Engine *e, View *v) = 0;
    virtual bool is_human() = 0;
};

class UserHuman : public User {
public:
    explicit UserHuman(PlayerName pn) : User(pn) {};
    GameOption request_option(Engine *e, View *v) override;
    bool is_human() override { return true; }
};

class UserBot : public User {
public:
    explicit UserBot(PlayerName pn) : User(pn) {};
    bool is_human() override { return false; }
};

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
