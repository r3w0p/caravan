// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "view.h"
#include <curses.h>

WINDOW *create_window(uint16_t h, uint16_t w, uint16_t y, uint16_t x) {
    WINDOW * local_win;
    local_win = newwin(h, w, y, x);
    return local_win;
}

WINDOW *create_window_caravan(uint16_t y, uint16_t x) {
    return create_window(31, 19, y, x);
}


WINDOW *create_window_player(uint16_t y, uint16_t x) {
    return create_window(31, 19, y, x);
}


std::string player_name_to_str(PlayerName pn) {
    switch (pn) {
        case PLAYER_A:
            return "PLAYER A";
        case PLAYER_B:
            return "PLAYER B";
        default:
            throw CaravanFatalException("Invalid player name.");
    }
}

wchar_t suit_to_wchar_t(Suit s) {
    switch (s) {
        case NO_SUIT:
            return L' ';
        case SPADES:
            return L'♠';
        case CLUBS:
            return L'♣';
        case HEARTS:
            return L'♥';
        case DIAMONDS:
            return L'♦';
        default:
            throw CaravanFatalException("Invalid suit.");
    }
}

std::string rank_to_str(Rank r, bool lead) {
    switch (r) {
        case ACE:
            return lead ? " A" : "A";
        case TWO:
            return lead ? " 2" : "2";
        case THREE:
            return lead ? " 3" : "3";
        case FOUR:
            return lead ? " 4" : "4";
        case FIVE:
            return lead ? " 5" : "5";
        case SIX:
            return lead ? " 6" : "6";
        case SEVEN:
            return lead ? " 7" : "7";
        case EIGHT:
            return lead ? " 8" : "8";
        case NINE:
            return lead ? " 9" : "9";
        case TEN:
            return "10";
        case JACK:
            return lead ? " J" : "J";
        case QUEEN:
            return lead ? " Q" : "Q";
        case KING:
            return lead ? " K" : "K";
        case JOKER:
            return "JO";
        default:
            throw CaravanFatalException("Invalid rank.");
    }
}

std::string card_num_to_str(uint8_t num) {
    switch (num) {
        case 1:
            return " 1";
        case 2:
            return " 2";
        case 3:
            return " 3";
        case 4:
            return " 4";
        case 5:
            return " 5";
        case 6:
            return " 6";
        case 7:
            return " 7";
        case 8:
            return " 8";
        case 9:
            return " 9";
        case 10:
            return "10";
        default:
            throw CaravanFatalException("Invalid card number.");
    }
}

void print_suit(WINDOW *win, uint16_t y, uint16_t x, Suit s) {
    short colour_pair;
    wchar_t wstr[2];
    bool stub_has_colour = true;

    if(stub_has_colour) {
        switch (s) {
            case DIAMONDS:
            case HEARTS:
                colour_pair = PAIR_RED_BLACK;
                break;
            case CLUBS:
            case SPADES:
                colour_pair = PAIR_CYAN_BLACK;
                break;
            default:
                return;
        }

        wattron(win, COLOR_PAIR(colour_pair));
    }

    wstr[0] = suit_to_wchar_t(s);
    wstr[1] = L'\0';

    if(mvwaddwstr(win, y, x, wstr) == ERR)
        throw CaravanFatalException(
                "Failed to print suit: mvwaddwstr error.");

    if(stub_has_colour)
        wattroff(win, COLOR_PAIR(colour_pair));
}

void print_card(WINDOW *win, uint16_t y, uint16_t x,
                Card c, uint8_t num, bool up, bool conceal) {

    if(!up) {
        if(num == 1)
            mvwprintw(win, y, x, " ___ ");
        else
            mvwprintw(win, y, x, "|___|");
    } else
        mvwprintw(win, y, x, " ___ ");

    mvwprintw(win, y+1, x, "|   |");

    mvwprintw(win, y+2, x, "|");

    if(!conceal) {
        mvwprintw(win, y + 2, x + 1, rank_to_str(c.rank, true).c_str());
        print_suit(win, y + 2, x + 3, c.suit);
    } else
        mvwprintw(win, y + 2, x + 1, "###");

    mvwprintw(win, y+2, x+4, "|");

    mvwprintw(win, y+3, x, "|___|");
}

void print_faces(WINDOW *win,
                 uint16_t y, uint16_t x,
                 Faces f, uint8_t i_faces) {
    Card c;
    std::string r;

    for(uint8_t i = 0; i < i_faces; i++) {
        c = f[i];
        r = c.rank == JOKER ? "J" : rank_to_str(c.rank, false);

        mvwprintw(win, y, x+i, "_");
        mvwprintw(win, y+1, x+i, r.c_str());

        if(c.suit != NO_SUIT)
            print_suit(win, y+2, x+i, c.suit);
        else
            mvwprintw(win, y+2, x+i, "O");
    }
}

ViewCLI::ViewCLI() {
    initscr();
    raw();
    getmaxyx(stdscr, r_max, c_max);
    has_colour = has_colors();

    if(has_colour) {
        start_color();
        init_pair(PAIR_WHITE_BLACK, COLOR_WHITE, COLOR_BLACK);
        init_pair(PAIR_RED_BLACK, COLOR_RED, COLOR_BLACK);
        init_pair(PAIR_CYAN_BLACK, COLOR_CYAN, COLOR_BLACK);
    }
}

void ViewCLI::close() {
    endwin();
}

void print_caravan_down(WINDOW *win, Table *t, CaravanName cn) {
    uint8_t offset;
    uint8_t amt = 3;

    uint16_t cvn_bid = t->get_caravan_bid(cn);
    Suit cvn_suit = t->get_caravan_suit(cn);
    Direction cvn_dir = t->get_caravan_direction(cn);
    uint8_t cvn_size = t->get_caravan_size(cn);
    Slot slot;
    uint8_t pos;

    if(cvn_bid > 0)
        mvwprintw(win, 1, 0, std::to_string(cvn_bid).c_str());

    if(cvn_suit != NO_SUIT)
        print_suit(win, 2, 0, cvn_suit);

    if(cvn_dir != ANY)
        mvwprintw(win, 3, 0, cvn_dir == ASCENDING ? "ASC" : "DES");

    for(uint8_t i = 0; i < cvn_size; i++) {
        pos = i+1;
        slot = t->get_slot_at(cn, pos);
        offset = amt * i;

        mvwprintw(win, offset + 2, 5, card_num_to_str(pos).c_str());
        print_card(win, offset, 8, slot.card, pos, false, false);
        print_faces(win, offset, 14, slot.faces, slot.i_faces);
    }
}

void print_caravan_up(WINDOW *win, Table *t, CaravanName cn) {
    uint8_t offset;
    uint8_t amt = 3;

    uint16_t cvn_bid = t->get_caravan_bid(cn);
    Suit cvn_suit = t->get_caravan_suit(cn);
    Direction cvn_dir = t->get_caravan_direction(cn);
    uint8_t cvn_size = t->get_caravan_size(cn);
    Slot slot;
    uint8_t pos;

    if(cvn_dir != ANY)
        mvwprintw(win, (amt * 9) + 1, 0, cvn_dir == ASCENDING ? "ASC" : "DES");

    if(cvn_suit != NO_SUIT)
        print_suit(win, (amt * 9) + 2, 0, cvn_suit);

    if(cvn_bid > 0)
        mvwprintw(win, (amt * 9) + 3, 0, std::to_string(cvn_bid).c_str());

    for(uint8_t i = 0; i < cvn_size; i++) {
        pos = i+1;
        slot = t->get_slot_at(cn, pos);
        offset = amt * (10 - pos);

        mvwprintw(win, offset + 2, 5, card_num_to_str(pos).c_str());
        print_card(win, offset, 8, slot.card, pos, true, false);
        print_faces(win, offset, 14, slot.faces, slot.i_faces);
    }
}

void print_player_up(WINDOW *win, Player *p, bool conceal) {
    uint8_t offset;
    uint8_t amt = 3;
    uint8_t pos;
    Card card;
    uint8_t xoff;
    uint8_t yoff;

    uint8_t size_deck = p->get_size_deck();
    uint8_t size_hand = p->get_size_hand();

    if(size_deck > 0) {
        yoff = amt * 9;

        if(size_deck < 10)
            xoff = 3;
        else if(size_deck < 100)
            xoff = 2;
        else
            xoff = 1;

        mvwprintw(win, yoff, 0, " ___ ");
        mvwprintw(win, yoff + 1, 0, "|   |");

        mvwprintw(win, yoff + 2, 0, "|");
        mvwprintw(win, yoff + 2, xoff, std::to_string(size_deck).c_str());
        mvwprintw(win, yoff + 2, 4, "|");

        mvwprintw(win, yoff + 3, 0, "|___|");

        if(size_deck > 1) {
            mvwprintw(win, yoff-1, 0, " ___ ");
            mvwprintw(win, yoff, 0, "/");
            mvwprintw(win, yoff, 4, "/|");
            mvwprintw(win, yoff+1, 5, "|");
            mvwprintw(win, yoff+2, 5, "|");
            mvwprintw(win, yoff+3, 5, "/");
        }
    }

    for(uint8_t i = 0; i < size_hand; i++) {
        pos = i+1;
        card = p->get_from_hand_at(pos);
        offset = amt * (10 - pos);

        if(!conceal)
            mvwprintw(win, offset + 2, 5 + 6, card_num_to_str(pos).c_str());

        print_card(win, offset, 8 + 6, card, pos, true, conceal);
    }
}

void print_player_down(WINDOW *win, Player *p, bool conceal) {
    uint8_t offset;
    uint8_t amt = 3;
    uint8_t pos;
    Card card;
    uint8_t xoff;

    uint8_t size_deck = p->get_size_deck();
    uint8_t size_hand = p->get_size_hand();

    if(size_deck > 0) {
        if(size_deck < 10)
            xoff = 3;
        else if(size_deck < 100)
            xoff = 2;
        else
            xoff = 1;

        mvwprintw(win, 0, 0, " ___ ");
        mvwprintw(win, 1, 0, "|   |");

        mvwprintw(win, 2, 0, "|");
        mvwprintw(win, 2, xoff, std::to_string(size_deck).c_str());
        mvwprintw(win, 2, 4, "|");

        mvwprintw(win, 3, 0, "|___|");

        if(size_deck > 1) {
            mvwprintw(win, 1, 5, "\\");
            mvwprintw(win, 2, 5, "|");
            mvwprintw(win, 3, 5, "|");
            mvwprintw(win, 4, 0, "\\___\\|");
        }
    }

    for(uint8_t i = 0; i < size_hand; i++) {
        pos = i+1;
        card = p->get_from_hand_at(pos);
        offset = amt * i;

        if(!conceal)
            mvwprintw(win, offset + 2, 5 + 6, card_num_to_str(pos).c_str());

        print_card(win, offset, 8 + 6, card, pos, false, conceal);
    }
}

// TODO update_table

// TODO update_window_caravan =>
//  update_window_caravan_up, update_window_caravan_down

// TODO update_window_hand =>
//  update_window_hand_up, update_window_hand_down
//  bool concealed (depends on who is playing next, always concealed if bot?)

// TODO update_window_deck =>
//  update_window_deck_up, update_window_deck_down


void update_table(WINDOW *win) {
    // TOP
    mvwprintw(win, 32, 0, "|");
    mvwprintw(win, 31, 0, "|");

    mvwprintw(win, 32, 1, "-----------");
    mvwprintw(win, 32, 12, "[D]");
    mvwprintw(win, 32, 15, "-----------");

    mvwprintw(win, 32, 26, "|");
    mvwprintw(win, 31, 26, "|");

    mvwprintw(win, 32, 27, "-----------");
    mvwprintw(win, 32, 38, "[E]");
    mvwprintw(win, 32, 41, "-----------");

    mvwprintw(win, 32, 52, "|");
    mvwprintw(win, 31, 52, "|");

    mvwprintw(win, 32, 53, "-----------");
    mvwprintw(win, 32, 64, "[F]");
    mvwprintw(win, 32, 67, "-----------");

    mvwprintw(win, 32, 78, "|");
    mvwprintw(win, 31, 78, "|");

    mvwprintw(win, 32, 84, "|");
    mvwprintw(win, 31, 84, "|");

    mvwprintw(win, 32, 85, "---------");
    mvwprintw(win, 32, 94, "[BOT]");
    mvwprintw(win, 32, 99, "---------");

    mvwprintw(win, 32, 108, "|");
    mvwprintw(win, 31, 108, "|");

    // BOTTOM
    mvwprintw(win, 34, 0, "|");
    mvwprintw(win, 35, 0, "|");

    mvwprintw(win, 34, 1, "-----------");
    mvwprintw(win, 34, 12, "[A]");
    mvwprintw(win, 34, 15, "-----------");

    mvwprintw(win, 34, 26, "|");
    mvwprintw(win, 35, 26, "|");

    mvwprintw(win, 34, 27, "-----------");
    mvwprintw(win, 34, 38, "[B]");
    mvwprintw(win, 34, 41, "-----------");

    mvwprintw(win, 34, 52, "|");
    mvwprintw(win, 35, 52, "|");

    mvwprintw(win, 34, 53, "-----------");
    mvwprintw(win, 34, 64, "[C]");
    mvwprintw(win, 34, 67, "-----------");

    mvwprintw(win, 34, 78, "|");
    mvwprintw(win, 35, 78, "|");

    mvwprintw(win, 34, 84, "|");
    mvwprintw(win, 35, 84, "|");

    mvwprintw(win, 34, 85, "---------");
    mvwprintw(win, 34, 94, "[YOU]");
    mvwprintw(win, 34, 99, "---------");

    mvwprintw(win, 34, 108, "|");
    mvwprintw(win, 35, 108, "|");
}

void ViewCLI::update(Engine *e, User *ua, User *ub) {
    WINDOW* win_cvn_a = create_window_caravan(35, 3);
    WINDOW* win_cvn_b = create_window_caravan(35, 29);
    WINDOW* win_cvn_c = create_window_caravan(35, 55);

    WINDOW* win_cvn_d = create_window_caravan(0, 3);
    WINDOW* win_cvn_e = create_window_caravan(0, 29);
    WINDOW* win_cvn_f = create_window_caravan(0, 55);

    WINDOW* win_player_b = create_window_player(0, 87);
    WINDOW* win_player_a = create_window_player(35, 87);

    refresh();


    e->get_table()->play_numeric_card(CARAVAN_D, {HEARTS, TWO});
    e->get_table()->play_numeric_card(CARAVAN_D, {SPADES, FOUR});

    e->get_table()->play_face_card(CARAVAN_D, {CLUBS, KING}, 2);
    e->get_table()->play_face_card(CARAVAN_D, {CLUBS, KING}, 2);
    e->get_table()->play_face_card(CARAVAN_D, {CLUBS, KING}, 2);
    e->get_table()->play_face_card(CARAVAN_D, {CLUBS, KING}, 2);
    e->get_table()->play_face_card(CARAVAN_D, {CLUBS, KING}, 2);


    e->get_table()->play_numeric_card(CARAVAN_A, {HEARTS, ACE});
    e->get_table()->play_numeric_card(CARAVAN_A, {SPADES, TWO});
    e->get_table()->play_numeric_card(CARAVAN_A, {SPADES, THREE});
    e->get_table()->play_numeric_card(CARAVAN_A, {SPADES, FOUR});
    e->get_table()->play_numeric_card(CARAVAN_A, {CLUBS, FIVE});
    e->get_table()->play_numeric_card(CARAVAN_A, {SPADES, SIX});
    e->get_table()->play_numeric_card(CARAVAN_A, {SPADES, SEVEN});
    e->get_table()->play_numeric_card(CARAVAN_A, {DIAMONDS, EIGHT});
    e->get_table()->play_numeric_card(CARAVAN_A, {SPADES, NINE});
    e->get_table()->play_numeric_card(CARAVAN_A, {SPADES, TEN});

    e->get_table()->play_face_card(CARAVAN_A, {CLUBS, KING}, 2);
    e->get_table()->play_face_card(CARAVAN_A, {CLUBS, KING}, 2);
    e->get_table()->play_face_card(CARAVAN_A, {CLUBS, KING}, 2);
    e->get_table()->play_face_card(CARAVAN_A, {CLUBS, KING}, 2);
    e->get_table()->play_face_card(CARAVAN_A, {CLUBS, KING}, 2);


    update_table(stdscr);
    wrefresh(stdscr);

    print_caravan_up(win_cvn_d, e->get_table(), CARAVAN_A);
    wrefresh(win_cvn_d);

    print_caravan_up(win_cvn_e, e->get_table(), CARAVAN_A);
    wrefresh(win_cvn_e);

    print_caravan_up(win_cvn_f, e->get_table(), CARAVAN_A);
    wrefresh(win_cvn_f);

    print_caravan_down(win_cvn_a, e->get_table(), CARAVAN_A);
    wrefresh(win_cvn_a);

    print_caravan_down(win_cvn_b, e->get_table(), CARAVAN_A);
    wrefresh(win_cvn_b);

    print_caravan_down(win_cvn_c, e->get_table(), CARAVAN_A);
    wrefresh(win_cvn_c);


    Player *pa = new Player(PLAYER_A, DeckBuilder::build_caravan_deck(30, 1, true));
    Player *pb = new Player(PLAYER_B, DeckBuilder::build_caravan_deck(30, 1, true));

    // TODO never conceal if: (1) both bots; (2) one bot one human
    bool conceal_a = !ua->is_human() or (ua->is_human() and e->get_player_turn() != pa->get_name());
    bool conceal_b = !ub->is_human() or (ub->is_human() and e->get_player_turn() != pb->get_name());

    print_player_up(win_player_b, pb, conceal_b);
    wrefresh(win_player_b);


    print_player_down(win_player_a, pa, conceal_a);
    wrefresh(win_player_a);

    getch();
}

void ViewCLI::message(std::string msg) {
    // TODO implementation
}

GameOption ViewCLI::option(Engine *e, User *u) {
    GameOption go;
    char input[6];
    bool reached_end_ok;

    if (e->is_closed())
        throw CaravanFatalException("The game has already closed.");

    if(!u->is_human())
        return ((UserBot*) u)->generate_option(e);

    reached_end_ok = false;

    do {
        wmove(stdscr, VIEW_STDSCR_ROW_OPTION, 0);
        wclrtoeol(stdscr);

        wmove(stdscr, VIEW_STDSCR_ROW_OPTION, 0);
        wprintw(stdscr, "[%s] > ", player_name_to_str(u->get_name()).c_str());
        wrefresh(stdscr);

        wscanw(stdscr, "%5s", input);

        /*
         * FIRST
         * - OPTION TYPE
         */

        switch (input[0]) {
            case 'P':
            case 'p':
                go.type = OPTION_PLAY;
                /*
                 * P2F
                 * "Play numeric card at hand pos 2 onto caravan F"
                 *
                 * P4F10
                 * "Play face card at hand pos 4 onto caravan F, slot 10"
                 */
                break;
            case 'D':
            case 'd':
                go.type = OPTION_DISCARD;
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
                go.type = OPTION_CLEAR;
                break;
            default:
                message("Invalid option type.");
                continue;  // mandatory: ask again...
        }

        /*
         * SECOND
         * - HAND POSITION or
         * - CARAVAN NAME
         */

        if (go.type == OPTION_PLAY or go.type == OPTION_DISCARD) {
            switch (input[1]) {
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                    go.pos_hand = (uint8_t) input[1] - '0';
                    break;
                default:
                    message("Invalid hand position.");
                    continue;  // mandatory for play/discard: ask again...
            }

        } else if (go.type == OPTION_CLEAR) {
            switch (input[1]) {
                case 'A':
                case 'a':
                    go.caravan_name = CARAVAN_A;
                    break;
                case 'B':
                case 'b':
                    go.caravan_name = CARAVAN_B;
                    break;
                case 'C':
                case 'c':
                    go.caravan_name = CARAVAN_C;
                    break;
                case 'D':
                case 'd':
                    go.caravan_name = CARAVAN_D;
                    break;
                case 'E':
                case 'e':
                    go.caravan_name = CARAVAN_E;
                    break;
                case 'F':
                case 'f':
                    go.caravan_name = CARAVAN_F;
                    break;
                default:
                    message("Invalid caravan name.");
                    continue;  // mandatory for clear: ask again...
            }
        }

        // Discard/clear does not require caravan name/pos
        if (go.type == OPTION_DISCARD or go.type == OPTION_CLEAR)
            return go;

        /*
         * THIRD
         * - CARAVAN NAME
         */

        switch (input[2]) {
            case 'A':
            case 'a':
                go.caravan_name = CARAVAN_A;
                break;
            case 'B':
            case 'b':
                go.caravan_name = CARAVAN_B;
                break;
            case 'C':
            case 'c':
                go.caravan_name = CARAVAN_C;
                break;
            case 'D':
            case 'd':
                go.caravan_name = CARAVAN_D;
                break;
            case 'E':
            case 'e':
                go.caravan_name = CARAVAN_E;
                break;
            case 'F':
            case 'f':
                go.caravan_name = CARAVAN_F;
                break;
            default:
                message("Invalid caravan name.");
                continue;
        }

        /*
         * FOURTH (and FIFTH)
         * - CARAVAN POSITION (used when selecting Face card only)
         */

        switch (input[3]) {
            case '1':
                go.pos_caravan = input[4] != '0' ? 1 : 10;
                break;
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                go.pos_caravan = (uint8_t) input[1] - '0';
                break;
            default:
                // Numeric card does not require caravan pos
                go.pos_caravan = 0;
        }

        reached_end_ok = true;

    } while(!reached_end_ok);

    return go;
}
