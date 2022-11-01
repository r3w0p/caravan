// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "bot_random.h"

uint8_t pos_card_numeric(Player *p) {
    uint8_t size_hand = p->get_size_hand();
    Hand h = p->get_hand();

    for(int i = 0; i < size_hand; ++i)
        if(is_numeric_card(h[i]))
            return i+1;

    return 0;
}

GameOption UserBotRandom::request_option(Engine *e, View *v) {
    Player *p;
    Table *t;
    PlayerCaravanNames pcns_me;
    PlayerCaravanNames pcns_opp;
    uint8_t pos_hand;
    uint16_t moves;
    Card c;

    if (e->is_closed())
        throw CaravanFatalException("The game has already closed.");

    p = e->get_player(name);

    if (p->get_size_hand() == 0)
        throw CaravanFatalException("Bot has an empty hand.");

    pcns_me = e->get_player_caravan_names(name);
    pcns_opp = e->get_player_caravan_names(
            name == PLAYER_A ? PLAYER_B : PLAYER_A);

    if(p->get_moves_count() < MOVES_START_ROUND) {
        pos_hand = pos_card_numeric(p);
        moves = p->get_moves_count();

        if(moves == 0)
            return {OPTION_PLAY, pos_hand, pcns_me.cn_t1};

        if(moves == 1)
            return {OPTION_PLAY, pos_hand, pcns_me.cn_t2};

        if(moves == 2)
            return {OPTION_PLAY, pos_hand, pcns_me.cn_t3};

        throw CaravanFatalException(
                "Bot cannot be in Start state after 3 moves.");

    } else {
        t = e->get_table();
        c = p->get_from_hand_at(1);

        if(is_numeric_card(c)) {

        } else {

        }
    }

    return GameOption();
}
