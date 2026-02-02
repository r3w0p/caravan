// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/model/caravan.h"
#include "caravan/core/exceptions.h"

/**
 * Remove all cards from the caravan.
 *
 * @throws CaravanIllegalModelException Caravan track is empty.
 */
void Caravan::clear() {
    if (i_track == 0) {
        throw CaravanIllegalModelException(
            "Cannot clear an empty caravan.");
    }

    i_track = 0;
}

/**
 * @return Current bid.
 */
uint16_t Caravan::get_bid() const {
    uint16_t bid = 0;

    for (int t = 0; t < i_track; ++t) {
        // CaravanFatalModelException in numeral_rank_to_uint8_t will not throw
        uint8_t value = numeral_rank_to_uint8_t(track[t].card.rank);
        uint8_t value_final = value;

        for (int f = 0; f < track[t].i_faces; ++f) {
            if (track[t].faces[f].rank == KING) {
                value_final <<= 1;
            }
        }

        bid += value_final;
    }

    return bid;
}

/**
 * @return Current caravan direction.
 */
Direction Caravan::get_direction() const {
    Direction dir;

    if (i_track < 2) {
        dir = ANY;
    } else {
        const int t_latest = i_track - 1;
        const int t_pen = i_track - 2;

        // The last two Numeric cards must be in the correct direction...
        if (track[t_latest].card.rank > track[t_pen].card.rank) {
            dir = ASCENDING;
        } else {
            dir = DESCENDING;
        }

        // ...unless Queens have been played against the latest numeral card.
        // The number of Queens determine if a change in direction has occurred.
        if (track[t_latest].i_faces > 0) {
            int num_queens = 0;

            for (int f = track[t_latest].i_faces - 1; f >= 0; --f) {
                if (track[t_latest].faces[f].rank == QUEEN) {
                    num_queens += 1;
                }
            }

            // An odd number of Queens on a card means the direction is flipped.
            if (num_queens > 0 and (num_queens % 2) != 0) {
                if (dir == ASCENDING) {
                    dir = DESCENDING;
                } else {
                    dir = ASCENDING;
                }
            }
        }
    }

    return dir;
}

/**
 * @return Caravan name.
 */
CaravanName Caravan::get_name() const {
    return name;
}

/**
 * @return Current number of numeral cards in caravan.
 */
uint8_t Caravan::get_size() const {
    return i_track;
}

/**
 * @param pos Caravan position.
 * @return Slot at position.
 *
 * @throws CaravanIllegalModelException Chosen card position is out of range.
 */
Slot Caravan::get_slot(uint8_t pos) const {
    if (pos < TRACK_NUMERIC_MIN or pos > i_track) {
        throw CaravanIllegalModelException(
            "The chosen card position is out of range.");
    }

    return track[pos - 1];
}

/**
 * @return Current caravan suit.
 */
Suit Caravan::get_suit() const {
    if (i_track == 0) {
        return NO_SUIT;
    }

    // The last numeral card is the caravan suit...
    int t = i_track - 1;
    Suit last = track[t].card.suit;

    // ...unless a QUEEN has been played against it.
    // The most recent QUEEN placement supersedes all others.
    if (track[t].i_faces > 0) {
        for (int f = track[t].i_faces - 1; f >= 0; --f) {
            if (track[t].faces[f].rank == QUEEN) {
                last = track[t].faces[f].suit;
                break;
            }
        }
    }

    return last;
}

/**
 * @param card Face card to put into caravan.
 * @param pos Position of numeral card on which to put the face card.
 * @return The numeral card on which the face card was placed.
 *
 * @throws CaravanIllegalModelException Caravan position not entered.
 * @throws CaravanIllegalModelException No numeral card at chosen position.
 * @throws CaravanIllegalModelException Chosen card is not a face card.
 * @throws CaravanIllegalModelException Numeral card is at maximum face card capacity.
 */
Card Caravan::put_face_card(Card card, uint8_t pos) {
    if (pos < TRACK_NUMERIC_MIN) {
        throw CaravanIllegalModelException(
            "A caravan position has not been entered.");
    }

    if (pos > i_track) {
        throw CaravanIllegalModelException(
            "There is not a numeral card at caravan position " +
            std::to_string(pos) + ".");
    }

    if (!card.is_face_card()) {
        throw CaravanIllegalModelException("The chosen card must be a face card.");
    }

    const uint8_t i = pos - 1;
    const Card c_on = track[i].card;

    if (card.rank == JACK) {
        remove_numeral_card(i);

    } else {
        if (track[i].i_faces == TRACK_FACE_MAX) {
            throw CaravanIllegalModelException("The caravan is at its maximum face card capacity.");
        }

        track[i].faces[track[i].i_faces] = card;
        track[i].i_faces += 1;
    }

    return c_on;
}

/**
 * @param card Numeral card to put into caravan.
 *
 * @throws CaravanIllegalModelException Card is not a numeral.
 * @throws CaravanIllegalModelException Caravan is at maximum numeral card capacity.
 * @throws CaravanIllegalModelException Numeral card has same rank as most recent card in caravan.
 * @throws CaravanIllegalModelException Numeral card does not follow direction of caravan.
 */
void Caravan::put_numeral_card(Card card) {
    if (!card.is_numeral_card()) {
        throw CaravanIllegalModelException(
            "The card must be a numeral card.");
    }

    if (i_track == TRACK_NUMERIC_MAX) {
        throw CaravanIllegalModelException(
            "The caravan is at its maximum numeral card capacity.");
    }

    if (i_track > 0) {
        if (card.rank == track[i_track - 1].card.rank) {
            throw CaravanIllegalModelException(
                "A numeral card must not have same rank as "
                "the most recent card in the caravan.");
        }

        if (i_track > 1) {
            const Direction dir = get_direction();
            const Suit suit = get_suit();
            const bool ascends = card.rank > track[i_track - 1].card.rank;

            const bool not_same_suit = card.suit != suit;
            const bool not_same_dir = (dir == ASCENDING and !ascends) or
                                      (dir == DESCENDING and ascends);

            if (not_same_suit and not_same_dir) {
                throw CaravanIllegalModelException(
                    "The numeral card must follow the caravan's "
                    "direction or match the caravan's suit.");
            }
        }
    }

    track[i_track] = {card, {}, 0};
    i_track += 1;
}

/**
 * Remove all numeral cards of a given rank.
 *
 * @param rank The rank to remove.
 * @param pos_exclude The numeral card at the position will be excluded from
 *        removal. If 0, no card is excluded.
 * 
 * @throws CaravanFatalModelException Exclude position is out of range.
 */
void Caravan::remove_rank(Rank rank, uint8_t pos_exclude) {
    if (i_track == 0) {
        return;
    }

    if (pos_exclude > i_track) {
        throw CaravanFatalModelException(
            "The exclude position is out of range.");
    }

    uint8_t i_track_original = i_track;

    for (int t = i_track_original - 1; t >= 0; --t) {
        if (pos_exclude > 0 and t == (pos_exclude - 1)) {
            continue;
        }

        if (track[t].card.rank == rank) {
            remove_numeral_card(t);
        }
    }
}

/**
 * Remove all numeral cards of a given suit.
 *
 * @param suit The suit to remove.
 * @param pos_exclude The numeral card at the position will be excluded from
 *        removal. If 0, no card is excluded.
 *
 * @throws CaravanFatalModelException Exclude position is out of range.
 */
void Caravan::remove_suit(Suit suit, uint8_t pos_exclude) {
    if (i_track == 0) {
        return;
    }

    if (pos_exclude > i_track) {
        throw CaravanFatalModelException(
            "The exclude position is out of range.");
    }

    const uint8_t i_track_original = i_track;

    for (int t = i_track_original - 1; t >= 0; --t) {
        if (pos_exclude > 0 and t == (pos_exclude - 1)) {
            continue;
        }

        if (track[t].card.suit == suit) {
            remove_numeral_card(t);
        }
    }
}

/*
 * PROTECTED
 */

/**
 * @param rank A numeral rank.
 * @return An integer equivalent of the numeral, range: 1-10.
 *
 * @throws CaravanFatalModelException If a non-numeral rank is provided.
 */
uint8_t Caravan::numeral_rank_to_uint8_t(const Rank rank) {
    switch (rank) {
        case ACE:
            return 1;
        case TWO:
            return 2;
        case THREE:
            return 3;
        case FOUR:
            return 4;
        case FIVE:
            return 5;
        case SIX:
            return 6;
        case SEVEN:
            return 7;
        case EIGHT:
            return 8;
        case NINE:
            return 9;
        case TEN:
            return 10;
        default:
            throw CaravanFatalModelException("Invalid rank.");
    }
}

/**
 * @param index The index of the numeral card to remove from the caravan.
 */
void Caravan::remove_numeral_card(uint8_t index) {
    for (; (index + 1) < i_track; ++index) {
        track[index] = track[index + 1];
    }

    i_track -= 1;
}
