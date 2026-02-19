// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "gtest/gtest.h"
#include "caravan/model/player.h"
#include "caravan/core/exceptions.h"

using namespace Caravan;


class PlayerTest : public testing::Test {
    protected:
        const uint8_t F_NUM_CARDS = 30;
        const uint8_t F_NUM_SAMPLES = 1;
        const bool F_BALANCED_SAMPLE = true;

        Model::Player plr_abc;
        Model::Player plr_def;

        explicit PlayerTest() : plr_abc(
                                    Model::PLAYER_ABC,
                                    std::unique_ptr<Model::Deck>(
                                        Model::DeckBuilder::build_caravan_deck(
                                            F_NUM_CARDS,
                                            F_NUM_SAMPLES,
                                            F_BALANCED_SAMPLE
                                        )
                                    )
                                ),
                                plr_def(
                                    Model::PLAYER_DEF,
                                    std::unique_ptr<Model::Deck>(
                                        Model::DeckBuilder::build_caravan_deck(
                                            F_NUM_CARDS,
                                            F_NUM_SAMPLES,
                                            F_BALANCED_SAMPLE
                                        )

                                    )
                                ) {
        };
};

TEST_F(PlayerTest, GetFromHandAt_Position1) {
    ASSERT_EQ(plr_abc.get_size_hand(), Model::HAND_SIZE_MAX_START);

    Model::Card c_get_pos1 = plr_abc.get_from_hand_at(1);
    ASSERT_EQ(plr_abc.get_size_hand(), Model::HAND_SIZE_MAX_START);

    Model::Card c_get_pos1_again = plr_abc.get_from_hand_at(1);
    ASSERT_EQ(plr_abc.get_size_hand(), Model::HAND_SIZE_MAX_START);
    ASSERT_EQ(c_get_pos1.suit, c_get_pos1_again.suit);
    ASSERT_EQ(c_get_pos1.rank, c_get_pos1_again.rank);
}

TEST_F(PlayerTest, GetName) {
    ASSERT_EQ(plr_abc.get_name(), Model::PLAYER_ABC);
    ASSERT_EQ(plr_def.get_name(), Model::PLAYER_DEF);
}

TEST_F(PlayerTest, Error_GetFromHandAt_HandEmpty) {
    for (int i = 0; i < 30; ++i) {
        plr_abc.discard_from_hand_at(1);
        plr_abc.increment_moves();
        plr_abc.maybe_add_card_to_hand_from_deck();
    }

    ASSERT_EQ(plr_abc.get_size_hand(), 0);

    try {
        Model::Card c = plr_abc.get_from_hand_at(1);
        FAIL();
    } catch (CaravanFatalModelException &) {
        SUCCEED();
    } catch (...) {
        FAIL();
    }
}

TEST_F(PlayerTest, Error_GetFromHandAt_PositionTooLow) {
    try {
        Model::Card c = plr_abc.get_from_hand_at(0);
        FAIL();
    } catch (CaravanIllegalModelException &) {
        SUCCEED();
    } catch (...) {
        FAIL();
    }
}

TEST_F(PlayerTest, Error_GetFromHandAt_PositionTooHigh) {
    try {
        Model::Card c = plr_abc.get_from_hand_at(9);
        FAIL();
    } catch (CaravanIllegalModelException &) {
        SUCCEED();
    } catch (...) {
        FAIL();
    }
}

TEST_F(PlayerTest, GetSizeDeck_Deck30_InitialDeckSize) {
    ASSERT_EQ(plr_abc.get_size_deck(), 22);
}

TEST_F(PlayerTest, GetSizeHand_Deck30_InitialHandSize) {
    ASSERT_EQ(plr_abc.get_size_hand(), Model::HAND_SIZE_MAX_START);
}

TEST_F(PlayerTest, IncrementMovesCount_ThreeTimes) {
    ASSERT_EQ(plr_abc.get_moves_count(), 0);
    plr_abc.increment_moves();
    ASSERT_EQ(plr_abc.get_moves_count(), 1);
    plr_abc.increment_moves();
    ASSERT_EQ(plr_abc.get_moves_count(), 2);
    plr_abc.increment_moves();
    ASSERT_EQ(plr_abc.get_moves_count(), 3);
}

TEST_F(PlayerTest, DiscardFromHandAt_Position1_StartRound) {
    ASSERT_EQ(plr_abc.get_size_hand(), Model::HAND_SIZE_MAX_START);

    // First and second card in hand
    Model::Card c_get_pos1 = plr_abc.get_from_hand_at(1);
    Model::Card c_get_pos2 = plr_abc.get_from_hand_at(2);

    // Discard first card, as though to play it into a caravan
    Model::Card c_discard_pos1 = plr_abc.discard_from_hand_at(1);

    // Hand size reduced by 1
    ASSERT_EQ(plr_abc.get_size_hand(), 7);

    ASSERT_EQ(c_get_pos1.suit, c_discard_pos1.suit);
    ASSERT_EQ(c_get_pos1.rank, c_discard_pos1.rank);

    // Increment moves and maybe add card to hand
    plr_abc.increment_moves();
    plr_abc.maybe_add_card_to_hand_from_deck();

    // Start round hand will be permanently reduced
    // So no additional card should be added to hand
    ASSERT_EQ(plr_abc.get_size_hand(), 7);

    Model::Card c_get_pos1_again = plr_abc.get_from_hand_at(1);

    // Card originally at position 2 will have dropped to 1
    ASSERT_EQ(c_get_pos1_again.suit, c_get_pos2.suit);
    ASSERT_EQ(c_get_pos1_again.rank, c_get_pos2.rank);
}

TEST_F(PlayerTest, DiscardFromHandAt_StartRoundAndPostStart) {
    ASSERT_EQ(plr_abc.get_size_hand(), Model::HAND_SIZE_MAX_START);

    // Start round has a hand size of 8 that permanently reduces to 5
    for (int i = 0; i < 3; i++) {
        plr_abc.discard_from_hand_at(1);
        plr_abc.increment_moves();
        plr_abc.maybe_add_card_to_hand_from_deck();
        ASSERT_EQ(plr_abc.get_size_hand(), Model::HAND_SIZE_MAX_START - (i+1));
    }

    // After start round, hand stays at 5 cards until it runs out of cards
    for (int i = 0; i < 3; i++) {
        plr_abc.discard_from_hand_at(1);
        plr_abc.increment_moves();
        plr_abc.maybe_add_card_to_hand_from_deck();
        ASSERT_EQ(plr_abc.get_size_hand(), Model::HAND_SIZE_MAX_POST_START);
    }
}

TEST_F(PlayerTest, DiscardFromHandAt_PostStartHandDepletion) {
    ASSERT_EQ(plr_abc.get_size_deck(), F_NUM_CARDS - Model::HAND_SIZE_MAX_START);
    ASSERT_EQ(plr_abc.get_size_hand(), Model::HAND_SIZE_MAX_START);

    // Play cards until last 5
    for (int i = 0; i < F_NUM_CARDS - Model::HAND_SIZE_MAX_POST_START; i++) {
        plr_abc.discard_from_hand_at(1);
        plr_abc.increment_moves();
        plr_abc.maybe_add_card_to_hand_from_deck();
    }

    ASSERT_EQ(plr_abc.get_size_deck(), 0);
    ASSERT_EQ(plr_abc.get_size_hand(), Model::HAND_SIZE_MAX_POST_START);

    // With 5 cards left, hand should deplete to 0
    for (int i = 0; i < Model::HAND_SIZE_MAX_POST_START; i++) {
        plr_abc.discard_from_hand_at(1);
        plr_abc.increment_moves();
        plr_abc.maybe_add_card_to_hand_from_deck();
        ASSERT_EQ(plr_abc.get_size_hand(), Model::HAND_SIZE_MAX_POST_START - (i+1));
    }

    ASSERT_EQ(plr_abc.get_size_deck(), 0);
    ASSERT_EQ(plr_abc.get_size_hand(), 0);
}

TEST_F(PlayerTest, Error_DiscardFromHandAt_HandEmpty) {
    for (int i = 0; i < F_NUM_CARDS; ++i) {
        plr_abc.discard_from_hand_at(1);
        plr_abc.increment_moves();
        plr_abc.maybe_add_card_to_hand_from_deck();
    }

    try {
        plr_abc.discard_from_hand_at(1);
        FAIL();
    } catch (CaravanFatalModelException &) {
        SUCCEED();
    } catch (...) {
        FAIL();
    }
}

TEST_F(PlayerTest, Error_DiscardFromHandAt_PositionTooLow) {
    try {
        plr_abc.discard_from_hand_at(0);
        FAIL();
    } catch (CaravanIllegalModelException &) {
        SUCCEED();
    } catch (...) {
        FAIL();
    }
}

TEST_F(PlayerTest, Error_DiscardFromHandAt_PositionTooHigh) {
    try {
        plr_abc.discard_from_hand_at(9);
        FAIL();
    } catch (CaravanIllegalModelException &) {
        SUCCEED();
    } catch (...) {
        FAIL();
    }
}
