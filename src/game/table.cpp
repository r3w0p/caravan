// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "table.h"
#include "exceptions.h"


void Table::clear_caravan(CaravanName cn) {
    caravans[caravan_name_to_index_value(cn)].clear();
}

uint16_t Table::get_caravan_bid(CaravanName cn) {
    return caravans[caravan_name_to_index_value(cn)].get_bid();
}

Slot Table::get_caravan_cards_at(CaravanName cn, uint8_t pos) {
    return caravans[caravan_name_to_index_value(cn)].get_cards_at(pos);
}

Direction Table::get_caravan_direction(CaravanName cn) {
    return caravans[caravan_name_to_index_value(cn)].get_direction();
}

uint8_t Table::get_caravan_size(CaravanName cn) {
    return caravans[caravan_name_to_index_value(cn)].get_size();
}

Suit Table::get_caravan_suit(CaravanName cn) {
    return caravans[caravan_name_to_index_value(cn)].get_suit();
}

void Table::play_face_card(CaravanName cn, Card c, uint8_t pos) {
    Caravan *cvn_target = &caravans[caravan_name_to_index_value(cn)];

    if (c.rank == QUEEN and pos != cvn_target->get_size())
        throw CaravanGameException(
                "A Queen must be played on the numeric card "
                "at the highest position in a caravan.");

    // Play Face card on Caravan.
    // Returns the Numeric card that the Face card was played on.
    Card c_target = cvn_target->put_face_card(c, pos);

    // If Face card was JOKER.
    if (c.rank == JOKER) {
        // Remove from original Caravan, excluding the affected card.
        if (c_target.rank == ACE)
            cvn_target->remove_suit(c_target.suit, pos);
        else
            cvn_target->remove_rank(c_target.rank, pos);

        // Remove from other Caravans, not excluding any cards.
        for (int k = 0; k < TABLE_CARAVANS_MAX; k++) {
            Caravan *p_next = &caravans[k];

            // Ignore original Caravan already handled.
            if (p_next->get_name() == cvn_target->get_name())
                continue;

            if (c_target.rank == ACE)
                p_next->remove_suit(c_target.suit, 0);
            else
                p_next->remove_rank(c_target.rank, 0);
        }
    }
}

void Table::play_numeric_card(CaravanName cn, Card c) {
    Caravan *caravan_ptr = &caravans[caravan_name_to_index_value(cn)];
    caravan_ptr->put_numeric_card(c);
}

/*
 * PROTECTED
 */

uint8_t Table::caravan_name_to_index_value(CaravanName cn) {
    switch (cn) {
        case CARAVAN_D:
            return 0;
        case CARAVAN_E:
            return 1;
        case CARAVAN_F:
            return 2;
        case CARAVAN_A:
            return 3;
        case CARAVAN_B:
            return 4;
        case CARAVAN_C:
            return 5;
        default:
            throw CaravanFatalException("Invalid caravan name.");
    }
}
