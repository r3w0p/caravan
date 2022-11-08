// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "gtest/gtest.h"
#include "../include/player.h"
#include "../include/exceptions.h"


TEST (TestGamePlayer, GetFromHandAt_Position1) {
    Deck *d = DeckBuilder::build_caravan_deck(30, 1, true);
    Player pl = Player(PLAYER_A, d);
    Card c_get;
    Card c_take;
    Card c_getagain;

    ASSERT_EQ(pl.get_size_hand(), 8);

    c_get = pl.get_hand()[0];
    c_take = pl.get_from_hand_at(1);

    ASSERT_EQ(pl.get_size_hand(), 8);
    ASSERT_TRUE(c_get.suit == c_take.suit and c_get.rank == c_take.rank);

    c_getagain = pl.get_hand()[0];
    ASSERT_TRUE(
            c_getagain.suit == c_take.suit and c_getagain.rank == c_take.rank);
}

TEST (TestGamePlayer, GetName) {
    Deck *d = DeckBuilder::build_caravan_deck(30, 1, true);
    Player pl = Player(PLAYER_A, d);

    ASSERT_EQ(pl.get_name(), PLAYER_A);
}

TEST (TestGamePlayer, GetFromHandAt_Error_HandEmpty) {
    Deck *d = DeckBuilder::build_caravan_deck(30, 1, true);
    Player pl = Player(PLAYER_A, d);

    for (int i = 0; i < 30; ++i) {
        pl.discard_from_hand_at(1);
        pl.increment_moves();
        pl.maybe_add_card_to_hand();
    }

    ASSERT_EQ(pl.get_size_hand(), 0);

    try {
        pl.get_from_hand_at(1);
        FAIL();

    } catch (CaravanFatalException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestGamePlayer, GetFromHandAt_Error_PositionTooLow) {
    Deck *d = DeckBuilder::build_caravan_deck(30, 1, true);
    Player pl = Player(PLAYER_A, d);

    try {
        pl.get_from_hand_at(0);
        FAIL();

    } catch (CaravanGameException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestGamePlayer, GetFromHandAt_Error_PositionTooHigh) {
    Deck *d = DeckBuilder::build_caravan_deck(30, 1, true);
    Player pl = Player(PLAYER_A, d);

    try {
        pl.get_from_hand_at(9);
        FAIL();

    } catch (CaravanGameException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestGamePlayer, GetSizeDeck_Deck30) {
    Deck *d = DeckBuilder::build_caravan_deck(30, 1, true);
    Player pl = Player(PLAYER_A, d);

    ASSERT_EQ(pl.get_size_deck(), 22);
}

TEST (TestGamePlayer, GetSizeHand_Deck30) {
    Deck *d = DeckBuilder::build_caravan_deck(30, 1, true);
    Player pl = Player(PLAYER_A, d);

    ASSERT_EQ(pl.get_size_hand(), 8);
}

TEST (TestGamePlayer, IncrementMovesCount_ThreeTimes) {
    Deck *d = DeckBuilder::build_caravan_deck(30, 1, true);
    Player pl = Player(PLAYER_A, d);

    ASSERT_EQ(pl.get_moves_count(), 0);
    pl.increment_moves();
    ASSERT_EQ(pl.get_moves_count(), 1);
    pl.increment_moves();
    ASSERT_EQ(pl.get_moves_count(), 2);
    pl.increment_moves();
    ASSERT_EQ(pl.get_moves_count(), 3);
}

TEST (TestGamePlayer, RemoveFromHandAt_Position1_StartRound) {
    Deck *d = DeckBuilder::build_caravan_deck(30, 1, true);
    Player pl = Player(PLAYER_A, d);
    Card c_get;
    Card c_take;
    Card c_getagain;

    ASSERT_EQ(pl.get_size_hand(), 8);

    c_get = pl.get_hand()[0];
    c_take = pl.discard_from_hand_at(1);
    pl.increment_moves();
    pl.maybe_add_card_to_hand();

    ASSERT_EQ(pl.get_size_hand(), 7);
    ASSERT_TRUE(c_get.suit == c_take.suit and
                c_get.rank == c_take.rank);

    c_getagain = pl.get_hand()[0];
    ASSERT_TRUE(c_getagain.suit != c_take.suit or
                c_getagain.rank != c_take.rank);
}

TEST (TestGamePlayer, RemoveFromHandAt_Error_HandEmpty) {
    Deck *d = DeckBuilder::build_caravan_deck(30, 1, true);
    Player pl = Player(PLAYER_A, d);

    for (int i = 0; i < 30; ++i) {
        pl.discard_from_hand_at(1);
        pl.increment_moves();
        pl.maybe_add_card_to_hand();
    }

    try {
        pl.discard_from_hand_at(1);
        FAIL();

    } catch (CaravanFatalException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestGamePlayer, RemoveFromHandAt_Error_PositionTooLow) {
    Deck *d = DeckBuilder::build_caravan_deck(30, 1, true);
    Player pl = Player(PLAYER_A, d);

    try {
        pl.discard_from_hand_at(0);
        FAIL();

    } catch (CaravanGameException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestGamePlayer, RemoveFromHandAt_Error_PositionTooHigh) {
    Deck *d = DeckBuilder::build_caravan_deck(30, 1, true);
    Player pl = Player(PLAYER_A, d);

    try {
        pl.discard_from_hand_at(9);
        FAIL();

    } catch (CaravanGameException &e) {

    } catch (...) {
        FAIL();
    }
}
