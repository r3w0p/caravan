// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_CONTROLLER_CONTROLLER_FTXUI_H
#define CARAVAN_CONTROLLER_CONTROLLER_FTXUI_H

#include <string>
#include <tuple>
#include "caravan/controller/base_controller.h"
#include "caravan/model/game.h"

namespace Caravan::Controller {
    class ControllerFTXUI : public BaseController<std::string, std::tuple<
            Model::GameMove, std::string>> {
        public:
            explicit ControllerFTXUI(Model::Game &game) : BaseController(game) {
            }

            ~ControllerFTXUI() override = default;

            /**
             * @param input User input.
             * @param confirmed If `true`, input represents a confirmed command;
             *        if `false`, input represents a partially completed command.
             *
             * @return The game move that could be parsed from the input
             *         (whether confirmed input or not), as well as any error
             *         messages that may have occurred (an empty string if
             *         no error).
             */
            std::tuple<Model::GameMove, std::string> on_user_input(
                std::string &input,
                bool confirmed
            ) override;
    };
}
#endif //CARAVAN_CONTROLLER_CONTROLLER_FTXUI_H
