// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <iostream>
#include "view.h"
#include "controller.h"
#include "bot_simple.h"
#include "curses.h"

int main() {
    GameConfig gc;
    UserHuman *ua;
    UserBotSimple *ub;
    Engine *e;
    ViewCLI *v;
    Controller *c;

    try {
        gc = {
                30, 1, true,
                30, 1, true,
                PLAYER_A
        };

        ua = new UserHuman(PLAYER_A);
        ub = new UserBotSimple(PLAYER_B);

        e = new Engine(gc);
        //v = new ViewCLI();
        //c = new Controller(e, v, ua, ub);

        //c->run();

        initscr();
        printw("Hello World !!!");
        refresh();
        getch();
        endwin();

    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    delete e;
    //delete v;
    //delete c;
    delete ua;
    delete ub;
}
