// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <chrono>
#include <random>
#include <stdexcept>
#include <functional>
#include <iostream>
#include "model.h"


/*
 * MISC
 */

bool is_numeric_card(Card c) {
    return (c.rank >= ACE and c.rank <= TEN);
}

bool is_face_card(Card c) {
    return (c.rank >= JACK and c.rank <= JOKER);
}

/*
 * DECK BUILDER
 */

Deck DeckBuilder::shuffle_deck(Deck d) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(d.begin(), d.end(), std::default_random_engine(seed));
    return d;
}

Deck DeckBuilder::build_standard_deck(bool shuffle) {
    Deck d;

    for(int i = CLUBS; i <= SPADES; ++i)
        for(int j = ACE; j <= KING; ++j)
            d.push_back({ static_cast<Suit>(i), static_cast<Rank>(j) });

    d.push_back({ NO_SUIT, JOKER });
    d.push_back({ NO_SUIT, JOKER });

    if(shuffle)
        return shuffle_deck(d);
    else
        return d;
}

Deck* DeckBuilder::build_caravan_deck(uint8_t num_cards, uint8_t num_sample_decks, bool balanced_sample) {
    uint8_t total_sample_cards;
    Deck sample_decks[num_sample_decks];
    Deck *d;
    uint8_t i_next;
    Card c_next;
    uint8_t first_hand_num_cards;

    if(num_cards < DECK_CARAVAN_MIN or num_cards > DECK_CARAVAN_MAX)
        throw std::out_of_range("A Caravan deck can only build deck with between 30 and 156 cards (inclusive).");

    if(num_sample_decks < SAMPLE_DECKS_MIN or num_sample_decks > SAMPLE_DECKS_MAX)
        throw std::out_of_range("A Caravan deck can only sample between 1 and 3 standard decks (inclusive).");

    total_sample_cards = num_sample_decks * DECK_STANDARD_MAX;

    if(total_sample_cards < num_cards)
        throw std::out_of_range("There are not enough sample cards to cover the required number of cards.");

    d = new Deck();

    do {
        d->clear();
        i_next = 0;
        first_hand_num_cards = 0;

        for (int i = 0; i < num_sample_decks; ++i)
            sample_decks[i] = DeckBuilder::build_standard_deck(true);

        if (balanced_sample) {
            // Sample decks in a round-robin fashion
            while (d->size() < num_cards) {
                c_next = sample_decks[i_next].back();
                d->push_back(c_next);
                sample_decks[i_next].pop_back();

                i_next = (i_next + 1) % num_sample_decks;

                if ((num_cards - d->size()) < HAND_SIZE_MAX and is_numeric_card(c_next))
                    first_hand_num_cards += 1;
            }

        } else {
            // Sample decks randomly
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distr(0, num_sample_decks - 1);

            while (d->size() < num_cards) {
                i_next = distr(gen);
                if (!sample_decks[i_next].empty()) {
                    c_next = sample_decks[i_next].back();
                    d->push_back(c_next);
                    sample_decks[i_next].pop_back();

                    if ((num_cards - d->size()) < HAND_SIZE_MAX and is_numeric_card(c_next))
                        first_hand_num_cards += 1;
                }
            }
        }

    } while(first_hand_num_cards < MOVES_START_ROUND);

    return d;
}

/*
 * PLAYER
 */

uint8_t Player::get_size_deck() {
    return deck->size();
}

uint8_t Player::get_size_hand() {
    return i_hand;
}

Card Player::remove_from_hand_at(uint8_t pos) {
    if(pos < HAND_POS_MIN or pos > HAND_POS_MAX)
        throw std::out_of_range("The chosen position is out of range.");

    if(i_hand == 0)
        throw std::out_of_range("The Player's hand is empty.");

    uint8_t i = pos - 1;

    if(i >= i_hand)
        throw std::out_of_range("There is no card at the chosen position.");

    // Get card at index
    Card c_ret = hand[i];

    // Move the cards above it down
    for(i; (i + 1) < i_hand; ++i)
        hand[i] = hand[i + 1];

    // If more cards in the deck
    if(!deck->empty()) {
        // Add new card from deck into top of hand
        hand[i_hand-1] = deck->back();
        deck->pop_back();

    } else {
        // Reduce hand size by 1
        i_hand -= 1;
    }

    return c_ret;
}

Card Player::get_from_hand_at(uint8_t pos) {
    if (pos < HAND_POS_MIN or pos > HAND_POS_MAX)
        throw std::out_of_range("The chosen position is out of range.");

    if (i_hand == 0)
        throw std::out_of_range("The Player's hand is empty.");

    uint8_t i = pos - 1;

    if (i >= i_hand)
        throw std::out_of_range("There is no card at the chosen position.");

    return hand[i];
}

Hand Player::get_hand() {
    return hand;
}

PlayerName Player::get_name() {
    return name;
}

uint16_t Player::get_moves_count() {
    return moves;
}

void Player::increment_moves_count() {
    moves += 1;
}


/*
 * CARAVAN
 */

void Caravan::clear() {
    if (i_track == 0)
        throw std::out_of_range("The Caravan is empty.");

    i_track = 0;
}

Suit Caravan::get_suit() {
    Suit last;
    int t;
    int f;

    if(i_track == 0)
        return NO_SUIT;

    // The last Numeric card is the caravan suit...
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

Direction Caravan::get_direction() {
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

uint8_t Caravan::numeric_rank_to_int_value(Rank r) {
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

uint16_t Caravan::get_bid() {
    uint16_t bid = 0;
    uint8_t value;
    uint8_t value_final;

    for(int t = 0; t < i_track; ++t) {
        value = numeric_rank_to_int_value(track[t].card.rank);
        value_final = value;

        for(int f = 0; f < track[t].i_faces; ++f)
            if(track[t].faces[f].rank == KING)
                value_final <<= 1;

        bid += value_final;
    }

    return bid;
}

void Caravan::remove_numeric_card(uint8_t i) {
    if(i_track == 0)
        throw std::out_of_range("There are no Numeric cards in the Caravan.");

    if(i >= i_track)
        throw std::out_of_range("A Numeric card does not exist at the given position.");

    for(i; (i + 1) < i_track; ++i)
        track[i] = track[i + 1];

    i_track -= 1;
}

void Caravan::put_numeric_card(Card c) {
    Direction dir;
    Suit suit;
    bool ascends;
    bool not_same_suit;
    bool not_same_dir;

    if(!is_numeric_card(c))
        throw CaravanGameModelException("The card must be a Numeric card.");

    if(i_track == TRACK_NUMERIC_MAX)
        throw CaravanGameModelException("The Caravan is at maximum Numeric card capacity.");

    if(i_track > 0) {
        if(c.rank == track[i_track - 1].card.rank)
            throw CaravanGameModelException("A Numeric card must not have same rank as the last card in the Caravan.");

        if(i_track > 1) {
            dir = get_direction();
            suit = get_suit();
            ascends = c.rank > track[i_track - 1].card.rank;

            not_same_suit = c.suit != suit;
            not_same_dir = (dir == ASCENDING and !ascends) or (dir == DESCENDING and ascends);

            if(not_same_suit and not_same_dir)
                throw CaravanGameModelException(
                        "Cards must continue the Numeric direction or match the suit of "
                        "the previous Numeric card and its Queen placements (if any).");
        }
    }

    this->track[i_track] = {c, {}, 0};
    this->i_track += 1;
}

Card Caravan::put_face_card(Card c, uint8_t pos) {
    if(pos < TRACK_NUMERIC_MIN or pos > TRACK_NUMERIC_MAX)
        throw std::out_of_range("The chosen position is out of range.");

    uint8_t i = pos - 1;
    Card c_on;

    if(i_track == 0)
        throw std::out_of_range("There are no Numeric cards in the Caravan.");

    if(i >= i_track)
        throw std::out_of_range("There are no Numeric cards at the given position in the Caravan.");

    if(!is_face_card(c))
        throw std::out_of_range("The chosen card is not a Face card.");

    if(c.rank == JACK) {
        remove_numeric_card(i);
    } else {
        if(track[i].i_faces >= TRACK_FACE_MAX)
            throw std::out_of_range("The Numeric card is at its maximum Face card capacity.");

        c_on = track[i].card;
        track[i].faces[track[i].i_faces] = c;
        track[i].i_faces += 1;
    }

    return c_on;
}

void Caravan::remove_suit(Suit s, uint8_t exclude) {
    if(i_track == 0)
        return;

    if(exclude > TRACK_NUMERIC_MAX)
        throw std::out_of_range("The exclude position is out of range.");

    uint8_t i_track_original = i_track;

    for(int t = i_track_original - 1; t >= 0; --t) {
        if(exclude > 0 and t == (exclude - 1))
            continue;

        if (track[t].card.suit == s)
            remove_numeric_card(t);
    }
}

void Caravan::remove_rank(Rank r, uint8_t exclude) {
    if(i_track == 0)
        return;

    if(exclude > TRACK_NUMERIC_MAX)
        throw std::out_of_range("The exclude position is out of range.");

    uint8_t i_track_original = i_track;

    for(int t = i_track_original - 1; t >= 0; --t) {
        if(exclude > 0 and t == (exclude - 1))
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
    if(pos < TRACK_NUMERIC_MIN or pos > TRACK_NUMERIC_MAX)
        throw std::out_of_range("The chosen card position is out of range.");

    uint8_t i = pos - 1;

    return track[i];
}


/*
 * TABLE
 */

uint8_t Table::caravan_name_to_index_value(CaravanName cn) {
    switch(cn) {
        case CARAVAN_A:
            return 0;
        case CARAVAN_B:
            return 1;
        case CARAVAN_C:
            return 2;
        case CARAVAN_D:
            return 3;
        case CARAVAN_E:
            return 4;
        case CARAVAN_F:
            return 5;
        default:
            throw std::out_of_range("Invalid Caravan name.");
    }
}

void Table::play_numeric_card(CaravanName cn, Card c) {
    Caravan* caravan_ptr = &caravans[caravan_name_to_index_value(cn)];
    caravan_ptr->put_numeric_card(c);
}

void Table::play_face_card(CaravanName cn, Card c, uint8_t pos) {
    Caravan* cvn_target = &caravans[caravan_name_to_index_value(cn)];

    if(c.rank == QUEEN and pos != cvn_target->size())
        throw CaravanGameModelException("A QUEEN must be played on the most recent numeric card in a caravan.");

    // Play Face card on Caravan.
    // Returns the Numeric card that the Face card was played on.
    Card c_target = cvn_target->put_face_card(c, pos);

    // If Face card was JOKER.
    if(c.rank == JOKER) {
        // Remove from original Caravan, excluding the affected card.
        if(c_target.rank == ACE)
            cvn_target->remove_suit(c_target.suit, pos);
        else
            cvn_target->remove_rank(c_target.rank, pos);

        // Remove from other Caravans, not excluding any cards.
        for(int k = 0; k < TABLE_CARAVANS_MAX; k++) {
            Caravan* p_next = &caravans[k];

            // Ignore original Caravan already handled.
            if(p_next->get_name() == cvn_target->get_name())
                continue;

            if(c_target.rank == ACE)
                p_next->remove_suit(c_target.suit, 0);
            else
                p_next->remove_rank(c_target.rank, 0);
        }
    }
}

void Table::clear_caravan(CaravanName cn) {
    caravans[caravan_name_to_index_value(cn)].clear();
}

uint16_t Table::get_caravan_bid(CaravanName cn) {
    return caravans[caravan_name_to_index_value(cn)].get_bid();
}

Suit Table::get_caravan_suit(CaravanName cn) {
    return caravans[caravan_name_to_index_value(cn)].get_suit();
}

Direction Table::get_caravan_direction(CaravanName cn) {
    return caravans[caravan_name_to_index_value(cn)].get_direction();
}

uint8_t Table::get_caravan_size(CaravanName cn) {
    return caravans[caravan_name_to_index_value(cn)].size();
}

TrackSlot Table::get_caravan_cards_at(CaravanName cn, uint8_t pos) {
    return caravans[caravan_name_to_index_value(cn)].get_cards_at(pos);
}

void Game::close() {
    if(closed)
        return;

    delete table_ptr;
    delete deck_you_ptr;
    delete deck_opp_ptr;
    delete player_you_ptr;
    delete player_opp_ptr;

    closed = true;
}

bool Game::has_sold(CaravanName cn) {
    uint8_t bid = table_ptr->get_caravan_bid(cn);
    return bid >= CARAVAN_SOLD_MIN and bid <= CARAVAN_SOLD_MAX;
}

int8_t Game::compare_bids(CaravanName cn1, CaravanName cn2) {
    uint8_t bid_cn1;
    uint8_t bid_cn2;

    if(has_sold(cn1)) {
        if(has_sold(cn2)) {
            bid_cn1 = table_ptr->get_caravan_bid(cn1);
            bid_cn2 = table_ptr->get_caravan_bid(cn2);

            if(bid_cn1 > bid_cn2)
                return -1;  // CN1 sold; CN2 sold; CN1 highest bid
            else if(bid_cn2 > bid_cn1)
                return 1;  // CN1 sold; CN2 sold; CN2 highest bid
            else
                return 0;  // CN1 sold; CN2 sold; matching bids

        } else
            return -1;  // CN1 sold; CN2 unsold

    } else if(has_sold(cn2))
        return 1;  // CN1 unsold; CN2 sold
    else
        return 0;  // CN1 unsold; CN2 unsold
}

PlayerName Game::get_winner() {
    uint8_t won_you = 0;
    uint8_t won_opp = 0;
    int8_t comp[3];

    comp[0] = compare_bids(CARAVAN_D, CARAVAN_A);
    comp[1] = compare_bids(CARAVAN_E, CARAVAN_B);
    comp[2] = compare_bids(CARAVAN_F, CARAVAN_C);

    for(int i = 0; i < 3; ++i) {
        if(comp[i] < 0)
            won_you += 1;
        else if(comp[i] > 0)
            won_opp += 1;
    }

    if(won_you >= 2)
        return PLAYER_YOU;
    else if(won_opp >= 2)
        return PLAYER_OPP;

    // Neither player has outbid the other...

    // Check if players have empty hands
    if(player_you_ptr->get_size_hand() > 0 and player_opp_ptr->get_size_hand() == 0)
        return PLAYER_YOU;
    else if(player_opp_ptr->get_size_hand() > 0 and player_you_ptr->get_size_hand() == 0)
        return PLAYER_OPP;

    return NO_PLAYER;
}

void Game::play_option(GameOption go) {
    if(closed)
        throw std::out_of_range("The game has already been closed.");

    if(get_winner() != NO_PLAYER)
        throw std::out_of_range("The game has already been won.");

    Player* p_ptr = player_turn == PLAYER_YOU ? player_you_ptr : player_opp_ptr;

    switch(go.type) {
        case OPTION_PLAY:
            option_play(p_ptr, go);
            break;

        case OPTION_REMOVE:
            if(p_ptr->get_moves_count() < MOVES_START_ROUND)
                throw std::out_of_range("Player cannot drop a card during the Start round.");

            option_drop(p_ptr, go);
            break;

        case OPTION_CLEAR:
            if(p_ptr->get_moves_count() < MOVES_START_ROUND)
                throw std::out_of_range("Player cannot clear a Caravan during the Start round.");

            option_clear(p_ptr, go);
            break;

        default:
            throw std::out_of_range("Unknown play_option.");
    }

    p_ptr->increment_moves_count();

    if(pn == PLAYER_YOU)
        player_turn = PLAYER_OPP;
    else
        player_turn = PLAYER_YOU;
}

void Game::option_play(Player* p_ptr, GameOption go) {
    Card c_hand = p_ptr->get_from_hand_at(go.pos_hand);

    bool is_in_start_stage = p_ptr->get_moves_count() < MOVES_START_ROUND;
    bool is_you_playing_num_into_you_caravans;
    bool is_opp_playing_num_into_opp_caravans;

    if (is_numeric_card(c_hand)) {
        is_you_playing_num_into_you_caravans = p_ptr->get_name() == PLAYER_YOU and (
                go.caravan_name == CARAVAN_D or
                go.caravan_name == CARAVAN_E or
                go.caravan_name == CARAVAN_F);

        is_opp_playing_num_into_opp_caravans = p_ptr->get_name() == PLAYER_OPP and (
                go.caravan_name == CARAVAN_A or
                go.caravan_name == CARAVAN_B or
                go.caravan_name == CARAVAN_C);

        if(!(is_you_playing_num_into_you_caravans or is_opp_playing_num_into_opp_caravans))
            throw std::out_of_range(
                    "Numeric card can only be played on Player's own Caravans.");

        if(is_in_start_stage and table_ptr->get_caravan_size(go.caravan_name) > 0)
            throw std::out_of_range(
                    "Numeric card must be played on an empty Caravan during the Start round.");

        table_ptr->play_numeric_card(go.caravan_name, c_hand);

    } else {
        if(is_in_start_stage)
            throw std::out_of_range(
                    "Face card cannot be played during the Start round.");

        table_ptr->play_face_card(go.caravan_name, c_hand, go.pos_caravan);
    }

    p_ptr->remove_from_hand_at(go.pos_hand);
}

void Game::option_drop(Player *p_ptr, GameOption go) {
    p_ptr->remove_from_hand_at(go.pos_hand);
}

void Game::option_clear(Player *p_ptr, GameOption go) {
    table_ptr->clear_caravan(go.caravan_name);
}

PlayerName Game::get_player_turn() {
    return player_turn;
}
