// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_VIEW_VIEW_FTXUI_H
#define CARAVAN_VIEW_VIEW_FTXUI_H

#include "caravan/view/base_view.h"
#include <cstdint>
#include <string>
#include "caravan/user/base_user.h"
#include "caravan/controller/base_controller.h"
#include "ftxui/dom/elements.hpp"

namespace Caravan::View {
    class ViewFTXUI : public BaseView<std::string, std::tuple<Model::GameMove,
            std::string>> {
        protected:
            // Pointers to users
            User::BaseUser<std::string> *user_turn{};
            User::BaseUser<std::string> *user_next{};

            // Names of users
            std::string name_abc;
            std::string name_def;
            std::string name_turn;
            std::string name_next;

            // Messages to users
            std::string msg_main; // move chosen, general messages, winner
            std::string msg_important; // game errors, next turn
            std::string msg_fatal; // program errors

            // Messages on moves made
            ftxui::Elements msg_move_abc;
            ftxui::Elements msg_move_def;

            // Most recent move
            Model::GameMove last_move;

            // Board highlight
            Model::GameMove highlight;

            std::uint16_t bot_delay_millis;
            bool cheat;
            bool colour;

            void update_current_turn();

        public:
            explicit ViewFTXUI(
                Model::Game &game,
                Controller::BaseController<std::string, std::tuple<
                    Model::GameMove, std::string>> &ctrl,
                User::BaseUser<std::string> &user_abc,
                User::BaseUser<std::string> &user_def,
                std::uint16_t bot_delay_millis,
                bool cheat,
                bool colour
                // TODO mention in docs that it is not a guarantee; setting to true only requests colour
            ) : BaseView(game, ctrl, user_abc, user_def),
                bot_delay_millis(bot_delay_millis),
                cheat(cheat),
                colour(colour) {
            }

            [[nodiscard]] std::uint16_t get_bot_delay_millis() const {
                return bot_delay_millis;
            }

            [[nodiscard]] bool is_cheating() const {
                return cheat;
            }

            [[nodiscard]] bool wants_colour() const {
                return colour;
            }

            [[nodiscard]] Model::GameMove get_highlight() const {
                return highlight;
            }

            [[nodiscard]] Model::GameMove get_move() const {
                return last_move;
            }

            [[nodiscard]] User::BaseUser<std::string> &get_user_abc() const {
                return user_abc;
            }

            [[nodiscard]] User::BaseUser<std::string> &get_user_def() const {
                return user_def;
            }

            [[nodiscard]] User::BaseUser<std::string> *get_user_turn() const {
                return user_turn;
            }

            [[nodiscard]] User::BaseUser<std::string> *get_user_next() const {
                return user_next;
            }

            [[nodiscard]] std::string get_name_abc() const {
                return name_abc;
            }

            [[nodiscard]] std::string get_name_def() const {
                return name_def;
            }

            [[nodiscard]] std::string get_name_turn() const {
                return name_turn;
            }

            [[nodiscard]] std::string get_name_next() const {
                return name_next;
            }

            [[nodiscard]] std::string get_msg_main() const {
                return msg_main;
            }

            [[nodiscard]] std::string get_msg_important() const {
                return msg_important;
            }

            [[nodiscard]] std::string get_msg_fatal() const {
                return msg_fatal;
            }

            [[nodiscard]] ftxui::Elements get_msg_move_abc() const {
                return msg_move_abc;
            }

            [[nodiscard]] ftxui::Elements get_msg_move_def() const {
                return msg_move_def;
            }

            void run() override;
    };
}

#endif //CARAVAN_VIEW_VIEW_FTXUI_H