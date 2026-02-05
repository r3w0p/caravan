// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/core/exceptions.h"
#include "caravan/controller/controller_ftxui.h"

#include <string>
#include <chrono>


namespace Caravan::Controller {
    void process_first(const std::string &input, Model::GameMove *move) {
        char c = input.at(0); // minimum input size already checked elsewhere

        switch (c) {
            case 'P':
            case 'p':
                move->option = Model::OPTION_PLAY;
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
                move->option = Model::OPTION_DISCARD;
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
                move->option = Model::OPTION_CLEAR;
                break;

            default:
                throw CaravanIllegalControllerException(
                    "Invalid option '" + std::string(1, c) +
                    "', must be one of: (P)lay, (D)iscard, (C)lear."
                );
        }
    }

    void process_second(const std::string &input, Model::GameMove *move) {
        if (move->option == Model::OPTION_PLAY
            or
        move->option == Model::OPTION_DISCARD
        ) {
            if (input.size() < 2) {
                throw CaravanIllegalControllerException(
                    "A hand position has not been entered."
                );
            }

            char c = input.at(1);

            switch (c) {
                case '1':
                    move->pos_hand = 1;
                    break;
                case '2':
                    move->pos_hand = 2;
                    break;
                case '3':
                    move->pos_hand = 3;
                    break;
                case '4':
                    move->pos_hand = 4;
                    break;
                case '5':
                    move->pos_hand = 5;
                    break;
                case '6':
                    move->pos_hand = 6;
                    break;
                case '7':
                    move->pos_hand = 7;
                    break;
                case '8':
                    move->pos_hand = 8;
                    break;
                default:
                    throw CaravanIllegalControllerException(
                        "Invalid hand position '" + std::string(1, c) + "'."
                    );
            }
        }
        else
        if (move->option == Model::OPTION_CLEAR) {
            if (input.size() < 2) {
                throw CaravanIllegalControllerException(
                    "A caravan name has not been entered."
                );
            }

            char c = input.at(1);

            switch (c) {
                case 'A':
                case 'a':
                    move->caravan_name = Model::CARAVAN_A;
                    break;
                case 'B':
                case 'b':
                    move->caravan_name = Model::CARAVAN_B;
                    break;
                case 'C':
                case 'c':
                    move->caravan_name = Model::CARAVAN_C;
                    break;
                case 'D':
                case 'd':
                    move->caravan_name = Model::CARAVAN_D;
                    break;
                case 'E':
                case 'e':
                    move->caravan_name = Model::CARAVAN_E;
                    break;
                case 'F':
                case 'f':
                    move->caravan_name = Model::CARAVAN_F;
                    break;
                default:
                    throw CaravanIllegalControllerException(
                        "Invalid caravan name '" + std::string(1, c) +
                        "', must be between: A-F."
                    );
            }
        } // else invalid move type, handled during parse of first character
    }

    void process_third(const std::string &input, Model::GameMove *move) {
        if (move->option == Model::OPTION_PLAY) {
            if (input.size() < 3) {
                throw CaravanIllegalControllerException(
                    "A caravan name has not been entered."
                );
            }

            char c = input.at(2);

            switch (c) {
                case 'A':
                case 'a':
                    move->caravan_name = Model::CARAVAN_A;
                    break;
                case 'B':
                case 'b':
                    move->caravan_name = Model::CARAVAN_B;
                    break;
                case 'C':
                case 'c':
                    move->caravan_name = Model::CARAVAN_C;
                    break;
                case 'D':
                case 'd':
                    move->caravan_name = Model::CARAVAN_D;
                    break;
                case 'E':
                case 'e':
                    move->caravan_name = Model::CARAVAN_E;
                    break;
                case 'F':
                case 'f':
                    move->caravan_name = Model::CARAVAN_F;
                    break;
                default:
                    throw CaravanIllegalControllerException(
                        "Invalid caravan name '" + std::string(1, c) +
                        "', must be between: A-F."
                    );
            }
        }
    }

    void process_fourth(const std::string &input, Model::GameMove *move) {
        if (move->option == Model::OPTION_PLAY) {
            if (input.size() < 4) {
                return;
            } // optional, not an error

            char c = input.at(3);

            switch (c) {
                case '1':
                    move->pos_caravan = 1;
                    break;
                case '2':
                    move->pos_caravan = 2;
                    break;
                case '3':
                    move->pos_caravan = 3;
                    break;
                case '4':
                    move->pos_caravan = 4;
                    break;
                case '5':
                    move->pos_caravan = 5;
                    break;
                case '6':
                    move->pos_caravan = 6;
                    break;
                case '7':
                    move->pos_caravan = 7;
                    break;
                case '8':
                    move->pos_caravan = 8;
                    break;
                default:
                    throw CaravanIllegalControllerException(
                        "Invalid caravan position '" + std::string(
                            1,
                            c
                        ) + "'."
                    );
            }
        }
    }

    Model::GameMove ControllerStrToMove::convert(
        const std::string &input,
        bool confirmed
    ) {
        Model::GameMove move;

        if (input.empty())
            return move;

        try {
            /*
             * FIRST
             * - MOVE TYPE
             */
            process_first(input, &move);

            /*
             * SECOND
             * - HAND POSITION or
             * - CARAVAN NAME
             */
            process_second(input, &move);

            /*
             * THIRD
             * - CARAVAN NAME
             */
            process_third(input, &move);

            /*
             * FOURTH
             * - CARAVAN POSITION (used when selecting Face card only)
             */
            process_fourth(input, &move);
        } catch (CaravanIllegalControllerException &e) {
            if (confirmed) {
                // For confirmed moves: throw to other handling that prints
                // move errors to the player
                throw;
            }

            // For unconfirmed moves: accept whatever was able to be parsed
            // so that it can be used for highlighting the game board
            return move;
        }

        return move;
    }
}