// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "view.h"
#include "exceptions.h"
#include <string>
#include <iostream>


void display_test_caravan(Table *t, CaravanName cn) {
    std::cout << caravan_name_to_str(cn) << " [";
    std::cout << "bid=" << t->get_caravan_bid(cn) << ", ";
    std::cout << "suit=" << suit_to_str(t->get_caravan_suit(cn)) << ", ";
    std::cout << "dir=" << direction_to_str(t->get_caravan_direction(cn))
              << ", ";
    int p_size = t->get_caravan_size(cn);
    std::cout << "size=" << p_size << "]: ";

    for (int pos = 1; pos <= p_size; ++pos) {
        if (pos > 1)
            std::cout << " ";

        Slot sn = t->get_caravan_cards_at(cn, pos);
        std::cout << rank_to_str(sn.card.rank) << suit_to_str(sn.card.suit)
                  << " (";

        for (int m = 0; m < sn.i_faces; ++m) {
            if (m > 0)
                std::cout << " ";

            std::cout << rank_to_str(sn.faces[m].rank)
                      << suit_to_str(sn.faces[m].suit);
        }
        std::cout << ")";
    }
    std::cout << std::endl;
}

void ViewCLI::display(Engine *e, std::string msg) {
    Table *t = e->get_table();
    Player *pa = e->get_player(PLAYER_A);
    Player *pb = e->get_player(PLAYER_B);
    Card c;

    display_test_caravan(t, CARAVAN_D);
    display_test_caravan(t, CARAVAN_E);
    display_test_caravan(t, CARAVAN_F);

    std::cout << std::endl;

    display_test_caravan(t, CARAVAN_A);
    display_test_caravan(t, CARAVAN_B);
    display_test_caravan(t, CARAVAN_C);

    std::cout << std::endl << "PLAYER A:";
    for (int i = 0; i < pa->get_size_hand(); ++i) {
        c = pa->get_from_hand_at(i + 1);
        std::cout << " " << rank_to_str(c.rank) << suit_to_str(c.suit);
    }

    std::cout << std::endl << "PLAYER B:";
    for (int i = 0; i < pb->get_size_hand(); ++i) {
        c = pb->get_from_hand_at(i + 1);
        std::cout << " " << rank_to_str(c.rank) << suit_to_str(c.suit);
    }
    std::cout << std::endl;

    if (msg.size() > 0)
        std::cout << std::endl << msg << std::endl;

    std::cout << std::endl;
}


GameOption ViewCLI::get_input_option(Engine *e, PlayerName pn) {
    GameOption go;
    char input[6];
    int c_flush;
    bool reached_end_ok;

    if (e->is_closed())
        throw CaravanFatalException("The game has already closed.");

    reached_end_ok = false;

    do {
        std::cout << "[" << player_name_to_str(pn) << "] >";
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
