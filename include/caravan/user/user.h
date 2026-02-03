// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_USER_H
#define CARAVAN_USER_H

#include "caravan/model/types.h"
#include "caravan/model/game.h"

namespace Caravan::User {

    class User {
    protected:
        Model::PlayerName name;
    public:
        virtual ~User() = default;

        explicit User(Model::PlayerName pname) : name(pname) {};

        Model::PlayerName get_name() { return name; }

        virtual bool is_human() = 0;
        virtual std::string request_move(Model::Game *game) = 0;
    };

    class UserFTXUI : public User {
    public:
        explicit UserFTXUI(Model::PlayerName pname) : User(pname) {};

        bool is_human() override { return true; }
        std::string request_move(Model::Game *game) override { return {}; }
    };

    class UserBot : public User {
    public:
        explicit UserBot(Model::PlayerName pname) : User(pname) {}

        bool is_human() override { return false; }
    };

}

#endif //CARAVAN_USER_H
