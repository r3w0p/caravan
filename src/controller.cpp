// Copyright (c) 2022-2023 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "controller.h"
#include <string>
#include <curses.h>

void Controller::run() {
    GameOption go_bottom;
    GameOption go_top;
    GameOption go_temp;
    User *user_turn;
    PlayerCaravanNames cvns;

    go_bottom = { NO_OPTION };
    go_top = { NO_OPTION };

    do {
        user_turn = engine_ptr->get_player_turn() == user_a_ptr->get_name() ?
                    user_a_ptr : user_b_ptr;

        // Only update view if current user is a human
        if (user_turn->is_human())
            view_ptr->update(engine_ptr, user_a_ptr, user_b_ptr,
                             &go_bottom, &go_top);

        try {
            // Get user's next move
            if(user_turn->get_name() == PLAYER_BOTTOM) {
                go_temp = go_bottom;
                go_bottom = view_ptr->option(engine_ptr, user_turn);

                // Immediately quit on exit request
                if(go_bottom.type == OPTION_EXIT)
                    return;

                // Attempt to play user's desired move
                engine_ptr->play_option(&go_bottom);

            } else {
                go_temp = go_top;
                go_top = view_ptr->option(engine_ptr, user_turn);

                // Immediately quit on exit request
                if(go_top.type == OPTION_EXIT)
                    return;

                // Attempt to play user's desired move
                engine_ptr->play_option(&go_top);
            }

        } catch (CaravanException &e) {
            view_ptr->error_message(e.what());

            if(user_turn->get_name() == PLAYER_BOTTOM)
                go_bottom = go_temp;
            else
                go_top = go_temp;
        }

    } while (engine_ptr->get_winner() == NO_PLAYER);

    // Update one last time in order to display the winner
    view_ptr->update(engine_ptr, user_a_ptr, user_b_ptr, &go_bottom, &go_top);
}
