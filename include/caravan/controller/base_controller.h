// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_CONTROLLER_BASE_CONTROLLER_H
#define CARAVAN_CONTROLLER_BASE_CONTROLLER_H

#include "caravan/model/game.h"

namespace Caravan::Controller {
    template <typename I, typename O>
    class BaseController {
        protected:
            Model::Game &game;
        public:
            explicit BaseController(Model::Game &game) : game(game) {}
            virtual ~BaseController() = default;

            virtual O on_user_input(I &input) = 0;
    };
}

#endif //CARAVAN_CONTROLLER_BASE_CONTROLLER_H
