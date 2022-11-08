// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "../include/view.h"
#include <string>
#include <iostream>

const std::string MIDDLE_B_PLAYER   = "|-------[D]-------|-------[E]-------|-------[F]-------|----[PLAYER]----|";
const std::string MIDDLE_B_PLAYER_B = "|-------[D]-------|-------[E]-------|-------[F]-------|---[PLAYER B]---|";
const std::string MIDDLE_B_COMPUTER = "|-------[D]-------|-------[E]-------|-------[F]-------|---[COMPUTER]---|";
const std::string MIDDLE_A_PLAYER   = "|-------[A]-------|-------[B]-------|-------[C]-------|----[PLAYER]----|";
const std::string MIDDLE_A_PLAYER_A = "|-------[A]-------|-------[B]-------|-------[C]-------|---[PLAYER A]---|";
const std::string MIDDLE_A_COMPUTER = "|-------[A]-------|-------[B]-------|-------[C]-------|---[COMPUTER]---|";
const std::string MIDDLE_BAR =        "|                 |                 |                 |                |";

void ViewCLI::flush_vda() {
    for(int r = 0; r < VIEW_DISPLAY_ROW_MAX; r++)
        for(int c = 0; c < VIEW_DISPLAY_COL_MAX; c++)
            vda[r][c] = ' ';
}

void ViewCLI::apply_player_b(Engine *e, User *ua, User *ub, uint8_t mrow) {
    // TODO
}

void ViewCLI::apply_player_a(Engine *e, User *ua, User *ub, uint8_t mrow) {
    std::string middle;

    // Select appropriate middle player bar depending on who/what is playing
    if(ua->is_human()) {
        if(ub->is_human())
            middle = MIDDLE_A_PLAYER_A;
        else
            middle = MIDDLE_A_PLAYER;
    } else {
        middle = MIDDLE_A_COMPUTER;
    }

    for(uint8_t c = 0; c < VIEW_DISPLAY_COL_MAX; c++)
        vda[mrow][c] = middle[c];

    // Extra bars
    for(uint8_t c = 0; c < VIEW_DISPLAY_COL_MAX; c++) {
        vda[mrow + 1][c] = MIDDLE_BAR[c];
        vda[mrow + 2][c] = MIDDLE_BAR[c];
    }
}

void ViewCLI::apply_corners() {
    vda[0][0] = '+';
    vda[0][1] = '-';
    vda[0][2] = '-';
    vda[1][0] = '|';
    vda[2][0] = '|';

    vda[0][VIEW_DISPLAY_COL_MAX-1] = '+';
    vda[0][VIEW_DISPLAY_COL_MAX-2] = '-';
    vda[0][VIEW_DISPLAY_COL_MAX-3] = '-';
    vda[1][VIEW_DISPLAY_COL_MAX-1] = '|';
    vda[2][VIEW_DISPLAY_COL_MAX-1] = '|';

    vda[VIEW_DISPLAY_ROW_MAX-1][0] = '+';
    vda[VIEW_DISPLAY_ROW_MAX-1][1] = '-';
    vda[VIEW_DISPLAY_ROW_MAX-1][2] = '-';
    vda[VIEW_DISPLAY_ROW_MAX-2][0] = '|';
    vda[VIEW_DISPLAY_ROW_MAX-3][0] = '|';

    vda[VIEW_DISPLAY_ROW_MAX-1][VIEW_DISPLAY_COL_MAX-1] = '+';
    vda[VIEW_DISPLAY_ROW_MAX-1][VIEW_DISPLAY_COL_MAX-2] = '-';
    vda[VIEW_DISPLAY_ROW_MAX-1][VIEW_DISPLAY_COL_MAX-3] = '-';
    vda[VIEW_DISPLAY_ROW_MAX-2][VIEW_DISPLAY_COL_MAX-1] = '|';
    vda[VIEW_DISPLAY_ROW_MAX-3][VIEW_DISPLAY_COL_MAX-1] = '|';
}

void ViewCLI::apply_caravan_down(
        uint8_t rref, uint8_t cref, Engine *e, CaravanName cn) {
    uint8_t size;
    std::string size_str;
    std::string suit_str;
    std::string dir_str;
    Table *t = e->get_table();

    if(t->get_caravan_size(cn) == 0)
        return;

    // Size
    size = t->get_caravan_size(cn);

    if(size > 99)
        size_str = "99+";
    else
        size_str = std::to_string(size);

    for(int i = 0; i < size_str.size(); ++i)
        vda[rref][cref+i] = size_str[i];

    // Suit
    suit_str = suit_to_str(t->get_caravan_suit(cn));
    vda[rref+1][cref] = suit_str[0];

    // Direction
    dir_str = direction_to_str(t->get_caravan_direction(cn));

    for(int i = 0; i < dir_str.size(); ++i)
        vda[rref+2][cref+i] = dir_str[i];

    Slot stest = t->get_slot_at(cn, 1);
    apply_pos(rref+1, cref + 3, 1);
    apply_numeric_down(rref, cref + 5, stest.card);
}

void ViewCLI::apply_pos(uint8_t rref, uint8_t cref, uint8_t pos) {
    std::string pos_str = std::to_string(pos);

    if(pos < 10)
        vda[rref][cref+1] = pos_str[0];
    else {
        vda[rref][cref] = pos_str[0];
        vda[rref][cref+1] = pos_str[1];
    }
}

void ViewCLI::apply_numeric_down(uint8_t rref, uint8_t cref, Card c) {
    if(!is_numeric_card(c))
        throw CaravanFatalException("Card is not numeric.");

    vda[rref][cref] = '|';
    vda[rref][cref+1] = '-'; // L'‾';
    vda[rref][cref+2] = '-'; // L'‾';
    vda[rref][cref+3] = '-'; // L'‾';
    vda[rref][cref+4] = '|';

    vda[rref+1][cref] = '|';
    vda[rref+1][cref+4] = '|';

    vda[rref+2][cref] = '|';
    vda[rref+2][cref+4] = '|';

    vda[rref+3][cref] = '|';
    vda[rref+3][cref+1] = '-'; // L'‾';
    vda[rref+3][cref+2] = '-'; // L'‾';
    vda[rref+3][cref+3] = '-'; // L'‾';
    vda[rref+3][cref+4] = '|';

    std::string rank_str = rank_to_str(c.rank);

    if(rank_str.size() < 2)
        vda[rref+1][cref+2] = rank_str[0];
    else {
        vda[rref+1][cref+1] = rank_str[0];
        vda[rref+1][cref+2] = rank_str[1];
    }

    vda[rref+1][cref+3] = suit_to_str(c.suit)[0];
}

void ViewCLI::display_table(Engine *e, User *ua, User *ub) {
    flush_vda();

    apply_corners();
    // apply_player_b(e, ua, ub, VIEW_DISPLAY_ROW_MIDDLE - 1);
    apply_player_a(e, ua, ub, VIEW_DISPLAY_ROW_MIDDLE + 1);

    apply_caravan_down(VIEW_DISPLAY_ROW_MIDDLE + 3, 2, e, CARAVAN_A);


    for(int r = 0; r < VIEW_DISPLAY_ROW_MAX; r++) {
        for (int c = 0; c < VIEW_DISPLAY_COL_MAX; c++)
            std::wcout << vda[r][c];

        std::wcout << std::endl;
    }
}

void ViewCLI::put_message(std::string msg) {
    if (!msg.empty())
        std::cout << std::endl << msg << std::endl;
}

GameOption ViewCLI::get_option(Engine *e, User *u) {
    GameOption go;
    char input[6];
    int c_flush;
    bool reached_end_ok;

    if (e->is_closed())
        throw CaravanFatalException("The game has already closed.");

    if(!u->is_human())
        return ((UserBot*) u)->generate_option(e);

    reached_end_ok = false;

    do {
        std::cout << "[" << player_name_to_str(u->get_name()) << "] >";
        scanf("%5s", input);
        while ((c_flush = fgetc(stdin)) != '\n' and c_flush != EOF);

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
    } while (!reached_end_ok);

    return go;
}
