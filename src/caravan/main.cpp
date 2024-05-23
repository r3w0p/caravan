// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/view/view_tui.h"
#include "caravan/user/bot_easy.h"
#include <iostream>

int main() {
    User *user_abc;
    User *user_def;
    Game *game;

    ViewTUI *v;

    GameConfig config = {  // TODO
    30, 1, true,
    30, 1, true,
    PLAYER_ABC
    };

    try {
        user_abc = new UserHuman(PLAYER_ABC);
        user_def = new UserHuman(PLAYER_DEF);
        game = new Game(config);  // TODO

        v = new ViewTUI(user_abc, user_def, game);
        v->run();

    } catch (CaravanFatalException &e) {
        std::cout << e.what() << std::endl;  // TODO
    }

    v->close();
    delete v;
}
