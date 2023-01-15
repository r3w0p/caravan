// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "view.h"
#include <curses.h>
#include <clocale>

WINDOW *create_window_caravan(int h, int w, int y, int x) {
    WINDOW * local_win;
    local_win = newwin(h, w, y, x);
    return local_win;
}

void print_suit(WINDOW *win, int y, int x, Suit s) {
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

    if(move(y, x) == ERR)
        throw CaravanFatalException(
                "Failed to print suit: move error.");

    if(waddnwstr(win, wstr, -1) == ERR)
        throw CaravanFatalException(
                "Failed to print suit: waddnwstr error.");

    if(stub_has_colour)
        wattroff(win, COLOR_PAIR(colour_pair));
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


std::string rank_to_str(Rank r) {
    switch (r) {
        case ACE:
            return " A";
        case TWO:
            return " 2";
        case THREE:
            return " 3";
        case FOUR:
            return " 4";
        case FIVE:
            return " 5";
        case SIX:
            return " 6";
        case SEVEN:
            return " 7";
        case EIGHT:
            return " 8";
        case NINE:
            return " 9";
        case TEN:
            return "10";
        case JACK:
            return " J";
        case QUEEN:
            return " Q";
        case KING:
            return " K";
        case JOKER:
            return "JO";
        default:
            throw CaravanFatalException("Invalid rank.");
    }
}

void print_card(WINDOW *win, int y, int x, Card c) {
    mvwprintw(win, y, x, " _____ ");
    mvwprintw(win, y+1, x, "|     |");
    mvwprintw(win, y+2, x, "|     |");

    mvwprintw(win, y+3, x, "| ");
    mvwprintw(win, y+3, x+2, rank_to_str(c.rank).c_str());
    print_suit(win, y+3, x+4, c.suit);
    mvwprintw(win, y+3, x+5, " |");

    mvwprintw(win, y+4, x, "|     |");
    mvwprintw(win, y+5, x, "|_____|");
}

void ViewCLI::update(Engine *e, User *ua, User *ub) {
    WINDOW* win_cvn_up = create_window_caravan(10, 12, 5, 5);
    refresh();

    // print_suit(win_cvn_up, 0, 0, DIAMONDS);
    Card c = { DIAMONDS, TEN };
    print_card(win_cvn_up, 0, 0, c);

    wrefresh(win_cvn_up);

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
