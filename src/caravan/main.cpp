// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/view/tui.h"
#include "caravan/user/bot_easy.h"


int main() {
    auto *utop = new UserHuman(PLAYER_TOP);
    auto *ubottom = new UserHuman(PLAYER_BOTTOM);

    auto *v = new TUI(utop, ubottom);

    v->run();

    delete v;
    delete utop;
    delete ubottom;
}
