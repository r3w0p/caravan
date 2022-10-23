// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "game/model.h"
#include "game/view.h"
#include "game/controller.h"
#include "iostream"

int main() {
    GameConfig config;
    Game* g;
    ViewCLI* v;
    ControllerCLI* c;

    // TODO set from user arguments
    config = {
            30, 1, true,
            30, 1, true,
            PLAYER_YOU
    };

    try {
        g = new Game(config);
        v = new ViewCLI();
        c = new ControllerCLI(g, v);

        c->run();

    } catch(std::exception & e) {
        std::cout << e.what() << std::endl;
    }

    delete g;
    delete v;
    delete c;
}
