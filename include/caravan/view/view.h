// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_VIEW_H
#define CARAVAN_VIEW_H

#include "caravan/model/game.h"


class View {
protected:
    Game *game;
    bool closed;
public:
    explicit View(Game *game);

    virtual ~View() = default;

    virtual void run() = 0;

    virtual void close() = 0;
};

#endif //CARAVAN_VIEW_H
