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
    ViewDisplayArray vda {};
    void flush_vda();
    void apply_player_a(Engine *e, User *ua, User *ub, uint8_t mrow);
    void apply_player_b(Engine *e, User *ua, User *ub, uint8_t mrow);
    void apply_corners();
    void apply_caravan_down(
            uint8_t rref, uint8_t cref, Engine *e, CaravanName cn);
    void apply_pos(uint8_t rref, uint8_t cref, uint8_t pos);
    void apply_numeric_down(
            uint8_t rref, uint8_t cref, Card c);

public:
    void display_table(Engine *e, User *ua, User *ub) override;
    void put_message(std::string msg) override;
    GameOption get_option(Engine *e, User *u) override;
};

#endif //CARAVAN_VIEW_H
