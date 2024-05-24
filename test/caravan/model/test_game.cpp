// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "gtest/gtest.h"
#include "caravan/model/game.h"


TEST (TestGame, Close) {
    GameConfig gc = {
        30, 1, true,
        30, 1, true,
        PLAYER_ABC
    };
    Game g{gc};

    ASSERT_FALSE(g.is_closed());
    g.close();
    ASSERT_TRUE(g.is_closed());
}

TEST (TestGame, GetPlayer_Both) {
    GameConfig gc = {
        30, 1, true,
        30, 1, true,
        PLAYER_ABC
    };
    Game g{gc};

    ASSERT_EQ(g.get_player(PLAYER_ABC)->get_name(), PLAYER_ABC);
    ASSERT_EQ(g.get_player(PLAYER_DEF)->get_name(), PLAYER_DEF);
}

TEST (TestGame, GetPlayer_Error_AlreadyClosed) {
    GameConfig gc = {
        30, 1, true,
        30, 1, true,
        PLAYER_ABC
    };
    Game g{gc};

    g.close();

    try {
        g.get_player(PLAYER_ABC);
        FAIL();

    } catch (CaravanFatalException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestGame, GetPlayer_Error_InvalidName) {
    GameConfig gc = {
        30, 1, true,
        30, 1, true,
        PLAYER_ABC
    };
    Game g{gc};

    try {
        g.get_player(NO_PLAYER);
        FAIL();

    } catch (CaravanFatalException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestGame, GetPlayerTurn) {
    GameConfig gc = {
        30, 1, true,
        30, 1, true,
        PLAYER_ABC
    };
    Game g{gc};

    ASSERT_EQ(g.get_player_turn(), PLAYER_ABC);
}

TEST (TestGame, GetPlayerTurn_Error_AlreadyClosed) {
    GameConfig gc = {
        30, 1, true,
        30, 1, true,
        PLAYER_ABC
    };
    Game g{gc};

    g.close();

    try {
        g.get_player_turn();
        FAIL();

    } catch (CaravanFatalException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestGame, GetTable_Error_AlreadyClosed) {
    GameConfig gc = {
        30, 1, true,
        30, 1, true,
        PLAYER_ABC
    };
    Game g{gc};

    g.close();

    try {
        g.get_table();
        FAIL();

    } catch (CaravanFatalException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestGame, GetWinner_NoMoves) {
    GameConfig gc = {
        30, 1, true,
        30, 1, true,
        PLAYER_ABC
    };
    Game g{gc};

    ASSERT_EQ(g.get_winner(), NO_PLAYER);
}

TEST (TestGame, GetWinner_Error_AlreadyClosed) {
    GameConfig gc = {
        30, 1, true,
        30, 1, true,
        PLAYER_ABC
    };
    Game g{gc};

    g.close();

    try {
        g.get_winner();
        FAIL();

    } catch (CaravanFatalException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestGame, PlayOption_Error_AlreadyClosed) {
    GameConfig gc = {
        30, 1, true,
        30, 1, true,
        PLAYER_ABC
    };
    GameCommand command = {OPTION_DISCARD, 1, NO_CARAVAN, 0};
    Game g{gc};

    g.close();

    try {
        g.play_option(&command);
        FAIL();

    } catch (CaravanFatalException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestGame, PlayOption_Error_StartRound_Remove) {
    GameConfig config = {
        30, 1, true,
        30, 1, true,
        PLAYER_ABC
    };
    GameCommand command = {OPTION_DISCARD, 1, NO_CARAVAN, 0};
    Game g{config};

    try {
        g.play_option(&command);
        FAIL();

    } catch (CaravanGameException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestGame, PlayOption_Error_StartRound_Clear) {
    GameConfig config = {
        30, 1, true,
        30, 1, true,
        PLAYER_ABC
    };
    GameCommand command = {OPTION_CLEAR, 0, CARAVAN_A, 0};
    Game g{config};

    try {
        g.play_option(&command);
        FAIL();

    } catch (CaravanGameException &e) {

    } catch (...) {
        FAIL();
    }
}
