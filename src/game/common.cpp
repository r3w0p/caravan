// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <string>
#include "common.h"
#include "exceptions.h"

bool is_numeric_card(Card c) {
    return (c.rank >= ACE and c.rank <= TEN);
}

bool is_face_card(Card c) {
    return (c.rank >= JACK and c.rank <= JOKER);
}

std::string caravan_name_to_str(CaravanName cn) {
    switch (cn) {
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
            throw CaravanFatalException("Invalid caravan name.");
    }
}