// Copyright (c) 2022-2025 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <string>
#include "caravan/model/types.h"

std::string caravan_letter(CaravanName cvname) {
    switch (cvname) {
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
            return ""; // TODO default / exception / option?
    }
}