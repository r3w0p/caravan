// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "gtest/gtest.h"
#include "caravan/model/player.h"
#include "caravan/core/exceptions.h"

using namespace Caravan;


class PlayerTest : public testing::Test {
    protected:
        // TODO
        explicit PlayerTest() = default;
};

TEST_F(PlayerTest, GetFromHandAt_Position1) {
    std::unique_ptr<Model::Deck> d(Model::DeckBuilder::build_caravan_deck(30, 1, true));
    Model::Player pl = Model::Player(Model::PLAYER_ABC, std::move(d));
    Model::Card c_get;
    Model::Card c_take;
    Model::Card c_getagain;

    ASSERT_EQ(pl.get_size_hand(), 8);

    c_get = pl.get_from_hand_at(1);
    c_take = pl.get_from_hand_at(1);

    ASSERT_EQ(pl.get_size_hand(), 8);
    ASSERT_TRUE(c_get.suit == c_take.suit and c_get.rank == c_take.rank);

    c_getagain = pl.get_from_hand_at(1);
    ASSERT_TRUE(
        c_getagain.suit == c_take.suit and c_getagain.rank == c_take.rank
    );
}

TEST_F(PlayerTest, GetName) {
    std::unique_ptr<Model::Deck> d(Model::DeckBuilder::build_caravan_deck(30, 1, true));
    Model::Player pl = Model::Player(Model::PLAYER_ABC, std::move(d));

    ASSERT_EQ(pl.get_name(), Model::PLAYER_ABC);
}

TEST_F(PlayerTest, GetFromHandAt_Error_HandEmpty) {
    std::unique_ptr<Model::Deck> d(Model::DeckBuilder::build_caravan_deck(30, 1, true));
    Model::Player pl = Model::Player(Model::PLAYER_ABC, std::move(d));

    for (int i = 0; i < 30; ++i) {
        pl.discard_from_hand_at(1);
        pl.increment_moves();
        pl.maybe_add_card_to_hand_from_deck();
    }

    ASSERT_EQ(pl.get_size_hand(), 0);

    try {
        Model::Card c = pl.get_from_hand_at(1);
        FAIL();
    } catch (CaravanFatalModelException &) {} catch (...) {
        FAIL();
    }
}

TEST_F(PlayerTest, GetFromHandAt_Error_PositionTooLow) {
    std::unique_ptr<Model::Deck> d(Model::DeckBuilder::build_caravan_deck(30, 1, true));
    Model::Player pl = Model::Player(Model::PLAYER_ABC, std::move(d));

    try {
        Model::Card c = pl.get_from_hand_at(0);
        FAIL();
    } catch (CaravanIllegalModelException &) {} catch (...) {
        FAIL();
    }
}

TEST_F(PlayerTest, GetFromHandAt_Error_PositionTooHigh) {
    std::unique_ptr<Model::Deck> d(Model::DeckBuilder::build_caravan_deck(30, 1, true));
    Model::Player pl = Model::Player(Model::PLAYER_ABC, std::move(d));

    try {
        Model::Card c = pl.get_from_hand_at(9);
        FAIL();
    } catch (CaravanIllegalModelException &) {} catch (...) {
        FAIL();
    }
}

TEST_F(PlayerTest, GetSizeDeck_Deck30) {
    std::unique_ptr<Model::Deck> d(Model::DeckBuilder::build_caravan_deck(30, 1, true));
    Model::Player pl = Model::Player(Model::PLAYER_ABC, std::move(d));

    ASSERT_EQ(pl.get_size_deck(), 22);
}

TEST_F(PlayerTest, GetSizeHand_Deck30) {
    std::unique_ptr<Model::Deck> d(Model::DeckBuilder::build_caravan_deck(30, 1, true));
    Model::Player pl = Model::Player(Model::PLAYER_ABC, std::move(d));

    ASSERT_EQ(pl.get_size_hand(), 8);
}

TEST_F(PlayerTest, IncrementMovesCount_ThreeTimes) {
    std::unique_ptr<Model::Deck> d(Model::DeckBuilder::build_caravan_deck(30, 1, true));
    Model::Player pl = Model::Player(Model::PLAYER_ABC, std::move(d));

    ASSERT_EQ(pl.get_moves_count(), 0);
    pl.increment_moves();
    ASSERT_EQ(pl.get_moves_count(), 1);
    pl.increment_moves();
    ASSERT_EQ(pl.get_moves_count(), 2);
    pl.increment_moves();
    ASSERT_EQ(pl.get_moves_count(), 3);
}

TEST_F(PlayerTest, RemoveFromHandAt_Position1_StartRound) {
    std::unique_ptr<Model::Deck> d(Model::DeckBuilder::build_caravan_deck(30, 1, true));
    Model::Player pl = Model::Player(Model::PLAYER_ABC, std::move(d));
    Model::Card c_get;
    Model::Card c_take;
    Model::Card c_getagain;

    ASSERT_EQ(pl.get_size_hand(), 8);

    c_get = pl.get_from_hand_at(1);
    c_take = pl.discard_from_hand_at(1);
    pl.increment_moves();
    pl.maybe_add_card_to_hand_from_deck();

    ASSERT_EQ(pl.get_size_hand(), 7);
    ASSERT_TRUE(
        c_get.suit == c_take.suit and
        c_get.rank == c_take.rank
    );

    c_getagain = pl.get_from_hand_at(1);
    ASSERT_TRUE(
        c_getagain.suit != c_take.suit or
        c_getagain.rank != c_take.rank
    );
}

TEST_F(PlayerTest, RemoveFromHandAt_Error_HandEmpty) {
    std::unique_ptr<Model::Deck> d(Model::DeckBuilder::build_caravan_deck(30, 1, true));
    Model::Player pl = Model::Player(Model::PLAYER_ABC, std::move(d));

    for (int i = 0; i < 30; ++i) {
        pl.discard_from_hand_at(1);
        pl.increment_moves();
        pl.maybe_add_card_to_hand_from_deck();
    }

    try {
        pl.discard_from_hand_at(1);
        FAIL();
    } catch (CaravanFatalModelException &) {} catch (...) {
        FAIL();
    }
}

TEST_F(PlayerTest, RemoveFromHandAt_Error_PositionTooLow) {
    std::unique_ptr<Model::Deck> d(Model::DeckBuilder::build_caravan_deck(30, 1, true));
    Model::Player pl = Model::Player(Model::PLAYER_ABC, std::move(d));

    try {
        pl.discard_from_hand_at(0);
        FAIL();
    } catch (CaravanIllegalModelException &) {} catch (...) {
        FAIL();
    }
}

TEST_F(PlayerTest, RemoveFromHandAt_Error_PositionTooHigh) {
    std::unique_ptr<Model::Deck> d(Model::DeckBuilder::build_caravan_deck(30, 1, true));
    Model::Player pl = Model::Player(Model::PLAYER_ABC, std::move(d));

    try {
        pl.discard_from_hand_at(9);
        FAIL();
    } catch (CaravanIllegalModelException &) {} catch (...) {
        FAIL();
    }
}