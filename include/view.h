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
    virtual void update(Engine *e, User *ua, User *ub) = 0;
    virtual GameOption option(Engine *e, User *u) = 0;
    virtual void message(std::string msg) = 0;
    virtual void close() = 0;
};

class ViewCLI : public View {
protected:
    void apply_player_a(Engine *e, User *ua, User *ub, uint8_t mrow);
    //void apply_player_b(Engine *e, User *ua, User *ub, uint8_t mrow);
    void apply_corners();
    void apply_caravan_down(
            uint8_t rref, uint8_t cref, Engine *e, CaravanName cn);
    void apply_numeric_down(
            uint8_t rref, uint8_t cref, Card c);
    void clear_row(uint8_t rref);
public:
    explicit ViewCLI();
    void update(Engine *e, User *ua, User *ub);
    GameOption option(Engine *e, User *u) override;
    void message(std::string msg) override;
    void close() override;
};

#endif //CARAVAN_VIEW_H
