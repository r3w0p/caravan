// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <chrono>
#include <random>
#include <stdexcept>
#include <functional>
#include <iostream>
#include "model.h"

Deck DeckBuilder::shuffle_deck(Deck d) {
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
    j1.suit = NO_SUIT;
    j1.rank = JOKER;

    Card j2;
    j2.suit = NO_SUIT;
    j2.rank = JOKER;

    d.push_back(j1);
    d.push_back(j2);

    if(shuffle)
        d = shuffle_deck(d);

    return d;
}

Deck DeckBuilder::build_caravan_deck(uint8_t num_cards, uint8_t num_sample_decks, bool balanced_sample) {
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

void Pile::remove_all_cards() {
    i_track = 0;
}

Suit Pile::get_suit() {
    Suit last;
    int t;
    int f;

    if(i_track == 0)
        return NO_SUIT;

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
        dir = NO_DIRECTION;
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

uint8_t Pile::numeric_rank_to_int_value(Rank r) {
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

bool Pile::is_numeric_card(Card c) {
    return (c.rank >= ACE and c.rank <= TEN);
}

bool Pile::is_face_card(Card c) {
    return (c.rank >= JACK and c.rank <= JOKER);
}

void Pile::remove_numeric_card(uint8_t i) {
    if(i_track == 0)
        throw std::out_of_range("Cannot remove Numeric card: there are no Numeric cards in the Pile.");

    if(i >= i_track)
        throw std::out_of_range("Cannot remove Numeric card: a Numeric card does not exist at given position.");

    for(i; (i + 1) < i_track; ++i)
        track[i] = track[i+1];

    i_track -= 1;
}

void Pile::put_numeric_card(Card c) {
    Direction dir;
    Suit suit;
    bool ascends;
    bool not_same_suit;
    bool not_same_dir;

    if(!is_numeric_card(c))
        throw std::out_of_range("Cannot put Numeric card: card is not a Numeric card.");

    if(i_track == TRACK_NUMERIC_MAX)
        throw std::out_of_range("Cannot put Numeric card: pile is at maximum Numeric card capacity.");

    if(i_track > 0) {
        if(c.rank == track[i_track - 1].card.rank)
            throw std::out_of_range("Cannot put Numeric card: card has the same rank as the last card in the Pile.");

        if(i_track > 1) {
            dir = get_direction();
            suit = get_suit();
            ascends = c.rank > track[i_track - 1].card.rank;

            not_same_suit = c.suit != suit;
            not_same_dir = (dir == ASCENDING and !ascends) or (dir == DESCENDING and ascends);

            if(not_same_suit and not_same_dir)
                throw std::out_of_range(
                        "Cannot put Numeric card: cards must continue the Numeric direction "
                        "or match the suit of the previous Numeric card and its Queen placements (if any).");
        }
    }

    this->track[i_track] = {c, {}, 0};
    this->i_track += 1;
}

Card Pile::put_face_card(Card c, uint8_t i) {
    Card c_on;

    if(i_track == 0)
        throw std::out_of_range("Cannot play Face card: there are no Numeric cards in the Pile.");

    if(i >= i_track)
        throw std::out_of_range("Cannot play Face card: there are no Numeric cards at the given position in the Pile.");

    if(!is_face_card(c))
        throw std::out_of_range("Cannot play Face card: chosen card is not a Face card.");

    if(c.rank == JACK) {
        remove_numeric_card(i);
    } else {
        if(track[i].i_faces >= TRACK_FACE_MAX)
            throw std::out_of_range("Cannot play Face card: the Numeric card is at its maximum Face card capacity.");

        c_on = track[i].card;
        track[i].faces[track[i].i_faces] = c;
        track[i].i_faces += 1;
    }

    return c_on;
}

void Pile::remove_suit(Suit s, int8_t exclude) {
    if(i_track == 0)
        throw std::out_of_range("Cannot remove suit: there are no Numeric cards in the Pile.");

    uint8_t i_track_original = i_track;

    for(int t = i_track_original - 1; t >= 0; --t) {
        if(exclude >= 0 and t == exclude)
            continue;

        if (track[t].card.suit == s)
            remove_numeric_card(t);
    }
}

void Pile::remove_rank(Rank r, int8_t exclude) {
    if(i_track == 0)
        throw std::out_of_range("Cannot remove rank: there are no Numeric cards in the Pile.");

    uint8_t i_track_original = i_track;

    for(int t = i_track_original - 1; t >= 0; --t) {
        if(exclude >= 0 and t == exclude)
            continue;

        if (track[t].card.rank == r)
            remove_numeric_card(t);
    }
}

PileName Pile::get_name() {
    return name;
}

uint8_t Pile::size() {
    return i_track;
}

TrackSlot Pile::get_cards_at(uint8_t i) {
    return track[i];
}

uint8_t Table::pile_name_to_index_value(PileName pn) {
    switch(pn) {
        case PILE_A:
            return 0;
        case PILE_B:
            return 1;
        case PILE_C:
            return 2;
        case PILE_D:
            return 3;
        case PILE_E:
            return 4;
        case PILE_F:
            return 5;
        default:
            throw std::out_of_range("Invalid Pile name.");
    }
}

void Table::play_numeric_card(PileName pn, Card c) {
    Pile* pile = &piles[pile_name_to_index_value(pn)];
    pile->put_numeric_card(c);
}

void Table::play_face_card(PileName pn, Card c, uint8_t i) {
    // Play Face card on Pile.
    Pile* p_target = &piles[pile_name_to_index_value(pn)];
    // Returns the Numeric card that the Face card was played on.
    Card c_target = p_target->put_face_card(c, i);

    // If Face card was JOKER.
    if(c.rank == JOKER) {
        // Remove from original Pile, excluding the affected card.
        if(c_target.rank == ACE)
            p_target->remove_suit(c_target.suit, int8_t(i));
        else
            p_target->remove_rank(c_target.rank, int8_t(i));

        // Remove from other Piles, not excluding any cards.
        for(int k = 0; k < NUM_PILES_MAX; k++) {
            Pile* p_next = &piles[k];

            // Ignore original Pile already handled.
            if(p_next->get_name() == p_target->get_name())
                continue;

            if(c_target.rank == ACE)
                p_next->remove_suit(c_target.suit, -1);
            else
                p_next->remove_rank(c_target.rank, -1);
        }
    }
}

void Table::clear_pile(PileName pn) {
    piles[pile_name_to_index_value(pn)].remove_all_cards();
}

uint16_t Table::get_pile_bid(PileName pn) {
    return piles[pile_name_to_index_value(pn)].get_bid();
}

Suit Table::get_pile_suit(PileName pn) {
    return piles[pile_name_to_index_value(pn)].get_suit();
}

Direction Table::get_pile_direction(PileName pn) {
    return piles[pile_name_to_index_value(pn)].get_direction();
}

uint8_t Table::get_pile_size(PileName pn) {
    return piles[pile_name_to_index_value(pn)].size();
}

TrackSlot Table::get_pile_cards_at(PileName pn, uint8_t i) {
    return piles[pile_name_to_index_value(pn)].get_cards_at(i);
}
