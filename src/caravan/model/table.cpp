// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/model/table.h"
#include "caravan/core/exceptions.h"


/*
 * PUBLIC
 */

Caravan *Table::get_caravan(CaravanName cvname) {
    switch (cvname) {
        case CARAVAN_A:
            return caravan_a.get();
        case CARAVAN_B:
            return caravan_b.get();
        case CARAVAN_C:
            return caravan_c.get();
        case CARAVAN_D:
            return caravan_d.get();
        case CARAVAN_E:
            return caravan_e.get();
        case CARAVAN_F:
            return caravan_f.get();
        default:
            throw CaravanFatalException("Invalid caravan name.");
    }
}

bool Table::clear_caravan(CaravanName cvname, bool check_only) {
    return get_caravan(cvname)->clear(check_only);
}

bool Table::play_face_card(CaravanName cvname, Card card, uint8_t pos, bool check_only) {
    // Intentionally not catching fatal exception if no caravan
    Caravan *cvn_target = get_caravan(cvname);

    if (card.rank == QUEEN and pos != cvn_target->get_size()) {
        if(check_only) {
            return false;
        } else {
            throw CaravanGameException(
                "A QUEEN can only be played on "
                "the latest numeral card in a caravan.");
        }
    }

    // Play Face card on Caravan.
    // Returns the Numeric card that the Face card was played on.
    Card c_target;
    if(!cvn_target->put_face_card(card, pos, &c_target, check_only)) {
        return false;
    }

    // Process effect of JOKER across all caravans
    if (card.rank == JOKER) {
        // Remove from original caravan, excluding the affected card.
        if (c_target.rank == ACE) {
            if(!cvn_target->remove_suit(c_target.suit, pos, check_only)) {
                return false;
            }
        } else {
            if(!cvn_target->remove_rank(c_target.rank, pos, check_only)) {
                return false;
            }
        }

        // Remove from other caravans, not excluding any cards.
        for (int k = 0; k < TABLE_CARAVANS_MAX; k++) {
            Caravan *cvn_other = caravans[k];

            // Ignore original caravan already handled.
            if (cvn_other->get_name() == cvn_target->get_name()) {
                continue;
            }

            if (c_target.rank == ACE) {
                if(!cvn_other->remove_suit(c_target.suit, 0, check_only)) {
                    return false;
                }
            } else {
                if(!cvn_other->remove_rank(c_target.rank, 0, check_only)) {
                    return false;
                }
            }
        }
    }

    return true;
}

bool Table::play_numeral_card(CaravanName cvname, Card card, bool check_only) {
    return get_caravan(cvname)->put_numeral_card(card, check_only);
}
