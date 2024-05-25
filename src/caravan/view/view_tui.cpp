// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/user/user.h"
#include "caravan/view/view_tui.h"

#include <memory>
#include <string>

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

const uint16_t MIN_X = 150;
const uint16_t MIN_Y = 68;

const uint16_t WIDTH_CARAVAN = 20;
const uint16_t HEIGHT_CARAVAN = 34;

const uint16_t WIDTH_DECK = 15;

const uint16_t WIDTH_CARAVAN_SLOT = WIDTH_CARAVAN;
const uint16_t HEIGHT_CARAVAN_SLOT = 4;

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
    // Pointers to users
    User *user_abc{};
    User *user_def{};
    User *user_turn{};

    // Messages to users
    std::string msg_notif;
    std::string msg_err;

    ftxui::Elements msg_move_abc;
    ftxui::Elements msg_move_def;

    // Names of users
    std::string name_abc;
    std::string name_def;
    std::string name_turn;
    std::string name_next;

    // Most recent command
    GameCommand command;

    // Colour support
    bool colour{};
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

std::shared_ptr<ftxui::Node> suit_to_text(ViewConfig *vc, Suit suit) {
    using namespace ftxui;
    std::shared_ptr<ftxui::Node> node_suit = text(suit_to_wstr(suit));

    if (vc->colour) {
        switch (suit) {
            case NO_SUIT:
                return node_suit;
            case SPADES:
            case CLUBS:
                return node_suit | color(Color::Palette16::CyanLight);
            case HEARTS:
            case DIAMONDS:
                return node_suit | color(Color::Palette16::RedLight);
            default:
                throw CaravanFatalException("Invalid suit.");
        }
    }

    return node_suit;
}

void push_card(ViewConfig *vc, ftxui::Elements *e, Card card, bool lead) {
    using namespace ftxui;

    if (card.rank == JOKER) {
        if(lead) { e->push_back(text(L" ")); }
        e->push_back(text(rank_to_wstr(card.rank, lead)));

    } else {
        e->push_back(text(rank_to_wstr(card.rank, lead)));
        e->push_back(suit_to_text(vc, card.suit));
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
            throw CaravanInputException(
                "Invalid option '" + std::string(1, c) + "', must be one of: (P)lay, (D)iscard, (C)lear.");
    }
}

void process_second(std::string input, GameCommand *command) {
    if (command->option == OPTION_PLAY or command->option == OPTION_DISCARD) {

        if (input.size() < 2) {
            throw CaravanInputException("A hand position has not been entered.");
        }

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

        if (input.size() < 2) {
            throw CaravanInputException("A caravan name has not been entered.");
        }

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

        if (input.size() < 3) {
            throw CaravanInputException("A caravan name has not been entered.");
        }

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

        if (input.size() < 4) { return; }  // optional, not an error

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

GameCommand ViewTUI::parse_user_input(std::string input, bool confirmed) {
    GameCommand command;

    if (closed) { return command; }
    if (input.empty()) { return command; }
    if (!confirmed) { return command; }

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

    return command;
}

std::shared_ptr<ftxui::Node> gen_position(uint8_t position, bool blank = false) {
    using namespace ftxui;
    return text(blank ? "" : std::to_string(position)) | borderEmpty | size(WIDTH, EQUAL, WIDTH_POSITION) | size(HEIGHT, EQUAL, HEIGHT_POSITION);
}

std::shared_ptr<ftxui::Node> gen_position_blank() {
    return gen_position(0, true);
}

std::shared_ptr<ftxui::Node> gen_card(ViewConfig *vc, Card card, bool hide, bool blank = false) {
    using namespace ftxui;
    Elements value;

    if (!blank) {
        if (hide) {
            value.push_back(text(L"###"));
        } else {
            push_card(vc, &value, card, true);
        }
    }

    return hbox(value) | (blank ? borderEmpty : borderDouble) | size(WIDTH, EQUAL, WIDTH_CARD) | size(HEIGHT, EQUAL, HEIGHT_CARD);
}

std::shared_ptr<ftxui::Node> gen_card_blank() {
    return gen_card({}, {}, false, true);
}

std::shared_ptr<ftxui::Node> gen_faces(ViewConfig *vc, Slot slot, bool blank = false) {
    using namespace ftxui;

    std::wstring ranks;
    Elements suits;

    if (!blank) {
        for (uint8_t i = 0; i < slot.i_faces; i++) {
            Rank r = slot.faces[i].rank;
            Suit s = slot.faces[i].suit;

            if(r == JOKER) {
                ranks += L"J";
                suits.push_back(text(L"O"));
            } else {
                ranks += rank_to_wstr(r, false);
                suits.push_back(suit_to_text(vc, s));
            }
        }
    }

    return vbox({
                    text(ranks),
                    hbox(suits)
                }) | borderEmpty | size(WIDTH, EQUAL, WIDTH_FACES) | size(HEIGHT, EQUAL, HEIGHT_FACES);
}

std::shared_ptr<ftxui::Node> gen_faces_blank() {
    return gen_faces({}, {}, true);
}

std::shared_ptr<ftxui::Node> gen_caravan_slot(ViewConfig *vc, uint8_t position, Slot slot, bool blank = false) {
    using namespace ftxui;
    return hbox({
                    blank ? gen_position_blank() : gen_position(position),
                    blank ? gen_card_blank() : gen_card(vc, slot.card, false),
                    blank ? gen_faces_blank() : gen_faces(vc, slot),
                }) | hcenter | size(WIDTH, EQUAL, WIDTH_CARAVAN_SLOT) | size(HEIGHT, EQUAL, HEIGHT_CARAVAN_SLOT);
}

std::shared_ptr<ftxui::Node> gen_caravan_slot_blank() {
    return gen_caravan_slot({}, 0, {}, true);
}

std::shared_ptr<ftxui::Node> gen_caravan(ViewConfig *vc, Game *game, CaravanName cn, bool top) {
    using namespace ftxui;
    std::shared_ptr<Node> content;
    Elements e;
    Elements title;

    Caravan *caravan = game->get_table()->get_caravan(cn);
    bool winning = game->is_caravan_winning(caravan->get_name());
    uint8_t caravan_size = caravan->get_size();

    for (uint8_t i = 0; i < TRACK_NUMERIC_MAX; i++) {
        if ((top && (TRACK_NUMERIC_MAX - i) <= caravan_size) || (!top && i + 1 <= caravan_size)) {
            uint8_t position = top ? TRACK_NUMERIC_MAX - i : i + 1;

            e.push_back(gen_caravan_slot(vc, position, caravan->get_slot(position)));

        } else {
            e.push_back(gen_caravan_slot_blank());
        }
    }

    content = vbox(e);

    std::function<Element(Element)> maybe_colour = winning ? (vc->colour ? color(Color::Palette16::YellowLight) : color(Color::Default)) : color(Color::Default);
    std::function<Element(Element)> maybe_colour_highlight = winning ? (vc->colour ? color(Color::Palette16::YellowLight) : color(Color::Default)) : color(Color::Default);
    if(winning) { maybe_colour_highlight = maybe_colour_highlight | underlined; }

    title.push_back(text(L" "));
    title.push_back(text(caravan_to_wstr(cn, true) + L" ") | maybe_colour);
    if (game->get_table()->get_caravan(cn)->get_size() > 0) {
        title.push_back(text(L"("));
        title.push_back(text(std::to_wstring(caravan->get_bid())) | maybe_colour_highlight);
        title.push_back(text(L", " + direction_to_wstr(caravan->get_direction()) + L", ") | maybe_colour);
        title.push_back(suit_to_text(vc, caravan->get_suit()));
        title.push_back(text(L")") | maybe_colour);
        title.push_back(text(L" "));
    }

    return window(
        hbox({title}) | hcenter | bold,
        content
    ) | center | size(WIDTH, EQUAL, WIDTH_CARAVAN) | size(HEIGHT, EQUAL, HEIGHT_CARAVAN);
}

std::shared_ptr<ftxui::Node> gen_deck_card(ViewConfig *vc, Game *game, uint8_t position, Card card, bool hide, bool blank = false) {
    using namespace ftxui;
    return hbox({
                    blank || game->get_winner() != NO_PLAYER ? gen_position_blank() : gen_position(position),
                    blank ? gen_card_blank() : gen_card(vc, card, hide),
                }) | size(HEIGHT, EQUAL, HEIGHT_CARAVAN_SLOT);
}

std::shared_ptr<ftxui::Node> gen_deck_card_blank() {
    return gen_deck_card({}, {}, 0, {}, false, true);
}

std::shared_ptr<ftxui::Node> gen_deck(ViewConfig *vc, Game *game, bool top) {
    using namespace ftxui;

    std::shared_ptr<Node> content;
    std::string title;
    Elements e;

    Player *player_abc = game->get_player(PLAYER_ABC);
    Player *player_def = game->get_player(PLAYER_DEF);

    Player *player_this = game->get_player(top ? PLAYER_DEF : PLAYER_ABC);
    User *user_this = player_this->get_name() == vc->user_abc->get_name() ? vc->user_abc : vc->user_def;

    uint8_t hand_size_abc = player_abc->get_size_hand();
    uint8_t hand_size_def = player_def->get_size_hand();
    uint8_t hand_size_turn = player_this->get_size_hand();

    uint8_t total_cards = player_this->get_size_deck() + hand_size_turn;
    uint8_t hand_max = std::max(std::max(hand_size_abc, hand_size_def), HAND_SIZE_MAX_POST_START);
    bool equalise = hand_size_abc != hand_size_def;

    // if not this user's turn and both players are human; or
    // if this user is a bot playing against a human; or
    // if there is a winner and this user is a bot
    bool hide =
        (
            game->get_winner() == NO_PLAYER &&
            vc->user_turn->get_name() != player_this->get_name() &&
            (vc->user_abc->is_human() && vc->user_def->is_human())
        ) ||
        (
            game->get_winner() == NO_PLAYER &&
            !user_this->is_human() &&
            (vc->user_abc->is_human() || vc->user_def->is_human())
        ) ||
        (
            game->get_winner() != NO_PLAYER &&
            !user_this->is_human()
        );

    for (uint8_t i = 0; i < hand_max; i++) {
        if ((top && (hand_max - i) <= hand_size_turn) || (!top && i + 1 <= hand_size_turn)) {
            uint8_t position = top ? hand_max - i : i + 1;
            Card card = player_this->get_hand()[position - 1];

            e.push_back(gen_deck_card(vc, game, position, card, hide));

        } else if (i < HAND_SIZE_MAX_POST_START || equalise) {
            e.push_back(gen_deck_card_blank());
        }
    }

    content = vbox(e);
    title = " " + (top ? vc->name_def : vc->name_abc) + " (" + std::to_string(total_cards) + ") ";

    return window(
        text(title) | hcenter | bold,
        content
    ) | size(WIDTH, EQUAL, WIDTH_DECK);
}

std::shared_ptr<ftxui::Node> gen_input(
    ViewConfig *vc,
    Game *game,
    std::shared_ptr<ftxui::ComponentBase> *comp_user_input) {
    using namespace ftxui;
    Elements e;

    bool is_top = game->get_winner() == NO_PLAYER;
    bool is_mid = !vc->msg_notif.empty() || !vc->msg_err.empty();
    bool is_low = !vc->msg_move_abc.empty() || !vc->msg_move_def.empty();

    if (is_top) {
        e.push_back(separatorEmpty());
        e.push_back(
            hbox(separatorEmpty(), text(vc->name_turn + " > "), (*comp_user_input)->Render(), separatorEmpty()));
        e.push_back(separatorEmpty());

        if(is_mid) { e.push_back(separator()); }
    }

    if (is_mid) {
        e.push_back(separatorEmpty());

        if (!vc->msg_notif.empty()) {
            e.push_back(hbox(separatorEmpty(), paragraph(vc->msg_notif), separatorEmpty()));
            e.push_back(separatorEmpty());
        }

        if (!vc->msg_err.empty()) {
            e.push_back(hbox(separatorEmpty(), paragraph(vc->msg_err), separatorEmpty()));
            e.push_back(separatorEmpty());
        }

        if(is_low) { e.push_back(separator()); }
    }

    if (is_low) {
        e.push_back(separatorEmpty());

        if (!vc->msg_move_abc.empty()) {
            e.push_back(hbox(separatorEmpty(), hbox(vc->msg_move_abc), separatorEmpty()));
            e.push_back(separatorEmpty());
        }

        if (!vc->msg_move_def.empty()) {
            e.push_back(hbox(separatorEmpty(), hbox(vc->msg_move_def), separatorEmpty()));
            e.push_back(separatorEmpty());
        }
    }

    return vbox(e) | border | size(WIDTH, EQUAL, 44);
}

std::shared_ptr<ftxui::Node> gen_game(
    ViewConfig *vc,
    Game *game,
    std::shared_ptr<ftxui::ComponentBase> *comp_user_input) {
    using namespace ftxui;
    return hbox({
                    vbox({  // GAME AREA

                             hbox({  // TOP GAME AREA
                                      gen_caravan(vc, game, CARAVAN_D, true),
                                      separatorEmpty(),
                                      separatorEmpty(),
                                      gen_caravan(vc, game, CARAVAN_E, true),
                                      separatorEmpty(),
                                      separatorEmpty(),
                                      gen_caravan(vc, game, CARAVAN_F, true),
                                  }),  // top game area

                             separatorEmpty(),

                             hbox({  // BOTTOM GAME AREA
                                      gen_caravan(vc, game, CARAVAN_A, false),
                                      separatorEmpty(),
                                      separatorEmpty(),
                                      gen_caravan(vc, game, CARAVAN_B, false),
                                      separatorEmpty(),
                                      separatorEmpty(),
                                      gen_caravan(vc, game, CARAVAN_C, false),
                                  }),  // bottom game area

                         }),  // game area

                    separatorEmpty(),
                    separatorEmpty(),
                    separatorEmpty(),
                    separatorEmpty(),
                    separatorEmpty(),

                    vbox({  // DECK AREA
                             gen_deck(vc, game, true),
                             separatorEmpty(),
                             gen_deck(vc, game, false),
                         }) | vcenter,  // deck area

                    separatorEmpty(),
                    separatorEmpty(),
                    separatorEmpty(),
                    separatorEmpty(),
                    separatorEmpty(),

                    vbox({  // INPUT AREA
                             hbox({}) | borderEmpty | size(HEIGHT, EQUAL, HEIGHT_CARAVAN),
                             separatorEmpty(),
                             gen_input(vc, game, comp_user_input)
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

ftxui::Elements get_move_description(ViewConfig *vc) {
    using namespace ftxui;
    Elements e;

    e.push_back(text(vc->name_turn));

    if (vc->command.option == OPTION_PLAY) {
        e.push_back(text(L" played "));
        push_card(vc, &e, vc->command.hand, false);

        if (vc->command.pos_caravan > 0) {
            e.push_back(text(L" on "));
            push_card(vc, &e, vc->command.board, false);
        }

        e.push_back(text(L" on " + caravan_to_wstr(vc->command.caravan_name, false) + L"."));

    } else if (vc->command.option == OPTION_DISCARD) {
        e.push_back(text(L" discarded "));
        push_card(vc, &e, vc->command.hand, false);
        e.push_back(text(L" from their hand."));

    } else {  // OPTION_CLEAR
        e.push_back(text(L" cleared " + caravan_to_wstr(vc->command.caravan_name, false) + L"."));
    }

    return e;
}

void set_current_turn(ViewConfig *vc, Game *game) {
    if (game->get_player_turn() == PLAYER_ABC) {
        vc->user_turn = vc->user_abc;

        vc->name_turn = vc->name_abc;
        vc->name_next = vc->name_def;

    } else {
        vc->user_turn = vc->user_def;

        vc->name_turn = vc->name_def;
        vc->name_next = vc->name_abc;
    }
}

/*
 * PUBLIC
 */

void ViewTUI::run() {
    using namespace ftxui;

    if (closed) { return; }

    // Screen config
    Dimensions terminal_size{};

    // User input
    std::string user_input;
    std::string raw_command;
    bool confirmed;
    std::wstring move_description;

    // Config for rendering game
    ViewConfig vc;
    vc.colour = Terminal::ColorSupport() != ftxui::Terminal::Palette1;
    vc.user_abc = user_abc;
    vc.user_def = user_def;

    // Set names of users based on who is human or not
    if (user_abc->is_human() and user_def->is_human()) {
        vc.name_abc = NAME_PL1;
        vc.name_def = NAME_PL2;

    } else if (user_abc->is_human() and !user_def->is_human()) {
        vc.name_abc = NAME_YOU;
        vc.name_def = NAME_BOT;

    } else if (!user_abc->is_human() and user_def->is_human()) {
        vc.name_abc = NAME_BOT;
        vc.name_def = NAME_YOU;

    } else {  // both are bots
        vc.name_abc = NAME_BOT1;
        vc.name_def = NAME_BOT2;
    }

    // Create screen
    ScreenInteractive screen = ScreenInteractive::Fullscreen();
    screen.TrackMouse(false);

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
    auto component = Container::Vertical({comp_user_input});

    // Initial notifications
    set_current_turn(&vc, game);
    vc.msg_notif = "Welcome to Caravan";
    vc.msg_err = vc.name_turn + " to move first.";

    // Tweak how the component tree is rendered:
    auto renderer = Renderer(component, [&] {
        try {
            if (closed) { return gen_closed(); }

            terminal_size = Terminal::Size();
            set_current_turn(&vc, game);
            confirmed = false;

            // Error screen if less than minimum terminal dimensions
            if (terminal_size.dimx < MIN_X || terminal_size.dimy < MIN_Y) {
                user_input = "";  // Prevent user input change during error
                return gen_terminal_too_small(terminal_size);
            }

            // If winner, display results
            if(game->get_winner() != NO_PLAYER) {
                std::string name_winner = game->get_winner() == user_abc->get_name() ? vc.name_abc : vc.name_def;
                user_input = "";

                vc.msg_notif = "WINNER: " + name_winner;
                vc.msg_err = "Press Esc to exit.";

                return gen_game(&vc, game, &comp_user_input);
            }

            if(vc.user_turn->is_human()) {
                // Create new command if ENTER key pressed (i.e., if newline)
                raw_command = user_input;
                if (raw_command.ends_with('\n')) {
                    raw_command.pop_back();  // remove newline
                    confirmed = true;
                    user_input = "";

                    if (!raw_command.empty()) {
                        vc.msg_notif = vc.name_turn + " entered: " + raw_command;
                    }
                }

            } else {  // user with current turn is a bot
                vc.command = vc.user_turn->generate_option(game);  // TODO generate raw command instead
                vc.msg_notif = vc.name_turn + " made its move.";
            }

            try {
                // Parse raw command to get usable command
                if (vc.user_turn->is_human()) { vc.command = parse_user_input(raw_command, confirmed); }

                switch (vc.command.option) {
                    case NO_OPTION:
                        break;
                    default:
                        // Screen refresh on game change
                        screen.PostEvent(Event::Custom);

                        // Send command to update game state
                        // Will throw exception if there is a problem with the command
                        game->play_option(&vc.command);

                        // Set new messages to notify user of game state changes
                        vc.msg_err = vc.name_next + " to move next.";

                        if (vc.user_turn->get_name() == PLAYER_ABC) {
                            vc.msg_move_abc = get_move_description(&vc);
                        } else {
                            vc.msg_move_def = get_move_description(&vc);
                        }
                }

            } catch (CaravanGameException &e) {
                vc.msg_err = e.what();

            } catch (CaravanInputException &e) {
                vc.msg_err = e.what();
            }

            return gen_game(&vc, game, &comp_user_input);

        } catch (...) {
            // Close gracefully on any unhandled exceptions
            closed = true;
            return gen_closed();
        }
    });

    renderer |= ftxui::CatchEvent([&](ftxui::Event event) {
        if (event == Event::Escape) {
            screen.Exit();
            return true;
        }
        return false;
    });

    screen.Loop(renderer);
    screen.Clear();
}

void ViewTUI::close() {
    if (!closed) {
        // TODO user_abc->close();
        // TODO user_def->close();
        game->close();

        delete user_abc;
        delete user_def;
        delete game;

        closed = true;
    }
}
