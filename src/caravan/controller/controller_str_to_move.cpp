// Copyright (c) 2022-2025 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/core/exceptions.h"
#include "caravan/controller/controller_str_to_move.h"

#include <string>
#include <chrono>

void process_first(const std::string &input, GameMove *command) {
    char c = input.at(0);  // minimum input size already checked elsewhere

    switch (c) {
        case 'P':
        case 'p':
            command->option = OPTION_PLAY;
            /*
             * P2F
             * "Play numeral card at hand pos 2 onto caravan F"
             *
             * P4F8
             * "Play face card at hand pos 4 onto caravan F, slot 8"
             */
            break;

        case 'D':
        case 'd':
            command->option = OPTION_DISCARD;
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
            command->option = OPTION_CLEAR;
            break;

        default:
            throw CaravanIllegalControllerException(
                "Invalid option '" + std::string(1, c) +
                "', must be one of: (P)lay, (D)iscard, (C)lear.");
    }
}

void process_second(const std::string &input, GameMove *command) {
    if (command->option == OPTION_PLAY or command->option == OPTION_DISCARD) {

        if (input.size() < 2) {
            throw CaravanIllegalControllerException("A hand position has not been entered.");
        }

        char c = input.at(1);

        switch (c) {
            case '1':
                command->pos_hand = 1;
                break;
            case '2':
                command->pos_hand = 2;
                break;
            case '3':
                command->pos_hand = 3;
                break;
            case '4':
                command->pos_hand = 4;
                break;
            case '5':
                command->pos_hand = 5;
                break;
            case '6':
                command->pos_hand = 6;
                break;
            case '7':
                command->pos_hand = 7;
                break;
            case '8':
                command->pos_hand = 8;
                break;
            default:
                throw CaravanIllegalControllerException(
                    "Invalid hand position '" + std::string(1, c) + "'.");
        }

    } else if (command->option == OPTION_CLEAR) {

        if (input.size() < 2) {
            throw CaravanIllegalControllerException("A caravan name has not been entered.");
        }

        char c = input.at(1);

        switch (c) {
            case 'A':
            case 'a':
                command->caravan_name = CARAVAN_A;
                break;
            case 'B':
            case 'b':
                command->caravan_name = CARAVAN_B;
                break;
            case 'C':
            case 'c':
                command->caravan_name = CARAVAN_C;
                break;
            case 'D':
            case 'd':
                command->caravan_name = CARAVAN_D;
                break;
            case 'E':
            case 'e':
                command->caravan_name = CARAVAN_E;
                break;
            case 'F':
            case 'f':
                command->caravan_name = CARAVAN_F;
                break;
            default:
                throw CaravanIllegalControllerException(
                    "Invalid caravan name '" + std::string(1, c) +
                    "', must be between: A-F.");
        }

    } // else invalid command type, handled during parse of first character
}

void process_third(const std::string &input, GameMove *command) {
    if (command->option == OPTION_PLAY) {

        if (input.size() < 3) {
            throw CaravanIllegalControllerException("A caravan name has not been entered.");
        }

        char c = input.at(2);

        switch (c) {
            case 'A':
            case 'a':
                command->caravan_name = CARAVAN_A;
                break;
            case 'B':
            case 'b':
                command->caravan_name = CARAVAN_B;
                break;
            case 'C':
            case 'c':
                command->caravan_name = CARAVAN_C;
                break;
            case 'D':
            case 'd':
                command->caravan_name = CARAVAN_D;
                break;
            case 'E':
            case 'e':
                command->caravan_name = CARAVAN_E;
                break;
            case 'F':
            case 'f':
                command->caravan_name = CARAVAN_F;
                break;
            default:
                throw CaravanIllegalControllerException(
                    "Invalid caravan name '" + std::string(1, c) +
                    "', must be between: A-F.");
        }
    }
}

void process_fourth(const std::string &input, GameMove *command) {
    if (command->option == OPTION_PLAY) {

        if (input.size() < 4) { return; }  // optional, not an error

        char c = input.at(3);

        switch (c) {
            case '1':
                command->pos_caravan = 1;
                break;
            case '2':
                command->pos_caravan = 2;
                break;
            case '3':
                command->pos_caravan = 3;
                break;
            case '4':
                command->pos_caravan = 4;
                break;
            case '5':
                command->pos_caravan = 5;
                break;
            case '6':
                command->pos_caravan = 6;
                break;
            case '7':
                command->pos_caravan = 7;
                break;
            case '8':
                command->pos_caravan = 8;
                break;
            default:
                throw CaravanIllegalControllerException(
                    "Invalid caravan position '" + std::string(1, c) + "'.");
        }
    }
}

GameMove ControllerStrToMove::convert(const std::string &input, bool confirmed) {
    GameMove command;

    if (input.empty()) return command;

    try {
        /*
         * FIRST
         * - COMMAND TYPE
         */
        process_first(input, &command);

        /*
         * SECOND
         * - HAND POSITION or
         * - CARAVAN NAME
         */
        process_second(input, &command);

        /*
         * THIRD
         * - CARAVAN NAME
         */
        process_third(input, &command);

        /*
         * FOURTH
         * - CARAVAN POSITION (used when selecting Face card only)
         */
        process_fourth(input, &command);

    } catch (CaravanIllegalControllerException &e) {
        if (confirmed) {
            // For confirmed commands: throw to other handling that prints
            // command errors to the player
            throw;
        }

        // For unconfirmed commands: accept whatever was able to be parsed
        // so that it can be used for highlighting the game board
        return command;
    }

    return command;
}
