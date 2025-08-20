// Copyright (c) 2022-2025 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_VIEW_FTXUI_H
#define CARAVAN_VIEW_FTXUI_H

#include "caravan/view/view.h"
#include <string>
#include "caravan/user/user.h"
#include "caravan/controller/controller_str_to_move.h"
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
    std::string msg_main; // command entered, general messages, winner
    std::string msg_important; // game errors, next turn
    std::string msg_fatal; // game closing due to major problem

    // Messages on moves made
    ftxui::Elements msg_move_abc;
    ftxui::Elements msg_move_def;

    // Most recent command
    GameMove command;

    // Board highlight
    GameMove highlight;

    // Colour support
    bool colour{};

    // Bot config
    float bot_delay_sec{0.0};
} ViewConfig;


class ViewFTXUI : public View {
protected:
    ControllerStrToMove *ctrl;
    ViewConfig *vc;

public:
    explicit ViewFTXUI(Game *game, ControllerStrToMove *ctrl, ViewConfig &vc);

    void run() override;
};

#endif //CARAVAN_VIEW_FTXUI_H
