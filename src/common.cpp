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
