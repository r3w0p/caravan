// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_CONTROLLER_H
#define CARAVAN_CONTROLLER_H

#include <array>
#include "model.h"

class User {};
class UserHuman: public User {};
// TODO UserBot

// ControllerGame stores two ControllerUser instances with two corresponding Player instances
// On User's turn, is passed (copy of) Table and Hand to make decision.
// Tells Game what move to make.

class Controller {};

class ControllerCLI : public Controller {};

#endif //CARAVAN_CONTROLLER_H
