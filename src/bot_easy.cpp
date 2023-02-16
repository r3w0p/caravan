// Copyright (c) 2022-2023 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <random>
#include "bot_easy.h"

uint8_t random_uint8_t(uint8_t min, uint8_t max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(min, max);

    return distr(gen);
}

uint8_t pos_card_numeral(Player *p) {
    uint8_t size_hand = p->get_size_hand();
    Hand h = p->get_hand();

    for (int i = 0; i < size_hand; ++i)
        if (is_numeral_card(h[i]))
            return i + 1;

    return 0;
}

GameOption UserBotEasy::generate_option(Engine *e) {
    Player *p;
    uint8_t p_hand_size;
    Table *t;
    PlayerCaravanNames pcns_me;
    PlayerCaravanNames pcns_opp;
    uint8_t pos_hand;
    uint16_t moves;
    Card c_hand;
    Slot slot_cvn;
    uint8_t bid_me;
    uint8_t bid_opp;
    uint8_t size_cvn;
    Direction dir_cvn;
    uint8_t i_cvn_most_cards;
    uint8_t n_cvn_most_cards;

    if (e->is_closed())
        throw CaravanFatalException("The game has already closed.");

    p = e->get_player(name);
    p_hand_size = p->get_size_hand();

    if (p_hand_size == 0)
        throw CaravanFatalException("Bot has an empty hand.");

    pcns_me = e->get_player_caravan_names(name);
    pcns_opp = e->get_player_caravan_names(
            name == PLAYER_BOTTOM ? PLAYER_TOP : PLAYER_BOTTOM);

    // Add numeral cards for start round
    if (p->get_moves_count() < MOVES_START_ROUND) {
        pos_hand = pos_card_numeral(p);

        if (pos_hand == 0)
            throw CaravanFatalException(
                    "Bot does not have enough numeral cards to finish "
                    "the Start round.");

        moves = p->get_moves_count();

        if (moves == 0)
            return {OPTION_PLAY, pos_hand, pcns_me[0], 0};

        if (moves == 1)
            return {OPTION_PLAY, pos_hand, pcns_me[1], 0};

        if (moves == 2)
            return {OPTION_PLAY, pos_hand, pcns_me[2], 0};

        throw CaravanFatalException(
                "Bot cannot be in Start state after 3 moves.");

    } else {
        // After start round
        t = e->get_table();

        for (uint8_t pos = 1; pos <= p_hand_size; ++pos) {
            c_hand = p->get_from_hand_at(pos);

            if (is_numeral_card(c_hand)) {
                for (uint8_t i = 0; i < PLAYER_CARAVANS_MAX; ++i) {
                    bid_me = t->get_caravan_bid(pcns_me[i]);
                    bid_opp = t->get_caravan_bid(pcns_opp[i]);

                    // Skip caravan if sold and winning
                    if (bid_me >= CARAVAN_SOLD_MIN and
                        bid_me <= CARAVAN_SOLD_MAX and
                        (bid_opp > CARAVAN_SOLD_MAX or bid_me > bid_opp))
                        continue;

                    // Clear caravan if bust
                    if (bid_me > CARAVAN_SOLD_MAX)
                        return {OPTION_CLEAR, 0, pcns_me[i], 0};

                    size_cvn = t->get_caravan_size(pcns_me[i]);

                    // Not sold and caravan full, clear
                    if (size_cvn == TRACK_NUMERIC_MAX)
                        return {OPTION_CLEAR, 0, pcns_me[i], 0};

                    // Play card if empty
                    if (size_cvn == 0)
                        return {OPTION_PLAY, pos, pcns_me[i], 0};

                    slot_cvn = t->get_slot_at(pcns_me[i], size_cvn);

                    // Cards have same rank
                    if (slot_cvn.card.rank == c_hand.rank)
                        continue;

                    // Same suit, play anything
                    if (slot_cvn.card.suit == c_hand.suit)
                        return {OPTION_PLAY, pos, pcns_me[i], 0};

                    dir_cvn = t->get_caravan_direction(pcns_me[i]);

                    // Card ascending with caravan
                    if (dir_cvn == ASCENDING and
                        c_hand.rank > slot_cvn.card.rank)
                        return {OPTION_PLAY, pos, pcns_me[i], 0};

                    // Card descending with caravan
                    if (dir_cvn == DESCENDING and
                        c_hand.rank < slot_cvn.card.rank)
                        return {OPTION_PLAY, pos, pcns_me[i], 0};
                }

            } else {
                i_cvn_most_cards = 3;
                n_cvn_most_cards = 0;

                // Put face card randomly on an opp caravan with the most cards
                for (uint8_t i = 0; i < PLAYER_CARAVANS_MAX; ++i) {
                    size_cvn = t->get_caravan_size(pcns_opp[i]);

                    if (size_cvn == 0)
                        continue;

                    if (size_cvn > n_cvn_most_cards) {
                        i_cvn_most_cards = i;
                        n_cvn_most_cards = size_cvn;
                    }
                }

                // If there exists an opp caravan with at least 1 card on it
                if (i_cvn_most_cards < PLAYER_CARAVANS_MAX) {
                    slot_cvn = t->get_slot_at(
                            pcns_opp[i_cvn_most_cards], size_cvn);

                    if (slot_cvn.i_faces < TRACK_FACE_MAX) {
                        return {OPTION_PLAY,
                                pos,
                                pcns_opp[i_cvn_most_cards],
                                n_cvn_most_cards};
                    }
                }
            }
        }
    }

    // Bot cannot make any move besides discarding a card
    return {OPTION_DISCARD, 1};
}
