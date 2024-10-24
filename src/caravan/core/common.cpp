// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/core/common.h"
#include "caravan/core/exceptions.h"

void process_first(std::string input, GameCommand *command) {
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
            throw CaravanInputException(
                "Invalid option '" + std::string(1, c) + "', must be one of: (P)lay, (D)iscard, (C)lear.");
    }
}

void process_second(std::string input, GameCommand *command) {
    if (command->option == OPTION_PLAY or command->option == OPTION_DISCARD) {

        if (input.size() < 2) {
            throw CaravanInputException("A hand position has not been entered.");
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
                throw CaravanInputException("Invalid hand position '" + std::string(1, c) + "'.");
        }

    } else if (command->option == OPTION_CLEAR) {

        if (input.size() < 2) {
            throw CaravanInputException("A caravan name has not been entered.");
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
                throw CaravanInputException("Invalid caravan name '" + std::string(1, c) + "', must be between: A-F.");
        }

    } // else invalid command type, handled during parse of first character
}

void process_third(std::string input, GameCommand *command) {
    if (command->option == OPTION_PLAY) {

        if (input.size() < 3) {
            throw CaravanInputException("A caravan name has not been entered.");
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
                throw CaravanInputException("Invalid caravan name '" + std::string(1, c) + "', must be between: A-F.");
        }
    }
}

void process_fourth(std::string input, GameCommand *command) {
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
                throw CaravanInputException("Invalid caravan position '" + std::string(1, c) + "'.");
        }
    }
}

GameCommand generate_command(std::string input, bool confirmed) {
    GameCommand command;

    if (input.empty()) { return command; }

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

    } catch (CaravanInputException &e) {
        if (confirmed) {
            // For confirmed commands: throw to other handling that prints
            // command errors to the player
            throw;
        } else {
            // For unconfirmed commands: accept whatever was able to be parsed
            // so that it can be used for highlighting the game board
            return command;
        }
    }

    return command;
}

bool is_numeral_card(Card c) {
    return (c.rank >= ACE and c.rank <= TEN);
}

bool is_face_card(Card c) {
    return (c.rank >= JACK and c.rank <= JOKER);
}

std::string caravan_letter(CaravanName caravan_name) {
    switch (caravan_name) {
        case CARAVAN_A:
            return "A";
        case CARAVAN_B:
            return "B";
        case CARAVAN_C:
            return "C";
        case CARAVAN_D:
            return "D";
        case CARAVAN_E:
            return "E";
        case CARAVAN_F:
            return "F";
        default:
            return "";
    }
}

uint8_t numeral_rank_value(Card c) {
    switch (c.rank) {
        case ACE:
            return 1;
        case TWO:
            return 2;
        case THREE:
            return 3;
        case FOUR:
            return 4;
        case FIVE:
            return 5;
        case SIX:
            return 6;
        case SEVEN:
            return 7;
        case EIGHT:
            return 8;
        case NINE:
            return 9;
        case TEN:
            return 10;
        default:
            throw CaravanFatalException("Card is not a numeral.");
    }
}