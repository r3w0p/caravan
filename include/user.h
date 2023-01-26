// Copyright (c) 2022-2023 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_USER_H
#define CARAVAN_USER_H

#include "common.h"
#include "engine.h"

class User {
protected:
    PlayerName name;
public:
    explicit User(PlayerName pn) : name(pn) {};

    PlayerName get_name() { return name; }

    virtual bool is_human() = 0;
};

class UserHuman : public User {
public:
    explicit UserHuman(PlayerName pn) : User(pn) {};

    bool is_human() override;
};

class UserBot : public User {
public:
    explicit UserBot(PlayerName pn) : User(pn) {};

    bool is_human() override;
    virtual GameOption generate_option(Engine *e) = 0;
};

#endif //CARAVAN_USER_H
