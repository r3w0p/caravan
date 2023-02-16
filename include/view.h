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
const short PAIR_YELLOW_BLACK = 4;

class View {
public:
    virtual void update(Engine *e, User *ubottom, User *utop, GameOption* go_bottom, GameOption* go_top) = 0;
    virtual GameOption option(Engine *e, User *u) = 0;
    virtual void close() = 0;
    virtual void error_message(std::string msg) = 0;
};

class ViewCLI : public View {
protected:
    bool has_colour;
    uint16_t y_max;
    uint16_t x_max;
    uint16_t y_offset;
    uint16_t x_offset;

    WINDOW* win_cvn_a;
    WINDOW* win_cvn_b;
    WINDOW* win_cvn_c;
    WINDOW* win_cvn_d;
    WINDOW* win_cvn_e;
    WINDOW* win_cvn_f;

    WINDOW* win_player_bottom;
    WINDOW* win_player_top;
    WINDOW* win_dialog;

    std::string uinput;
    std::string err_msg;
    bool err_display;

public:
    explicit ViewCLI();
    void update(Engine *e, User *ubottom, User *utop, GameOption* go_bottom, GameOption* go_top);
    GameOption option(Engine *e, User *u) override;
    void close() override;
    void error_message(std::string msg);
};

#endif //CARAVAN_VIEW_H
