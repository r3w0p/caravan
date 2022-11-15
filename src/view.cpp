// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "view.h"
#include <curses.h>
#include <clocale>

const std::string BAR_B_PLAYER   = "|---------[D]---------|---------[E]---------|---------[F]---------|-----[PLAYER]-----|";
const std::string BAR_B_PLAYER_B = "|---------[D]---------|---------[E]---------|---------[F]---------|----[PLAYER B]----|";
const std::string BAR_B_COMPUTER = "|---------[D]---------|---------[E]---------|---------[F]---------|----[COMPUTER]----|";
const std::string BAR_A_PLAYER   = "|---------[A]---------|---------[B]---------|---------[C]---------|-----[PLAYER]-----|";
const std::string BAR_A_PLAYER_A = "|---------[A]---------|---------[B]---------|---------[C]---------|----[PLAYER A]----|";
const std::string BAR_A_COMPUTER = "|---------[A]---------|---------[B]---------|---------[C]---------|----[COMPUTER]----|";

ViewCLI::ViewCLI() {
    //setlocale(LC_ALL, "");
    initscr();
    refresh();

    resize_term(VIEW_STDSCR_ROW_MAX, VIEW_STDSCR_COL_MAX);
    wresize(stdscr, VIEW_STDSCR_ROW_MAX, VIEW_STDSCR_COL_MAX);
    refresh();
}

void ViewCLI::close() {
    endwin();
}

void ViewCLI::clear_row(uint8_t rref) {
    wmove(stdscr, rref, 0);
    wclrtoeol(stdscr);
    wmove(stdscr, rref, 0);
}

void ViewCLI::update(Engine *e, User *ua, User *ub) {
    for(int r = 0; r < VIEW_TABLE_ROW_MAX; r++)
        clear_row(r);
    wrefresh(stdscr);

    apply_player_a(e, ua, ub, VIEW_TABLE_ROW_MIDDLE + 1);
    apply_caravan_down(VIEW_TABLE_ROW_MIDDLE + 3, 2, e, CARAVAN_A);
    apply_corners();

    wmove(stdscr, VIEW_STDSCR_ROW_OPTION, 0);
    wrefresh(stdscr);
}

void ViewCLI::message(std::string msg) {
    clear_row(VIEW_STDSCR_ROW_MSG);
    wprintw(stdscr, "%s", msg.c_str());
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

void ViewCLI::apply_player_a(Engine *e, User *ua, User *ub, uint8_t mrow) {
    std::string middle;

    // Select appropriate middle player bar depending on who/what is playing
    if(ua->is_human()) {
        if(ub->is_human())
            middle = BAR_A_PLAYER_A;
        else
            middle = BAR_A_PLAYER;
    } else {
        middle = BAR_A_COMPUTER;
    }

    mvwprintw(stdscr, mrow, 0, middle.c_str());

    apply_caravan_down(VIEW_TABLE_ROW_MIDDLE+3, 3, e, CARAVAN_A);
    // TODO Caravan B
    // TODO Caravan C

    // Deck

    // Hand
}

void ViewCLI::apply_corners() {
    mvwaddch(stdscr, 0, 0, '+');
    mvwaddch(stdscr, 0, 1, '-');
    mvwaddch(stdscr, 0, 2, '-');
    mvwaddch(stdscr, 1, 0, '|');
    mvwaddch(stdscr, 2, 0, '|');

    mvwaddch(stdscr, 0, VIEW_TABLE_COL_MAX - 1, '+');
    mvwaddch(stdscr, 0, VIEW_TABLE_COL_MAX - 2, '-');
    mvwaddch(stdscr, 0, VIEW_TABLE_COL_MAX - 3, '-');
    mvwaddch(stdscr, 1, VIEW_TABLE_COL_MAX - 1, '|');
    mvwaddch(stdscr, 2, VIEW_TABLE_COL_MAX - 1, '|');

    mvwaddch(stdscr, VIEW_TABLE_ROW_MAX - 1, 0, '+');
    mvwaddch(stdscr, VIEW_TABLE_ROW_MAX - 1, 1, '-');
    mvwaddch(stdscr, VIEW_TABLE_ROW_MAX - 1, 2, '-');
    mvwaddch(stdscr, VIEW_TABLE_ROW_MAX - 2, 0, '|');
    mvwaddch(stdscr, VIEW_TABLE_ROW_MAX - 3, 0, '|');

    mvwaddch(stdscr, VIEW_TABLE_ROW_MAX - 1, VIEW_TABLE_COL_MAX - 1, '+');
    mvwaddch(stdscr, VIEW_TABLE_ROW_MAX - 1, VIEW_TABLE_COL_MAX - 2, '-');
    mvwaddch(stdscr, VIEW_TABLE_ROW_MAX - 1, VIEW_TABLE_COL_MAX - 3, '-');
    mvwaddch(stdscr, VIEW_TABLE_ROW_MAX - 2, VIEW_TABLE_COL_MAX - 1, '|');
    mvwaddch(stdscr, VIEW_TABLE_ROW_MAX - 3, VIEW_TABLE_COL_MAX - 1, '|');
}

void ViewCLI::apply_caravan_down(
        uint8_t rref, uint8_t cref, Engine *e, CaravanName cn) {
    uint8_t size;
    std::string size_str;
    Slot stest;
    Table *t = e->get_table();

    if(t->get_caravan_size(cn) == 0)
        return;

    // Size
    size = t->get_caravan_size(cn);

    if(size > 99)
        size_str = "99+";
    else
        size_str = std::to_string(size);

    mvwprintw(stdscr, rref, cref, size_str.c_str());

    // Suit
    mvwaddch(stdscr, rref+1, cref, suit_to_char(t->get_caravan_suit(cn)));

    // Direction
    mvwprintw(stdscr, rref+2, cref, direction_to_str(t->get_caravan_direction(cn)).c_str());

    stest = t->get_slot_at(cn, 1);
    mvwprintw(stdscr, rref+1, cref + 3, std::to_string(1).c_str());
    apply_numeric_down(rref, cref + 5, stest.card);
}

void ViewCLI::apply_numeric_down(uint8_t rref, uint8_t cref, Card c) {
    std::string rank_str;

    if(!is_numeric_card(c))
        throw CaravanFatalException("Card is not numeric.");

    // Row 1
    mvwprintw(stdscr, rref, cref, std::string(".-------.").c_str());

    // Row 2
    mvwaddch(stdscr, rref + 1, cref, '|');

    rank_str = rank_to_str(c.rank);

    if(rank_str.size() < 2) {
        mvwaddch(stdscr, rref + 1, cref + 3, ' ');
        mvwaddch(stdscr, rref + 1, cref + 4, rank_str.at(0));
    } else {
        mvwaddch(stdscr, rref + 1, cref + 3, rank_str.at(0));
        mvwaddch(stdscr, rref + 1, cref + 4, rank_str.at(1));
    }

    mvwaddch(stdscr, rref + 1, cref + 5, suit_to_char(c.suit));

    mvwaddch(stdscr, rref + 1, cref + 6, '|');

    // Row 3
    // Row 4
}