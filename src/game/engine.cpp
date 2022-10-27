// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "engine.h"
#include "exceptions.h"


void Engine::close() {
    if (closed)
        throw CaravanFatalException("The game has already closed.");

    delete table_ptr;
    delete deck_pa_ptr;
    delete deck_pb_ptr;
    delete pa_ptr;
    delete pb_ptr;

    closed = true;
}

Player *Engine::get_player(PlayerName pn) {
    if (closed)
        throw CaravanFatalException("The game has already closed.");

    if (pa_ptr->get_name() == pn)
        return pa_ptr;

    if (pb_ptr->get_name() == pn)
        return pb_ptr;

    throw CaravanFatalException("Invalid player name.");
}

PlayerCaravanNames Engine::get_player_caravan_names(PlayerName pn) {
    if (closed)
        throw CaravanFatalException("The game has already closed.");

    if (pa_ptr->get_name() == pn)
        return PlayerCaravanNames{CARAVAN_D, CARAVAN_E, CARAVAN_F};

    if (pb_ptr->get_name() == pn)
        return PlayerCaravanNames{CARAVAN_A, CARAVAN_B, CARAVAN_C};

    throw CaravanFatalException("Invalid player name.");
}

PlayerName Engine::get_player_turn() {
    if (closed)
        throw CaravanFatalException("The game has already closed.");

    return p_turn->get_name();
}

Table *Engine::get_table() {
    if (closed)
        throw CaravanFatalException("The game has already closed.");

    return table_ptr;
}

PlayerName Engine::get_winner() {
    if (closed)
        throw CaravanFatalException("The game has already closed.");

    uint8_t won_pa = 0;
    uint8_t won_pb = 0;
    int8_t comp[3];

    comp[0] = compare_bids(CARAVAN_D, CARAVAN_A);
    comp[1] = compare_bids(CARAVAN_E, CARAVAN_B);
    comp[2] = compare_bids(CARAVAN_F, CARAVAN_C);

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

bool Engine::is_closed() {
    return closed;
}

void Engine::play_option(GameOption go) {
    if (closed)
        throw CaravanFatalException(
                "The game has already closed.");

    if (get_winner() != NO_PLAYER)
        throw CaravanFatalException(
                "The game has already been won.");

    switch (go.type) {
        case OPTION_PLAY:
            option_play(p_turn, go);
            break;

        case OPTION_REMOVE:
            if (p_turn->get_moves_count() < MOVES_START_ROUND)
                throw CaravanGameException(
                        "A player cannot drop a card during "
                        "the Start round.");

            option_drop(p_turn, go);
            break;

        case OPTION_CLEAR:
            if (p_turn->get_moves_count() < MOVES_START_ROUND)
                throw CaravanGameException(
                        "A player cannot clear a Caravan during "
                        "the Start round.");

            option_clear(p_turn, go);
            break;

        default:
            throw CaravanFatalException("Unknown play option.");
    }

    p_turn->increment_moves_count();

    if (pa_ptr->get_name() == p_turn->get_name())
        p_turn = pb_ptr;
    else
        p_turn = pa_ptr;
}

/*
 * PROTECTED
 */

int8_t Engine::compare_bids(CaravanName cn1, CaravanName cn2) {
    uint8_t bid_cn1;
    uint8_t bid_cn2;

    if (has_sold(cn1)) {
        if (has_sold(cn2)) {
            bid_cn1 = table_ptr->get_caravan_bid(cn1);
            bid_cn2 = table_ptr->get_caravan_bid(cn2);

            if (bid_cn1 > bid_cn2)
                return -1;  // CN1 sold; CN2 sold; CN1 highest bid
            else if (bid_cn2 > bid_cn1)
                return 1;  // CN1 sold; CN2 sold; CN2 highest bid
            else
                return 0;  // CN1 sold; CN2 sold; matching bids

        } else
            return -1;  // CN1 sold; CN2 unsold

    } else if (has_sold(cn2))
        return 1;  // CN1 unsold; CN2 sold
    else
        return 0;  // CN1 unsold; CN2 unsold
}

bool Engine::has_sold(CaravanName cn) {
    uint8_t bid = table_ptr->get_caravan_bid(cn);
    return bid >= CARAVAN_SOLD_MIN and bid <= CARAVAN_SOLD_MAX;
}

void Engine::option_clear(Player *p_ptr, GameOption go) {
    table_ptr->clear_caravan(go.caravan_name);
}

void Engine::option_drop(Player *p_ptr, GameOption go) {
    p_ptr->remove_from_hand_at(go.pos_hand);
}

void Engine::option_play(Player *p_ptr, GameOption go) {
    Card c_hand = p_ptr->get_from_hand_at(go.pos_hand);

    bool in_start_stage = p_ptr->get_moves_count() < MOVES_START_ROUND;
    bool pa_playing_num_into_pa_caravans;
    bool pb_playing_num_into_pb_caravans;

    if (is_numeric_card(c_hand)) {
        pa_playing_num_into_pa_caravans =
                p_ptr->get_name() == pa_ptr->get_name() and
                (go.caravan_name == CARAVAN_D or
                 go.caravan_name == CARAVAN_E or
                 go.caravan_name == CARAVAN_F);

        pb_playing_num_into_pb_caravans =
                p_ptr->get_name() == pb_ptr->get_name() and
                (go.caravan_name == CARAVAN_A or
                 go.caravan_name == CARAVAN_B or
                 go.caravan_name == CARAVAN_C);

        if (!(pa_playing_num_into_pa_caravans or
              pb_playing_num_into_pb_caravans))
            throw CaravanGameException(
                    "A numeric card can only be played on "
                    "a player's own caravans.");

        if (in_start_stage and
            table_ptr->get_caravan_size(go.caravan_name) > 0)
            throw CaravanGameException(
                    "A numeric card must be played on an empty caravan "
                    "during the Start round.");

        table_ptr->play_numeric_card(go.caravan_name, c_hand);

    } else {
        if (in_start_stage)
            throw CaravanGameException(
                    "A face card cannot be played during the "
                    "Start round.");

        table_ptr->play_face_card(
                go.caravan_name,
                c_hand,
                go.pos_caravan);
    }

    p_ptr->remove_from_hand_at(go.pos_hand);
}
