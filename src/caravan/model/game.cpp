// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/model/game.h"


void Game::close() {
    if (closed)
        throw CaravanFatalException("The game has already closed.");

    delete table_ptr;
    delete deck_pa_ptr;
    delete deck_pb_ptr;
    delete pa_ptr;
    delete pb_ptr;

    closed = true;
}

Player *Game::get_player(PlayerName pname) {
    if (closed)
        throw CaravanFatalException("The game has already closed.");

    if (pa_ptr->get_name() == pname)
        return pa_ptr;

    if (pb_ptr->get_name() == pname)
        return pb_ptr;

    throw CaravanFatalException("Invalid player name.");
}

PlayerCaravanNames Game::get_player_caravan_names(PlayerName pname) {
    if (closed)
        throw CaravanFatalException("The game has already closed.");

    if (pa_ptr->get_name() == pname)
        return PlayerCaravanNames{CARAVAN_A, CARAVAN_B, CARAVAN_C};

    if (pb_ptr->get_name() == pname)
        return PlayerCaravanNames{CARAVAN_D, CARAVAN_E, CARAVAN_F};

    throw CaravanFatalException("Invalid player name.");
}

PlayerName Game::get_player_turn() {
    if (closed)
        throw CaravanFatalException("The game has already closed.");

    return p_turn->get_name();
}

Table *Game::get_table() {
    if (closed)
        throw CaravanFatalException("The game has already closed.");

    return table_ptr;
}

PlayerName Game::get_winner() {
    uint8_t won_pa;
    uint8_t won_pb;
    int8_t comp[3];

    if (closed)
        throw CaravanFatalException("The game has already closed.");

    // The first player with an empty hand loses

    if (get_player(PLAYER_BOTTOM)->get_size_hand() == 0)
        return PLAYER_TOP;

    if (get_player(PLAYER_TOP)->get_size_hand() == 0)
        return PLAYER_BOTTOM;

    // Check bid sizes

    won_pa = 0;
    won_pb = 0;

    comp[0] = compare_bids(CARAVAN_A, CARAVAN_D);
    comp[1] = compare_bids(CARAVAN_B, CARAVAN_E);
    comp[2] = compare_bids(CARAVAN_C, CARAVAN_F);

    for (int i = 0; i < 3; ++i) {
        if (comp[i] < 0)
            won_pa += 1;
        else if (comp[i] > 0)
            won_pb += 1;
        else {
            // All three must be sold for there to be a winner
            return NO_PLAYER;
        }
    }

    if (won_pa >= 2)
        return pa_ptr->get_name();
    else if (won_pb >= 2)
        return pb_ptr->get_name();

    // Neither player has outbid the other...

    // Check if players have empty hands
    if (pa_ptr->get_size_hand() > 0 and pb_ptr->get_size_hand() == 0)
        return pa_ptr->get_name();
    else if (pb_ptr->get_size_hand() > 0 and pa_ptr->get_size_hand() == 0)
        return pb_ptr->get_name();

    return NO_PLAYER;
}

bool Game::is_closed() {
    return closed;
}

void Game::play_option(GameOption *go) {
    if (closed)
        throw CaravanFatalException(
                "The game has already closed.");

    if (get_winner() != NO_PLAYER)
        throw CaravanFatalException(
                "The game has already been won.");

    switch (go->type) {
        case OPTION_PLAY:
            option_play(p_turn, go);
            break;

        case OPTION_DISCARD:
            if (p_turn->get_moves_count() < MOVES_START_ROUND)
                throw CaravanGameException(
                        "A player cannot discard a card during "
                        "the Start round.");

            option_discard(p_turn, go);
            break;

        case OPTION_CLEAR:
            if (p_turn->get_moves_count() < MOVES_START_ROUND)
                throw CaravanGameException(
                        "A player cannot clear a caravan during "
                        "the Start round.");

            option_clear(p_turn, go);
            break;

        default:
            throw CaravanFatalException("Unknown play option.");
    }

    p_turn->increment_moves();
    p_turn->maybe_add_card_to_hand();

    if (pa_ptr->get_name() == p_turn->get_name())
        p_turn = pb_ptr;
    else
        p_turn = pa_ptr;
}

CaravanName Game::winning_bid(CaravanName cvname1, CaravanName cvname2) {
    int8_t bidcomp = compare_bids(cvname1, cvname2);

    if (bidcomp < 0)
        return cvname1;
    else if (bidcomp > 0)
        return cvname2;
    else
        return NO_CARAVAN;
}

/*
 * PROTECTED
 */

int8_t Game::compare_bids(CaravanName cvname1, CaravanName cvname2) {
    uint8_t bid_cn1;
    uint8_t bid_cn2;

    if (has_sold(cvname1)) {
        if (has_sold(cvname2)) {
            bid_cn1 = table_ptr->get_caravan_bid(cvname1);
            bid_cn2 = table_ptr->get_caravan_bid(cvname2);

            if (bid_cn1 > bid_cn2)
                return -1;  // CN1 sold; CN2 sold; CN1 highest bid
            else if (bid_cn1 < bid_cn2)
                return 1;  // CN1 sold; CN2 sold; CN2 highest bid
            else
                return 0;  // CN1 sold; CN2 sold; matching bids

        } else
            return -1;  // CN1 sold; CN2 unsold

    } else if (has_sold(cvname2))
        return 1;  // CN1 unsold; CN2 sold

    else
        return 0;  // CN1 unsold; CN2 unsold
}

bool Game::has_sold(CaravanName cvname) {
    uint8_t bid = table_ptr->get_caravan_bid(cvname);
    return bid >= CARAVAN_SOLD_MIN and bid <= CARAVAN_SOLD_MAX;
}

void Game::option_clear(Player *pptr, GameOption *go) {
    PlayerCaravanNames pcns = get_player_caravan_names(pptr->get_name());

    if (pcns[0] != go->caravan_name and
        pcns[1] != go->caravan_name and
        pcns[2] != go->caravan_name)
        throw CaravanGameException(
                "A player cannot clear their opponent's caravans.");

    table_ptr->clear_caravan(go->caravan_name);
}

void Game::option_discard(Player *pptr, GameOption *go) {
    Card c_hand;
    c_hand = pptr->discard_from_hand_at(go->pos_hand);
    go->card = c_hand;
}

void Game::option_play(Player *pptr, GameOption *go) {
    Card c_hand = pptr->get_from_hand_at(go->pos_hand);

    bool in_start_stage = pptr->get_moves_count() < MOVES_START_ROUND;
    bool pa_playing_num_onto_pa_caravans;
    bool pb_playing_num_onto_pb_caravans;

    if (is_numeral_card(c_hand)) {
        pa_playing_num_onto_pa_caravans =
                pptr->get_name() == pa_ptr->get_name() and
                (go->caravan_name == CARAVAN_A or
                 go->caravan_name == CARAVAN_B or
                 go->caravan_name == CARAVAN_C);

        pb_playing_num_onto_pb_caravans =
                pptr->get_name() == pb_ptr->get_name() and
                (go->caravan_name == CARAVAN_D or
                 go->caravan_name == CARAVAN_E or
                 go->caravan_name == CARAVAN_F);

        if (!(pa_playing_num_onto_pa_caravans or
              pb_playing_num_onto_pb_caravans))
            throw CaravanGameException(
                    "A numeral card can only be played on "
                    "a player's own caravan.");

        if (in_start_stage and
            table_ptr->get_caravan_size(go->caravan_name) > 0)
            throw CaravanGameException(
                    "A numeral card must be played on an empty caravan "
                    "during the Start round.");

        table_ptr->play_numeral_card(go->caravan_name, c_hand);

    } else {
        if (in_start_stage)
            throw CaravanGameException(
                    "A face card cannot be played during the "
                    "Start round.");

        table_ptr->play_face_card(
                go->caravan_name,
                c_hand,
                go->pos_caravan);
    }

    pptr->discard_from_hand_at(go->pos_hand);
    go->card = c_hand;
}
