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

wchar_t suit_to_wchar_t(Suit s) {
    switch (s) {
        case NO_SUIT:
            return L' '; // '\0'
        case SPADES:
            return L'♠'; // '\x06', 0x2660 = 0xE2 0x99 0xA0
        case CLUBS:
            return L'♣'; // '\x05', 0x2663 = 0xE2 0x99 0xA3
        case HEARTS:
            return L'♥'; // '\x03', 0x2665 = 0xE2 0x99 0xA5
        case DIAMONDS:
            return L'♦'; // '\x04', 0x2666 = 0xE2 0x99 0xA6
        default:
            throw CaravanFatalException("Invalid suit.");
    }
}
