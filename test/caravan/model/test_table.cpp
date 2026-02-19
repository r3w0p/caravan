// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "gtest/gtest.h"
#include "caravan/model/table.h"
#include "caravan/core/exceptions.h"

using namespace Caravan;

class TableTest : public testing::Test {
    protected:
        Model::Table tbl;
        
        Model::Caravan &cvn_a = tbl.get_caravan(Model::CARAVAN_A);
        Model::Caravan &cvn_b = tbl.get_caravan(Model::CARAVAN_B);
        Model::Caravan &cvn_c = tbl.get_caravan(Model::CARAVAN_C);
        Model::Caravan &cvn_d = tbl.get_caravan(Model::CARAVAN_D);
        Model::Caravan &cvn_e = tbl.get_caravan(Model::CARAVAN_E);
        Model::Caravan &cvn_f = tbl.get_caravan(Model::CARAVAN_F);
        
        Model::CaravanName cvname_a = cvn_a.get_name();
        Model::CaravanName cvname_b = cvn_b.get_name();
        Model::CaravanName cvname_c = cvn_c.get_name();
        Model::CaravanName cvname_d = cvn_d.get_name();
        Model::CaravanName cvname_e = cvn_e.get_name();
        Model::CaravanName cvname_f = cvn_f.get_name();
        
        Model::Card c_c_a = {Model::CLUBS, Model::ACE};
        Model::Card c_c_2 = {Model::CLUBS, Model::TWO};
        Model::Card c_c_3 = {Model::CLUBS, Model::THREE};
        Model::Card c_c_4 = {Model::CLUBS, Model::FOUR};
        Model::Card c_c_5 = {Model::CLUBS, Model::FIVE};
        Model::Card c_c_6 = {Model::CLUBS, Model::SIX};
        Model::Card c_c_7 = {Model::CLUBS, Model::SEVEN};
        Model::Card c_c_8 = {Model::CLUBS, Model::EIGHT};
        Model::Card c_c_9 = {Model::CLUBS, Model::NINE};
        Model::Card c_c_10 = {Model::CLUBS, Model::TEN};
        Model::Card c_c_ja = {Model::CLUBS, Model::JACK};
        Model::Card c_c_q = {Model::CLUBS, Model::QUEEN};
        Model::Card c_c_k = {Model::CLUBS, Model::KING};
        Model::Card c_c_jo = {Model::CLUBS, Model::JOKER};

        Model::Card c_d_a = {Model::DIAMONDS, Model::ACE};
        Model::Card c_d_2 = {Model::DIAMONDS, Model::TWO};
        Model::Card c_d_3 = {Model::DIAMONDS, Model::THREE};
        Model::Card c_d_4 = {Model::DIAMONDS, Model::FOUR};
        Model::Card c_d_5 = {Model::DIAMONDS, Model::FIVE};
        Model::Card c_d_6 = {Model::DIAMONDS, Model::SIX};
        Model::Card c_d_7 = {Model::DIAMONDS, Model::SEVEN};
        Model::Card c_d_8 = {Model::DIAMONDS, Model::EIGHT};
        Model::Card c_d_9 = {Model::DIAMONDS, Model::NINE};
        Model::Card c_d_10 = {Model::DIAMONDS, Model::TEN};
        Model::Card c_d_ja = {Model::DIAMONDS, Model::JACK};
        Model::Card c_d_q = {Model::DIAMONDS, Model::QUEEN};
        Model::Card c_d_k = {Model::DIAMONDS, Model::KING};
        Model::Card c_d_jo = {Model::DIAMONDS, Model::JOKER};

        Model::Card c_h_a = {Model::HEARTS, Model::ACE};
        Model::Card c_h_2 = {Model::HEARTS, Model::TWO};
        Model::Card c_h_3 = {Model::HEARTS, Model::THREE};
        Model::Card c_h_4 = {Model::HEARTS, Model::FOUR};
        Model::Card c_h_5 = {Model::HEARTS, Model::FIVE};
        Model::Card c_h_6 = {Model::HEARTS, Model::SIX};
        Model::Card c_h_7 = {Model::HEARTS, Model::SEVEN};
        Model::Card c_h_8 = {Model::HEARTS, Model::EIGHT};
        Model::Card c_h_9 = {Model::HEARTS, Model::NINE};
        Model::Card c_h_10 = {Model::HEARTS, Model::TEN};
        Model::Card c_h_ja = {Model::HEARTS, Model::JACK};
        Model::Card c_h_q = {Model::HEARTS, Model::QUEEN};
        Model::Card c_h_k = {Model::HEARTS, Model::KING};
        Model::Card c_h_jo = {Model::HEARTS, Model::JOKER};

        Model::Card c_s_a = {Model::SPADES, Model::ACE};
        Model::Card c_s_2 = {Model::SPADES, Model::TWO};
        Model::Card c_s_3 = {Model::SPADES, Model::THREE};
        Model::Card c_s_4 = {Model::SPADES, Model::FOUR};
        Model::Card c_s_5 = {Model::SPADES, Model::FIVE};
        Model::Card c_s_6 = {Model::SPADES, Model::SIX};
        Model::Card c_s_7 = {Model::SPADES, Model::SEVEN};
        Model::Card c_s_8 = {Model::SPADES, Model::EIGHT};
        Model::Card c_s_9 = {Model::SPADES, Model::NINE};
        Model::Card c_s_10 = {Model::SPADES, Model::TEN};
        Model::Card c_s_ja = {Model::SPADES, Model::JACK};
        Model::Card c_s_q = {Model::SPADES, Model::QUEEN};
        Model::Card c_s_k = {Model::SPADES, Model::KING};
        Model::Card c_s_jo = {Model::SPADES, Model::JOKER};

        explicit TableTest() = default;
};

TEST_F(TableTest, ClearCaravan_TwoNumeric_OneFace) {
    tbl.play_numeral_card(cvname_a, c_s_a);
    tbl.play_numeral_card(cvname_a, c_s_2);
    tbl.play_face_card(cvname_a, c_h_k, 2);
    ASSERT_EQ(cvn_a.get_size(), 2);

    tbl.clear_caravan(cvname_a);
    ASSERT_EQ(cvn_a.get_size(), 0);
}

TEST_F(TableTest, GetCaravanBid_ThreeNumeric) {
    ASSERT_EQ(cvn_a.get_bid(), 0);
    tbl.play_numeral_card(cvname_a, c_s_a);
    ASSERT_EQ(cvn_a.get_bid(), 1);
    tbl.play_numeral_card(cvname_a, c_s_2);
    ASSERT_EQ(cvn_a.get_bid(), 3);
    tbl.play_numeral_card(cvname_a, c_s_3);
    ASSERT_EQ(cvn_a.get_bid(), 6);
}

TEST_F(TableTest, GetCaravanCardsAt_ThreeNumeric) {
    tbl.play_numeral_card(cvname_a, c_s_a);
    tbl.play_numeral_card(cvname_a, c_s_2);
    tbl.play_numeral_card(cvname_a, c_s_3);

    Model::Slot slt_1 = cvn_a.get_slot(1);
    ASSERT_EQ(slt_1.card.suit, c_s_a.suit);
    ASSERT_EQ(slt_1.card.rank, c_s_a.rank);

    Model::Slot slt_2 = cvn_a.get_slot(2);
    ASSERT_EQ(slt_2.card.suit, c_s_2.suit);
    ASSERT_EQ(slt_2.card.rank, c_s_2.rank);

    Model::Slot slt_3 = cvn_a.get_slot(3);
    ASSERT_EQ(slt_3.card.suit, c_s_3.suit);
    ASSERT_EQ(slt_3.card.rank, c_s_3.rank);
}

TEST_F(TableTest, GetCaravanDirection_Ascending) {
    ASSERT_EQ(cvn_a.get_direction(), Model::ANY);
    tbl.play_numeral_card(cvname_a, c_s_a);
    ASSERT_EQ(cvn_a.get_direction(), Model::ANY);
    tbl.play_numeral_card(cvname_a, c_s_2);
    ASSERT_EQ(cvn_a.get_direction(), Model::ASCENDING);
}

TEST_F(TableTest, GetCaravanDirection_Descending) {
    ASSERT_EQ(cvn_a.get_direction(), Model::ANY);
    tbl.play_numeral_card(cvname_a, c_s_2);
    ASSERT_EQ(cvn_a.get_direction(), Model::ANY);
    tbl.play_numeral_card(cvname_a, c_s_a);
    ASSERT_EQ(cvn_a.get_direction(), Model::DESCENDING);
}

TEST_F(TableTest, GetCaravanSize_ThreeNumeric) {
    ASSERT_EQ(cvn_a.get_size(), 0);

    tbl.play_numeral_card(cvname_a, c_s_a);
    tbl.play_numeral_card(cvname_a, c_c_2);
    tbl.play_numeral_card(cvname_a, c_h_3);

    ASSERT_EQ(cvn_a.get_size(), 3);
}

TEST_F(TableTest, GetCaravanSuit_BeforeAfter) {
    ASSERT_EQ(cvn_a.get_suit(), Model::NO_SUIT);
    tbl.play_numeral_card(cvname_a, c_s_a);
    ASSERT_EQ(cvn_a.get_suit(), Model::SPADES);
}

TEST_F(TableTest, PlayFaceCard_Jack) {
    tbl.play_numeral_card(cvname_a, c_s_a);
    tbl.play_numeral_card(cvname_a, c_c_2);
    tbl.play_numeral_card(cvname_a, c_h_3);

    ASSERT_EQ(cvn_a.get_size(), 3);
    ASSERT_EQ(cvn_a.get_suit(), c_h_3.suit);

    tbl.play_face_card(cvname_a, c_d_ja, 2);

    ASSERT_EQ(cvn_a.get_size(), 2);
    ASSERT_EQ(cvn_a.get_suit(), c_h_3.suit);
    ASSERT_EQ(cvn_a.get_direction(), Model::ASCENDING);

    Model::Slot slt_1 = cvn_a.get_slot(1);
    ASSERT_EQ(slt_1.card.suit, c_s_a.suit);
    ASSERT_EQ(slt_1.card.rank, c_s_a.rank);

    Model::Slot slt_2 = cvn_a.get_slot(2);
    ASSERT_EQ(slt_2.card.suit, c_h_3.suit);
    ASSERT_EQ(slt_2.card.rank, c_h_3.rank);
}

TEST_F(TableTest, PlayFaceCard_Queen) {
    tbl.play_numeral_card(cvname_a, c_s_a);
    tbl.play_numeral_card(cvname_a, c_c_2);
    tbl.play_numeral_card(cvname_a, c_h_3);

    ASSERT_EQ(cvn_a.get_size(), 3);
    ASSERT_EQ(cvn_a.get_suit(), c_h_3.suit);
    ASSERT_EQ(cvn_a.get_direction(), Model::ASCENDING);

    tbl.play_face_card(cvname_a, c_d_q, 3);

    ASSERT_EQ(cvn_a.get_size(), 3);
    ASSERT_EQ(cvn_a.get_suit(), c_d_q.suit);
    ASSERT_EQ(cvn_a.get_direction(), Model::DESCENDING);
}

TEST_F(TableTest, PlayFaceCard_Error_Queen_NotPlayedOnTopCard) {
    tbl.play_numeral_card(cvname_a, c_s_a);
    tbl.play_numeral_card(cvname_a, c_c_2);
    tbl.play_numeral_card(cvname_a, c_h_3);

    try {
        tbl.play_face_card(cvname_a, c_d_q, 2);
        FAIL();
    } catch (CaravanIllegalModelException &) {} catch (...) {
        FAIL();
    }
}

TEST_F(TableTest, PlayFaceCard_King_OneNumeric_ThreeKings) {
    tbl.play_numeral_card(cvname_a, c_s_5);
    ASSERT_EQ(cvn_a.get_bid(), 5);

    tbl.play_face_card(cvname_a, c_d_k, 1);
    ASSERT_EQ(cvn_a.get_bid(), 10);

    tbl.play_face_card(cvname_a, c_c_k, 1);
    ASSERT_EQ(cvn_a.get_bid(), 20);

    tbl.play_face_card(cvname_a, c_h_k, 1);
    ASSERT_EQ(cvn_a.get_bid(), 40);
}

TEST_F(TableTest, PlayFaceCard_Joker_Ace) {
    tbl.play_numeral_card(cvname_d, c_s_a);
    tbl.play_numeral_card(cvname_d, c_h_3);
    tbl.play_numeral_card(cvname_d, c_s_7);
    tbl.play_numeral_card(cvname_e, c_s_5);
    tbl.play_numeral_card(cvname_f, c_d_10);

    ASSERT_EQ(cvn_d.get_size(), 3);
    ASSERT_EQ(cvn_e.get_size(), 1);
    ASSERT_EQ(cvn_f.get_size(), 1);

    tbl.play_face_card(cvname_d, c_d_jo, 1);

    ASSERT_EQ(cvn_d.get_size(), 2);
    ASSERT_EQ(cvn_e.get_size(), 0);
    ASSERT_EQ(cvn_f.get_size(), 1);

    Model::Slot slt = cvn_d.get_slot(1);
    ASSERT_EQ(slt.card.suit, c_s_a.suit);
    ASSERT_EQ(slt.card.rank, c_s_a.rank);
}

TEST_F(TableTest, PlayFaceCard_Joker_2To10) {
    tbl.play_numeral_card(cvname_d, c_s_2);
    tbl.play_numeral_card(cvname_d, c_h_3);
    tbl.play_numeral_card(cvname_d, c_h_2);
    tbl.play_numeral_card(cvname_e, c_s_5);
    tbl.play_numeral_card(cvname_f, c_d_2);

    ASSERT_EQ(cvn_d.get_size(), 3);
    ASSERT_EQ(cvn_e.get_size(), 1);
    ASSERT_EQ(cvn_f.get_size(), 1);

    tbl.play_face_card(cvname_d, c_d_jo, 1);

    ASSERT_EQ(cvn_d.get_size(), 2);
    ASSERT_EQ(cvn_e.get_size(), 1);
    ASSERT_EQ(cvn_f.get_size(), 0);

    Model::Slot slt = cvn_d.get_slot(1);
    ASSERT_EQ(slt.card.suit, c_s_2.suit);
    ASSERT_EQ(slt.card.rank, c_s_2.rank);
}

TEST_F(TableTest, PlayNumericCard) {
    ASSERT_EQ(cvn_a.get_size(), 0);
    ASSERT_EQ(cvn_a.get_suit(), Model::NO_SUIT);
    ASSERT_EQ(cvn_a.get_direction(), Model::ANY);

    tbl.play_numeral_card(cvname_a, c_s_a);

    ASSERT_EQ(cvn_a.get_size(), 1);
    ASSERT_EQ(cvn_a.get_suit(), c_s_a.suit);
    ASSERT_EQ(cvn_a.get_direction(), Model::ANY);

    tbl.play_numeral_card(cvname_a, c_s_3);

    ASSERT_EQ(cvn_a.get_size(), 2);
    ASSERT_EQ(cvn_a.get_suit(), c_s_3.suit);
    ASSERT_EQ(cvn_a.get_direction(), Model::ASCENDING);
}

TEST_F(TableTest, PlayNumericCard_Error_TwoCards_SameRank_InSequence) {
    tbl.play_numeral_card(cvname_a, c_s_3);

    try {
        tbl.play_numeral_card(cvname_a, c_d_3);
        FAIL();
    } catch (CaravanIllegalModelException &) {} catch (...) {
        FAIL();
    }
}

TEST_F(TableTest, PlayNumericCard_Error_OppositeDirection_DifferentSuit) {
    tbl.play_numeral_card(cvname_a, c_s_5);
    tbl.play_numeral_card(cvname_a, c_d_7);
    ASSERT_EQ(cvn_a.get_direction(), Model::ASCENDING);

    try {
        tbl.play_numeral_card(cvname_a, c_c_2);
        FAIL();
    } catch (CaravanIllegalModelException &) {} catch (...) {
        FAIL();
    }
}

TEST_F(TableTest, PlayNumericCard_OppositeDirection_SameSuit) {
    tbl.play_numeral_card(cvname_a, c_s_5);
    tbl.play_numeral_card(cvname_a, c_d_7);

    ASSERT_EQ(cvn_a.get_suit(), Model::DIAMONDS);
    ASSERT_EQ(cvn_a.get_direction(), Model::ASCENDING);

    tbl.play_numeral_card(cvname_a, c_d_2);

    ASSERT_EQ(cvn_a.get_suit(), Model::DIAMONDS);
    ASSERT_EQ(cvn_a.get_direction(), Model::DESCENDING);
}
