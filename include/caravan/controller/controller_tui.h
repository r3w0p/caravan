// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_CONTROLLER_TUI_H
#define CARAVAN_CONTROLLER_TUI_H

#include "caravan/controller/controller.h"

class ControllerTUI : public Controller {
public:
    explicit ControllerTUI() = default;

    void on_view_user_input(std::string input, bool confirmed) override;  // from ViewSubscriber
    void close() override;
};

#endif //CARAVAN_CONTROLLER_TUI_H
