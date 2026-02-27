// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_GYM_CONSTANTS_H
#define CARAVAN_GYM_CONSTANTS_H
#include <cstdint>

namespace Caravan::Gym {
    // Observation:
    // Hand = 8
    // Caravans = 6
    // Slots per caravan = 8
    // Data per slot = 5 (numeral rank + # queens + # kings + # jokers + last queen)
    // 5 x 8 = 40 + cvn dir + cvn suit = 42
    // 42 x 6 = 252 + hand = 260
    constexpr std::uint16_t SIZE_OBSERVATION = 260;
}


#endif //CARAVAN_GYM_CONSTANTS_H
