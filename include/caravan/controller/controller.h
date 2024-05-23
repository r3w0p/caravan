// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_CONTROLLER_H
#define CARAVAN_CONTROLLER_H

#include <array>
#include "caravan/core/common.h"
#include "caravan/model/game.h"
#include "caravan/user/user.h"
#include "caravan/view/view.h"

class ControllerSubscriber {
public:
    virtual void on_controller_command(GameCommand command) = 0;  // TODO
};

class Controller : public ViewSubscriber, public Publisher<ControllerSubscriber> {
protected:
    bool closed;
public:
    virtual ~Controller() = default;
    explicit Controller() : closed(false) {};

    void subscribe(ControllerSubscriber *sub) override;
    virtual void close() = 0;
};

#endif //CARAVAN_CONTROLLER_H
