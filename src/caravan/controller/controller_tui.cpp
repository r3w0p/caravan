// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/controller/controller_tui.h"

void ControllerTUI::on_view_user_input(std::string input) {
    if(closed) return;

    // Decipher input
    GameCommand command;

    if(
            input.size() >= 4 and
            (input.at(0) == 'E' or input.at(0) == 'e') and
            (input.at(1) == 'X' or input.at(1) == 'x') and
            (input.at(2) == 'I' or input.at(2) == 'i') and
            (input.at(3) == 'T' or input.at(3) == 't')
    ) {
        command.type = OPTION_EXIT;

    } else if(input.size() >= 2) {

    } else return;

    // Pass to subscribers
    for(ControllerSubscriber *s : subscribers) {
        s->on_controller_command(command);
    }
}
