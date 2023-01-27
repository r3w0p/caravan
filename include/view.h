// Copyright (c) 2022-2023 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_VIEW_H
#define CARAVAN_VIEW_H

#include <string>
#include "engine.h"
#include "user.h"
#include "curses.h"

const short PAIR_WHITE_BLACK = 1;
const short PAIR_RED_BLACK = 2;
const short PAIR_CYAN_BLACK = 3;

class View {
public:
    virtual void update(Engine *e, User *ua, User *ub) = 0;
    virtual GameOption option(Engine *e, User *u) = 0;
    virtual void close() = 0;
};

class ViewCLI : public View {
protected:
    bool has_colour;
    uint8_t r_max;
    uint8_t c_max;

    WINDOW* win_cvn_a;
    WINDOW* win_cvn_b;
    WINDOW* win_cvn_c;

    WINDOW* win_cvn_d;
    WINDOW* win_cvn_e;
    WINDOW* win_cvn_f;

    WINDOW* win_player_b;
    WINDOW* win_player_a;

    WINDOW* win_dialog;
    WINDOW* win_input;

    GameOption pa_option;
    GameOption pb_option;

    std::string pa_err;
    std::string pb_err;

public:
    explicit ViewCLI();
    void update(Engine *e, User *ua, User *ub);
    GameOption option(Engine *e, User *u) override;
    void close() override;
    void set_err(PlayerName pn, std::string msg);
};

#endif //CARAVAN_VIEW_H
