// Copyright (c) 2022-2025 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_VIEW_H
#define CARAVAN_VIEW_H

#include "caravan/model/game.h"
#include "caravan/core/pubsub.h"
#include "caravan/user/user.h"


class View {
protected:
    Game *game;

public:
    explicit View(Game *g) : game(g) {};

    virtual ~View() = default;

    virtual void run() = 0;
};

class ViewSubscriber : public CaravanSubscriber {
public:
    virtual void on_view_user_input(User &u, std::string input) = 0;
};

#endif //CARAVAN_VIEW_H
