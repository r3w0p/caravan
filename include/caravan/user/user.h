// Copyright (c) 2022-2025 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_USER_H
#define CARAVAN_USER_H

#include "caravan/model/types.h"
#include "caravan/model/game.h"

class User {
protected:
    PlayerName name;
public:
    virtual ~User() = default;

    explicit User(PlayerName pname) : name(pname) {};

    PlayerName get_name() { return name; }

    virtual bool is_human() = 0;
    virtual std::string request_move(Game *game) = 0;
};

class UserFTXUI : public User {
public:
    explicit UserFTXUI(PlayerName pname) : User(pname) {};

    bool is_human() override { return true; }
    std::string request_move(Game *game) override { return {}; }
};

class UserBot : public User {
public:
    explicit UserBot(PlayerName pname) : User(pname) {}

    bool is_human() override { return false; }
};

#endif //CARAVAN_USER_H
