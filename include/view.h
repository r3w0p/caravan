// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_VIEW_H
#define CARAVAN_VIEW_H

#include <string>
#include "engine.h"
#include "user.h"

class View {
public:
    virtual void display_table(Engine *e, User *ua, User *ub) = 0;
    virtual void put_message(std::string msg) = 0;
    virtual GameOption get_option(Engine *e, User *u) = 0;
};

class ViewCLI : public View {
protected:
public:
};

#endif //CARAVAN_VIEW_H
