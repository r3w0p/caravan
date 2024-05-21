// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/view/view_tui.h"
#include "caravan/user/bot_easy.h"
#include "caravan/controller/controller_tui.h"
#include <iostream>

int main() {
    User *utop;
    User *ubottom;

    ViewTUI *v;
    Controller *c;
    // Model *m;

    try {
        utop = new UserHuman(PLAYER_TOP);
        ubottom = new UserHuman(PLAYER_BOTTOM);

        v = new ViewTUI(utop, ubottom);
        c = new ControllerTUI();
        // m = ...

        // TODO Subscribe components
        v->subscribe(c);

        v->run();

    } catch (CaravanFatalException &e) {
        std::cout << e.what() << std::endl;
    }

    v->close();
    c->close();
    // TODO m->close();

    delete v;
    delete c;
    delete utop;
    delete ubottom;
}
