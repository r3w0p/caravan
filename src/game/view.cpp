// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "view.h"
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
            return "Jo";
        default:
            throw std::out_of_range("Invalid rank.");
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
            throw std::out_of_range("Invalid suit.");
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
            throw std::out_of_range("Invalid Caravan name.");
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
            throw std::out_of_range("Invalid direction.");
    }
}


void display_test(Table t) {
    // std::cout << ... << std::endl;
    for(int i = CARAVAN_A; i <= CARAVAN_F; ++i) {
        auto pn = static_cast<CaravanName>(i);

        std::cout << caravan_name_to_str(pn) << " [";
        std::cout << "bid=" << t.get_caravan_bid(pn) << ", ";
        std::cout << "suit=" << suit_to_str(t.get_caravan_suit(pn)) << ", ";
        std::cout << "dir=" << direction_to_str(t.get_caravan_direction(pn)) << ", ";
        int p_size = t.get_caravan_size(pn);
        std::cout << "size=" << p_size << "]: ";

        for(int pos = 1; pos <= p_size; ++pos) {
            if(pos > 1)
                std::cout << " ";

            TrackSlot sn = t.get_caravan_cards_at(pn, pos);
            std::cout << rank_to_str(sn.card.rank) << suit_to_str(sn.card.suit) << " (";

            for(int m = 0; m < sn.i_faces; ++m) {
                if(m > 0)
                    std::cout << " ";

                std::cout << rank_to_str(sn.faces[m].rank) << suit_to_str(sn.faces[m].suit);
            }
            std::cout << ")";
        }
        std::cout << std::endl;
    }
}

void ViewCLI::refresh_view(Table t, std::string msg) {
    display_test(t);
}

std::string ViewCLI::prompt_next_move(std::string msg) {
    // TODO
    return std::string();
}
