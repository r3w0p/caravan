// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/model/caravan.h"
#include "caravan/core/exceptions.h"
#include "caravan/core/functions.h"

namespace Caravan::Model {
    bool Caravan::check_card(Card card, uint8_t pos) const {
        if (card.is_numeral_card()) {
            // Can play any numeral card in an empty caravan
            if (pos_track == 0) {
                return true;
            }

            // Cannot play a numeral on a full caravan
            if (pos_track == TRACK_NUMERIC_MAX) {
                return false;
            }

            Card card_last = get_slot(pos_track).card;
            Direction direction = get_direction();

            // Card has a different rank to the last card, and
            // either matches the direction or matches the caravan's suit
            bool ranks_do_not_match = card.rank != card_last.rank;
            bool in_direction =
                direction == ANY ||
                (direction == ASCENDING && card.rank > card_last.rank) ||
                (direction == DESCENDING && card.rank < card_last.rank);
            bool suit_matches_cvn = card.suit == get_suit();

            if (
                ranks_do_not_match and
                (in_direction or suit_matches_cvn)
            ) {
                return true;
            }

        } else {

            // Cannot play a face card in an empty caravan
            if (pos_track == 0) {
                return false;
            }

            // Cannot play a face card in an empty slot
            if (pos < TRACK_NUMERIC_MIN || pos > pos_track) {
                return false;
            }

            // Can play a face card if there is still room in the slot
            return get_slot(pos).n_faces < TRACK_FACE_MAX;
        }

        return false;
    }

    /**
     * Remove all cards from the caravan.
     *
     * @throws CaravanIllegalModelException Caravan track is empty.
     */
    void Caravan::clear() {
        if (pos_track == 0) {
            throw CaravanIllegalModelException(
                "Cannot clear an empty caravan."
            );
        }

        pos_track = 0;
    }

    /**
     * @return Current bid.
     */
    uint16_t Caravan::get_bid() const {
        uint16_t bid = 0;

        for (int t = 0; t < pos_track; ++t) {
            // CaravanFatalModelException in numeral_rank_to_uint8_t will not throw
            uint8_t value = numeral_rank_to_uint8_t(track[t].card.rank);
            uint8_t value_final = value;

            for (int f = 0; f < track[t].n_faces; ++f) {
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

        if (pos_track < 2) {
            dir = ANY;
        } else {
            const int t_latest = pos_track - 1;
            const int t_pen = pos_track - 2;

            // The last two Numeric cards must be in the correct direction...
            if (track[t_latest].card.rank > track[t_pen].card.rank) {
                dir = ASCENDING;
            } else {
                dir = DESCENDING;
            }

            // ...unless Queens have been played against the latest numeral card.
            // The number of Queens determine if a change in direction has occurred.
            if (track[t_latest].n_faces > 0) {
                int num_queens = 0;

                for (int f = track[t_latest].n_faces - 1; f >= 0; --f) {
                    if (track[t_latest].faces[f].rank == QUEEN) {
                        num_queens += 1;
                    }
                }

                // An odd number of Queens on a card means the direction is flipped.
                if (num_queens > 0
                    and (num_queens % 2) != 0
                ) {
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
        return pos_track;
    }

    /**
     * @param pos Caravan position.
     * @return Slot at position.
     *
     * @throws CaravanIllegalModelException Chosen card position is out of range.
     */
    Slot Caravan::get_slot(uint8_t pos) const {
        if (pos < TRACK_NUMERIC_MIN or pos > pos_track) {
            std::string err =
                "Must provide a valid slot number "
                "to play a face card on Caravan " +
                caravan_name_to_str(name, true);

            if (pos_track == TRACK_NUMERIC_MIN) {
                err += " (currently " + std::to_string(pos_track) + " only).";
            } else if (pos_track == TRACK_NUMERIC_MIN + 1) {
                err += " (currently " + std::to_string(TRACK_NUMERIC_MIN) +
                    " or " + std::to_string(pos_track) + ").";
            } else {
                err += " (currently " + std::to_string(TRACK_NUMERIC_MIN) +
                    "-" + std::to_string(pos_track) + ", inclusive).";
            }

            throw CaravanIllegalModelException(err);
        }

        return track[pos - 1];
    }

    /**
     * @return Current caravan suit.
     */
    Suit Caravan::get_suit() const {
        if (pos_track == 0) {
            return NO_SUIT;
        }

        // The last numeral card is the caravan suit...
        int t = pos_track - 1;
        Suit last = track[t].card.suit;

        // ...unless a QUEEN has been played against it.
        // The most recent QUEEN placement supersedes all others.
        if (track[t].n_faces > 0) {
            for (int f = track[t].n_faces - 1; f >= 0; --f) {
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
                "A caravan position has not been entered."
            );
        }

        if (pos > pos_track) {
            throw CaravanIllegalModelException(
                "There is not a numeral card at position " +
                std::to_string(pos) +
                " for Caravan " +
                caravan_name_to_str(this->name, true) +
                "."
            );
        }

        if (!card.is_face_card()) {
            throw CaravanIllegalModelException(
                "The chosen card must be a face card "
                "to play it at position " +
                std::to_string(pos) +
                " on Caravan " +
                caravan_name_to_str(this->name, true) +
                "."
            );
        }

        const uint8_t i = pos - 1;
        const Card c_on = track[i].card;

        if (card.rank == JACK) {
            remove_numeral_card(i);
        } else {
            if (track[i].n_faces == TRACK_FACE_MAX) {
                throw CaravanIllegalModelException(
                    "Caravan " +
                    caravan_name_to_str(this->name, true) +
                    " is at its maximum face card capacity."
                );
            }

            track[i].faces[track[i].n_faces] = card;
            track[i].n_faces += 1;
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
                "The card must be a numeral card."
            );
        }

        if (pos_track == TRACK_NUMERIC_MAX) {
            throw CaravanIllegalModelException(
                "Caravan " +
                caravan_name_to_str(name, true) +
                " is at its maximum numeral card capacity."
            );
        }

        if (pos_track > 0) {
            if (card.rank == track[pos_track - 1].card.rank) {
                throw CaravanIllegalModelException(
                    "The numeral card must not have the same rank "
                    "as the most recent card in Caravan " +
                    caravan_name_to_str(name, true) +
                    "."
                );
            }

            if (pos_track > 1) {
                const Direction dir = get_direction();
                const Suit suit = get_suit();
                const bool ascends = card.rank > track[pos_track - 1].card.rank;

                const bool not_same_suit = card.suit != suit;
                const bool not_same_dir =
                    (dir == ASCENDING and !ascends) or
                    (dir == DESCENDING and ascends);

                if (not_same_suit and not_same_dir) {
                    throw CaravanIllegalModelException(
                        "The numeral card must follow Caravan " +
                        caravan_name_to_str(name, true) +
                        "'s direction or match its suit."
                    );
                }
            }
        }

        track[pos_track] = {card, {}, 0};
        pos_track += 1;
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
        if (pos_track == 0) {
            return;
        }

        if (pos_exclude > pos_track) {
            throw CaravanFatalModelException(
                "The exclude position is out of range."
            );
        }

        uint8_t i_track_original = pos_track;

        for (int t = i_track_original - 1; t >= 0; --t) {
            if (
                pos_exclude > 0 and
                t == pos_exclude - 1
            ) {
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
        if (pos_track == 0) {
            return;
        }

        if (pos_exclude > pos_track) {
            throw CaravanFatalModelException(
                "The exclude position is out of range."
            );
        }

        const uint8_t i_track_original = pos_track;

        for (int t = i_track_original - 1; t >= 0; --t) {
            if (
                pos_exclude > 0 and
                t == pos_exclude - 1
            ) {
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
     * @param index The index of the numeral card to remove from the caravan.
     */
    void Caravan::remove_numeral_card(uint8_t index) {
        for (; index + 1 < pos_track; ++index) {
            track[index] = track[index + 1];
        }

        pos_track -= 1;
    }
}
