// Copyright (c) 2022-2023 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "controller.h"
#include <string>

void Controller::run() {
    GameOption option;
    User *user_turn;
    PlayerCaravanNames cvns;

    do {
        user_turn = engine_ptr->get_player_turn() == user_a_ptr->get_name() ?
                    user_a_ptr : user_b_ptr;

        // Only update view if current user is a human
        if (user_turn->is_human())
            view_ptr->update(engine_ptr, user_a_ptr, user_b_ptr);

        // Get user's next move
        option = view_ptr->option(engine_ptr, user_turn);

        // Attempt to play user's desired move
        try {
            engine_ptr->play_option(option);

        } catch (CaravanGameException &e) {}

    } while (engine_ptr->get_winner() == NO_PLAYER);

    // Update one last time in order to display the winner
    view_ptr->update(engine_ptr, user_a_ptr, user_b_ptr);
}
