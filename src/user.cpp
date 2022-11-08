// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "../include/user.h"

bool UserHuman::is_human() {
    return true;
}

bool UserBot::is_human() {
    return false;
}
