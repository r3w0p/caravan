// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "game/view.h"
#include "game/controller.h"
#include "iostream"

int main() {
    GameConfig gc;
    Engine *e;
    ViewCLI *v;
    ControllerCLI *c;
    User *ua = new UserHumanCLI(PLAYER_1);
    User *ub = new UserHumanCLI(PLAYER_2);  // TODO bot

    // TODO set from bots arguments
    gc = {
            30, 1, true,
            30, 1, true,
            PLAYER_1
    };

    try {
        e = new Engine(gc, PLAYER_1, PLAYER_2);
        v = new ViewCLI();
        c = new ControllerCLI(e, v, ua, ub);

        c->run();

    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    delete e;
    delete v;
    delete c;
}
