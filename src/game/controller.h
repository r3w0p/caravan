// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_CONTROLLER_H
#define CARAVAN_CONTROLLER_H

#include <array>
#include "model.h"
#include "view.h"

class User {};
class UserHuman: public User {};
// TODO UserBot

class Controller {
protected:
    Game* game_ptr;
    View* view_ptr;
    virtual GameOption request_option();
public:
    explicit Controller(Game* g, View* v) : game_ptr(g), view_ptr(v) {};
    virtual void run() = 0;
};

class ControllerCLI : public Controller {
protected:
    GameOption request_option();
public:
    // TODO pass cli arguments to controller as "ControllerConfig" struct
    explicit ControllerCLI(Game* g, View* v) : Controller(g, v) {};
    void run();
};

#endif //CARAVAN_CONTROLLER_H
