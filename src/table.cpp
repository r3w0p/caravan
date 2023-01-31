// Copyright (c) 2022-2023 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "table.h"
#include "exceptions.h"

/**
 * The table on which caravan tracks are placed, as well as the decks and hands
 * of both players.
 */
Table::Table() {}

/**
 * @param cvname The caravan to clear.
 */
void Table::clear_caravan(CaravanName cvname) {
    caravans[caravan_name_to_uint8_t_index(cvname)]->clear();
}

/**
 * @param cvname A caravan name.
 * @return The current bid of the named caravan.
 */
uint16_t Table::get_caravan_bid(CaravanName cvname) {
    return caravans[caravan_name_to_uint8_t_index(cvname)]->get_bid();
}

/**
 * @param cvname A caravan name.
 * @param pos The position at the named caravan.
 * @return The slot (i.e. numeral card and attached face cards) at the position.
 */
Slot Table::get_slot_at(CaravanName cvname, uint8_t pos) {
    return caravans[caravan_name_to_uint8_t_index(cvname)]->get_slot(pos);
}

/**
 * @param cvname A caravan name.
 * @return The current direction of the named caravan.
 */
Direction Table::get_caravan_direction(CaravanName cvname) {
    return caravans[caravan_name_to_uint8_t_index(cvname)]->get_direction();
}

/**
 * @param cvname A caravan name.
 * @return The current number of numeral cards in the named caravan.
 */
uint8_t Table::get_caravan_size(CaravanName cvname) {
    return caravans[caravan_name_to_uint8_t_index(cvname)]->get_size();
}

/**
 * @param cvname A caravan name.
 * @return The current suit of the named caravan.
 */
Suit Table::get_caravan_suit(CaravanName cvname) {
    return caravans[caravan_name_to_uint8_t_index(cvname)]->get_suit();
}

/**
 * @param cvname A caravan name.
 * @param card A face card.
 * @param pos The position of the numeral card on which to place the face card.
 */
void Table::play_face_card(CaravanName cvname, Card card, uint8_t pos) {
    Caravan *cvn_target = caravans[caravan_name_to_uint8_t_index(cvname)];

    if (card.rank == QUEEN and pos != cvn_target->get_size())
        throw CaravanGameException(
                "A QUEEN can only be played on the latest numeral card "
                "in a caravan.");

    // Play Face card on Caravan.
    // Returns the Numeric card that the Face card was played on.
    Card c_target = cvn_target->put_face_card(card, pos);

    // If Face card was JOKER.
    if (card.rank == JOKER) {
        // Remove from original caravan, excluding the affected card.
        if (c_target.rank == ACE)
            cvn_target->remove_suit(c_target.suit, pos);
        else
            cvn_target->remove_rank(c_target.rank, pos);

        // Remove from other caravans, not excluding any cards.
        for (int k = 0; k < TABLE_CARAVANS_MAX; k++) {
            Caravan *p_next = caravans[k];

            // Ignore original caravan already handled.
            if (p_next->get_name() == cvn_target->get_name())
                continue;

            if (c_target.rank == ACE)
                p_next->remove_suit(c_target.suit, 0);
            else
                p_next->remove_rank(c_target.rank, 0);
        }
    }
}

/**
 * @param cvname A caravan name.
 * @param card A numeral card to place in the caravan.
 */
void Table::play_numeral_card(CaravanName cvname, Card card) {
    Caravan *caravan_ptr = caravans[caravan_name_to_uint8_t_index(cvname)];
    caravan_ptr->put_numeral_card(card);
}

/*
 * PROTECTED
 */

/**
 * @param cvname A caravan name.
 * @return The index of the named caravan, as it is stored in the table's
 *         internal array of caravans.
 */
uint8_t Table::caravan_name_to_uint8_t_index(CaravanName cvname) {
    switch (cvname) {
        case CARAVAN_A:
            return 0;
        case CARAVAN_B:
            return 1;
        case CARAVAN_C:
            return 2;
        case CARAVAN_D:
            return 3;
        case CARAVAN_E:
            return 4;
        case CARAVAN_F:
            return 5;
        default:
            throw CaravanFatalException("Invalid caravan name.");
    }
}
