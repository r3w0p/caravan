// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_VIEW_H
#define CARAVAN_VIEW_H

#include <string>
#include "engine.h"
#include "user.h"

const short PAIR_WHITE_BLACK = 1;
const short PAIR_RED_BLACK = 2;
const short PAIR_CYAN_BLACK = 3;

class View {
public:
    virtual void update(Engine *e, User *ua, User *ub) = 0;
    virtual GameOption option(Engine *e, User *u) = 0;
    virtual void message(std::string msg) = 0;
    virtual void close() = 0;
};

class ViewCLI : public View {
protected:
    bool has_colour;
    uint8_t r_max;
    uint8_t c_max;
public:
    explicit ViewCLI();
    void update(Engine *e, User *ua, User *ub);
    GameOption option(Engine *e, User *u) override;
    void message(std::string msg) override;
    void close() override;
};

#endif //CARAVAN_VIEW_H
