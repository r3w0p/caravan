// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_VIEW_FTXUI_H
#define CARAVAN_VIEW_FTXUI_H

#include "caravan/view/view.h"
#include <string>
#include "caravan/user/user.h"
#include "caravan/controller/controller_str_to_move.h"
#include "ftxui/dom/elements.hpp"

namespace Caravan::View {

    typedef struct ViewConfig {
        // Pointers to users
        User::User *user_abc{nullptr};
        User::User *user_def{nullptr};
        User::User *user_turn{};
        User::User *user_next{};

        // Names of users
        std::string name_abc;
        std::string name_def;
        std::string name_turn;
        std::string name_next;

        // Messages to users
        std::string msg_main; // move chosen, general messages, winner
        std::string msg_important; // game errors, next turn
        std::string msg_fatal; // game closing due to major problem

        // Messages on moves made
        ftxui::Elements msg_move_abc;
        ftxui::Elements msg_move_def;

        // Most recent move
        Model::GameMove move;

        // Board highlight
        Model::GameMove highlight;

        // Colour support
        bool colour{true};

        // Bot config
        float bot_delay_sec{0.0};

        // Cheat
        bool cheat{false};
    } ViewConfig;


    class ViewFTXUI : public View {
    protected:
        Controller::ControllerStrToMove *ctrl;
        ViewConfig *config;

    public:
        explicit ViewFTXUI(
            Model::Game *game,
            Controller::ControllerStrToMove *ctrl,
            ViewConfig &config);

        void run() override;
    };

}

#endif //CARAVAN_VIEW_FTXUI_H
