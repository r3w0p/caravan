// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "bot_random.h"

GameOption UserBotRandom::request_option(Engine *e) {
    Player *p;
    Table *t;
    PlayerCaravanNames pcns;
    bool start_round;

    if (e->is_closed())
        throw CaravanFatalException("The game has already closed.");

    p = e->get_player(name);
    t = e->get_table();
    pcns = e->get_player_caravan_names(name);

    start_round = p->get_moves_count() < MOVES_START_ROUND;

    return GameOption();  // TODO
}
