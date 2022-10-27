// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "controller.h"
#include "iostream"
#include "exceptions.h"
#include "common.h"
#include "engine.h"
#include <cstdio>
#include <string>
#include <cstring>


// if(instanceof<Parent>(chld1))
template<typename A, typename B>
inline bool instanceof(const B *ptr) {
    return dynamic_cast<const A *>(ptr) != nullptr;
}

std::string player_name_to_str(PlayerName pn) {
    switch (pn) {
        case PLAYER_1:
            return "PLAYER 1";
        case PLAYER_2:
            return "PLAYER 2";
        case BOT_1:
        case BOT_2:
            return "COMPUTER";
        default:
            throw CaravanFatalException("Invalid player name.");
    }
}

GameOption UserHumanCLI::request_option(Engine *e) {
    GameOption go;
    char input[6];
    int c_flush;
    bool reached_end_ok;

    if (e->is_closed())
        throw CaravanFatalException("The game has already closed.");

    reached_end_ok = false;

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
                /*
                 * P2F
                 * "Play numeric card at hand pos 2 into caravan F"
                 *
                 * P8F10
                 * "Play face card at hand pos 8 into caravan F, slot 10"
                 */
                break;
            case 'R':
            case 'r':
                go.type = OPTION_REMOVE;
                /*
                 * R8
                 * "Remove card at hand pos 8"
                 */
                break;
            case 'C':
            case 'c':
                /*
                 * CF
                 * "Clear caravan F"
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
                    continue;  // mandatory for play/remove: ask again...
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

        // Remove/clear does not require caravan name/pos
        if (go.type == OPTION_REMOVE or go.type == OPTION_CLEAR) {
            go.caravan_name = NO_CARAVAN;
            go.pos_caravan = 0;
            return go;
        }

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

std::string generate_option_msg(PlayerName pn, GameOption go) {
    if (go.type == OPTION_PLAY) {
        return player_name_to_str(pn) +
               " played " +
               (go.pos_caravan == 0
                ? "numeric"
                : "face") +
               " card into caravan " +
               caravan_name_to_str(go.caravan_name) +
               (go.pos_caravan == 0
                ? ""
                : " at position " + std::to_string(go.pos_caravan)) +
               ".";

    } else if (go.type == OPTION_REMOVE) {
        return player_name_to_str(pn) +
               " removed card from hand.";

    } else if (go.type == OPTION_CLEAR) {
        return player_name_to_str(pn) +
               " cleared caravan " +
               caravan_name_to_str(go.caravan_name) +
               ".";
    }

    throw CaravanFatalException("Invalid option for message generation.");
}

void ControllerCLI::run() {
    GameOption option;
    std::string msg;
    User *user_turn;
    PlayerName winner;
    PlayerCaravanNames cvns;

    do {
        view_ptr->display(engine_ptr, msg);
        msg = "";

        user_turn = engine_ptr->get_player_turn() == user_a_ptr->get_name() ?
                    user_a_ptr : user_b_ptr;

        option = user_turn->request_option(engine_ptr);

        try {
            engine_ptr->play_option(option);
            msg = generate_option_msg(user_turn->get_name(), option);

        } catch (CaravanGameException &e) {
            msg = player_name_to_str(winner) +
                  " illegal move. " +
                  e.what() +
                  "\n";
        }

    } while ((winner = engine_ptr->get_winner()) == NO_PLAYER);

    msg = "Winner is: " + player_name_to_str(winner) + "\n";
    view_ptr->display(engine_ptr, msg);
}
