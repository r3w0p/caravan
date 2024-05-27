// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/user/user.h"
#include "caravan/view/view.h"

View::View(ViewConfig *vc, Game *game) {
    this->vc = vc;
    this->game = game;
    this->closed = false;

    if(vc->user_abc == nullptr || vc->user_def == nullptr) {
        throw CaravanFatalException("Users must be provided to view.");
    }
}