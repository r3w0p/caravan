// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "view.h"
#include <curses.h>
#include <clocale>

ViewCLI::ViewCLI() {
    initscr();
    raw();

    getmaxyx(stdscr, r_max, c_max);
}

void ViewCLI::close() {
    endwin();
}

void ViewCLI::update(Engine *e, User *ua, User *ub) {
    if(has_colors() == FALSE)
    {	endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }
    start_color();			/* Start color 			*/
    init_pair(1, COLOR_BLUE, COLOR_BLACK);

    attron(COLOR_PAIR(1));

    const wchar_t test[] = {suit_to_wchar_t(SPADES), L'\0'};

    mvaddwstr(0, 0, test);

    attroff(COLOR_PAIR(1));

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
