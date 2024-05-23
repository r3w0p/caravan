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

const uint8_t INPUT_MAX = 4;

const std::string NAME_YOU = "YOU";
const std::string NAME_BOT = "BOT";

const std::string NAME_PL1 = "PL1";
const std::string NAME_PL2 = "PL2";

const std::string NAME_BOT1 = "BOT1";
const std::string NAME_BOT2 = "BOT2";


typedef struct ViewConfig {
    // Messages to users
    std::string msg_notif;
    std::string msg_err;
    
    std::string msg_move_abc;
    std::string msg_move_def;
    
    // Names of users
    std::string name_abc;
    std::string name_def;
    std::string name_turn;

    // Most recent command
    GameCommand command;
} ViewConfig;


std::wstring caravan_to_wstr(CaravanName caravan_name, bool letter_only) {
    switch (caravan_name) {
        case CARAVAN_A:
            return letter_only ? L"A" : L"Caravan A";
        case CARAVAN_B:
            return letter_only ? L"B" : L"Caravan B";
        case CARAVAN_C:
            return letter_only ? L"C" : L"Caravan C";
        case CARAVAN_D:
            return letter_only ? L"D" : L"Caravan D";
        case CARAVAN_E:
            return letter_only ? L"E" : L"Caravan E";
        case CARAVAN_F:
            return letter_only ? L"F" : L"Caravan F";
        default:
            throw CaravanFatalException("Invalid caravan name.");
    }
}

std::wstring suit_to_wstr(Suit suit) {
    switch (suit) {
        case NO_SUIT:
            return L" ";
        case SPADES:
            return L"♠";
        case CLUBS:
            return L"♣";
        case HEARTS:
            return L"♥";
        case DIAMONDS:
            return L"♦";
        default:
            throw CaravanFatalException("Invalid suit.");
    }
}

std::wstring direction_to_wstr(Direction direction) {
    switch (direction) {
        case ANY:
            return L"ANY";
        case ASCENDING:
            return L"ASC";
        case DESCENDING:
            return L"DES";
        default:
            throw CaravanFatalException("Invalid direction.");
    }
}

std::wstring rank_to_wstr(Rank rank, bool lead) {
    switch (rank) {
        case ACE:
            return lead ? L" A" : L"A";
        case TWO:
            return lead ? L" 2" : L"2";
        case THREE:
            return lead ? L" 3" : L"3";
        case FOUR:
            return lead ? L" 4" : L"4";
        case FIVE:
            return lead ? L" 5" : L"5";
        case SIX:
            return lead ? L" 6" : L"6";
        case SEVEN:
            return lead ? L" 7" : L"7";
        case EIGHT:
            return lead ? L" 8" : L"8";
        case NINE:
            return lead ? L" 9" : L"9";
        case TEN:
            return L"10";
        case JACK:
            return lead ? L" J" : L"J";
        case QUEEN:
            return lead ? L" Q" : L"Q";
        case KING:
            return lead ? L" K" : L"K";
        case JOKER:
            return L"JO";
        default:
            throw CaravanFatalException("Invalid rank.");
    }
}

std::wstring card_num_to_wstr(uint8_t card_num, bool lead) {
    switch (card_num) {
        case 1:
            return lead ? L" 1" : L"1";
        case 2:
            return lead ? L" 2" : L"2";
        case 3:
            return lead ? L" 3" : L"3";
        case 4:
            return lead ? L" 4" : L"4";
        case 5:
            return lead ? L" 5" : L"5";
        case 6:
            return lead ? L" 6" : L"6";
        case 7:
            return lead ? L" 7" : L"7";
        case 8:
            return lead ? L" 8" : L"8";
        case 9:
            return lead ? L" 9" : L"9";
        case 10:
            return L"10";
        default:
            throw CaravanFatalException("Invalid card numeral.");
    }
}

void process_first(std::string input, GameCommand *command) {
    char c = input.at(0);  // minimum input size already checked elsewhere

    switch (c) {
        case 'P':
        case 'p':
            command->option = OPTION_PLAY;
            /*
             * P2F
             * "Play numeral card at hand pos 2 onto caravan F"
             *
             * P4F8
             * "Play face card at hand pos 4 onto caravan F, slot 8"
             */
            break;

        case 'D':
        case 'd':
            command->option = OPTION_DISCARD;
            /*
             * D3
             * "Discard card at hand pos 3"
             */
            break;

        case 'C':
        case 'c':
            /*
             * CE
             * "Clear caravan E"
             */
            command->option = OPTION_CLEAR;
            break;

        default:
            throw CaravanInputException("Invalid option '" + std::string(1, c) + "', must be one of: (P)lay, (D)iscard, (C)lear.");
    }
}

void process_second(std::string input, GameCommand *command) {
    if (command->option == OPTION_PLAY or command->option == OPTION_DISCARD) {

        if (input.size() < 2)
            throw CaravanInputException("A hand position has not been entered.");

        char c = input.at(1);

        switch (c) {
            case '1':
                command->pos_hand = 1;
                break;
            case '2':
                command->pos_hand = 2;
                break;
            case '3':
                command->pos_hand = 3;
                break;
            case '4':
                command->pos_hand = 4;
                break;
            case '5':
                command->pos_hand = 5;
                break;
            case '6':
                command->pos_hand = 6;
                break;
            case '7':
                command->pos_hand = 7;
                break;
            case '8':
                command->pos_hand = 8;
                break;
            default:
                throw CaravanInputException("Invalid hand position '" + std::string(1, c) + "'.");
        }

    } else if (command->option == OPTION_CLEAR) {

        if (input.size() < 2)
            throw CaravanInputException("A caravan name has not been entered.");

        char c = input.at(1);

        switch (c) {
            case 'A':
            case 'a':
                command->caravan_name = CARAVAN_A;
                break;
            case 'B':
            case 'b':
                command->caravan_name = CARAVAN_B;
                break;
            case 'C':
            case 'c':
                command->caravan_name = CARAVAN_C;
                break;
            case 'D':
            case 'd':
                command->caravan_name = CARAVAN_D;
                break;
            case 'E':
            case 'e':
                command->caravan_name = CARAVAN_E;
                break;
            case 'F':
            case 'f':
                command->caravan_name = CARAVAN_F;
                break;
            default:
                throw CaravanInputException("Invalid caravan name '" + std::string(1, c) + "', must be between: A-F.");
        }

    } // else invalid command type, handled during parse of first character
}

void process_third(std::string input, GameCommand *command) {
    if (command->option == OPTION_PLAY) {

        if (input.size() < 3)
            throw CaravanInputException("A caravan name has not been entered.");

        char c = input.at(2);

        switch (c) {
            case 'A':
            case 'a':
                command->caravan_name = CARAVAN_A;
                break;
            case 'B':
            case 'b':
                command->caravan_name = CARAVAN_B;
                break;
            case 'C':
            case 'c':
                command->caravan_name = CARAVAN_C;
                break;
            case 'D':
            case 'd':
                command->caravan_name = CARAVAN_D;
                break;
            case 'E':
            case 'e':
                command->caravan_name = CARAVAN_E;
                break;
            case 'F':
            case 'f':
                command->caravan_name = CARAVAN_F;
                break;
            default:
                throw CaravanInputException("Invalid caravan name '" + std::string(1, c) + "', must be between: A-F.");
        }
    }
}

void process_fourth(std::string input, GameCommand *command) {
    if (command->option == OPTION_PLAY) {

        if (input.size() < 4) return;  // optional, not an error

        char c = input.at(3);

        switch (c) {
            case '1':
                command->pos_caravan = 1;
                break;
            case '2':
                command->pos_caravan = 2;
                break;
            case '3':
                command->pos_caravan = 3;
                break;
            case '4':
                command->pos_caravan = 4;
                break;
            case '5':
                command->pos_caravan = 5;
                break;
            case '6':
                command->pos_caravan = 6;
                break;
            case '7':
                command->pos_caravan = 7;
                break;
            case '8':
                command->pos_caravan = 8;
                break;
            default:
                throw CaravanInputException("Invalid caravan position '" + std::string(1, c) + "'.");
        }
    }
}

bool process_exit(std::string input, GameCommand *command) {
    if(
        input.size() == 4 and
        (input.at(0) == 'E' or input.at(0) == 'e') and
        (input.at(1) == 'X' or input.at(1) == 'x') and
        (input.at(2) == 'I' or input.at(2) == 'i') and
        (input.at(3) == 'T' or input.at(3) == 't')
    ) {
        command->option = OPTION_EXIT;
        return true;
    }

    return false;
}

GameCommand ViewTUI::parse_user_input(std::string input, bool confirmed) {
    GameCommand command;
    bool is_exit;

    if(closed) return command;

    if(input.empty()) return command;

    // TODO A future feature that highlights parts of the board as a command is typed,
    //  so that the user has a better idea of what their command will do.
    if(!confirmed) return command;

    /*
     * EXIT
     */
    is_exit = process_exit(input, &command);

    if(!is_exit) {
        /*
         * FIRST
         * - COMMAND TYPE
         */
        process_first(input, &command);

        /*
         * SECOND
         * - HAND POSITION or
         * - CARAVAN NAME
         */
        process_second(input, &command);

        /*
         * THIRD
         * - CARAVAN NAME
         */
        process_third(input, &command);

        /*
         * FOURTH
         * - CARAVAN POSITION (used when selecting Face card only)
         */
         process_fourth(input, &command);
    }

    return command;
}

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

std::shared_ptr<ftxui::Node> gen_caravan(Game *game, CaravanName cn, bool top) {
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

    std::wstring title = L" " + caravan_to_wstr(cn, true) + L" ";

    if (game->get_table()->get_caravan_size(cn) > 0) {
        title +=
                L"(" +
                std::to_wstring(game->get_table()->get_caravan_bid(cn)) +
                L", " +
                direction_to_wstr(game->get_table()->get_caravan_direction(cn)) +
                L", " +
                suit_to_wstr(game->get_table()->get_caravan_suit(cn)) +
                L") ";
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

std::shared_ptr<ftxui::Node> gen_deck(Game *game, ViewConfig *vc, bool top, bool hide) {
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

    Player *p = game->get_player(top ? PLAYER_DEF : PLAYER_ABC);
    uint8_t total_cards = p->get_size_deck() + p->get_size_hand();
    std::string title = " " + (top ? vc->name_def : vc->name_abc) + " (" + std::to_string(total_cards) + ") ";

    return window(
        text(title) | hcenter | bold,
        content
    ) | size(WIDTH, EQUAL, WIDTH_DECK);
}

std::shared_ptr<ftxui::Node> gen_input(
        ViewConfig *vc,
        std::shared_ptr<ftxui::ComponentBase> *comp_user_input) {
    using namespace ftxui;
    Elements e;

    e.push_back(separatorEmpty());
    e.push_back(hbox(separatorEmpty(), text(vc->name_turn + " > "), (*comp_user_input)->Render(), separatorEmpty()));  // TODO
    e.push_back(separatorEmpty());

    if(!vc->msg_notif.empty() || !vc->msg_err.empty()) {
        e.push_back(separator());
        e.push_back(separatorEmpty());

        if(!vc->msg_notif.empty()) {
            e.push_back(hbox(separatorEmpty(), paragraph(vc->msg_notif), separatorEmpty()));
            e.push_back(separatorEmpty());
        }

        if(!vc->msg_err.empty()) {
            e.push_back(hbox(separatorEmpty(), paragraph(vc->msg_err), separatorEmpty()));
            e.push_back(separatorEmpty());
        }
    }

    if(!vc->msg_move_abc.empty() || !vc->msg_move_def.empty()) {
        e.push_back(separator());
        e.push_back(separatorEmpty());

        if(!vc->msg_move_abc.empty()) {
            e.push_back(hbox(separatorEmpty(), paragraph(vc->msg_move_abc), separatorEmpty()));
            e.push_back(separatorEmpty());
        }

        if(!vc->msg_move_def.empty()) {
            e.push_back(hbox(separatorEmpty(), paragraph(vc->msg_move_def), separatorEmpty()));
            e.push_back(separatorEmpty());
        }
    }

    return vbox(e) | border | size(WIDTH, EQUAL, 44);
}

std::shared_ptr<ftxui::Node> gen_game(
        Game *game,
        ViewConfig *vc,
        std::shared_ptr<ftxui::ComponentBase> *comp_user_input) {
    using namespace ftxui;
    return hbox({  // OUTERMOST AREA

        vbox({  // GAME AREA

            hbox({  // TOP GAME AREA
                gen_caravan(game, CARAVAN_D, true),
                separatorEmpty(),
                separatorEmpty(),
                gen_caravan(game, CARAVAN_E, true),
                separatorEmpty(),
                separatorEmpty(),
                gen_caravan(game, CARAVAN_F, true),
            }),  // top game area

            separatorEmpty(),

            hbox({  // BOTTOM GAME AREA
                gen_caravan(game, CARAVAN_A, false),
                separatorEmpty(),
                separatorEmpty(),
                gen_caravan(game, CARAVAN_B, false),
                separatorEmpty(),
                separatorEmpty(),
                gen_caravan(game, CARAVAN_C, false),
            }),  // bottom game area

        }),  // game area

        separatorEmpty(),
        separatorEmpty(),
        separatorEmpty(),
        separatorEmpty(),
        separatorEmpty(),

        vbox({  // DECK AREA
            gen_deck(game, vc, true, true),
            separatorEmpty(),
            gen_deck(game, vc, false, false),
        }),  // deck area

        separatorEmpty(),
        separatorEmpty(),
        separatorEmpty(),
        separatorEmpty(),
        separatorEmpty(),

        vbox({  // INPUT AREA
            hbox({}) | borderEmpty | size(HEIGHT, EQUAL, HEIGHT_CARAVAN),
            separatorEmpty(),
            gen_input(vc, comp_user_input)
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
        text("Resize terminal or type Ctrl+C"),
    }) | center;
}

std::shared_ptr<ftxui::Node> gen_closed() {
    using namespace ftxui;
    return vbox({
        text(""),
    }) | center;
}

void set_name_turn(ViewConfig *vc, Game *game) {
    vc->name_turn = game->get_player_turn() == PLAYER_ABC ? vc->name_abc : vc->name_def;
}

/*
 * PUBLIC
 */

void ViewTUI::run() {
    using namespace ftxui;

    if(closed) return;

    // Screen config
    bool called_exit;
    Dimensions terminal_size {};

    // User input
    std::string user_input;
    std::string raw_command;
    bool confirmed;
    
    // Config for rendering game
    ViewConfig vc;

    // Set names of users based on who is human or not
    if(user_abc->is_human() and user_def->is_human()) {
        vc.name_abc = NAME_PL1;
        vc.name_def = NAME_PL2;

    } else if(user_abc->is_human() and !user_def->is_human()) {
        vc.name_abc = NAME_YOU;
        vc.name_def = NAME_BOT;

    } else if(!user_abc->is_human() and user_def->is_human()) {
        vc.name_abc = NAME_BOT;
        vc.name_def = NAME_YOU;

    } else {  // both are bots
        vc.name_abc = NAME_BOT1;
        vc.name_def = NAME_BOT2;
    }

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
        return event.is_character() && user_input.size() >= INPUT_MAX;
    });

    // Component tree
    auto component = Container::Vertical({ comp_user_input });

    // Initial notifications
    set_name_turn(&vc, game);
    vc.msg_notif = "Welcome to Caravan";
    vc.msg_err = vc.name_turn + " to move first.";

    // Tweak how the component tree is rendered:
    auto renderer = Renderer(component, [&] {
        try {
            if(closed) {
                if (!called_exit) {
                    called_exit = true;
                    screen.Exit();
                }
                return gen_closed();
            }
            terminal_size = Terminal::Size();
            set_name_turn(&vc, game);
            confirmed = false;

            // Error screen if less than minimum terminal dimensions
            if (terminal_size.dimx < MIN_X || terminal_size.dimy < MIN_Y) {
                user_input = "";  // Prevent user input change during error
                return gen_terminal_too_small(terminal_size);
            }

            // TODO if current turn is bot... (also, block input)

            // Create new command if ENTER key pressed (i.e., if newline)
            raw_command = user_input;
            if(raw_command.ends_with('\n')) {
                raw_command.pop_back();  // remove newline
                confirmed = true;

                user_input = "";

                if(!raw_command.empty())
                    vc.msg_notif = vc.name_turn + " entered: " + raw_command;
            }

            // Process command
            try {
                vc.command = parse_user_input(raw_command, confirmed);

                switch (vc.command.option) {
                    case NO_OPTION:
                        break;
                    case OPTION_EXIT:
                        closed = true;
                        return gen_closed();
                    default:
                        game->play_option(&vc.command);
                        // TODO display what happened
                }

            } catch(CaravanGameException &e) {
                vc.msg_err = e.what();

            } catch(CaravanInputException &e) {
                vc.msg_err = e.what();
            }

            return gen_game(game, &vc, &comp_user_input);

        } catch(...) {
            // Close gracefully on any unhandled exceptions
            closed = true;
            return gen_closed();
        }
    });

    screen.Loop(renderer);
    screen.Clear();
}

void ViewTUI::close() {
    if(!closed) {
        // TODO user_abc->close();
        // TODO user_def->close();
        game->close();

        delete user_abc;
        delete user_def;
        delete game;

        closed = true;
    }
}
