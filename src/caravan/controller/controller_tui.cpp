// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/controller/controller_tui.h"

void ControllerTUI::on_view_user_input(std::string input, bool confirmed) {
    if(closed) return;

    // TODO A future feature that highlights parts of the board as a command is typed,
    //  so that the user has a better idea of what their command will do.
    if(!confirmed) return;

    // Decipher input
    GameCommand command;

    /*
     * EXIT
     */
    if(
        input.size() == 4 and
        (input.at(0) == 'E' or input.at(0) == 'e') and
        (input.at(1) == 'X' or input.at(1) == 'x') and
        (input.at(2) == 'I' or input.at(2) == 'i') and
        (input.at(3) == 'T' or input.at(3) == 't')
    ) {
        command.type = OPTION_EXIT;
    }

    /*
     * FIRST
     * - OPTION TYPE
     */

    if (input.size() < 1)
        throw CaravanFatalException("A command type has not been entered.");

    switch (input.at(0)) {
        case 'P':
        case 'p':
            command.type = OPTION_PLAY;
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
            command.type = OPTION_DISCARD;
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
            command.type = OPTION_CLEAR;
            break;
        default:
            throw CaravanGameException("Invalid option '" + std::to_string(input.at(0)) + "', must be one of: (P)lay, (D)iscard, (C)lear.");
    }

    /*
     * SECOND
     * - HAND POSITION or
     * - CARAVAN NAME
     */

    if (command.type == OPTION_PLAY or command.type == OPTION_DISCARD) {

        if (input.size() < 2)
            throw CaravanFatalException("A hand position has not been entered.");

        switch (input.at(1)) {
            case '1':
                command.pos_hand = 1;
                break;
            case '2':
                command.pos_hand = 2;
                break;
            case '3':
                command.pos_hand = 3;
                break;
            case '4':
                command.pos_hand = 4;
                break;
            case '5':
                command.pos_hand = 5;
                break;
            case '6':
                command.pos_hand = 6;
                break;
            case '7':
                command.pos_hand = 7;
                break;
            case '8':
                command.pos_hand = 8;
                break;
            default:
                throw CaravanGameException("Invalid hand position '" + std::to_string(input.at(1)) + "'.");
        }

    } else if (command.type == OPTION_CLEAR) {

        if (input.size() < 2)
            throw CaravanFatalException("A caravan name has not been entered.");

        switch (input.at(1)) {
            case 'A':
            case 'a':
                command.caravan_name = CARAVAN_A;
                break;
            case 'B':
            case 'b':
                command.caravan_name = CARAVAN_B;
                break;
            case 'C':
            case 'c':
                command.caravan_name = CARAVAN_C;
                break;
            case 'D':
            case 'd':
                command.caravan_name = CARAVAN_D;
                break;
            case 'E':
            case 'e':
                command.caravan_name = CARAVAN_E;
                break;
            case 'F':
            case 'f':
                command.caravan_name = CARAVAN_F;
                break;
            default:
                throw CaravanGameException("Invalid caravan name '" + std::to_string(input.at(1)) + "', must be between: A-F.");
        }
    }

    // Discard/clear does not require caravan name/pos
    if (command.type == OPTION_DISCARD or command.type == OPTION_CLEAR)
        return command;  // TODO fix

    /*
     * THIRD
     * - CARAVAN NAME
     */

    if (input.size() < 3)
        throw CaravanFatalException("A caravan name has not been entered.");

    switch (input.at(2)) {
        case 'A':
        case 'a':
            command.caravan_name = CARAVAN_A;
            break;
        case 'B':
        case 'b':
            command.caravan_name = CARAVAN_B;
            break;
        case 'C':
        case 'c':
            command.caravan_name = CARAVAN_C;
            break;
        case 'D':
        case 'd':
            command.caravan_name = CARAVAN_D;
            break;
        case 'E':
        case 'e':
            command.caravan_name = CARAVAN_E;
            break;
        case 'F':
        case 'f':
            command.caravan_name = CARAVAN_F;
            break;
        default:
            throw CaravanGameException(
                    "Invalid caravan name '" + std::to_string(input.at(2)) + "', must be between: A-F.");
    }

    /*
     * FOURTH
     * - CARAVAN POSITION (used when selecting Face card only)
     */

    // Caravan position only specified here when playing a face card
    if ((char) input.at(3) == '\0') {
        command.pos_caravan = 0;
        return command;  // TODO fix
    }

    switch (input.at(3)) {
        case '1':
            command.pos_caravan = 1;
            break;
        case '2':
            command.pos_caravan = 2;
            break;
        case '3':
            command.pos_caravan = 3;
            break;
        case '4':
            command.pos_caravan = 4;
            break;
        case '5':
            command.pos_caravan = 5;
            break;
        case '6':
            command.pos_caravan = 6;
            break;
        case '7':
            command.pos_caravan = 7;
            break;
        case '8':
            command.pos_caravan = 8;
            break;
        case '9':
            command.pos_caravan = 9;
            break;
        default:
            throw CaravanGameException("Invalid caravan position '" + std::to_string(input.at(3)) + "'.");
    }

    // Pass to subscribers (i.e., Model)
    for(ControllerSubscriber *s : subscribers) {
        s->on_controller_command(command);
    }
}
