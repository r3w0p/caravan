// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_GYM_TYPES_H
#define CARAVAN_GYM_TYPES_H

#include "caravan/model/constants.h"
#include <array>
#include <string>
#include <vector>

#include "constants.h"
#include "caravan/model/types.h"

namespace Caravan::Gym {

    using Observation = std::array<uint8_t, SIZE_OBSERVATION>;

    // TODO placeholders only
    enum Action {};
    enum ActionSpace {};
    enum Reward {};
    enum Info {};

    using ActionMove = struct ActionMove {
        Model::OptionType option{Model::NO_OPTION};
        Model::Card card_hand{};
        Model::CaravanName caravan_name{Model::NO_CARAVAN};
        uint8_t pos_caravan{0};
    };

}

#endif //CARAVAN_GYM_TYPES_H
