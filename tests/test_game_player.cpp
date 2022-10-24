// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "gtest/gtest.h"
#include "../src/game/model.h"


TEST (TestGameModelPlayer, Deck30_Init) {
    Deck* d = DeckBuilder::build_caravan_deck(30, 1, true);
    Player pl = Player(PLAYER_1, d);

    ASSERT_EQ(pl.get_size_deck(), 22);
    ASSERT_EQ(pl.get_size_hand(), 8);
}

TEST (TestGameModelPlayer, Deck30_TakeFromHand) {
    Deck* d = DeckBuilder::build_caravan_deck(30, 1, true);
    Player pl = Player(PLAYER_1, d);
    Card c_get;
    Card c_take;
    Card c_getagain;

    ASSERT_EQ(pl.get_size_hand(), 8);

    c_get = pl.get_hand()[0];
    c_take = pl.remove_from_hand_at(1);

    ASSERT_EQ(pl.get_size_hand(), 8);
    ASSERT_TRUE(c_get.suit == c_take.suit and c_get.rank == c_take.rank);

    c_getagain = pl.get_hand()[0];
    ASSERT_TRUE(c_getagain.suit != c_take.suit or c_getagain.rank != c_take.rank);
}
