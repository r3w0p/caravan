// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan.h"
#include "exceptions.h"


void Caravan::clear() {
    if (i_track == 0)
        throw CaravanGameException("The Caravan is empty.");

    i_track = 0;
}

Suit Caravan::get_suit() {
    Suit last;
    int t;
    int f;

    if (i_track == 0)
        return NO_SUIT;

    // The last Numeric card is the caravan suit...
    t = i_track - 1;
    last = track[t].card.suit;

    // ...unless a Queen has been played against it.
    // The most recent Queen placement supersedes all others.
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

Direction Caravan::get_direction() {
    Direction dir;
    int t_last;
    int t_pen;
    int f;
    int num_queens;

    if (i_track < 2) {
        dir = NO_DIRECTION;
    } else {
        t_last = i_track - 1;
        t_pen = i_track - 2;

        // The last two Numeric cards must be in the correct direction...
        if (track[t_last].card.rank > track[t_pen].card.rank)
            dir = ASCENDING;
        else
            dir = DESCENDING;

        // ...unless Queens have been played against the last card.
        // The number of Queens determines if a change in direction has occurred or not.
        if (track[t_last].i_faces > 0) {
            f = track[t_last].i_faces - 1;
            num_queens = 0;

            for (f; f >= 0; --f)
                if (track[t_last].faces[f].rank == QUEEN)
                    num_queens += 1;

            // An odd number of Queens on a card means the direction is flipped.
            if (num_queens > 0 and (num_queens % 2) != 0) {
                if (dir == ASCENDING)
                    dir = DESCENDING;
                else
                    dir = ASCENDING;
            }
        }
    }

    return dir;
}

uint8_t Caravan::numeric_rank_to_int_value(Rank r) {
    switch (r) {
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
            throw CaravanGameException("Rank is not Numeric.");
    }
}

uint16_t Caravan::get_bid() {
    uint16_t bid = 0;
    uint8_t value;
    uint8_t value_final;

    for (int t = 0; t < i_track; ++t) {
        value = numeric_rank_to_int_value(track[t].card.rank);
        value_final = value;

        for (int f = 0; f < track[t].i_faces; ++f)
            if (track[t].faces[f].rank == KING)
                value_final <<= 1;

        bid += value_final;
    }

    return bid;
}

void Caravan::remove_numeric_card(uint8_t i) {
    if (i_track == 0)
        throw CaravanGameException("There are no Numeric cards in the Caravan.");

    if (i >= i_track)
        throw CaravanGameException("A Numeric card does not exist at the given position.");

    for (i; (i + 1) < i_track; ++i)
        track[i] = track[i + 1];

    i_track -= 1;
}

void Caravan::put_numeric_card(Card c) {
    Direction dir;
    Suit suit;
    bool ascends;
    bool not_same_suit;
    bool not_same_dir;

    if (!is_numeric_card(c))
        throw CaravanGameException("The card must be a Numeric card.");

    if (i_track == TRACK_NUMERIC_MAX)
        throw CaravanGameException("The Caravan is at maximum Numeric card capacity.");

    if (i_track > 0) {
        if (c.rank == track[i_track - 1].card.rank)
            throw CaravanGameException("A Numeric card must not have same rank as the last card in the Caravan.");

        if (i_track > 1) {
            dir = get_direction();
            suit = get_suit();
            ascends = c.rank > track[i_track - 1].card.rank;

            not_same_suit = c.suit != suit;
            not_same_dir = (dir == ASCENDING and !ascends) or (dir == DESCENDING and ascends);

            if (not_same_suit and not_same_dir)
                throw CaravanGameException(
                        "Numeric card must follow the caravan's direction or match the suit of the last Numeric card / most recent Queen placement.");
        }
    }

    this->track[i_track] = {c, {}, 0};
    this->i_track += 1;
}

Card Caravan::put_face_card(Card c, uint8_t pos) {
    if (pos < TRACK_NUMERIC_MIN or pos > TRACK_NUMERIC_MAX)
        throw CaravanGameException("The chosen position is out of range.");

    uint8_t i = pos - 1;
    Card c_on;

    if (i_track == 0)
        throw CaravanGameException("There are no Numeric cards in the Caravan.");

    if (i >= i_track)
        throw CaravanGameException("There are no Numeric cards at the given position in the Caravan.");

    if (!is_face_card(c))
        throw CaravanGameException("The chosen card is not a Face card.");

    if (c.rank == JACK) {
        remove_numeric_card(i);
    } else {
        if (track[i].i_faces >= TRACK_FACE_MAX)
            throw CaravanGameException("The Numeric card is at its maximum Face card capacity.");

        c_on = track[i].card;
        track[i].faces[track[i].i_faces] = c;
        track[i].i_faces += 1;
    }

    return c_on;
}

void Caravan::remove_suit(Suit s, uint8_t exclude) {
    if (i_track == 0)
        return;

    if (exclude > TRACK_NUMERIC_MAX)
        throw CaravanGameException("The exclude position is out of range.");

    uint8_t i_track_original = i_track;

    for (int t = i_track_original - 1; t >= 0; --t) {
        if (exclude > 0 and t == (exclude - 1))
            continue;

        if (track[t].card.suit == s)
            remove_numeric_card(t);
    }
}

void Caravan::remove_rank(Rank r, uint8_t exclude) {
    if (i_track == 0)
        return;

    if (exclude > TRACK_NUMERIC_MAX)
        throw CaravanGameException("The exclude position is out of range.");

    uint8_t i_track_original = i_track;

    for (int t = i_track_original - 1; t >= 0; --t) {
        if (exclude > 0 and t == (exclude - 1))
            continue;

        if (track[t].card.rank == r)
            remove_numeric_card(t);
    }
}

CaravanName Caravan::get_name() {
    return name;
}

uint8_t Caravan::size() {
    return i_track;
}

TrackSlot Caravan::get_cards_at(uint8_t pos) {
    if (pos < TRACK_NUMERIC_MIN or pos > TRACK_NUMERIC_MAX)
        throw CaravanGameException("The chosen card position is out of range.");

    uint8_t i = pos - 1;

    return track[i];
}
