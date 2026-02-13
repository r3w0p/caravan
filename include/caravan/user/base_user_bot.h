// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_USER_USER_BOT_H
#define CARAVAN_USER_USER_BOT_H

#include "caravan/model/types.h"
#include "caravan/user/base_user.h"

namespace Caravan::User {
    template<typename I>
    class BaseUserBot : public BaseUser<I> {
        protected:
            explicit BaseUserBot(Model::PlayerName pname) : BaseUser<I>(pname) {
            }

        public:
            bool is_human() final {
                return false;
            }
    };
}

#endif //CARAVAN_USER_USER_BOT_H
