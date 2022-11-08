// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "gtest/gtest.h"
#include "../include/engine.h"


TEST (TestGameEngine, Close) {
    GameConfig gc = {
            30, 1, true,
            30, 1, true,
            PLAYER_A
    };
    Engine e{gc};

    ASSERT_FALSE(e.is_closed());
    e.close();
    ASSERT_TRUE(e.is_closed());
}

TEST (TestGameEngine, Close_Error_AlreadyClosed) {
    GameConfig gc = {
            30, 1, true,
            30, 1, true,
            PLAYER_A
    };
    Engine e{gc};

    e.close();

    try {
        e.close();
        FAIL();

    } catch (CaravanFatalException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestGameEngine, GetPlayer_Both) {
    GameConfig gc = {
            30, 1, true,
            30, 1, true,
            PLAYER_A
    };
    Engine e{gc};

    ASSERT_EQ(e.get_player(PLAYER_A)->get_name(), PLAYER_A);
    ASSERT_EQ(e.get_player(PLAYER_B)->get_name(), PLAYER_B);
}

TEST (TestGameEngine, GetPlayer_Error_AlreadyClosed) {
    GameConfig gc = {
            30, 1, true,
            30, 1, true,
            PLAYER_A
    };
    Engine e{gc};

    e.close();

    try {
        e.get_player(PLAYER_A);
        FAIL();

    } catch (CaravanFatalException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestGameEngine, GetPlayer_Error_InvalidName) {
    GameConfig gc = {
            30, 1, true,
            30, 1, true,
            PLAYER_A
    };
    Engine e{gc};

    try {
        e.get_player(NO_PLAYER);
        FAIL();

    } catch (CaravanFatalException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestGameEngine, GetPlayerTurn) {
    GameConfig gc = {
            30, 1, true,
            30, 1, true,
            PLAYER_A
    };
    Engine e{gc};

    ASSERT_EQ(e.get_player_turn(), PLAYER_A);
}

TEST (TestGameEngine, GetPlayerTurn_Error_AlreadyClosed) {
    GameConfig gc = {
            30, 1, true,
            30, 1, true,
            PLAYER_A
    };
    Engine e{gc};

    e.close();

    try {
        e.get_player_turn();
        FAIL();

    } catch (CaravanFatalException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestGameEngine, GetTable_Error_AlreadyClosed) {
    GameConfig gc = {
            30, 1, true,
            30, 1, true,
            PLAYER_A
    };
    Engine e{gc};

    e.close();

    try {
        e.get_table();
        FAIL();

    } catch (CaravanFatalException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestGameEngine, GetWinner_NoMoves) {
    GameConfig gc = {
            30, 1, true,
            30, 1, true,
            PLAYER_A
    };
    Engine e{gc};

    ASSERT_EQ(e.get_winner(), NO_PLAYER);
}

TEST (TestGameEngine, GetWinner_Error_AlreadyClosed) {
    GameConfig gc = {
            30, 1, true,
            30, 1, true,
            PLAYER_A
    };
    Engine e{gc};

    e.close();

    try {
        e.get_winner();
        FAIL();

    } catch (CaravanFatalException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestGameEngine, PlayOption_Error_AlreadyClosed) {
    GameConfig gc = {
            30, 1, true,
            30, 1, true,
            PLAYER_A
    };
    Engine e{gc};

    e.close();

    try {
        e.play_option({OPTION_DISCARD, 1, NO_CARAVAN, 0});
        FAIL();

    } catch (CaravanFatalException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestGameEngine, PlayOption_Error_StartRound_Remove) {
    GameConfig gc = {
            30, 1, true,
            30, 1, true,
            PLAYER_A
    };
    Engine e{gc};

    try {
        e.play_option({OPTION_DISCARD, 1, NO_CARAVAN, 0});
        FAIL();

    } catch (CaravanGameException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestGameEngine, PlayOption_Error_StartRound_Clear) {
    GameConfig gc = {
            30, 1, true,
            30, 1, true,
            PLAYER_A
    };
    Engine e{gc};

    try {
        e.play_option({OPTION_CLEAR, 0, CARAVAN_A, 0});
        FAIL();

    } catch (CaravanGameException &e) {

    } catch (...) {
        FAIL();
    }
}
