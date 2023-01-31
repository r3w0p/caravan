// Copyright (c) 2022-2023 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "view.h"
#include <curses.h>
#include <iostream>
#include <stdlib.h>
#include <sstream>


WINDOW *create_window(uint16_t h, uint16_t w, uint16_t y, uint16_t x) {
    WINDOW * local_win;
    local_win = newwin(h, w, y, x);
    return local_win;
}

WINDOW *create_window_caravan(uint16_t y, uint16_t x) {
    return create_window(31, 19, y, x);
}

WINDOW *create_window_player(uint16_t y, uint16_t x) {
    return create_window(25, 19, y, x);
}

WINDOW *create_window_dialog(uint16_t y, uint16_t x) {
    return create_window(4, 24, y, x);
}

WINDOW *create_window_input(uint16_t y, uint16_t x) {
    return create_window(1, 5, y, x);
}


char caravan_name_to_letter(CaravanName cn) {
    switch(cn) {
        case CARAVAN_A:
            return 'A';
        case CARAVAN_B:
            return 'B';
        case CARAVAN_C:
            return 'C';
        case CARAVAN_D:
            return 'D';
        case CARAVAN_E:
            return 'E';
        case CARAVAN_F:
            return 'F';
        default:
            throw CaravanFatalException("Invalid caravan name.");
    }
}

std::string player_name_to_str(PlayerName pn, bool ahuman, bool bhuman) {
    if(pn == PLAYER_BOTTOM) {
        if(ahuman) {
            if(bhuman)
                // A is human; B is human
                return "PL1";
            else
                // A is human; B is not human
                return "YOU";

        } else
            // A is not human
            return "BOT";

    } else if(pn == PLAYER_TOP) {
        if(bhuman) {
            if(ahuman)
                // B is human; A is human
                return "PL2";
            else
                // B is human; A is not human
                return "YOU";

        } else
            // B is not human
            return "BOT";

    } else
        throw CaravanFatalException("Invalid player name.");

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

std::string card_num_to_str(uint8_t num, bool lead) {
    switch (num) {
        case 1:
            return lead ? " 1" : "1";
        case 2:
            return lead ? " 2" : "2";
        case 3:
            return lead ? " 3" : "3";
        case 4:
            return lead ? " 4" : "4";
        case 5:
            return lead ? " 5" : "5";
        case 6:
            return lead ? " 6" : "6";
        case 7:
            return lead ? " 7" : "7";
        case 8:
            return lead ? " 8" : "8";
        case 9:
            return lead ? " 9" : "9";
        case 10:
            return "10";
        default:
            throw CaravanFatalException("Invalid card numeral.");
    }
}

void mv_print_suit(WINDOW *win, uint16_t y, uint16_t x, Suit s, bool colour) {
    short colour_pair;
    wchar_t wstr[2];

    if(colour) {
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

    if(colour)
        wattroff(win, COLOR_PAIR(colour_pair));
}

void w_print_suit(WINDOW *win, Suit s, bool colour) {
    short colour_pair;
    wchar_t wstr[2];

    if(colour) {
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

    if(waddwstr(win, wstr) == ERR)
        throw CaravanFatalException(
                "Failed to print suit: mvwaddwstr error.");

    if(colour)
        wattroff(win, COLOR_PAIR(colour_pair));
}

void print_card(WINDOW *win, uint16_t y, uint16_t x,
                Card c, uint8_t num, bool up, bool conceal, bool colour) {

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
        if(c.rank != JOKER) {
            mvwprintw(win, y + 2, x + 1, rank_to_str(c.rank, true).c_str());
            mv_print_suit(win, y + 2, x + 3, c.suit, colour);
        } else
            mvwprintw(win, y + 2, x + 2, "JO");
    } else
        mvwprintw(win, y + 2, x + 1, "###");

    mvwprintw(win, y+2, x+4, "|");

    mvwprintw(win, y+3, x, "|___|");
}

void print_faces(WINDOW *win,
                 uint16_t y, uint16_t x,
                 Faces f, uint8_t i_faces, bool colour) {
    Card c;
    std::string r;

    for(uint8_t i = 0; i < i_faces; i++) {
        c = f[i];
        r = c.rank == JOKER ? "J" : rank_to_str(c.rank, false);

        mvwprintw(win, y, x+i, "_");
        mvwprintw(win, y+1, x+i, r.c_str());

        if(c.suit != NO_SUIT)
            mv_print_suit(win, y + 2, x + i, c.suit, colour);
        else
            mvwprintw(win, y+2, x+i, "O");
    }
}

void print_caravan_down(WINDOW *win, Table *t, CaravanName cn, bool colour) {
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
        mv_print_suit(win, 2, 0, cvn_suit, colour);

    if(cvn_dir != ANY)
        mvwprintw(win, 3, 0, cvn_dir == ASCENDING ? "ASC" : "DES");

    for(uint8_t i = 0; i < cvn_size; i++) {
        pos = i+1;
        slot = t->get_slot_at(cn, pos);
        offset = amt * i;

        mvwprintw(win, offset + 2, 5, card_num_to_str(pos, true).c_str());
        print_card(win, offset, 8, slot.card, pos, false, false, colour);
        print_faces(win, offset, 14, slot.faces, slot.i_faces, colour);
    }
}

void print_caravan_up(WINDOW *win, Table *t, CaravanName cn, bool colour) {
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
        mv_print_suit(win, (amt * 9) + 2, 0, cvn_suit, colour);

    if(cvn_bid > 0)
        mvwprintw(win, (amt * 9) + 3, 0, std::to_string(cvn_bid).c_str());

    for(uint8_t i = 0; i < cvn_size; i++) {
        pos = i+1;
        slot = t->get_slot_at(cn, pos);
        offset = amt * (10 - pos);

        mvwprintw(win, offset + 2, 5, card_num_to_str(pos, true).c_str());
        print_card(win, offset, 8, slot.card, pos, true, false, colour);
        print_faces(win, offset, 14, slot.faces, slot.i_faces, colour);
    }
}

void print_option_msg(WINDOW *win, int y, int x, std::string pname,
                      GameOption* go, bool conceal, bool colour) {
    wmove(win, y, x);
    wclrtoeol(win);

    if (go->type == OPTION_PLAY) {
        wprintw(win, pname.c_str());
        wprintw(win, ": PLAY ");

        wprintw(win, rank_to_str(go->card.rank, false).c_str());

        if(go->card.suit != NO_SUIT)
            w_print_suit(win, go->card.suit, colour);

        wprintw(win, " ON ");

        waddch(win, caravan_name_to_letter(go->caravan_name));
        if(go->pos_caravan > 0)
            wprintw(win, card_num_to_str(go->pos_caravan, false).c_str());

    } else if(go->type == OPTION_DISCARD) {
        wprintw(win, pname.c_str());
        wprintw(win, ": DISCARD ");
        if(conceal)
            wprintw(win, "FROM HAND");

        else {
            wprintw(win, rank_to_str(go->card.rank, false).c_str());

            if(go->card.suit != NO_SUIT)
                w_print_suit(win, go->card.suit, colour);
        }

    } else if(go->type == OPTION_CLEAR) {
        wprintw(win, pname.c_str());
        wprintw(win, ": CLEAR ");
        waddch(win, caravan_name_to_letter(go->caravan_name));
    }
}

void update_dialog(
        WINDOW *win, Engine *e, User *ua, User *ub,
        GameOption* go_bottom, GameOption* go_top,
        bool colour) {

    std::string pna = player_name_to_str(
            PLAYER_BOTTOM, ua->is_human(), ub->is_human());
    std::string pnb = player_name_to_str(
            PLAYER_TOP, ua->is_human(), ub->is_human());
    std::string pturn = e->get_player_turn() == ua->get_name() ? pna : pnb;
    std::string pwinner;

    if(e->get_winner() == NO_PLAYER) {
        if(e->get_player(PLAYER_BOTTOM)->get_moves_count() > 0)
            print_option_msg(win, 0, 0, pna, go_bottom, !ua->is_human(), colour);

        if(e->get_player(PLAYER_TOP)->get_moves_count() > 0)
            print_option_msg(win, 1, 0, pnb, go_top, !ub->is_human(), colour);

        mvwprintw(win, 3, 0, "[");
        mvwprintw(win, 3, 1, pturn.c_str());
        mvwprintw(win, 3, 4, "] > ");

    } else {
        mvwprintw(win, 0, 0, "");
        mvwprintw(win, 1, 0, "");

        pwinner = e->get_winner() == ua->get_name() ? pna : pnb;

        mvwprintw(win, 3, 0, "WINNER:");
        mvwprintw(win, 3, 8, pwinner.c_str());
    }

}

void print_player_up(WINDOW *win, Player *p, bool conceal, bool colour) {
    uint8_t offset;
    uint8_t amt = 3;
    uint8_t pos;
    Card card;
    uint8_t xoff;
    uint8_t yoff;

    uint8_t size_deck = p->get_size_deck();
    uint8_t size_hand = p->get_size_hand();

    if(size_deck > 0) {
        yoff = amt * 7;

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
        offset = amt * (8 - pos);

        if(!conceal)
            mvwprintw(win, offset + 2, 5 + 6, card_num_to_str(pos, true).c_str());

        print_card(win, offset, 8 + 6, card, pos, true, conceal, colour);
    }
}

void print_player_down(WINDOW *win, Player *p, bool conceal, bool colour) {
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
            mvwprintw(win, offset + 2, 5 + 6, card_num_to_str(pos, true).c_str());

        print_card(win, offset, 8 + 6, card, pos, false, conceal, colour);
    }
}

void update_table(WINDOW *win) {
    // CORNERS
    mvwprintw(win, 0, 0, "+--");
    mvwprintw(win, 1, 0, "|");
    mvwprintw(win, 2, 0, "|");

    mvwprintw(win, 0, 103, "--+");
    mvwprintw(win, 1, 105, "|");
    mvwprintw(win, 2, 105, "|");

    mvwprintw(win, 64, 0, "|");
    mvwprintw(win, 65, 0, "|");
    mvwprintw(win, 66, 0, "+--");

    mvwprintw(win, 64, 105, "|");
    mvwprintw(win, 65, 105, "|");
    mvwprintw(win, 66, 103, "--+");

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

    mvwprintw(win, 32, 84-3, "|");
    mvwprintw(win, 31, 84-3, "|");

    mvwprintw(win, 32, 85-3, "---------");
    mvwprintw(win, 32, 94-3, "[BOT]");
    mvwprintw(win, 32, 99-3, "---------");

    mvwprintw(win, 32, 105, "|");
    mvwprintw(win, 31, 105, "|");

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

    mvwprintw(win, 34, 84-3, "|");
    mvwprintw(win, 35, 84-3, "|");

    mvwprintw(win, 34, 85-3, "---------");
    mvwprintw(win, 34, 94-3, "[YOU]");
    mvwprintw(win, 34, 99-3, "---------");

    mvwprintw(win, 34, 108-3, "|");
    mvwprintw(win, 35, 108-3, "|");
}

ViewCLI::ViewCLI() {
    initscr();
    raw();
    getmaxyx(stdscr, r_max, c_max);

    if(r_max < VIEW_ROW_MIN or c_max < VIEW_COL_MIN)
        throw CaravanFatalException("Terminal is too small.");

    has_colour = has_colors();

    if(has_colour) {
        start_color();
        init_pair(PAIR_WHITE_BLACK, COLOR_WHITE, COLOR_BLACK);
        init_pair(PAIR_RED_BLACK, COLOR_RED, COLOR_BLACK);
        init_pair(PAIR_CYAN_BLACK, COLOR_CYAN, COLOR_BLACK);
    }

    uinput = "";
    err_msg = "";
    err_display = false;

    win_cvn_a = create_window_caravan(35, 3);
    win_cvn_b = create_window_caravan(35, 29);
    win_cvn_c = create_window_caravan(35, 55);

    win_cvn_d = create_window_caravan(0, 3);
    win_cvn_e = create_window_caravan(0, 29);
    win_cvn_f = create_window_caravan(0, 55);

    win_player_top = create_window_player(6, 84);
    win_player_bottom = create_window_player(35, 84);

    win_dialog = create_window_dialog(62, 81);

    refresh();
}

void ViewCLI::update(Engine *e, User *ubottom, User *utop, GameOption* go_bottom, GameOption* go_top) {
    bool conceal_bottom;
    bool conceal_top;
    Player *pbottom;
    Player *ptop;

    wclear(stdscr);
    wclear(win_cvn_a);
    wclear(win_cvn_b);
    wclear(win_cvn_c);
    wclear(win_cvn_d);
    wclear(win_cvn_e);
    wclear(win_cvn_f);
    wclear(win_player_bottom);
    wclear(win_player_top);
    wclear(win_dialog);

    try {
        // Print table
        update_table(stdscr);

        // Print caravans
        print_caravan_down(win_cvn_a, e->get_table(), CARAVAN_A, has_colour);
        print_caravan_down(win_cvn_b, e->get_table(), CARAVAN_B, has_colour);
        print_caravan_down(win_cvn_c, e->get_table(), CARAVAN_C, has_colour);

        print_caravan_up(win_cvn_d, e->get_table(), CARAVAN_D, has_colour);
        print_caravan_up(win_cvn_e, e->get_table(), CARAVAN_E, has_colour);
        print_caravan_up(win_cvn_f, e->get_table(), CARAVAN_F, has_colour);

        // Print hands
        pbottom = e->get_player(ubottom->get_name());
        ptop = e->get_player(utop->get_name());

        if (ubottom->is_human() and !utop->is_human()) {
            // Do not conceal human; conceal bot
            conceal_bottom = false;
            conceal_top = true;

        } else if (!ubottom->is_human() and utop->is_human()) {
            // Conceal bot; do not conceal human
            conceal_bottom = true;
            conceal_top = false;

        } else if (ubottom->is_human() and utop->is_human()) {
            // Human vs human; conceal if not player's turn
            conceal_bottom = e->get_player_turn() != pbottom->get_name();
            conceal_top = e->get_player_turn() != ptop->get_name();

        } else {
            // Never conceal if both players are bots
            conceal_bottom = false;
            conceal_top = false;
        }

        print_player_down(win_player_bottom, pbottom, conceal_bottom, has_colour);
        print_player_up(win_player_top, ptop, conceal_top, has_colour);

        // Update dialog
        update_dialog(win_dialog, e, ubottom, utop, go_bottom, go_top, has_colour);

    } catch (CaravanException &e) {
        err_msg = e.what();
    }

    // Update message

    if(!err_msg.empty()) {
        wmove(stdscr, 68, 0);
        wprintw(stdscr, err_msg.c_str());
        err_msg = "";
        err_display = true;

    } else
        err_display = false;

    // Refresh windows

    wrefresh(stdscr);

    wrefresh(win_cvn_a);
    wrefresh(win_cvn_b);
    wrefresh(win_cvn_c);
    wrefresh(win_cvn_d);
    wrefresh(win_cvn_e);
    wrefresh(win_cvn_f);
    wrefresh(win_player_bottom);
    wrefresh(win_player_top);
    wrefresh(win_dialog);

    if(e->get_winner() != NO_PLAYER)
        getch();
}

void ViewCLI::error_message(std::string msg) {
    err_msg = msg;
}

/**
 * @param win
 * @param y
 * @param x
 * @param input Where to store the input. Length must be >= max.
 * @param max The maximum length of the input (+1 for null-termination).
 * @param start The starting string to populate the input space.
 */
void wget_input_alphanum(WINDOW * win, int y, int x, int* input, int max, std::string start) {
    int i_input;
    int c_input;

    i_input = 0;
    wmove(win, y, x);

    for(i_input = 0; start[i_input] != '\0'; i_input++) {
        waddch(win, start[i_input]);
        input[i_input] = (unsigned char) start[i_input];
    }

    //wmove(win, y, x + i_input);
    wclrtoeol(win);
    wrefresh(win);

    while(true) {
        c_input = wgetch(win);

        if(c_input == x) {
            // ASCII 8 == BACKSPACE
            if(i_input == 0)
                // Pressing backspace when no characters, reset cursor
                wmove(win, y, x);
            else {
                // Remove existing character on screen
                i_input -= 1;
                wmove(win, y, x + i_input);
                wclrtoeol(win);
            }

        } else if(c_input == 13) {
            // ASCII 13 == ENTER
            // Write null to end of string and exit
            input[i_input] = '\0';
            break;

        } else if(
                (c_input >= 48 and c_input <= 57) or
                (c_input >= 65 and c_input <= 90) or
                (c_input >= 97 and c_input <= 122)) {
            // ALPHANUMERIC
            if(i_input == max-1) {
                // Remove character if overrun
                wmove(win, y, x + i_input);
                waddch(win, ' ');
                wmove(win, y, x + i_input);
            } else {
                // Account for new character
                input[i_input] = c_input;
                i_input += 1;
            }

        } else {
            // OTHER
            // Remove any other character
            wmove(win, y, x + i_input);
            wclrtoeol(win);
        }

        wrefresh(win);
    }
}

GameOption ViewCLI::option(Engine *e, User *u) {
    GameOption go;
    int input[6];
    std::ostringstream instream;

    if (e->is_closed())
        throw CaravanFatalException("The game has already closed.");

    if(!u->is_human())
        return ((UserBot*) u)->generate_option(e);

    wget_input_alphanum(win_dialog, 3, 8, input, 6, err_display ? uinput : "");

    for (int i: input) {
        instream << (char) i;

        if ((char) i == '\0')
            break;
    }

    uinput = instream.str();

    if(input[0] == '\0')
        throw CaravanFatalException("A command has not been entered.");

    // EXIT

    if((input[0] == 'E' or input[0] == 'e') and
       (input[1] == 'X' or input[1] == 'x') and
       (input[2] == 'I' or input[2] == 'i') and
       (input[3] == 'T' or input[3] == 't'))

        return { OPTION_EXIT };

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
             * "Play numeral card at hand pos 2 onto caravan F"
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
            throw CaravanGameException("Invalid option '" + uinput.substr(0, 1) + "', must be one of: (P)lay, (D)iscard, (C)lear.");
    }

    /*
     * SECOND
     * - HAND POSITION or
     * - CARAVAN NAME
     */

    if (go.type == OPTION_PLAY or go.type == OPTION_DISCARD) {

        if(input[1] == '\0')
            throw CaravanFatalException("A hand position has not been entered.");

        switch (input[1]) {
            case '1':
                go.pos_hand = 1;
                break;
            case '2':
                go.pos_hand = 2;
                break;
            case '3':
                go.pos_hand = 3;
                break;
            case '4':
                go.pos_hand = 4;
                break;
            case '5':
                go.pos_hand = 5;
                break;
            case '6':
                go.pos_hand = 6;
                break;
            case '7':
                go.pos_hand = 7;
                break;
            case '8':
                go.pos_hand = 8;
                break;
            default:
                throw CaravanGameException("Invalid hand position '" + uinput.substr(1, 1) + "'.");
        }

    } else if (go.type == OPTION_CLEAR) {

        if(input[1] == '\0')
            throw CaravanFatalException("A caravan name has not been entered.");

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
                throw CaravanGameException("Invalid caravan name '" + uinput.substr(1, 1) + "', must be between: A-F.");
        }
    }

    // Discard/clear does not require caravan name/pos
    if (go.type == OPTION_DISCARD or go.type == OPTION_CLEAR)
        return go;

    /*
     * THIRD
     * - CARAVAN NAME
     */

    if(input[2] == '\0')
        throw CaravanFatalException("A caravan name has not been entered.");

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
            throw CaravanGameException("Invalid caravan name '" + uinput.substr(2, 1) + "', must be between: A-F.");
    }

    /*
     * FOURTH (and FIFTH)
     * - CARAVAN POSITION (used when selecting Face card only)
     */

    // Caravan position only specified here when playing a face card
    if((char) input[3] == '\0') {
        go.pos_caravan = 0;
        return go;
    }

    switch (input[3]) {
        case '1':
            if (input[4] != '0')
                go.pos_caravan = 1;
            else
                go.pos_caravan = 10;
            break;
        case '2':
            go.pos_caravan = 2;
            break;
        case '3':
            go.pos_caravan = 3;
            break;
        case '4':
            go.pos_caravan = 4;
            break;
        case '5':
            go.pos_caravan = 5;
            break;
        case '6':
            go.pos_caravan = 6;
            break;
        case '7':
            go.pos_caravan = 7;
            break;
        case '8':
            go.pos_caravan = 8;
            break;
        case '9':
            go.pos_caravan = 9;
            break;
        default:
            throw CaravanGameException("Invalid caravan position '" + uinput.substr(3, ((char) input[4] == '\0') ? 1 : 2) + "'.");
    }

    return go;
}

void ViewCLI::close() {
    endwin();
}
