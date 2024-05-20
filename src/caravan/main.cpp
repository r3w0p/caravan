// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/view/view_tui.h"
#include "caravan/user/bot_easy.h"
#include "caravan/controller/controller_tui.h"


int main() {
    auto *utop = new UserHuman(PLAYER_TOP);
    auto *ubottom = new UserHuman(PLAYER_BOTTOM);

    auto *v = new ViewTUI(utop, ubottom);
    auto *c = new ControllerTUI();
    // auto *m = ...

    // TODO Subscribe
    v->subscribe(c);

    v->run();

    v->close();
    // TODO c->close();
    // TODO m->close();

    delete v;
    delete c;
    delete utop;
    delete ubottom;
}
