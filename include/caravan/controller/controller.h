// Copyright (c) 2022-2025 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_CONTROLLER_CONTROLLER_H
#define CARAVAN_CONTROLLER_CONTROLLER_H

#include "caravan/core/pubsub.h"
#include "caravan/model/types.h"

class Controller {};

class ControllerSubscriber : public CaravanSubscriber {
public:
    virtual void on_controller_game_command(GameCommand gc) = 0;
};

#endif //CARAVAN_CONTROLLER_CONTROLLER_H
