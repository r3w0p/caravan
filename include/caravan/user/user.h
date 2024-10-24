// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_USER_H
#define CARAVAN_USER_H

#include "caravan/core/common.h"
#include "caravan/model/game.h"

class User {
protected:
    PlayerName name;
public:
    explicit User(PlayerName pn) : name(pn) {};

    virtual ~User() = default;
    virtual bool is_human() = 0;
    virtual std::string request_move(Game *game) = 0;

    PlayerName get_name() { return name; }
};

class UserHuman : public User {
public:
    explicit UserHuman(PlayerName pn) : User(pn) {};

    bool is_human() override { return true; }
    std::string request_move(Game *game) override { return {}; }
};

class UserBot : public User {
public:
    explicit UserBot(PlayerName pn) : User(pn) {};

    bool is_human() override { return false; }
};

#endif //CARAVAN_USER_H
