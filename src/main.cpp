// Copyright (c) 2022-2023 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <iostream>
#include "view.h"
#include "controller.h"
#include "bot_easy.h"

int main(int argc, char **argv) {
    GameConfig gc;
    UserHuman *ua;
    UserBotEasy *ub;
    Engine *e;
    ViewCLI *v;
    Controller *c;

    try {
        gc = {
                30, 1, true,
                30, 1, true,
                PLAYER_BOTTOM
        };

        ua = new UserHuman(PLAYER_BOTTOM);
        ub = new UserBotEasy(PLAYER_TOP);

        e = new Engine(gc);
        v = new ViewCLI();
        c = new Controller(e, v, ua, ub);

        c->run();

    } catch (CaravanFatalException &e) {
        std::cout << e.what() << std::endl;
    }

    e->close();
    v->close();

    delete e;
    delete v;
    delete c;
    delete ua;
    delete ub;
}
