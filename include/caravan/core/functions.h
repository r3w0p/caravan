// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_CORE_FUNCTIONS_H
#define CARAVAN_CORE_FUNCTIONS_H

#include <random>

namespace Caravan {
    std::string caravan_name_to_str(
        Model::CaravanName caravan_name,
        bool letter_only
    );

    std::wstring caravan_name_to_wstr(
        Model::CaravanName caravan_name,
        bool letter_only
    );

    std::wstring direction_to_wstr(Model::Direction direction);

    std::mt19937 generate_seed();

    std::wstring rank_to_wstr(Model::Rank rank, bool lead);

    std::wstring suit_to_wstr(Model::Suit suit);
}

#endif //CARAVAN_CORE_FUNCTIONS_H
