// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_VIEW_H
#define CARAVAN_VIEW_H

#include <string>
#include "caravan/model/game.h"
#include "caravan/user/user.h"
#include "caravan/core/common.h"
#include "ftxui/dom/elements.hpp"


typedef struct ViewConfig {
    // Pointers to users
    User *user_abc{nullptr};
    User *user_def{nullptr};
    User *user_turn{};
    User *user_next{};

    // Names of users
    std::string name_abc;
    std::string name_def;
    std::string name_turn;
    std::string name_next;

    // Messages to users
    std::string msg_main;       // command entered, general messages, winner
    std::string msg_important;  // game errors, next turn
    std::string msg_fatal;      // game closing due to major problem

    // Messages on moves made
    ftxui::Elements msg_move_abc;
    ftxui::Elements msg_move_def;

    // Most recent command
    GameCommand command;

    // Board highlight
    GameCommand highlight;

    // Colour support
    bool colour{};

    // Bot config
    float bot_delay_sec{0.0};
} ViewConfig;


class View {
protected:
    ViewConfig *vc;
    Game *game;
    bool closed;
public:
    explicit View(ViewConfig *vc, Game *game);

    virtual void run() = 0;

    virtual void close() = 0;
};

#endif //CARAVAN_VIEW_H
