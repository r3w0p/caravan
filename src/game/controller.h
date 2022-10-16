// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_CONTROLLER_H
#define CARAVAN_CONTROLLER_H

#include <array>
#include "model.h"


typedef struct Players {
    Player you;
    Player opp;
} Players;

typedef struct OptionPlay {} OptionPlay;
typedef struct OptionDrop {} OptionDrop;
typedef struct OptionRemove {} OptionRemove;

class Game {
public:
    Game();
};

#endif //CARAVAN_CONTROLLER_H
