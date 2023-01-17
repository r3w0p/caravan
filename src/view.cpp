// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "view.h"
#include <curses.h>
#include <clocale>

WINDOW *create_window_caravan(uint16_t h, uint16_t w, uint16_t y, uint16_t x) {
    WINDOW * local_win;
    local_win = newwin(h, w, y, x);
    return local_win;
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
                Card c, uint8_t num, bool up) {

    if(up) {
        // TODO

    } else {
        if(num == 1)
            mvwprintw(win, y, x, " ___ ");
        else
            mvwprintw(win, y, x, "|___|");
    }

    mvwprintw(win, y+1, x, "|   |");

    mvwprintw(win, y+2, x, "|");
    mvwprintw(win, y+2, x+1, rank_to_str(c.rank, true).c_str());
    print_suit(win, y+2, x+3, c.suit);
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
    Card c = { DIAMONDS, TEN };
    Faces f;
    f[0] = { DIAMONDS, KING };
    f[1] = { SPADES, KING };
    f[2] = { NO_SUIT, JOKER };
    f[3] = { CLUBS, QUEEN };
    f[4] = { HEARTS, QUEEN };

    uint8_t num_cards = 2;
    uint8_t offset = 0;
    uint8_t amt = 3;

    uint16_t cnbid = 25; //t->get_caravan_bid(cn);
    Suit cnsuit = SPADES; //t->get_caravan_suit(cn);
    Direction cndir = DESCENDING; //t->get_caravan_direction(cn);

    if(cnbid > 0)
        mvwprintw(win, 1, 0, std::to_string(cnbid).c_str());

    if(cnsuit != NO_SUIT)
        print_suit(win, 2, 0, cnsuit);

    if(cndir != ANY)
        mvwprintw(win, 3, 0, cndir == ASCENDING ? "ASC" : "DES");

    for(int i = 0; i < 10; i++) {
        offset = amt * i;
        mvwprintw(win, offset + 2, 5 + 0, card_num_to_str(i+1).c_str());
        print_card(win, offset, 5 + 3, c, i+1, false);
        // TODO y for UP is different for DOWN
        print_faces(win, offset, 5 + 9, f, 5);
    }


}

void ViewCLI::update(Engine *e, User *ua, User *ub) {
    WINDOW* win_cvn_down = create_window_caravan(31, 19, 0, 0);
    refresh();

    for(int y = 0; y < 60; y++)
        mvwprintw(win_cvn_down, y, 0, y % 10 == 0 ? "-" : "*");

    for(int x = 0; x < 16; x++)
        mvwprintw(win_cvn_down, 0, x, x % 10 == 0 ? "-" : "*");

    print_caravan_down(win_cvn_down, e->get_table(), CARAVAN_D);

    wrefresh(win_cvn_down);

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
