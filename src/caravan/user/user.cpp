// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/user/user.h"

bool UserHuman::is_human() {
    return true;
}

GameCommand UserHuman::generate_option(Game *g) {
    return {};
}

void UserHuman::close() {
    if (!closed) {
        closed = true;
    }
}