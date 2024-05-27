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
    bool closed;
public:
    virtual ~User() = default;

    explicit User(PlayerName pn) : name(pn), closed(false) {};

    PlayerName get_name() { return name; }

    virtual void close() = 0;
    virtual bool is_human() = 0;
    virtual std::string request_move(Game *game) = 0;
};

class UserHuman : public User {
public:
    explicit UserHuman(PlayerName pn) : User(pn) {};

    void close() override { closed = true; }
    bool is_human() override { return true; }
    std::string request_move(Game *game) override { return {}; }
};

class UserBot : public User {
public:
    explicit UserBot(PlayerName pn) : User(pn) {};

    void close() override { closed = true; }
    bool is_human() override { return false; }
    std::string request_move(Game *game) override { return {}; }
};

#endif //CARAVAN_USER_H
