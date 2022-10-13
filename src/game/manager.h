// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_MANAGER_H
#define CARAVAN_MANAGER_H

class Player {
public:
    Player();
};

typedef struct Players {
    Player you;
    Player opp;
} Players;

typedef struct OptionPlay {} OptionPlay;
typedef struct OptionDrop {} OptionDrop;
typedef struct OptionRemove {} OptionRemove;

class GameManager {
public:
    GameManager();
};

#endif //CARAVAN_MANAGER_H
