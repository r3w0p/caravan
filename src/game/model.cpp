// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <chrono>
#include <random>
#include <stdexcept>
#include <functional>
#include <iostream>
#include "model.h"

Deck shuffle_deck(Deck d) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(d.begin(), d.end(), std::default_random_engine(seed));
    return d;
}

Deck DeckBuilder::build_standard_deck(bool shuffle) {
    Deck d;

    for(int i = CLUBS; i <= SPADES; ++i) {
        for(int j = ACE; j <= KING; ++j) {
            Card c;
            c.suit = static_cast<Suit>(i);
            c.rank = static_cast<Rank>(j);

            d.push_back(c);
        }
    }

    Card j1;
    j1.suit = NONE;
    j1.rank = JOKER;

    Card j2;
    j2.suit = NONE;
    j2.rank = JOKER;

    d.push_back(j1);
    d.push_back(j2);

    if(shuffle)
        d = shuffle_deck(d);

    return d;
}

Deck DeckBuilder::build_caravan_deck(uint8_t num_cards, uint8_t num_sample_decks, bool balanced_sample) {
    // TODO format exception strings to use constants

    if(num_cards < DECK_CARAVAN_MIN or num_cards > DECK_CARAVAN_MAX)
        throw std::out_of_range("Can only build deck with between 30 and 156 cards (inclusive).");

    if(num_sample_decks < NUM_SAMPLE_DECKS_MIN or num_sample_decks > NUM_SAMPLE_DECKS_MAX)
        throw std::out_of_range("Can only sample between 1 and 3 standard decks (inclusive).");

    uint8_t total_sample_cards = num_sample_decks * DECK_STANDARD_MAX;

    if(total_sample_cards < num_cards)
        throw std::out_of_range("Not enough sample cards to cover required number of cards.");

    Deck sample_decks[num_sample_decks];

    for(int i = 0; i < num_sample_decks; ++i)
        sample_decks[i] = DeckBuilder::build_standard_deck(true);

    Deck d;
    uint8_t next = 0;

    if(balanced_sample) {
        // Sample decks in a round-robin fashion
        while (d.size() < num_cards) {
            d.push_back(sample_decks[next].back());
            sample_decks[next].pop_back();

            next = (next + 1) % num_sample_decks;
        }

    } else {
        // Sample decks randomly
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(0, num_sample_decks-1);

        while (d.size() < num_cards) {
            next = distr(gen);
            if(!sample_decks[next].empty()) {
                d.push_back(sample_decks[next].back());
                sample_decks[next].pop_back();
            }
        }
    }

    return d;
}

void Pile::clear() {
    i_track = 0;
}

Suit Pile::get_suit() {
    Suit last;
    int t;
    int f;

    if(i_track == 0)
        throw std::out_of_range("There are no Numeric cards in the Pile.");

    // The last Numeric card must be in the correct suit...
    t = i_track - 1;
    last = track[t].card.suit;

    // ...unless a Queen has been played against it.
    // The most recent Queen placement supersedes all others.
    if(track[t].i_faces > 0) {
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

Direction Pile::get_direction() {
    Direction dir;
    int t_last;
    int t_pen;
    int f;
    int num_queens;

    if(i_track < 2) {
        dir = BIDIRECTIONAL;
    } else {
        t_last = i_track - 1;
        t_pen = i_track - 2;

        // The last two Numeric cards must be in the correct direction...
        if(track[t_last].card.rank > track[t_pen].card.rank)
            dir = ASCENDING;
        else
            dir = DESCENDING;

        // ...unless Queens have been played against the last card.
        // The number of Queens determines if a change in direction has occurred or not.
        if(track[t_last].i_faces > 0) {
            f = track[t_last].i_faces - 1;
            num_queens = 0;

            for (f; f >= 0; --f)
                if (track[t_last].faces[f].rank == QUEEN)
                    num_queens += 1;

            // An odd number of Queens on a card means the direction is flipped.
            if(num_queens > 0 and (num_queens % 2) != 0) {
                if(dir == ASCENDING)
                    dir = DESCENDING;
                else
                    dir = ASCENDING;
            }
        }
    }

    return dir;
}

uint8_t Pile::size() {
    return i_track;
}

uint8_t numeric_rank_to_int_value(Rank r) {
    switch(r) {
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
            throw std::out_of_range("Rank is not Numeric.");
    }
}

uint16_t Pile::get_bid() {
    uint16_t bid = 0;
    uint8_t value;
    uint8_t multiplier;

    for(int t = 0; t < i_track; ++t) {
        value = numeric_rank_to_int_value(track[t].card.rank);
        multiplier = 1;

        for(int f = 0; f < track[t].i_faces; ++f)
            if(track[t].faces[f].rank == KING)
                multiplier += 1;

        bid += (value * multiplier);
    }

    return bid;
}

bool is_numeric_card(Card c) {
    return (c.rank >= ACE and c.rank <= TEN);
}

void Pile::play_numeric_card(Card c) {
    if(!is_numeric_card(c))
        throw std::out_of_range("Card is not Numeric.");

    if(i_track == TRACK_NUMERIC_MAX)
        throw std::out_of_range("Pile is at maximum Numeric card capacity.");

    if(i_track > 0) {
        if (c.suit != get_suit())
            throw std::out_of_range("Card's suit does not match the suit of the Pile.");

        if(c.rank == track[i_track - 1].card.rank)
            throw std::out_of_range("Card has the same rank as the last card in the Pile.");

        if(i_track > 1) {
            Direction dir = get_direction();
            bool ascends = c.rank > track[i_track - 1].card.rank;

            if ((dir == ASCENDING and !ascends) or
                (dir == DESCENDING and ascends))
                throw std::out_of_range("Card's direction does not match the direction of the Pile.");
        }
    }

    track[i_track].card = c;
    track[i_track].faces = {};
    track[i_track].i_faces = 0;

    i_track += 1;
}

bool is_face_card(Card c) {
    return (c.rank >= JACK and c.rank <= JOKER);
}
