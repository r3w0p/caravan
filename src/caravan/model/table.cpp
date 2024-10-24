// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/model/table.h"
#include "caravan/core/exceptions.h"

Table::~Table() {
    delete a;
    delete b;
    delete c;
    delete d;
    delete e;
    delete f;
}

/**
 * @param cvname The caravan to get.
 * @return Pointer to the caravan.
 *
 * @throws CaravanFatalException Invalid caravan name.
 */
Caravan *Table::get_caravan(CaravanName cvname) {
    switch (cvname) {
        case CARAVAN_A:
            return a;
        case CARAVAN_B:
            return b;
        case CARAVAN_C:
            return c;
        case CARAVAN_D:
            return d;
        case CARAVAN_E:
            return e;
        case CARAVAN_F:
            return f;
        default:
            throw CaravanFatalException("Invalid caravan name.");
    }
}

/**
 * @param cvname The caravan to clear.
 */
void Table::clear_caravan(CaravanName cvname) {
    get_caravan(cvname)->clear();
}

/**
 * @param cvname A caravan name.
 * @param card A face card.
 * @param pos The position of the numeral card on which to place the face card.
 *
 * @throws CaravanGameException QUEEN not played on latest numeral card in caravan.
 */
void Table::play_face_card(CaravanName cvname, Card card, uint8_t pos) {
    Caravan *cvn_target = get_caravan(cvname);

    if (card.rank == QUEEN and pos != cvn_target->get_size()) {
        throw CaravanGameException(
            "A QUEEN can only be played on the latest numeral card in a caravan.");
    }

    // Play Face card on Caravan.
    // Returns the Numeric card that the Face card was played on.
    Card c_target = cvn_target->put_face_card(card, pos);

    // Process effect of JOKER across all caravans
    if (card.rank == JOKER) {
        // Remove from original caravan, excluding the affected card.
        if (c_target.rank == ACE) {
            cvn_target->remove_suit(c_target.suit, pos);
        } else {
            cvn_target->remove_rank(c_target.rank, pos);
        }

        // Remove from other caravans, not excluding any cards.
        for (int k = 0; k < TABLE_CARAVANS_MAX; k++) {
            Caravan *p_next = caravans[k];

            // Ignore original caravan already handled.
            if (p_next->get_name() == cvn_target->get_name()) {
                continue;
            }

            if (c_target.rank == ACE) {
                p_next->remove_suit(c_target.suit, 0);
            } else {
                p_next->remove_rank(c_target.rank, 0);
            }
        }
    }
}

/**
 * @param cvname A caravan name.
 * @param card A numeral card to place in the caravan.
 */
void Table::play_numeral_card(CaravanName cvname, Card card) {
    get_caravan(cvname)->put_numeral_card(card);
}
