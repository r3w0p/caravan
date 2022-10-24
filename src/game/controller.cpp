// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "controller.h"
#include "iostream"
#include "exceptions.h"
#include "common.h"
#include "engine.h"
#include <cstdio>

std::string player_name_to_str(PlayerName pn) {
    switch (pn) {
        case PLAYER_1:
            return "PLAYER 1";
        case PLAYER_2:
            return "PLAYER 2";
        case COMPUTER:
            return "COMPUTER";
        default:
            throw CaravanFatalException("Invalid player name.");
    }
}

void Controller::run() {
    GameOption option;
    std::string msg;
    User *user_turn;
    PlayerName winner;

    do {
        view_ptr->display(engine_ptr, msg);
        msg = "";
        user_turn = engine_ptr->get_player_turn() == PLAYER_1 ? user_p1_ptr : user_p2_ptr;
        option = user_turn->request_option(engine_ptr);

        try {
            engine_ptr->play_option(option);

        } catch (CaravanGameException &e) {
            msg = e.what();
        }

    } while ((winner = engine_ptr->get_winner()) == NO_PLAYER);

    msg = "Winner is: " + player_name_to_str(winner) + "\n";
    view_ptr->display(engine_ptr, msg);
}

GameOption UserHumanCLI::request_option(Engine *e) {
    GameOption go;
    char input[6];
    int c_flush;
    bool reached_end_ok = false;

    do {
        std::cout << "[" << player_name_to_str(name) << "] >";
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
                break;
            case 'R':
            case 'r':
                go.type = OPTION_REMOVE;
                break;
            case 'C':
            case 'c':
                go.type = OPTION_CLEAR;
                break;
            default:
                continue;
        }

        /*
         * SECOND
         * - HAND POSITION or
         * - CARAVAN NAME
         */

        if (go.type == OPTION_PLAY or go.type == OPTION_REMOVE) {
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
                    continue;
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
                    continue;
            }
        }

        if (go.type == OPTION_REMOVE or go.type == OPTION_CLEAR)
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
            case '0':
                go.pos_caravan = (uint8_t) input[1] - '0';
                break;
        }

        reached_end_ok = true;
    } while (!reached_end_ok);

    return go;
}
