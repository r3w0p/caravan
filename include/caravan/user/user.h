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

    explicit User(PlayerName pn) : name(pn) {};

    virtual void close() = 0;

    PlayerName get_name() { return name; }
    virtual bool is_human() = 0;
    virtual GameCommand generate_option(Game *g) = 0;
};

class UserHuman : public User {
public:
    explicit UserHuman(PlayerName pn) : User(pn) {};

    bool is_human() override;
    GameCommand generate_option(Game *g) override;
    void close() override;
};

#endif //CARAVAN_USER_H
