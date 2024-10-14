// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <array>
#include <map>
#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>
#include "caravan/user/bot/train.h"
#include "caravan/core/training.h"

UserBotTrain::UserBotTrain() : UserBot(NO_PLAYER) {
    // Empty by default and populated during training
    q_table = {};
    populate_action_space(&action_space);
}

std::string UserBotTrain::make_move_train(Game *game, TrainConfig *tc) {
    if (closed) { throw CaravanFatalException("Bot is closed."); }

    // Get name of player whose turn it is
    PlayerName pname = game->get_player_turn();

    // Read game state and maybe add to the q-table if not present
    GameState gs;
    get_game_state(&gs, game, pname);

    if (!q_table.contains(gs)) {
        // TODO maybe is this needed: q_table[gs] = {};
        for(uint16_t i = 0; i < SIZE_ACTION_SPACE; i++) {
            q_table[gs][action_space[i]] = 0;
        }
    }

    // Choose an action
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<> distr_explore(0, 1);
    std::uniform_int_distribution<> distr_action(0, SIZE_ACTION_SPACE-1);

    bool explore = distr_explore(gen) < tc->explore;
    std::string action;

    if(explore) {
        // If exploring, fetch a random action from the action space
        action = action_space[distr_action(gen)];

    } else {
        // Otherwise, pick the optimal action from the q-table
        // (Or random action if all actions are equal in value)
        // TODO
    }

    // TODO perform action

    // TODO measure reward (1 = win, -1 = loss, 0 = neither)

    // TODO update q_table
    //  if a winner: +1 for winning player, -1 for losing player

    return "D1";  // TODO
}
