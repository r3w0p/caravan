// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "engine.h"
#include "exceptions.h"


void Engine::close() {
    if (closed)
        return;

    delete table_ptr;
    delete deck_p1_ptr;
    delete deck_p2_ptr;
    delete p1_ptr;
    delete p2_ptr;

    closed = true;
}

bool Engine::has_sold(CaravanName cn) {
    uint8_t bid = table_ptr->get_caravan_bid(cn);
    return bid >= CARAVAN_SOLD_MIN and bid <= CARAVAN_SOLD_MAX;
}

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

PlayerName Engine::get_winner() {
    if (closed)
        throw CaravanFatalException("The game has closed.");

    uint8_t won_p1 = 0;
    uint8_t won_p2 = 0;
    int8_t comp[3];

    comp[0] = compare_bids(CARAVAN_D, CARAVAN_A);
    comp[1] = compare_bids(CARAVAN_E, CARAVAN_B);
    comp[2] = compare_bids(CARAVAN_F, CARAVAN_C);

    for (int i = 0; i < 3; ++i) {
        if (comp[i] < 0)
            won_p1 += 1;
        else if (comp[i] > 0)
            won_p2 += 1;
        else {
            // All three must be sold for there to be a winner
            return NO_PLAYER;
        }
    }

    if (won_p1 >= 2)
        return PLAYER_1;
    else if (won_p2 >= 2)
        return PLAYER_2;

    // Neither player has outbid the other...

    // Check if players have empty hands
    if (p1_ptr->get_size_hand() > 0 and p2_ptr->get_size_hand() == 0)
        return PLAYER_1;
    else if (p2_ptr->get_size_hand() > 0 and p1_ptr->get_size_hand() == 0)
        return PLAYER_2;

    return NO_PLAYER;
}

void Engine::play_option(GameOption go) {
    if (closed)
        throw CaravanGameException("The game has closed.");

    if (get_winner() != NO_PLAYER)
        throw CaravanGameException("The game has already been won.");

    Player *p_ptr = p_turn == PLAYER_1 ? p1_ptr : p2_ptr;

    switch (go.type) {
        case OPTION_PLAY:
            option_play(p_ptr, go);
            break;

        case OPTION_REMOVE:
            if (p_ptr->get_moves_count() < MOVES_START_ROUND)
                throw CaravanGameException("Player cannot drop a card during the Start round.");

            option_drop(p_ptr, go);
            break;

        case OPTION_CLEAR:
            if (p_ptr->get_moves_count() < MOVES_START_ROUND)
                throw CaravanGameException("Player cannot clear a Caravan during the Start round.");

            option_clear(p_ptr, go);
            break;

        default:
            throw CaravanGameException("Unknown play_option.");
    }

    p_ptr->increment_moves_count();

    if (p_turn == PLAYER_1)
        p_turn = PLAYER_2;
    else
        p_turn = PLAYER_1;
}

void Engine::option_play(Player *p_ptr, GameOption go) {
    Card c_hand = p_ptr->get_from_hand_at(go.pos_hand);

    bool is_in_start_stage = p_ptr->get_moves_count() < MOVES_START_ROUND;
    bool is_you_playing_num_into_you_caravans;
    bool is_opp_playing_num_into_opp_caravans;

    if (is_numeric_card(c_hand)) {
        is_you_playing_num_into_you_caravans = p_ptr->get_name() == PLAYER_1 and (
                go.caravan_name == CARAVAN_D or
                go.caravan_name == CARAVAN_E or
                go.caravan_name == CARAVAN_F);

        is_opp_playing_num_into_opp_caravans = p_ptr->get_name() == PLAYER_2 and (
                go.caravan_name == CARAVAN_A or
                go.caravan_name == CARAVAN_B or
                go.caravan_name == CARAVAN_C);

        if (!(is_you_playing_num_into_you_caravans or is_opp_playing_num_into_opp_caravans))
            throw CaravanGameException(
                    "Numeric card can only be played on Player's own Caravans.");

        if (is_in_start_stage and table_ptr->get_caravan_size(go.caravan_name) > 0)
            throw CaravanGameException(
                    "Numeric card must be played on an empty Caravan during the Start round.");

        table_ptr->play_numeric_card(go.caravan_name, c_hand);

    } else {
        if (is_in_start_stage)
            throw CaravanGameException(
                    "Face card cannot be played during the Start round.");

        table_ptr->play_face_card(go.caravan_name, c_hand, go.pos_caravan);
    }

    p_ptr->remove_from_hand_at(go.pos_hand);
}

void Engine::option_drop(Player *p_ptr, GameOption go) {
    p_ptr->remove_from_hand_at(go.pos_hand);
}

void Engine::option_clear(Player *p_ptr, GameOption go) {
    table_ptr->clear_caravan(go.caravan_name);
}

PlayerName Engine::get_player_turn() {
    if (closed)
        throw CaravanFatalException("The game has closed.");

    return p_turn;
}

Table *Engine::get_table() {
    if (closed)
        throw CaravanFatalException("The game has closed.");

    return table_ptr;
}

Player *Engine::get_player(PlayerName pn) {
    if (closed)
        throw CaravanFatalException("The game has closed.");

    switch (pn) {
        case PLAYER_1:
            return p1_ptr;
        case PLAYER_2:
            return p2_ptr;
        default:
            throw CaravanFatalException("Invalid player name.");
    }
}
