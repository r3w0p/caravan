// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_VIEW_H
#define CARAVAN_VIEW_H


#include <string>
#include "engine.h"

class View {
public:
    virtual void display(Engine *g, std::string msg) = 0;

    virtual GameOption get_input_option(Engine *e, PlayerName pn) = 0;
};

class ViewCLI : public View {
public:
    void display(Engine *g, std::string msg) override;

    GameOption get_input_option(Engine *e, PlayerName pn) override;
};

#endif //CARAVAN_VIEW_H
