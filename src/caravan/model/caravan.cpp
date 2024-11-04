// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/model/caravan.h"
#include "caravan/core/common.h"
#include "caravan/core/exceptions.h"


/**
 * Remove all cards from the caravan.
 *
 * @throws CaravanGameException Caravan track is empty.
 */
bool Caravan::clear(bool check_only) {
    if (i_track == 0) {
        if(check_only) {
            return false;

        } else {
            throw CaravanGameException("Cannot clear empty caravan.");
        }
    }

    if(!check_only) {
        i_track = 0;
    }

    return true;
}

/**
 * @return Current bid.
 */
uint16_t Caravan::get_bid() {
    uint16_t bid;
    uint8_t value;
    uint8_t value_final;

    bid = 0;

    for (int t = 0; t < i_track; ++t) {
        value = numeral_rank_to_uint8_t(track[t].card.rank);
        value_final = value;

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
 * @param pos Caravan position.
 * @return Slot at position.
 *
 * @throws CaravanGameException Chosen card position is out of range.
 */
Slot Caravan::get_slot(uint8_t pos) {
    if (pos < TRACK_NUMERIC_MIN or pos > i_track) {
        throw CaravanGameException(
            "The chosen card position is out of range.");
    }

    return track[pos - 1];
}

/**
 * @return Current caravan direction.
 */
Direction Caravan::get_direction() {
    Direction dir;
    int t_latest;
    int t_pen;
    int f;
    int num_queens;

    if (i_track < 2) {
        dir = ANY;
    } else {
        t_latest = i_track - 1;
        t_pen = i_track - 2;

        // The last two Numeric cards must be in the correct direction...
        if (track[t_latest].card.rank > track[t_pen].card.rank) {
            dir = ASCENDING;
        } else {
            dir = DESCENDING;
        }

        // ...unless Queens have been played against the latest numeral card.
        // The number of Queens determine if a change in direction has occurred.
        if (track[t_latest].i_faces > 0) {
            f = track[t_latest].i_faces - 1;
            num_queens = 0;

            for (f; f >= 0; --f) {
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
CaravanName Caravan::get_name() {
    return name;
}

/**
 * @return Current number of numeral cards in caravan.
 */
uint8_t Caravan::get_size() {
    return i_track;
}

/**
 * @return Current caravan suit.
 */
Suit Caravan::get_suit() {
    Suit last;
    int t;
    int f;

    if (i_track == 0) {
        return NO_SUIT;
    }

    // The last numeral card is the caravan suit...
    t = i_track - 1;
    last = track[t].card.suit;

    // ...unless a QUEEN has been played against it.
    // The most recent QUEEN placement supersedes all others.
    if (track[t].i_faces > 0) {
        f = track[t].i_faces - 1;
        for (f; f >= 0; --f) {
            if (track[t].faces[f].rank == QUEEN) {
                last = track[t].faces[f].suit;
                break;
            }
        }
    }

    return last;
}

/**
 * @param card Numeral card to put into caravan.
 *
 * @throws CaravanGameException Card is not a numeral.
 * @throws CaravanGameException Caravan is at maximum numeral card capacity.
 * @throws CaravanGameException Numeral card has same rank as most recent card in caravan.
 * @throws CaravanGameException Numeral card does not follow direction of caravan.
 */
bool Caravan::put_numeral_card(Card card, bool check_only) {
    Direction dir;
    Suit suit;
    bool ascends;
    bool not_same_suit;
    bool not_same_dir;

    if (!is_numeral_card(card)) {
        if(check_only) {
            return false;
        } else {
            throw CaravanGameException("The card must be a numeral card.");
        }
    }

    if (i_track == TRACK_NUMERIC_MAX) {
        if (check_only) {
            return false;
        } else {
            throw CaravanGameException(
                "The caravan is at its maximum numeral card capacity.");
        }
    }

    if (i_track > 0) {
        if (card.rank == track[i_track - 1].card.rank) {
            if (check_only) {
                return false;
            } else {
                throw CaravanGameException(
                    "A numeral card must not have same rank as "
                    "the most recent card in the caravan.");
            }
        }

        if (i_track > 1) {
            dir = get_direction();
            suit = get_suit();
            ascends = card.rank > track[i_track - 1].card.rank;

            not_same_suit = card.suit != suit;
            not_same_dir = (dir == ASCENDING and !ascends) or
                           (dir == DESCENDING and ascends);

            if (not_same_suit and not_same_dir) {
                if (check_only) {
                    return false;
                } else {
                    throw CaravanGameException(
                        "The numeral card must follow the caravan's "
                        "direction or match the caravan's suit.");
                }
            }
        }
    }

    if(!check_only) {
        track[i_track] = {card, {}, 0};
        i_track += 1;
    }

    return true;
}

/**
 * @param card Face card to put into caravan.
 * @param pos Position of numeral card on which to put the face card.
 * @return The numeral card on which the face card was placed.
 *
 * @throws CaravanGameException Caravan position not entered.
 * @throws CaravanGameException No numeral card at chosen position.
 * @throws CaravanGameException Chosen card is not a face card.
 * @throws CaravanGameException Numeral card is at maximum face card capacity.
 */
bool Caravan::put_face_card(Card card, uint8_t pos, Card *target, bool check_only) {
    uint8_t i;
    Card c_on;

    if (pos < TRACK_NUMERIC_MIN) {
        if(check_only) {
            return false;
        } else {
            throw CaravanGameException(
                "A caravan position has not been entered.");
        }
    }

    if (pos > i_track) {
        if(check_only) {
            return false;
        } else {
            throw CaravanGameException(
                "There is not a numeral card at caravan position " +
                std::to_string(pos) + ".");
        }
    }

    if (!is_face_card(card)) {
        if(check_only) {
            return false;
        } else {
            throw CaravanGameException(
                "The chosen card must be a face card.");
        }
    }

    i = pos - 1;
    c_on = track[i].card;

    if (card.rank == JACK) {
        if(!check_only) {
            remove_numeral_card(i);
        }

    } else {
        if (track[i].i_faces == TRACK_FACE_MAX) {
            if(check_only) {
                return false;
            } else {
                throw CaravanGameException(
                    "The caravan is at its maximum face card capacity.");
            }
        }

        if(!check_only) {
            track[i].faces[track[i].i_faces] = card;
            track[i].i_faces += 1;
        }
    }

    if(!check_only and target != nullptr) {
        *target = c_on;
    }

    return true;
}

/**
 * Remove all numeral cards of a given rank.
 *
 * @param rank The rank to remove.
 * @param pos_exclude The numeral card at the position will be excluded from
 *                    removal. If 0, no card is excluded.
 * 
 * @throws CaravanFatalException Exclude position is out of range.
 */
bool Caravan::remove_rank(Rank rank, uint8_t pos_exclude, bool check_only) {
    uint8_t i_track_original;

    if (i_track == 0) {
        return true;
    }

    if (pos_exclude > i_track) {
        if(check_only) {
            return false;
        } else {
            throw CaravanFatalException(
                "The exclude position is out of range.");
        }
    }

    if(!check_only) {
        i_track_original = i_track;

        for (int t = i_track_original - 1; t >= 0; --t) {
            if (pos_exclude > 0 and t == (pos_exclude - 1)) {
                continue;
            }

            if (track[t].card.rank == rank) {
                remove_numeral_card(t);
            }
        }
    }

    return true;
}

/**
 * Remove all numeral cards of a given suit.
 *
 * @param suit The suit to remove.
 * @param pos_exclude The numeral card at the position will be excluded from
 *                    removal. If 0, no card is excluded.
 *
 * @throws CaravanFatalException Exclude position is out of range.
 */
bool Caravan::remove_suit(Suit suit, uint8_t pos_exclude, bool check_only) {
    uint8_t i_track_original;

    if (i_track == 0) {
        return true;
    }

    if (pos_exclude > i_track) {
        if(check_only) {
            return false;
        } else {
            throw CaravanFatalException(
                "The exclude position is out of range.");
        }
    }

    if(!check_only) {
        i_track_original = i_track;

        for (int t = i_track_original - 1; t >= 0; --t) {
            if (pos_exclude > 0 and t == (pos_exclude - 1)) {
                continue;
            }

            if (track[t].card.suit == suit) {
                remove_numeral_card(t);
            }
        }
    }

    return true;
}

/*
 * PROTECTED
 */

/**
 * @param rank A numeral rank.
 * @return An integer equivalent of the numeral, range: 1-10.
 *
 * @throws CaravanFatalException If a non-numeral rank is provided.
 */
uint8_t Caravan::numeral_rank_to_uint8_t(Rank rank) {
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
            throw CaravanFatalException("Invalid rank.");
    }
}

/**
 * @param index The index of the numeral card to remove from the caravan.
 */
void Caravan::remove_numeral_card(uint8_t index) {
    for (index; (index + 1) < i_track; ++index) {
        track[index] = track[index + 1];
    }

    i_track -= 1;
}
