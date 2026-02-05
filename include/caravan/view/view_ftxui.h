// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_VIEW_VIEW_FTXUI_H
#define CARAVAN_VIEW_VIEW_FTXUI_H

#include "caravan/view/base_view.h"
#include <cstdint>
#include <string>
#include "caravan/user/user.h"
#include "caravan/controller/base_controller.h"
#include "ftxui/dom/elements.hpp"

namespace Caravan::View {
    // TODO rename to ViewFTXUIConfig?
    //  or perhaps remove this altogether as this is changed within View
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


    class ViewFTXUI : public BaseView<std::string, std::string> {
        protected:
            std::uint16_t bot_delay_millis;
            bool cheat;
            bool colour;

            // Board highlight
            Model::GameMove highlight;

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

        public:
            explicit ViewFTXUI(
                Model::Game &game,
                Controller::BaseController<std::string, std::string> &ctrl,
                User::User &user_abc,
                User::User &user_def,
                std::uint16_t bot_delay_millis,
                bool cheat,
                bool colour  // TODO mention in docs that it is not a guarantee; setting to true only requests colour
            ) : BaseView(game, ctrl, user_abc, user_def),
                bot_delay_millis(bot_delay_millis),
                cheat(cheat),
                colour(colour) {}

            void run() override;
    };
}

#endif //CARAVAN_VIEW_VIEW_FTXUI_H
