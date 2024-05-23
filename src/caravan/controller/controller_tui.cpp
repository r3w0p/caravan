// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/controller/controller_tui.h"
#include "caravan/core/exceptions.h"


void process_first(std::string input, GameCommand *command) {
    char c = input.at(0);  // minimum input size already checked elsewhere

    switch (c) {
        case 'P':
        case 'p':
            command->type = OPTION_PLAY;
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
            command->type = OPTION_DISCARD;
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
            command->type = OPTION_CLEAR;
            break;

        default:
            throw CaravanInputException("Invalid option '" + std::string(1, c) + "', must be one of: (P)lay, (D)iscard, (C)lear.");
    }
}

void process_second(std::string input, GameCommand *command) {
    if (command->type == OPTION_PLAY or command->type == OPTION_DISCARD) {

        if (input.size() < 2)
            throw CaravanInputException("A hand position has not been entered.");

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
                throw CaravanInputException("Invalid hand position '" + std::string(1, c) + "'.");
        }

    } else if (command->type == OPTION_CLEAR) {

        if (input.size() < 2)
            throw CaravanInputException("A caravan name has not been entered.");

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
                throw CaravanInputException("Invalid caravan name '" + std::string(1, c) + "', must be between: A-F.");
        }

    } // else invalid command type, handled during parse of first character
}

void process_third(std::string input, GameCommand *command) {
    if (command->type == OPTION_PLAY) {

        if (input.size() < 3)
            throw CaravanInputException("A caravan name has not been entered.");

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
                throw CaravanInputException("Invalid caravan name '" + std::string(1, c) + "', must be between: A-F.");
        }
    }
}

void process_fourth(std::string input, GameCommand *command) {
    if (command->type == OPTION_PLAY) {

        if (input.size() < 4) return;  // optional, not an error

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
                throw CaravanInputException("Invalid caravan position '" + std::string(1, c) + "'.");
        }
    }
}

bool process_exit(std::string input, GameCommand *command) {
    if(
        input.size() == 4 and
        (input.at(0) == 'E' or input.at(0) == 'e') and
        (input.at(1) == 'X' or input.at(1) == 'x') and
        (input.at(2) == 'I' or input.at(2) == 'i') and
        (input.at(3) == 'T' or input.at(3) == 't')
    ) {
        command->type = OPTION_EXIT;
        return true;
    }

    return false;
}

void ControllerTUI::on_view_user_input(std::string input, bool confirmed) {
    if(closed) return;

    if(input.empty()) return;

    // TODO A future feature that highlights parts of the board as a command is typed,
    //  so that the user has a better idea of what their command will do.
    if(!confirmed) return;

    GameCommand command;
    bool is_exit;

    /*
     * EXIT
     */
    is_exit = process_exit(input, &command);

    if(!is_exit) {
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
    }

    // Pass to subscribers (i.e., Model)
    for(ControllerSubscriber *s : subscribers) {
        s->on_controller_command(command);
    }
}

void ControllerTUI::close() {
    if(!closed) {
        closed = true;
    }
}