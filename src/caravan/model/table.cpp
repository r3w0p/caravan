// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/model/table.h"
#include "caravan/core/exceptions.h"

Table::Table() {
    cvn_a = std::make_unique<Caravan>(CARAVAN_A);
    cvn_b = std::make_unique<Caravan>(CARAVAN_B);
    cvn_c = std::make_unique<Caravan>(CARAVAN_C);
    cvn_d = std::make_unique<Caravan>(CARAVAN_D);
    cvn_e = std::make_unique<Caravan>(CARAVAN_E);
    cvn_f = std::make_unique<Caravan>(CARAVAN_F);

    caravans = {
        cvn_a.get(),
        cvn_b.get(),
        cvn_c.get(),
        cvn_d.get(),
        cvn_e.get(),
        cvn_f.get()
    };
}

/**
 * @param cvname The caravan to clear.
 */
void Table::clear_caravan(CaravanName cvname) const {
    get_caravan(cvname)->clear();
}

/**
 * @param cvname The caravan to get.
 * @return Pointer to the caravan.
 *
 * @throws CaravanFatalModelException Invalid caravan name.
 */
Caravan* Table::get_caravan(CaravanName cvname) const {
    switch (cvname) {
        case CARAVAN_A:
            return caravans[0];
        case CARAVAN_B:
            return caravans[1];
        case CARAVAN_C:
            return caravans[2];
        case CARAVAN_D:
            return caravans[3];
        case CARAVAN_E:
            return caravans[4];
        case CARAVAN_F:
            return caravans[5];
        default:
            throw CaravanFatalModelException("Invalid caravan name.");
    }
}

/**
 * @param cvname A caravan name.
 * @param card A face card.
 * @param pos The position of the numeral card on which to place the face card.
 *
 * @throws CaravanFatalModelException Invalid caravan name.
 * @throws CaravanIllegalModelException QUEEN not played on latest numeral card in caravan.
 */
void Table::play_face_card(
    CaravanName cvname,
    const Card card,
    const uint8_t pos) const {

    Caravan *cvn_target = get_caravan(cvname);

    if (card.rank == QUEEN and pos != cvn_target->get_size()) {
        throw CaravanIllegalModelException(
            "A QUEEN can only be played on the "
            "latest numeral card in a caravan.");
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
        for (int i = 0; i < TABLE_CARAVANS_MAX; ++i) {
            Caravan *p_next = caravans[i];

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
 *
 * @throws CaravanFatalModelException Invalid caravan name.
 */
void Table::play_numeral_card(CaravanName cvname, const Card card) const {
    get_caravan(cvname)->put_numeral_card(card);
}
