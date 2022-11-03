// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "game/view.h"
#include "game/controller.h"
#include "iostream"
#include "bots/bot_simple.h"

int main() {
    GameConfig gc;
    Engine *e;
    ViewCLI *v;
    Controller *c;
    User *ua = new UserHuman(PLAYER_A);
    User *ub = new UserBotSimple(PLAYER_B);

    gc = {
            30, 1, true,
            30, 1, true,
            PLAYER_A
    };

    try {
        e = new Engine(gc);
        v = new ViewCLI();
        c = new Controller(e, v, ua, ub);

        c->run();

    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    delete e;
    delete v;
    delete c;
}
