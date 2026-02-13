// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_USER_BASE_USER_H
#define CARAVAN_USER_BASE_USER_H

#include "caravan/model/types.h"
#include "caravan/model/game.h"

namespace Caravan::User {
    template<typename I>
    class BaseUser {
        protected:
            Model::PlayerName name;

            explicit BaseUser(Model::PlayerName pname) : name(pname) {
            }

        public:
            virtual ~BaseUser() = default;

            [[nodiscard]] Model::PlayerName get_name() const {
                return name;
            }

            virtual bool is_human() = 0;

            virtual I request_input(Model::Game *game) = 0;
    };
}

#endif //CARAVAN_USER_BASE_USER_H
