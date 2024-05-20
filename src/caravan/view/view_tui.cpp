// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/user/user.h"
#include "caravan/view/view.h"
#include "caravan/view/view_tui.h"

#include <memory>
#include <string>

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"


void ViewTUI::run() {
    using namespace ftxui;

    // Input data
    std::string user_input;
    std::string command;

    // User input component
    Component comp_user_input = Input(&user_input, "");

    // Ensure input is alphanumeric only
    comp_user_input |= CatchEvent([&](Event event) {
        return event.is_character() && !std::isalnum(event.character()[0]);
    });

    // Ensure maximum input length
    comp_user_input |= CatchEvent([&](Event event) {
        return event.is_character() && user_input.size() >= 5;  // TODO what is max input size?
    });

    // Component tree
    auto component = Container::Vertical({ comp_user_input });

    // Tweak how the component tree is rendered:
    auto renderer = Renderer(component, [&] {
        auto terminal_size = Terminal::Size();
        auto min_terminal_x = 150;
        auto min_terminal_y = 50;

        // Ensure minimum terminal dimensions
        // TODO prevent user input change during this display
        if (terminal_size.dimx < min_terminal_x || terminal_size.dimy < min_terminal_y) {
            user_input = "";
            return vbox({
                    text("Terminal too small"),
                    separatorEmpty(),
                    text("Width:  " + std::to_string(terminal_size.dimx) + " < " +
                         std::to_string(min_terminal_x)),
                    text("Height: " + std::to_string(terminal_size.dimy) + " < " +
                         std::to_string(min_terminal_y)),
                    separatorEmpty(),
                    text("Resize terminal or press Ctrl+C"),
            }) | center;
        }

        if(user_input.ends_with('\n')) {
            command = user_input;
            user_input = "";
        }

        // TODO immediately wipe command once passed to controller

        // Generate game as normal
        return vbox({
                hbox(text(" YOU > "), comp_user_input->Render()),
                separator(),
                text("Current: " + user_input),
                text("Command: " + command),
                text("Terminal: x=" + std::to_string(terminal_size.dimx) + " y=" + std::to_string(terminal_size.dimy)),
        }) | border;
    });

    auto screen = ScreenInteractive::Fullscreen();
    screen.Loop(renderer);
}
