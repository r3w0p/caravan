// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_VIEW_BASE_VIEW_H
#define CARAVAN_VIEW_BASE_VIEW_H

#include "caravan/controller/base_controller.h"
#include "caravan/model/game.h"
#include "caravan/user/user.h"

namespace Caravan::View {
    template <typename I, typename O>
    class BaseView {
        protected:
            Model::Game &game;
            Controller::BaseController<I, O> &ctrl;
            User::User &user_abc;
            User::User &user_def;

        public:
            explicit BaseView(
                Model::Game &game,
                Controller::BaseController<I, O> &ctrl,
                User::User &user_abc,
                User::User &user_def
            ) : game(game),
                ctrl(ctrl),
                user_abc(user_abc),
                user_def(user_def) {}

            virtual ~BaseView() = default;

            virtual void run() = 0;
    };
}

#endif //CARAVAN_VIEW_BASE_VIEW_H
