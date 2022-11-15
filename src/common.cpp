// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <string>
#include "common.h"
#include "exceptions.h"

std::string caravan_name_to_str(CaravanName cn) {
    switch (cn) {
        case CARAVAN_A:
            return "CARAVAN A";
        case CARAVAN_B:
            return "CARAVAN B";
        case CARAVAN_C:
            return "CARAVAN C";
        case CARAVAN_D:
            return "CARAVAN D";
        case CARAVAN_E:
            return "CARAVAN E";
        case CARAVAN_F:
            return "CARAVAN F";
        default:
            throw CaravanFatalException("Invalid caravan name.");
    }
}

std::string direction_to_str(Direction dir) {
    switch (dir) {
        case ANY:
            return "ANY";
        case ASCENDING:
            return "ASC";
        case DESCENDING:
            return "DES";
        default:
            throw CaravanFatalException("Invalid direction.");
    }
}

bool is_numeric_card(Card c) {
    return (c.rank >= ACE and c.rank <= TEN);
}

bool is_face_card(Card c) {
    return (c.rank >= JACK and c.rank <= JOKER);
}

std::string player_name_to_str(PlayerName pn) {
    switch (pn) {
        case PLAYER_A:
            return "PLAYER A";
        case PLAYER_B:
            return "PLAYER B";
        default:
            throw CaravanFatalException("Invalid player name.");
    }
}

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

char suit_to_char(Suit s) {
    switch (s) {
        case NO_SUIT:
            return ' '; // '\0';
        case CLUBS:
            return 'c'; // '\x05';
        case DIAMONDS:
            return 'd'; // '\x04';
        case HEARTS:
            return 'h'; // '\x03';
        case SPADES:
            return 's'; // '\x06';
        default:
            throw CaravanFatalException("Invalid suit.");
    }
}
