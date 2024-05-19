// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_VIEW_H
#define CARAVAN_VIEW_H

#include <string>
#include "caravan/model/game.h"
#include "caravan/user/user.h"
#include "caravan/core/common.h"

class ViewSubscriber {
public:
    virtual void on_view_update() = 0;  // TODO
};

class View : Publisher<ViewSubscriber> {
protected:
    User *user_top_ptr;
    User *user_bottom_ptr;
    bool closed;
public:
    explicit View(User *utop, User *ubottom) :
        user_top_ptr(utop), user_bottom_ptr(ubottom), closed(false) {}

    virtual void run() = 0;
    void close() { closed = true; };
};

#endif //CARAVAN_VIEW_H
