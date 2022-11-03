// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "controller.h"
#include "iostream"
#include "exceptions.h"
#include "common.h"
#include "engine.h"
#include <string>


// if(instanceof<Parent>(chld1))
template<typename A, typename B>
inline bool instanceof(const B *ptr) {
    return dynamic_cast<const A *>(ptr) != nullptr;
}

std::string generate_option_msg(PlayerName pn, GameOption go) {
    if (go.type == OPTION_PLAY) {
        return player_name_to_str(pn) +
               " played " +
               (go.pos_caravan == 0
                ? "numeric"
                : "face") +
               " card onto " +
               caravan_name_to_str(go.caravan_name) +
               (go.pos_caravan == 0
                ? ""
                : " at position " + std::to_string(go.pos_caravan)) +
               ".";

    } else if (go.type == OPTION_DISCARD) {
        return player_name_to_str(pn) +
               " discarded a card from their hand.";

    } else if (go.type == OPTION_CLEAR) {
        return player_name_to_str(pn) +
               " cleared caravan " +
               caravan_name_to_str(go.caravan_name) +
               ".";
    }

    throw CaravanFatalException("Invalid option for message generation.");
}

void Controller::run() {
    GameOption option;
    std::string msg;
    User *user_turn;
    PlayerName winner;
    PlayerCaravanNames cvns;

    do {
        user_turn = engine_ptr->get_player_turn() == user_a_ptr->get_name() ?
                    user_a_ptr : user_b_ptr;

        if (user_turn->is_human())
            view_ptr->display(engine_ptr, msg);

        msg = "";
        option = user_turn->request_option(engine_ptr, view_ptr);

        try {
            engine_ptr->play_option(option);
            msg = generate_option_msg(user_turn->get_name(), option);

        } catch (CaravanGameException &e) {
            msg = e.what();
            std::cout << typeid(e).name() << std::endl;
        }

    } while ((winner = engine_ptr->get_winner()) == NO_PLAYER);

    msg = "Winner is: " + player_name_to_str(winner) + "\n";
    view_ptr->display(engine_ptr, msg);
}

GameOption UserHuman::request_option(Engine *e, View *v) {
    return v->get_input_option(e, name);
}
