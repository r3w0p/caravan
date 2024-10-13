// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <array>
#include <map>
#include "caravan/user/bot/train.h"
#include "caravan/core/training.h"

UserBotTrain::UserBotTrain() : UserBot(NO_PLAYER) {
    // Empty by default and populated during training.
    q_table = {};
}

GameState UserBotTrain::get_game_state(Game *game, PlayerName pname) {
    // caravans A to F...
    // get numerals from 1-8
    // for each numeral, get face cards attached to it
    // {AH, KC, 0, 0} x 8 (all ints, with 0 for any empty slot)

    // my hand...
    // ordered list of cards in hand
    // {AC, 2D, 8S, 8S, JO, 0, 0, 0} (all ints, with 0 for empty slot)

    // player indicator...
    // 0 = player abc
    // 1 = player def

    // total space needed for game state
    // 32 per caravan, 6 caravans = 192
    // 8 for hand
    // 1 for player
    // total = 201
}

std::string UserBotTrain::make_move_train(Game *game, TrainConfig *tc) {
    if (closed) { throw CaravanFatalException("Bot is closed."); }

    PlayerName pturn = game->get_player_turn();

    // Read game state and maybe add to the q-table if not present
    // game_state = get_game_state()
    // if !q_table.contains(game_state) ...
    // for each action state, set to 0

    // TODO choose an action
    //  need to determine which actions are available based on current hand

    // TODO perform action

    // TODO measure reward (1 = win, -1 = loss, 0 = neither)

    // TODO update q_table
    //  if a winner: +1 for winning player, -1 for losing player

    return "D1";  // TODO
}
