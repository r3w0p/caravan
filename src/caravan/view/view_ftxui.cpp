// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/core/exceptions.h"
#include "caravan/user/user.h"
#include "caravan/view/view_ftxui.h"

#include <memory>
#include <string>
#include <chrono>

#include "caravan/core/functions.h"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

namespace Caravan::View {
    constexpr uint16_t MIN_X = 150;
    constexpr uint16_t MIN_Y = 68;

    constexpr uint16_t WIDTH_CARAVAN = 20;
    constexpr uint16_t HEIGHT_CARAVAN = 34;

    constexpr uint16_t WIDTH_DECK = 15;

    constexpr uint16_t WIDTH_CARAVAN_SLOT = WIDTH_CARAVAN;
    constexpr uint16_t HEIGHT_CARAVAN_SLOT = 4;

    constexpr uint16_t WIDTH_POSITION = 4;
    constexpr uint16_t HEIGHT_POSITION = 2;

    constexpr uint16_t WIDTH_CARD = 5;
    constexpr uint16_t HEIGHT_CARD = 2;

    constexpr uint16_t WIDTH_FACES = 5;
    constexpr uint16_t HEIGHT_FACES = 2;

    constexpr uint8_t INPUT_MAX = 4;

    const std::string NAME_YOU = "YOU";
    const std::string NAME_BOT = "BOT";

    const std::string NAME_PL1 = "PL1";
    const std::string NAME_PL2 = "PL2";

    const std::string NAME_BOT1 = "BOT1";
    const std::string NAME_BOT2 = "BOT2";

    /*
     * PRIVATE
     */

    std::shared_ptr<ftxui::Node> suit_to_text(
        Model::Suit suit,
        bool colour
    ) {
        using namespace ftxui;
        std::shared_ptr<Node> node_suit = text(suit_to_wstr(suit));

        if (colour) {
            switch (suit) {
                case Model::NO_SUIT:
                    return node_suit;
                case Model::SPADES:
                case Model::CLUBS:
                    return node_suit | color(Color::Palette16::CyanLight);
                case Model::HEARTS:
                case Model::DIAMONDS:
                    return node_suit | color(Color::Palette16::RedLight);
                default:
                    throw CaravanFatalViewException("Invalid suit.");
            }
        }

        return node_suit;
    }

    void push_card(
        ViewFTXUI *config,
        ftxui::Elements *e,
        Model::Card card,
        bool lead
    ) {
        using namespace ftxui;

        if (card.rank == Model::JOKER) {
            // Push lead then "JO" to compensate for lack of suit
            if (lead) {
                e->push_back(text(L" "));
            }
            e->push_back(
                text(rank_to_wstr(card.rank, lead)) | color(Color::Default)
            );
        } else {
            // Push rank then suit
            e->push_back(
                text(rank_to_wstr(card.rank, lead)) | color(Color::Default)
            );
            e->push_back(suit_to_text(card.suit, config->wants_colour()));
        }
    }

    std::shared_ptr<ftxui::Node> gen_position(
        uint8_t position,
        bool blank = false
    ) {
        using namespace ftxui;
        return text(blank ? "" : std::to_string(position)) | borderEmpty |
               color(Color::Default) | size(WIDTH, EQUAL, WIDTH_POSITION) |
               size(HEIGHT, EQUAL, HEIGHT_POSITION);
    }

    std::shared_ptr<ftxui::Node> gen_position_blank() {
        return gen_position(0, true);
    }

    std::shared_ptr<ftxui::Node> gen_card(
        ViewFTXUI *config,
        Model::Card card,
        bool hide,
        bool highlight,
        bool blank = false
    ) {
        using namespace ftxui;
        std::shared_ptr<Node> ret;
        Elements value;

        if (!blank) {
            if (hide) {
                value.push_back(text(L"###"));
            } else {
                push_card(config, &value, card, true);
            }
        }

        ret = hbox(value);

        if (blank) {
            ret = ret | borderEmpty;
        } else if (highlight) {
            ret = ret | borderHeavy | (config->wants_colour()
                                       ? color(Color::Palette16::MagentaLight)
                                       : color(Color::Default));
        } else {
            ret = ret | borderDouble;
        }

        ret = ret | size(WIDTH, EQUAL, WIDTH_CARD) | size(
                  HEIGHT,
                  EQUAL,
                  HEIGHT_CARD
              );

        return ret;
    }

    std::shared_ptr<ftxui::Node> gen_card_blank() {
        return gen_card({}, {}, false, false, true);
    }

    std::shared_ptr<ftxui::Node> gen_faces(
        ViewFTXUI *config,
        const Model::Slot &slot,
        const bool blank = false
    ) {
        using namespace ftxui;

        std::wstring ranks;
        Elements suits;

        if (!blank) {
            for (uint8_t i = 0; i < slot.i_faces; i++) {
                Model::Rank r = slot.faces[i].rank;
                Model::Suit s = slot.faces[i].suit;

                if (r == Model::JOKER) {
                    ranks += L"J";
                    suits.push_back(text(L"O") | color(Color::Default));
                } else {
                    ranks += rank_to_wstr(r, false);
                    suits.push_back(suit_to_text(s, config->wants_colour()));
                }
            }
        }

        return vbox(
                   {
                       text(ranks) | color(Color::Default),
                       hbox(suits)
                   }
               ) | borderEmpty | size(WIDTH, EQUAL, WIDTH_FACES) | size(
                   HEIGHT,
                   EQUAL,
                   HEIGHT_FACES
               );
    }

    std::shared_ptr<ftxui::Node> gen_faces_blank() {
        return gen_faces({}, {}, true);
    }

    std::shared_ptr<ftxui::Node> gen_caravan_slot(
        ViewFTXUI *config,
        const uint8_t position,
        const Model::Slot &slot,
        const bool highlight,
        const bool blank = false
    ) {
        using namespace ftxui;
        std::shared_ptr<Node> ret;
        Elements e;
        e.push_back(blank ? gen_position_blank() : gen_position(position));

        if (blank) {
            e.push_back(gen_card_blank());
        } else if (highlight) {
            e.push_back(gen_card(config, slot.card, false, highlight));
        } else {
            e.push_back(
                gen_card(config, slot.card, false, highlight) | color(
                    Color::Default
                )
            );
        }

        e.push_back(blank ? gen_faces_blank() : gen_faces(config, slot));

        ret = hbox(e) | hcenter | size(WIDTH, EQUAL, WIDTH_CARAVAN_SLOT) | size(
                  HEIGHT,
                  EQUAL,
                  HEIGHT_CARAVAN_SLOT
              );

        return ret;
    }

    std::shared_ptr<ftxui::Node> gen_caravan_slot_blank() {
        return gen_caravan_slot({}, 0, {}, false, true);
    }

    std::shared_ptr<ftxui::Node> gen_caravan(
        ViewFTXUI *config,
        Model::Game *game,
        Model::CaravanName cn,
        bool top
    ) {
        using namespace ftxui;
        std::shared_ptr<Node> ret;
        std::shared_ptr<Node> content;
        Elements e;
        Elements title;

        Model::Caravan *caravan = game->get_table()->get_caravan(cn);
        uint8_t caravan_size = caravan->get_size();

        for (uint8_t i = 0; i < Model::TRACK_NUMERIC_MAX; i++) {
            if ((top && (Model::TRACK_NUMERIC_MAX - i) <= caravan_size) || (
                    !top && i + 1 <= caravan_size)) {
                uint8_t position = top ? Model::TRACK_NUMERIC_MAX - i : i + 1;

                // Highlight caravan slot if selected for placement of face card
                bool highlight =
                    config->get_highlight().option != Model::NO_OPTION &&
                    config->get_highlight().caravan_name == cn &&
                    config->get_highlight().pos_caravan == position;

                e.push_back(
                    gen_caravan_slot(
                        config,
                        position,
                        caravan->get_slot(position),
                        highlight
                    )
                );
            } else {
                e.push_back(gen_caravan_slot_blank());
            }
        }

        content = vbox(e);

        bool winning = game->is_caravan_winning(caravan->get_name());
        bool bust = game->is_caravan_bust(caravan->get_name());

        std::function<Element(Element)> maybe_colour = color(Color::Default);
        std::function<Element(Element)> maybe_colour_underlined = color(
            Color::Default
        );

        if (config->wants_colour()) {
            if (winning) {
                maybe_colour = color(Color::Palette16::YellowLight);
                maybe_colour_underlined =
                    color(Color::Palette16::YellowLight) | underlined;
            } else if (bust) {
                maybe_colour = color(Color::Palette16::RedLight);
                maybe_colour_underlined =
                    color(Color::Palette16::RedLight) | underlined;
            }
        }

        title.push_back(text(L" "));
        title.push_back(
            text(caravan_name_to_wstr(cn, true) + L" ") | maybe_colour
        );
        if (game->get_table()->get_caravan(cn)->get_size() > 0) {
            title.push_back(text(L"(") | maybe_colour);
            title.push_back(
                text(std::to_wstring(caravan->get_bid())) |
                maybe_colour_underlined
            );
            title.push_back(
                text(
                    L", " + direction_to_wstr(caravan->get_direction()) + L", "
                ) | maybe_colour
            );
            title.push_back(
                suit_to_text(caravan->get_suit(), config->wants_colour())
            );
            title.push_back(text(L")") | maybe_colour);
            title.push_back(text(L" "));
        }

        ret = window(
                  hbox({title}) | hcenter | bold,
                  content
              ) | center | size(WIDTH, EQUAL, WIDTH_CARAVAN) | size(
                  HEIGHT,
                  EQUAL,
                  HEIGHT_CARAVAN
              );

        // Highlight card if caravan selected in unconfirmed move
        bool highlight =
            config->get_highlight().option != Model::NO_OPTION &&
            config->get_highlight().caravan_name == cn;

        if (highlight) {
            ret = ret | (config->wants_colour()
                         ? color(Color::Palette16::MagentaLight)
                         : color(Color::Default));
        }

        return ret;
    }

    std::shared_ptr<ftxui::Node> gen_deck_card(
        ViewFTXUI *config,
        Model::Game *game,
        uint8_t position,
        Model::Card card,
        bool hide,
        bool highlight,
        bool blank = false
    ) {
        using namespace ftxui;
        return hbox(
                   {
                       blank || game->get_winner() != Model::NO_PLAYER
                       ? gen_position_blank() : gen_position(position),
                       blank ? gen_card_blank() : gen_card(
                           config,
                           card,
                           hide,
                           highlight
                       ),
                   }
               ) | size(HEIGHT, EQUAL, HEIGHT_CARAVAN_SLOT);
    }

    std::shared_ptr<ftxui::Node> gen_deck_card_blank() {
        return gen_deck_card({}, {}, 0, {}, false, false, true);
    }

    std::shared_ptr<ftxui::Node> gen_deck(
        ViewFTXUI *config,
        Model::Game *game,
        bool top
    ) {
        using namespace ftxui;

        std::shared_ptr<Node> content;
        std::string title;
        Elements e;

        Model::Player *player_abc = game->get_player(Model::PLAYER_ABC);
        Model::Player *player_def = game->get_player(Model::PLAYER_DEF);

        Model::Player *player_this = game->get_player(
            top ? Model::PLAYER_DEF : Model::PLAYER_ABC
        );
        User::User &user_this = player_this->get_name() == config->
                                get_user_abc().get_name()
                                ? config->get_user_abc()
                                : config->get_user_def();
        User::User &user_other = player_this->get_name() == config->
                                 get_user_abc().get_name()
                                 ? config->get_user_def()
                                 : config->get_user_abc();

        uint8_t hand_size_abc = player_abc->get_size_hand();
        uint8_t hand_size_def = player_def->get_size_hand();
        uint8_t hand_size_turn = player_this->get_size_hand();

        uint8_t total_cards = player_this->get_size_deck() + hand_size_turn;
        uint8_t hand_max = std::max(
            std::max(hand_size_abc, hand_size_def),
            Model::HAND_SIZE_MAX_POST_START
        );
        bool equalise = hand_size_abc != hand_size_def;

        // if not cheating; or
        // if not this user's turn and both players are human; or
        // if this user is a bot playing against a human; or
        // if there is a winner and this user is a bot playing against a human
        bool hide =
            !config->is_cheating() && (
                (
                    game->get_winner() == Model::NO_PLAYER &&
                    config->get_user_turn()->get_name() != player_this->
                    get_name() &&
                    (config->get_user_abc().is_human() && config->get_user_def()
                     .is_human())
                ) ||
                (
                    game->get_winner() == Model::NO_PLAYER &&
                    !user_this.is_human() &&
                    (config->get_user_abc().is_human() || config->get_user_def()
                     .is_human())
                ) ||
                (
                    game->get_winner() != Model::NO_PLAYER &&
                    (!user_this.is_human() && user_other.is_human())
                ));

        for (uint8_t i = 0; i < hand_max; i++) {
            if ((top && (hand_max - i) <= hand_size_turn) || (
                    !top && i + 1 <= hand_size_turn)) {
                uint8_t position = top ? hand_max - i : i + 1;
                Model::Card card = player_this->get_from_hand_at(position);

                // Highlight card if it is this player's turn and unconfirmed
                // move wants to use this hand card
                bool highlight =
                    config->get_user_turn()->get_name() == player_this->
                    get_name() &&
                    config->get_highlight().option != Model::NO_OPTION &&
                    config->get_highlight().pos_hand == position;

                e.push_back(
                    gen_deck_card(config, game, position, card, hide, highlight)
                );
            } else if (i < Model::HAND_SIZE_MAX_POST_START || equalise) {
                e.push_back(gen_deck_card_blank());
            }
        }

        content = vbox(e);
        title = " " + (top ? config->get_name_def() : config->get_name_abc()) +
                " (" + std::to_string(total_cards) + ") ";

        return window(
                   text(title) | hcenter | bold,
                   content
               ) | size(WIDTH, EQUAL, WIDTH_DECK);
    }

    std::shared_ptr<ftxui::Node> gen_input(
        ViewFTXUI *config,
        Model::Game *game,
        std::shared_ptr<ftxui::ComponentBase> *comp_user_input
    ) {
        using namespace ftxui;
        Elements e;

        bool is_top = game->get_winner() == Model::NO_PLAYER;
        bool is_mid = !config->get_msg_main().empty() || !config->
                      get_msg_important().empty();
        bool is_low = !config->get_msg_move_abc().empty() || !config->
                      get_msg_move_def().empty();

        if (is_top) {
            e.push_back(separatorEmpty());
            e.push_back(
                hbox(
                    separatorEmpty(),
                    text(config->get_name_turn() + " > "),
                    (*comp_user_input)->Render(),
                    separatorEmpty()
                )
            );
            e.push_back(separatorEmpty());

            if (is_mid) {
                e.push_back(separator());
            }
        }

        if (is_mid) {
            e.push_back(separatorEmpty());

            if (!config->get_msg_main().empty()) {
                e.push_back(
                    hbox(
                        separatorEmpty(),
                        paragraph(config->get_msg_main()),
                        separatorEmpty()
                    )
                );
                e.push_back(separatorEmpty());
            }

            if (!config->get_msg_important().empty()) {
                e.push_back(
                    hbox(
                        separatorEmpty(),
                        paragraph(config->get_msg_important()),
                        separatorEmpty()
                    )
                );
                e.push_back(separatorEmpty());
            }

            if (is_low) {
                e.push_back(separator());
            }
        }

        if (is_low) {
            e.push_back(separatorEmpty());

            if (!config->get_msg_move_abc().empty()) {
                e.push_back(
                    hbox(
                        separatorEmpty(),
                        hbox(config->get_msg_move_abc()),
                        separatorEmpty()
                    )
                );
                e.push_back(separatorEmpty());
            }

            if (!config->get_msg_move_def().empty()) {
                e.push_back(
                    hbox(
                        separatorEmpty(),
                        hbox(config->get_msg_move_def()),
                        separatorEmpty()
                    )
                );
                e.push_back(separatorEmpty());
            }
        }

        return vbox(e) | border | size(WIDTH, EQUAL, 44);
    }

    std::shared_ptr<ftxui::Node> gen_game(
        ViewFTXUI *config,
        Model::Game *game,
        std::shared_ptr<ftxui::ComponentBase> *comp_user_input
    ) {
        using namespace ftxui;
        return hbox(
                   {
                       vbox(
                           {
                               // GAME AREA

                               hbox(
                                   {
                                       // TOP GAME AREA
                                       gen_caravan(
                                           config,
                                           game,
                                           Model::CARAVAN_D,
                                           true
                                       ),
                                       separatorEmpty(),
                                       separatorEmpty(),
                                       gen_caravan(
                                           config,
                                           game,
                                           Model::CARAVAN_E,
                                           true
                                       ),
                                       separatorEmpty(),
                                       separatorEmpty(),
                                       gen_caravan(
                                           config,
                                           game,
                                           Model::CARAVAN_F,
                                           true
                                       ),
                                   }
                               ),
                               // top game area

                               separatorEmpty(),

                               hbox(
                                   {
                                       // BOTTOM GAME AREA
                                       gen_caravan(
                                           config,
                                           game,
                                           Model::CARAVAN_A,
                                           false
                                       ),
                                       separatorEmpty(),
                                       separatorEmpty(),
                                       gen_caravan(
                                           config,
                                           game,
                                           Model::CARAVAN_B,
                                           false
                                       ),
                                       separatorEmpty(),
                                       separatorEmpty(),
                                       gen_caravan(
                                           config,
                                           game,
                                           Model::CARAVAN_C,
                                           false
                                       ),
                                   }
                               ),
                               // bottom game area

                           }
                       ),
                       // game area

                       separatorEmpty(),
                       separatorEmpty(),
                       separatorEmpty(),
                       separatorEmpty(),
                       separatorEmpty(),

                       vbox(
                           {
                               // DECK AREA
                               gen_deck(config, game, true),
                               separatorEmpty(),
                               gen_deck(config, game, false),
                           }
                       ) | vcenter,
                       // deck area

                       separatorEmpty(),
                       separatorEmpty(),
                       separatorEmpty(),
                       separatorEmpty(),
                       separatorEmpty(),

                       vbox(
                           {
                               // INPUT AREA
                               hbox({}) | borderEmpty | size(
                                   HEIGHT,
                                   EQUAL,
                                   HEIGHT_CARAVAN
                               ),
                               separatorEmpty(),
                               gen_input(config, game, comp_user_input)
                           }
                       ),
                       // input area

                   }
               ) | center; // outermost area
    }

    std::shared_ptr<ftxui::Node> gen_terminal_too_small(
        ftxui::Dimensions terminal_size
    ) {
        using namespace ftxui;
        return vbox(
                   {
                       text("Terminal too small"),
                       separatorEmpty(),
                       text(
                           "Width:  " + std::to_string(terminal_size.dimx) +
                           " / " + std::to_string(MIN_X)
                       ),
                       text(
                           "Height: " + std::to_string(terminal_size.dimy) +
                           " / " + std::to_string(MIN_Y)
                       ),
                       separatorEmpty(),
                       text("Resize terminal or press Esc"),
                   }
               ) | center;
    }

    std::shared_ptr<ftxui::Node> gen_closed(ViewFTXUI *config) {
        using namespace ftxui;
        Elements e;

        if (!config->get_msg_fatal().empty()) {
            e.push_back(text(config->get_msg_fatal()));
            e.push_back(separatorEmpty());
        }
        e.push_back(text("Press Esc to exit."));

        return vbox(e) | center;
    }

    ftxui::Elements get_move_description(ViewFTXUI *config) {
        using namespace ftxui;
        Elements e;

        e.push_back(text(config->get_name_turn()));

        if (config->get_move().option == Model::OPTION_PLAY) {
            e.push_back(text(L" played "));
            push_card(config, &e, config->get_move().hand, false);

            if (config->get_move().hand.is_face_card() && config->get_move().
                pos_caravan > 0) {
                e.push_back(text(L" on "));
                push_card(config, &e, config->get_move().board, false);
            }

            e.push_back(
                text(
                    L" on " + caravan_name_to_wstr(
                        config->get_move().caravan_name,
                        false
                    ) + L"."
                )
            );
        } else if (config->get_move().option == Model::OPTION_DISCARD) {
            e.push_back(text(L" discarded "));
            push_card(config, &e, config->get_move().hand, false);
            e.push_back(text(L" from their hand."));
        } else {
            // OPTION_CLEAR
            e.push_back(
                text(
                    L" cleared " + caravan_name_to_wstr(
                        config->get_move().caravan_name,
                        false
                    ) + L"."
                )
            );
        }

        return e;
    }

    /*
     * PROTECTED
     */

    void ViewFTXUI::update_current_turn() {
        if (game.get_player_turn() == Model::PLAYER_ABC) {
            user_turn = &user_abc;
            user_next = &user_def;

            name_turn = name_abc;
            name_next = name_def;
        } else {
            user_turn = &user_def;
            user_next = &user_abc;

            name_turn = name_def;
            name_next = name_abc;
        }
    }

    /*
     * PUBLIC
     */

    void ViewFTXUI::run() {
        using namespace ftxui;

        bool closed = false;

        // Screen config
        Dimensions terminal_size{};

        // User input
        std::string user_input;
        std::string raw_command;
        bool confirmed;
        std::wstring move_description;

        // Make sure colour is available if it has been requested
        if (colour) {
            colour = Terminal::ColorSupport() != Terminal::Palette1;
        }

        // Set names of users based on who is human or not
        if (user_abc.is_human() and user_def.is_human()) {
            name_abc = NAME_PL1;
            name_def = NAME_PL2;
        } else if (user_abc.is_human() and !user_def.is_human()) {
            name_abc = NAME_YOU;
            name_def = NAME_BOT;
        } else if (!user_abc.is_human() and user_def.is_human()) {
            name_abc = NAME_BOT;
            name_def = NAME_YOU;
        } else {
            // both are bots
            name_abc = NAME_BOT1;
            name_def = NAME_BOT2;
        }

        // Create screen
        ScreenInteractive screen = ScreenInteractive::Fullscreen();
        screen.TrackMouse(false);

        // User input component
        Component comp_user_input = Input(&user_input, "");

        // Ensure input is alphanumeric only
        comp_user_input |= CatchEvent(
            [&](const Event &event) {
                return event.is_character() && !std::isalnum(
                           event.character()[0]
                       );
            }
        );

        // Ensure maximum input length
        comp_user_input |= CatchEvent(
            [&](const Event &event) {
                return event.is_character() && user_input.size() >= INPUT_MAX;
            }
        );

        // Component tree
        auto component = Container::Vertical({comp_user_input});

        // Initial notifications
        update_current_turn();
        msg_main = "Welcome to Caravan";
        msg_important = name_turn + " to move first.";

        // Monitor bot delay
        uint64_t time_bot_start = time_milliseconds();
        uint64_t time_bot_end;

        // Tweak how the component tree is rendered:
        auto renderer = Renderer(
            component,
            [&] {
                screen.SetCursor(
                    Screen::Cursor({.shape = Screen::Cursor::Hidden})
                );

                if (closed) {
                    return gen_closed(this);
                }

                try {
                    // Reset values
                    terminal_size = Terminal::Size();
                    update_current_turn();
                    confirmed = false;
                    move = {};
                    highlight = {};

                    // Error screen if less than minimum terminal dimensions
                    if (terminal_size.dimx < MIN_X || terminal_size.dimy <
                        MIN_Y) {
                        user_input = "";
                        // Prevent user input change during error
                        return gen_terminal_too_small(terminal_size);
                    }

                    // If winner, display results
                    if (game.get_winner() != Model::NO_PLAYER) {
                        std::string name_winner =
                            game.get_winner() == user_abc.get_name() ? name_abc
                            : name_def;

                        user_input = "";
                        // Prevent user input change at game end

                        msg_main = "WINNER: " + name_winner;
                        msg_important = "Press Esc to exit.";

                        return gen_game(this, &game, &comp_user_input);
                    }

                    // Get input from human or bot
                    if (user_turn->is_human()) {
                        raw_command = user_input;

                        // Create new move if ENTER key pressed (i.e., if newline)
                        if (raw_command.ends_with('\n')) {
                            // A confirmed move ready to send to the game model
                            raw_command.pop_back(); // remove newline
                            user_input = "";
                            confirmed = true;
                        }
                    } else {
                        // user with current turn is a bot
                        user_input = "";
                        time_bot_end = time_milliseconds();

                        if ((time_bot_end - time_bot_start) >=
                            bot_delay_millis) {
                            // Bot delay has elapsed, make move
                            raw_command = user_turn->request_move(&game);
                            confirmed = true;
                        } else {
                            // Bot is still thinking of its next move
                            msg_important = name_turn + " is thinking...";
                            // Event needed so that delay checks occur
                            screen.PostEvent(Event::Custom);
                        }
                    }

                    // Show confirmed input
                    if (confirmed && !raw_command.empty()) {
                        msg_main = name_turn + " entered: " + raw_command;
                    }

                    // Send input to controller
                    // TODO value then error or vice versa?
                    auto [gm, err] = ctrl.on_user_input(raw_command, confirmed);
                    raw_command = "";

                    if (confirmed && gm.option != Model::NO_OPTION) {  // TODO is NO_OPTION check needed?

                        // Confirmed move was erroneous
                        if (!err.empty()) {
                            // TODO what happens when bot messes up? test this
                            if (user_turn->is_human()) {
                                // Notify human user of illegal move
                                msg_important = err;
                            }

                        } else {
                            // Log successful move
                            move = gm;

                            // Set message to log next player's turn
                            msg_important = name_next + " to move next.";

                            if (user_turn->get_name() == Model::PLAYER_ABC) {
                                msg_move_abc = get_move_description(this);
                            } else {
                                msg_move_def = get_move_description(this);
                            }

                            // If next user is bot, start logging bot delay
                            if (!user_next->is_human()) {
                                time_bot_start = time_milliseconds();
                            }
                        }

                        // Screen refresh on game change
                        screen.PostEvent(Event::Custom);

                    } else {
                        // An incomplete move that can be used to highlight
                        // areas of the board as a hint to the player
                        highlight = gm;
                    }

                    return gen_game(this, &game, &comp_user_input);
                } catch (CaravanFatalException &e) {
                    // Close gracefully on any unhandled exceptions
                    msg_fatal = e.what();
                    closed = true;
                    return gen_closed(this);
                } catch (std::exception &e) {
                    // Close gracefully on any unhandled exceptions
                    msg_fatal = "A fatal error occurred.";
                    closed = true;
                    return gen_closed(this);
                }
            }
        );

        renderer |= CatchEvent(
            [&](const Event &event) {
                if (event == Event::Escape) {
                    screen.Exit();
                    return true;
                }
                return false;
            }
        );

        screen.Loop(renderer);
        screen.Clear();
    }
}
