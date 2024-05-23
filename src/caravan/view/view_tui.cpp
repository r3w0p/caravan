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

const uint16_t MIN_X = 130;
const uint16_t MIN_Y = 68;

const uint16_t WIDTH_CARAVAN = 16;
const uint16_t HEIGHT_CARAVAN = 34;

const uint16_t WIDTH_DECK = 15;

const uint16_t HEIGHT_CARAVAN_CARD = 4;

const uint16_t WIDTH_POSITION = 4;
const uint16_t HEIGHT_POSITION = 2;

const uint16_t WIDTH_CARD = 5;
const uint16_t HEIGHT_CARD = 2;

const uint16_t WIDTH_FACES = 5;
const uint16_t HEIGHT_FACES = 2;

std::shared_ptr<ftxui::Node> gen_position(std::string position) {
    using namespace ftxui;
    return text(position) | borderEmpty | size(WIDTH, EQUAL, WIDTH_POSITION) | size(HEIGHT, EQUAL, HEIGHT_POSITION);
}

std::shared_ptr<ftxui::Node> gen_card(std::wstring card) {
    using namespace ftxui;
    return text(card) | borderDouble | size(WIDTH, EQUAL, WIDTH_CARD) | size(HEIGHT, EQUAL, HEIGHT_CARD);
}

std::shared_ptr<ftxui::Node> gen_faces() {
    using namespace ftxui;
    return vbox({
        text(L"QQK"),
        text(L"♠♠♠")
    }) | borderEmpty | size(WIDTH, EQUAL, WIDTH_FACES) | size(HEIGHT, EQUAL, HEIGHT_FACES);
}

std::shared_ptr<ftxui::Node> gen_caravan_card(std::string position, std::wstring card) {
    using namespace ftxui;
    return hbox({
        gen_position(position),
        gen_card(card),
        gen_faces(),
    }) | size(HEIGHT, EQUAL, HEIGHT_CARAVAN_CARD);
}

std::shared_ptr<ftxui::Node> gen_caravan(std::string title, bool top) {
    using namespace ftxui;
    std::shared_ptr<Node> content;

    if(top) {
        content = vbox({
            gen_caravan_card("8", L" K♠"),
            gen_caravan_card("7", L" K♠"),
            gen_caravan_card("6", L" K♠"),
            gen_caravan_card("5", L" K♠"),
            gen_caravan_card("4", L" K♠"),
            gen_caravan_card("3", L" K♠"),
            gen_caravan_card("2", L" K♠"),
            gen_caravan_card("1", L" K♠"),
        });
    } else {
        content = vbox({
            gen_caravan_card("1", L" K♠"),
            gen_caravan_card("2", L" K♠"),
            gen_caravan_card("3", L" K♠"),
            gen_caravan_card("4", L" K♠"),
            gen_caravan_card("5", L" K♠"),
            gen_caravan_card("6", L" K♠"),
            gen_caravan_card("7", L" K♠"),
            gen_caravan_card("8", L" K♠"),
        });
    }

    return window(
        text(title) | hcenter | bold,
        content
    ) | center | size(WIDTH, EQUAL, WIDTH_CARAVAN) | size(HEIGHT, EQUAL, HEIGHT_CARAVAN);
}

std::shared_ptr<ftxui::Node> gen_deck_card(std::string position, std::wstring card) {
    using namespace ftxui;
    return hbox({
        gen_position(position),
        gen_card(card),
    }) | size(HEIGHT, EQUAL, HEIGHT_CARAVAN_CARD);
}

std::shared_ptr<ftxui::Node> gen_deck(std::string title, bool top, bool hide) {
    // TODO bool hide
    using namespace ftxui;
    std::shared_ptr<Node> content;

    if(top) {
        content = vbox({
            gen_deck_card(" ", L"###"),
            gen_deck_card(" ", L"###"),
            gen_deck_card(" ", L"###"),
            gen_deck_card(" ", L"###"),
            gen_deck_card(" ", L"###"),
            gen_deck_card(" ", L"###"),
            gen_deck_card(" ", L"###"),
            gen_deck_card(" ", L"###"),
        });
    } else {
        content = vbox({
            gen_deck_card("1", L" K♠"),
            gen_deck_card("2", L" K♠"),
            gen_deck_card("3", L" K♠"),
            gen_deck_card("4", L" K♠"),
            gen_deck_card("5", L" K♠"),
            gen_deck_card("6", L" K♠"),
            gen_deck_card("7", L" K♠"),
            gen_deck_card("8", L" K♠"),
        });
    }

    return window(
        text(title) | hcenter | bold,
        content
    ) | size(WIDTH, EQUAL, WIDTH_DECK);
}

std::shared_ptr<ftxui::Node> gen_input(
        std::shared_ptr<ftxui::ComponentBase> comp_user_input,
        std::string user_input,
        std::string command,
        std::string message) {
    using namespace ftxui;
    return vbox({
        separatorEmpty(),
        hbox(separatorEmpty(), text("YOU > "), comp_user_input->Render(), separatorEmpty()),
        separatorEmpty(),

        separator(),

        separatorEmpty(),
        hbox(separatorEmpty(), paragraph("YOU played KH onto 8C on Caravan C."), separatorEmpty()),
        separatorEmpty(),
        hbox(separatorEmpty(), paragraph("OPP has yet to move."), separatorEmpty()),
        separatorEmpty(),

        separator(),

        separatorEmpty(),
        hbox(separatorEmpty(), paragraph("YOU to move next."), separatorEmpty()),
        separatorEmpty(),
    }) | border | size(WIDTH, EQUAL, 44);
}

std::shared_ptr<ftxui::Node> gen_game(
        std::shared_ptr<ftxui::ComponentBase> comp_user_input,
        std::string user_input,
        std::string command,
        std::string message) {
    using namespace ftxui;
    return hbox({  // OUTERMOST AREA

        vbox({  // GAME AREA

            hbox({  // TOP GAME AREA
                gen_caravan(" D ", true),
                separatorEmpty(),
                separatorEmpty(),
                gen_caravan(" E ", true),
                separatorEmpty(),
                separatorEmpty(),
                gen_caravan(" F (100, ASC) ", true),
            }),  // top game area

            separatorEmpty(),

            hbox({  // BOTTOM GAME AREA
                gen_caravan(" A ", false),
                separatorEmpty(),
                separatorEmpty(),
                gen_caravan(" B (100, ASC) ", false),
                separatorEmpty(),
                separatorEmpty(),
                gen_caravan(" C ", false),
            }),  // bottom game area

        }),  // game area

        separatorEmpty(),
        separatorEmpty(),
        separatorEmpty(),
        separatorEmpty(),
        separatorEmpty(),

        vbox({  // DECK AREA
            gen_deck(" BOT (100) ", true, true),
            separatorEmpty(),
            gen_deck(" YOU ", false, false),
        }),  // deck area

        separatorEmpty(),
        separatorEmpty(),
        separatorEmpty(),
        separatorEmpty(),
        separatorEmpty(),

        vbox({  // INPUT AREA
            hbox({}) | borderEmpty | size(HEIGHT, EQUAL, HEIGHT_CARAVAN),
            separatorEmpty(),
            gen_input(comp_user_input, user_input, command, message)
        }),  // input area

    }) | center;  // outermost area
}

std::shared_ptr<ftxui::Node> gen_terminal_too_small(
        ftxui::Dimensions terminal_size) {
    using namespace ftxui;
    return vbox({
        text("Terminal too small"),
        separatorEmpty(),
        text("Width:  " + std::to_string(terminal_size.dimx) + " < " + std::to_string(MIN_X)),
        text("Height: " + std::to_string(terminal_size.dimy) + " < " + std::to_string(MIN_Y)),
        separatorEmpty(),
        text("Resize terminal or press Ctrl+C"),
    }) | center;
}

void ViewTUI::run() {
    // TODO on_model_exit, close the game
    using namespace ftxui;

    if(closed) return;

    // Input data
    std::string user_input;
    bool confirmed;
    std::string command;
    std::string message;
    Dimensions terminal_size {};

    // Create screen
    ScreenInteractive screen = ScreenInteractive::Fullscreen();

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
        // Closes on exit command from user input
        if(closed) screen.Exit();

        terminal_size = Terminal::Size();

        // Error screen if less than minimum terminal dimensions
        if (terminal_size.dimx < MIN_X || terminal_size.dimy < MIN_Y) {
            user_input = "";  // Prevent user input change during error
            return gen_terminal_too_small(terminal_size);
        }

        // Handle user input
        command = user_input;
        if(command.ends_with('\n')) {
            command.pop_back();  // removes '\n'
            user_input = "";
            confirmed = true;
        } else {
            confirmed = false;
        }

        // Send input to subscribers (i.e., Controller)
        for (ViewSubscriber *vs: subscribers) {
            vs->on_view_user_input(command, confirmed);
        }

        // TODO replace this with an 'exit early' signal from the model
        //  (i.e. not exciting because game has finished, but because of an exit signal from user)
        //if(command == "EXIT") screen.Exit();

        return gen_game(comp_user_input, user_input, command, message);
    });

    screen.Loop(renderer);
    screen.Clear();
}
