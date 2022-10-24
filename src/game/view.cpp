// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "view.h"
#include "exceptions.h"
#include <string>
#include <iostream>

std::string rank_to_str(Rank r) {
    switch (r) {
        case ACE:
            return "A";
        case TWO:
            return "2";
        case THREE:
            return "3";
        case FOUR:
            return "4";
        case FIVE:
            return "5";
        case SIX:
            return "6";
        case SEVEN:
            return "7";
        case EIGHT:
            return "8";
        case NINE:
            return "9";
        case TEN:
            return "10";
        case JACK:
            return "J";
        case QUEEN:
            return "Q";
        case KING:
            return "K";
        case JOKER:
            return "JO";
        default:
            throw CaravanFatalException("Invalid rank.");
    }
}

std::string suit_to_str(Suit s) {
    switch (s) {
        case NO_SUIT:
            return "";
        case CLUBS:
            return "\x05";
        case DIAMONDS:
            return "\x04";
        case HEARTS:
            return "\x03";
        case SPADES:
            return "\x06";
        default:
            throw CaravanFatalException("Invalid suit.");
    }
}

std::string caravan_name_to_str(CaravanName pn) {
    switch (pn) {
        case CARAVAN_A:
            return "A";
        case CARAVAN_B:
            return "B";
        case CARAVAN_C:
            return "C";
        case CARAVAN_D:
            return "D";
        case CARAVAN_E:
            return "E";
        case CARAVAN_F:
            return "F";
        default:
            throw CaravanFatalException("Invalid Caravan name.");
    }
}

std::string direction_to_str(Direction dir) {
    switch (dir) {
        case NO_DIRECTION:
            return "ANY";
        case ASCENDING:
            return "ASC";
        case DESCENDING:
            return "DES";
        default:
            throw CaravanFatalException("Invalid direction.");
    }
}

void display_test_caravan(Table *t, CaravanName cn) {
    std::cout << caravan_name_to_str(cn) << " [";
    std::cout << "bid=" << t->get_caravan_bid(cn) << ", ";
    std::cout << "suit=" << suit_to_str(t->get_caravan_suit(cn)) << ", ";
    std::cout << "dir=" << direction_to_str(t->get_caravan_direction(cn)) << ", ";
    int p_size = t->get_caravan_size(cn);
    std::cout << "size=" << p_size << "]: ";

    for (int pos = 1; pos <= p_size; ++pos) {
        if (pos > 1)
            std::cout << " ";

        TrackSlot sn = t->get_caravan_cards_at(cn, pos);
        std::cout << rank_to_str(sn.card.rank) << suit_to_str(sn.card.suit) << " (";

        for (int m = 0; m < sn.i_faces; ++m) {
            if (m > 0)
                std::cout << " ";

            std::cout << rank_to_str(sn.faces[m].rank) << suit_to_str(sn.faces[m].suit);
        }
        std::cout << ")";
    }
    std::cout << std::endl;
}


void display_test(Engine *g) {
    Table *t = g->get_table();
    Player *you = g->get_player(PLAYER_1);
    Player *opp = g->get_player(PLAYER_2);
    Card c;

    display_test_caravan(t, CARAVAN_A);
    display_test_caravan(t, CARAVAN_D);

    std::cout << std::endl;

    display_test_caravan(t, CARAVAN_B);
    display_test_caravan(t, CARAVAN_E);

    std::cout << std::endl;

    display_test_caravan(t, CARAVAN_C);
    display_test_caravan(t, CARAVAN_F);

    std::cout << std::endl << "YOU:";
    for (int i = 0; i < you->get_size_hand(); ++i) {
        c = you->get_from_hand_at(i + 1);
        std::cout << " " << rank_to_str(c.rank) << suit_to_str(c.suit);
    }

    std::cout << std::endl << "OPP:";
    for (int i = 0; i < opp->get_size_hand(); ++i) {
        c = opp->get_from_hand_at(i + 1);
        std::cout << " " << rank_to_str(c.rank) << suit_to_str(c.suit);
    }
    std::cout << std::endl;
}

void ViewCLI::display(Engine *g, std::string msg) {
    // TODO
    display_test(g);
    std::cout << msg << std::endl;
}
