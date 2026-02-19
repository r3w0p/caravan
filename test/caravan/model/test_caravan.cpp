// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "gtest/gtest.h"
#include "caravan/model/caravan.h"
#include "caravan/model/types.h"
#include "caravan/model/constants.h"
#include "caravan/core/exceptions.h"

using namespace Caravan;

class TableTest : public testing::Test {
    protected:
        Model::Caravan cvn_a = Model::Caravan(Model::CARAVAN_A);
        Model::Caravan cvn_b = Model::Caravan(Model::CARAVAN_B);
        Model::Caravan cvn_c = Model::Caravan(Model::CARAVAN_C);
        Model::Caravan cvn_d = Model::Caravan(Model::CARAVAN_D);
        Model::Caravan cvn_e = Model::Caravan(Model::CARAVAN_E);
        Model::Caravan cvn_f = Model::Caravan(Model::CARAVAN_F);

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


TEST_F(TableTest, Clear_ThreeNumeral) {
    // Caravan starts empty
    ASSERT_EQ(cvn_a.get_size(), 0);

    // Add three numeral cards to the caravan
    cvn_a.put_numeral_card(c_s_a);
    cvn_a.put_numeral_card(c_s_2);
    cvn_a.put_numeral_card(c_s_3);
    ASSERT_EQ(cvn_a.get_size(), 3);

    // Clear all cards from the caravan
    cvn_a.clear();
    ASSERT_EQ(cvn_a.get_size(), 0);
}

TEST_F(TableTest, Error_Clear_EmptyCaravan) {
    try {
        // Caravan starts empty; clearing is an illegal move when empty
        cvn_a.clear();
        FAIL();
    } catch (CaravanIllegalModelException &) {
    } catch (...) {
        FAIL();
    }
}

TEST_F(TableTest, GetBid_ThreeNumeral) {
    // Caravan starts empty
    ASSERT_EQ(cvn_a.get_bid(), 0);

    // Add first numeral card of value 1
    cvn_a.put_numeral_card(c_s_a);
    ASSERT_EQ(cvn_a.get_bid(), 1);

    // Add second numeral card of value 2
    cvn_a.put_numeral_card(c_s_2);
    ASSERT_EQ(cvn_a.get_bid(), 3);

    // Add third numeral card of value 3
    cvn_a.put_numeral_card(c_s_3);
    ASSERT_EQ(cvn_a.get_bid(), 6);
}

TEST_F(TableTest, GetBid_Value_RankAce) {
    ASSERT_EQ(cvn_a.get_bid(), 0);
    cvn_a.put_numeral_card(c_s_a);
    ASSERT_EQ(cvn_a.get_bid(), 1);
}

TEST_F(TableTest, GetBid_Value_RankTwo) {
    ASSERT_EQ(cvn_a.get_bid(), 0);
    cvn_a.put_numeral_card(c_s_2);
    ASSERT_EQ(cvn_a.get_bid(), 2);
}

TEST_F(TableTest, GetBid_Value_RankThree) {
    ASSERT_EQ(cvn_a.get_bid(), 0);
    cvn_a.put_numeral_card(c_s_3);
    ASSERT_EQ(cvn_a.get_bid(), 3);
}

TEST_F(TableTest, GetBid_Value_RankFour) {
    ASSERT_EQ(cvn_a.get_bid(), 0);
    cvn_a.put_numeral_card(c_s_4);
    ASSERT_EQ(cvn_a.get_bid(), 4);
}

TEST_F(TableTest, GetBid_Value_RankFive) {
    ASSERT_EQ(cvn_a.get_bid(), 0);
    cvn_a.put_numeral_card(c_s_5);
    ASSERT_EQ(cvn_a.get_bid(), 5);
}

TEST_F(TableTest, GetBid_Value_RankSix) {
    ASSERT_EQ(cvn_a.get_bid(), 0);
    cvn_a.put_numeral_card(c_s_6);
    ASSERT_EQ(cvn_a.get_bid(), 6);
}

TEST_F(TableTest, GetBid_Value_RankSeven) {
    ASSERT_EQ(cvn_a.get_bid(), 0);
    cvn_a.put_numeral_card(c_s_7);
    ASSERT_EQ(cvn_a.get_bid(), 7);
}

TEST_F(TableTest, GetBid_Value_RankEight) {
    ASSERT_EQ(cvn_a.get_bid(), 0);
    cvn_a.put_numeral_card(c_s_8);
    ASSERT_EQ(cvn_a.get_bid(), 8);
}

TEST_F(TableTest, GetBid_Value_RankNine) {
    ASSERT_EQ(cvn_a.get_bid(), 0);
    cvn_a.put_numeral_card(c_s_9);
    ASSERT_EQ(cvn_a.get_bid(), 9);
}

TEST_F(TableTest, GetBid_Value_RankTen) {
    ASSERT_EQ(cvn_a.get_bid(), 0);
    cvn_a.put_numeral_card(c_s_10);
    ASSERT_EQ(cvn_a.get_bid(), 10);
}

TEST_F(TableTest, GetCardsAt_TwoNumeral_OneFace) {
    // Add two numerals
    cvn_a.put_numeral_card(c_s_a);
    cvn_a.put_numeral_card(c_s_2);

    // Add face card to the second numeral
    cvn_a.put_face_card(c_d_k, 2);

    // Check first numeral: should not have face card added to it
    Model::Slot slt_1 = cvn_a.get_slot(1);
    ASSERT_EQ(slt_1.card.suit, c_s_a.suit);
    ASSERT_EQ(slt_1.card.rank, c_s_a.rank);
    ASSERT_EQ(slt_1.i_faces, 0);

    // Check second numeral: should have face card added to it
    Model::Slot slt_2 = cvn_a.get_slot(2);
    ASSERT_EQ(slt_2.card.suit, c_s_2.suit);
    ASSERT_EQ(slt_2.card.rank, c_s_2.rank);
    ASSERT_EQ(slt_2.i_faces, 1);

    // Check face card details match what was added to slot
    ASSERT_EQ(slt_2.faces[0].suit, c_d_k.suit);
    ASSERT_EQ(slt_2.faces[0].rank, c_d_k.rank);
}

TEST_F(TableTest, Error_GetCardsAt_OneNumeral_OutOfRange) {
    cvn_a.put_numeral_card(c_s_a);

    try {
        Model::Slot slt = cvn_a.get_slot(2);
        FAIL();
    } catch (CaravanIllegalModelException &) {
    } catch (...) {
        FAIL();
    }
}

TEST_F(TableTest, Error_GetCardsAt_TwoNumerals_OutOfRange) {
    cvn_a.put_numeral_card(c_s_a);
    cvn_a.put_numeral_card(c_s_2);

    try {
        Model::Slot slt = cvn_a.get_slot(3);
        FAIL();
    } catch (CaravanIllegalModelException &) {
    } catch (...) {
        FAIL();
    }
}

TEST_F(TableTest, Error_GetCardsAt_SevenNumerals_OutOfRange) {
    cvn_a.put_numeral_card(c_s_a);
    cvn_a.put_numeral_card(c_s_2);
    cvn_a.put_numeral_card(c_s_3);
    cvn_a.put_numeral_card(c_s_4);
    cvn_a.put_numeral_card(c_s_5);
    cvn_a.put_numeral_card(c_s_6);
    cvn_a.put_numeral_card(c_s_7);

    try {
        Model::Slot slt = cvn_a.get_slot(8);
        FAIL();
    } catch (CaravanIllegalModelException &) {
    } catch (...) {
        FAIL();
    }
}

TEST_F(TableTest, GetDirection_Ascending) {
    // No direction when caravan is empty
    ASSERT_EQ(cvn_a.get_direction(), Model::ANY);

    // No direction with one numeral card
    cvn_a.put_numeral_card(c_s_5);
    ASSERT_EQ(cvn_a.get_direction(), Model::ANY);

    // Ascending direction with second card of greater rank than first
    cvn_a.put_numeral_card(c_s_10);
    ASSERT_EQ(cvn_a.get_direction(), Model::ASCENDING);
}

TEST_F(TableTest, GetDirection_Descending) {
    // No direction when caravan is empty
    ASSERT_EQ(cvn_a.get_direction(), Model::ANY);

    // No direction with one numeral card
    cvn_a.put_numeral_card(c_s_10);
    ASSERT_EQ(cvn_a.get_direction(), Model::ANY);

    // Descending direction with second card of lower rank than first
    cvn_a.put_numeral_card(c_s_5);
    ASSERT_EQ(cvn_a.get_direction(), Model::DESCENDING);
}

TEST_F(TableTest, GetDirection_Ascending_ThreeQueens) {
    // Ascending caravan
    cvn_a.put_numeral_card(c_s_a);
    cvn_a.put_numeral_card(c_h_2);
    ASSERT_EQ(cvn_a.get_direction(), Model::ASCENDING);

    // First QUEEN flips caravan to descending
    cvn_a.put_face_card(c_s_q, 2);
    ASSERT_EQ(cvn_a.get_direction(), Model::DESCENDING);

    // Second QUEEN flips caravan to ascending
    cvn_a.put_face_card(c_d_q, 2);
    ASSERT_EQ(cvn_a.get_direction(), Model::ASCENDING);

    // Third QUEEN flips caravan to descending
    cvn_a.put_face_card(c_h_q, 2);
    ASSERT_EQ(cvn_a.get_direction(), Model::DESCENDING);
}

TEST_F(TableTest, GetDirection_Descending_ThreeQueens) {
    // Descending caravan
    cvn_a.put_numeral_card(c_s_10);
    cvn_a.put_numeral_card(c_h_8);
    ASSERT_EQ(cvn_a.get_direction(), Model::DESCENDING);

    // First QUEEN flips caravan to ascending
    cvn_a.put_face_card(c_c_q, 2);
    ASSERT_EQ(cvn_a.get_direction(), Model::ASCENDING);

    // Second QUEEN flips caravan to descending
    cvn_a.put_face_card(c_d_q, 2);
    ASSERT_EQ(cvn_a.get_direction(), Model::DESCENDING);

    // Third QUEEN flips caravan to ascending
    cvn_a.put_face_card(c_h_q, 2);
    ASSERT_EQ(cvn_a.get_direction(), Model::ASCENDING);
}

TEST_F(TableTest, GetName) {
    ASSERT_EQ(cvn_a.get_name(), Model::CARAVAN_A);
}

TEST_F(TableTest, GetSize_BeforeAfterNumeral) {
    ASSERT_EQ(cvn_a.get_size(), 0);
    cvn_a.put_numeral_card(c_s_a);
    ASSERT_EQ(cvn_a.get_size(), 1);
}

TEST_F(TableTest, GetSuit) {
    ASSERT_EQ(cvn_a.get_suit(), Model::NO_SUIT);
    cvn_a.put_numeral_card(c_s_a);
    ASSERT_EQ(cvn_a.get_suit(), c_s_a.suit);
}

TEST_F(TableTest, PutNumeralCard_PutFaceNotJack) {
    cvn_a.put_numeral_card(c_s_a);
    cvn_a.put_face_card(c_h_k, 1);

    Model::Slot slt = cvn_a.get_slot(1);

    ASSERT_EQ(slt.card.suit, c_s_a.suit);
    ASSERT_EQ(slt.card.rank, c_s_a.rank);

    ASSERT_EQ(slt.i_faces, 1);
    ASSERT_EQ(slt.faces[0].suit, c_h_k.suit);
    ASSERT_EQ(slt.faces[0].rank, c_h_k.rank);
}

TEST_F(TableTest, PutNumeralCard_PutFaceJack) {
    cvn_a.put_numeral_card(c_s_a);
    ASSERT_EQ(cvn_a.get_size(), 1);

    cvn_a.put_face_card(c_h_ja, 1);
    ASSERT_EQ(cvn_a.get_size(), 0);
}

TEST_F(TableTest, Error_PutNumeralCard_NotNumeral) {
    try {
        cvn_a.put_numeral_card(c_h_k);
        FAIL();
    } catch (CaravanIllegalModelException &) {
    } catch (...) {
        FAIL();
    }
}

TEST_F(TableTest, Error_PutNumeralCard_CaravanFull) {
    cvn_a.put_numeral_card(c_s_a);
    cvn_a.put_numeral_card(c_s_3);
    cvn_a.put_numeral_card(c_s_5);
    cvn_a.put_numeral_card(c_s_3);
    cvn_a.put_numeral_card(c_s_5);
    cvn_a.put_numeral_card(c_s_3);
    cvn_a.put_numeral_card(c_s_5);
    cvn_a.put_numeral_card(c_s_3);

    ASSERT_EQ(cvn_a.get_size(), Model::TRACK_NUMERIC_MAX);

    try {
        cvn_a.put_numeral_card(c_s_5);
        FAIL();
    } catch (CaravanIllegalModelException &) {
    } catch (...) {
        FAIL();
    }
}

TEST_F(TableTest, PutFaceCard) {
    cvn_a.put_numeral_card(c_s_a);
    cvn_a.put_face_card(c_h_k, 1);

    ASSERT_EQ(cvn_a.get_size(), 1);
    Model::Slot slt = cvn_a.get_slot(1);

    ASSERT_EQ(slt.i_faces, 1);
    ASSERT_EQ(slt.faces[0].suit, c_h_k.suit);
    ASSERT_EQ(slt.faces[0].rank, c_h_k.rank);
}

TEST_F(TableTest, Error_PutFaceCard_EmptyCaravan) {
    try {
        cvn_a.put_face_card(c_h_k, 1);
        FAIL();
    } catch (CaravanIllegalModelException &) {
    } catch (...) {
        FAIL();
    }
}

TEST_F(TableTest, Error_PutFaceCard_OutOfRange_Low) {
    cvn_a.put_numeral_card(c_s_a);

    try {
        cvn_a.put_face_card(c_h_k, 0);
        FAIL();
    } catch (CaravanIllegalModelException &) {
    } catch (...) {
        FAIL();
    }
}

TEST_F(TableTest, Error_PutFaceCard_OutOfRange_High) {
    cvn_a.put_numeral_card(c_s_a);

    try {
        cvn_a.put_face_card(c_h_k, 2);
        FAIL();
    } catch (CaravanIllegalModelException &) {
    } catch (...) {
        FAIL();
    }
}

TEST_F(TableTest, Error_PutFaceCard_NotFaceCard) {
    cvn_a.put_numeral_card(c_s_a);

    try {
        cvn_a.put_face_card(c_s_2, 1);
        FAIL();
    } catch (CaravanIllegalModelException &) {
    } catch (...) {
        FAIL();
    }
}

TEST_F(TableTest, Error_PutFaceCard_FullFaceCardCapacity) {
    cvn_a.put_numeral_card(c_s_a);
    cvn_a.put_face_card(c_h_k, 1);
    cvn_a.put_face_card(c_d_k, 1);
    cvn_a.put_face_card(c_c_k, 1);

    ASSERT_EQ(cvn_a.get_slot(1).faces.size(), Model::TRACK_FACE_MAX);

    try {
        cvn_a.put_face_card(c_s_k, 1);
        FAIL();
    } catch (CaravanIllegalModelException &) {
    } catch (...) {
        FAIL();
    }
}

TEST_F(TableTest, RemoveRank_FiveNumeral_OneFace_ExcludeNone) {
    // Add five numerals, two of which are ACE cards
    cvn_a.put_numeral_card(c_s_a);
    cvn_a.put_numeral_card(c_h_2);
    cvn_a.put_numeral_card(c_c_5);
    cvn_a.put_numeral_card(c_c_2);
    cvn_a.put_numeral_card(c_d_a);
    cvn_a.put_face_card(c_h_k, 4);
    ASSERT_EQ(cvn_a.get_size(), 5);

    // Remove all ACE cards, excluding none
    cvn_a.remove_rank(Model::ACE, 0);
    ASSERT_EQ(cvn_a.get_size(), 3);

    ASSERT_EQ(cvn_a.get_slot(1).card.suit, c_h_2.suit);
    ASSERT_EQ(cvn_a.get_slot(1).card.rank, c_h_2.rank);

    ASSERT_EQ(cvn_a.get_slot(2).card.suit, c_c_5.suit);
    ASSERT_EQ(cvn_a.get_slot(2).card.rank, c_c_5.rank);

    ASSERT_EQ(cvn_a.get_slot(3).card.suit, c_c_2.suit);
    ASSERT_EQ(cvn_a.get_slot(3).card.rank, c_c_2.rank);
}

TEST_F(TableTest, RemoveRank_FiveNumeral_OneFace_ExcludeOne) {
    // Add five numerals, two of which are ACE cards
    cvn_a.put_numeral_card(c_s_a);
    cvn_a.put_numeral_card(c_h_2);
    cvn_a.put_numeral_card(c_c_5);
    cvn_a.put_numeral_card(c_c_2);
    cvn_a.put_numeral_card(c_d_a);
    cvn_a.put_face_card(c_h_k, 4);
    ASSERT_EQ(cvn_a.get_size(), 5);

    // Remove all ACE cards, excluding the last one placed
    // This should remove the first numeral card placed only
    cvn_a.remove_rank(Model::ACE, 5);
    ASSERT_EQ(cvn_a.get_size(), 4);

    ASSERT_EQ(cvn_a.get_slot(1).card.suit, c_h_2.suit);
    ASSERT_EQ(cvn_a.get_slot(1).card.rank, c_h_2.rank);

    ASSERT_EQ(cvn_a.get_slot(2).card.suit, c_c_5.suit);
    ASSERT_EQ(cvn_a.get_slot(2).card.rank, c_c_5.rank);

    ASSERT_EQ(cvn_a.get_slot(3).card.suit, c_c_2.suit);
    ASSERT_EQ(cvn_a.get_slot(3).card.rank, c_c_2.rank);

    ASSERT_EQ(cvn_a.get_slot(4).card.suit, c_d_a.suit);
    ASSERT_EQ(cvn_a.get_slot(4).card.rank, c_d_a.rank);
}

TEST_F(TableTest, Error_RemoveRank_ExcludeOutOfRange) {
    cvn_a.put_numeral_card(c_s_a);
    cvn_a.put_numeral_card(c_h_2);
    cvn_a.put_numeral_card(c_c_5);
    cvn_a.put_numeral_card(c_c_2);
    cvn_a.put_numeral_card(c_d_a);
    cvn_a.put_face_card(c_h_k, 4);

    try {
        cvn_a.remove_rank(Model::ACE, 7);
        FAIL();
    } catch (CaravanFatalModelException &) {
    } catch (...) {
        FAIL();
    }
}

TEST_F(TableTest, RemoveSuit_FiveNumeral_OneFace_ExcludeNone) {
    cvn_a.put_numeral_card(c_s_a);
    cvn_a.put_numeral_card(c_h_2);
    cvn_a.put_numeral_card(c_c_5);
    cvn_a.put_numeral_card(c_c_2);
    cvn_a.put_numeral_card(c_d_a);
    cvn_a.put_face_card(c_h_k, 4);
    ASSERT_EQ(cvn_a.get_size(), 5);

    // Remove all cards of CLUBS suit, excluding none
    cvn_a.remove_suit(Model::CLUBS, 0);
    ASSERT_EQ(cvn_a.get_size(), 3);

    ASSERT_EQ(cvn_a.get_slot(1).card.suit, c_s_a.suit);
    ASSERT_EQ(cvn_a.get_slot(1).card.rank, c_s_a.rank);

    ASSERT_EQ(cvn_a.get_slot(2).card.suit, c_h_2.suit);
    ASSERT_EQ(cvn_a.get_slot(2).card.rank, c_h_2.rank);

    ASSERT_EQ(cvn_a.get_slot(3).card.suit, c_d_a.suit);
    ASSERT_EQ(cvn_a.get_slot(3).card.rank, c_d_a.rank);
}

TEST_F(TableTest, RemoveSuit_FiveNumeral_OneFace_ExcludeOne) {
    cvn_a.put_numeral_card(c_s_a);
    cvn_a.put_numeral_card(c_h_2);
    cvn_a.put_numeral_card(c_c_5);
    cvn_a.put_numeral_card(c_c_2);
    cvn_a.put_numeral_card(c_d_a);
    cvn_a.put_face_card(c_h_k, 4);
    ASSERT_EQ(cvn_a.get_size(), 5);

    // Remove all cards of CLUBS suit, excluding none
    cvn_a.remove_suit(Model::CLUBS, 3);
    ASSERT_EQ(cvn_a.get_size(), 4);

    ASSERT_EQ(cvn_a.get_slot(1).card.suit, c_s_a.suit);
    ASSERT_EQ(cvn_a.get_slot(1).card.rank, c_s_a.rank);

    ASSERT_EQ(cvn_a.get_slot(2).card.suit, c_h_2.suit);
    ASSERT_EQ(cvn_a.get_slot(2).card.rank, c_h_2.rank);

    ASSERT_EQ(cvn_a.get_slot(3).card.suit, c_c_5.suit);
    ASSERT_EQ(cvn_a.get_slot(3).card.rank, c_c_5.rank);

    ASSERT_EQ(cvn_a.get_slot(4).card.suit, c_d_a.suit);
    ASSERT_EQ(cvn_a.get_slot(4).card.rank, c_d_a.rank);
}

TEST_F(TableTest, Error_RemoveSuit_ExcludeOutOfRange) {
    cvn_a.put_numeral_card(c_s_a);
    cvn_a.put_numeral_card(c_h_2);
    cvn_a.put_numeral_card(c_c_5);
    cvn_a.put_numeral_card(c_c_2);
    cvn_a.put_numeral_card(c_d_a);
    cvn_a.put_face_card(c_h_k, 4);

    try {
        cvn_a.remove_suit(Model::CLUBS, 7);
        FAIL();
    } catch (CaravanFatalModelException &) {
    } catch (...) {
        FAIL();
    }
}

TEST_F(TableTest, RemoveNumeralCard_WithJack_Position8) {
    cvn_a.put_numeral_card(c_s_a);
    cvn_a.put_numeral_card(c_s_2);
    cvn_a.put_numeral_card(c_s_3);
    cvn_a.put_numeral_card(c_s_4);
    cvn_a.put_numeral_card(c_s_5);
    cvn_a.put_numeral_card(c_s_6);
    cvn_a.put_numeral_card(c_s_7);
    cvn_a.put_numeral_card(c_s_8);

    ASSERT_EQ(cvn_a.get_size(), 8);

    cvn_a.put_face_card(c_s_ja, 8);

    ASSERT_EQ(cvn_a.get_size(), 7);

    ASSERT_EQ(cvn_a.get_slot(1).card.suit, c_s_a.suit);
    ASSERT_EQ(cvn_a.get_slot(1).card.rank, c_s_a.rank);

    ASSERT_EQ(cvn_a.get_slot(7).card.suit, c_s_7.suit);
    ASSERT_EQ(cvn_a.get_slot(7).card.rank, c_s_7.rank);

    try {
        Model::Slot slt = cvn_a.get_slot(8);
        FAIL();
    } catch (CaravanIllegalModelException &) {
    } catch (...) {
        FAIL();
    }
}


TEST_F(TableTest, RemoveNumeralCard_WithJack_Position1) {
    cvn_a.put_numeral_card(c_s_a);
    cvn_a.put_numeral_card(c_s_2);
    cvn_a.put_numeral_card(c_s_3);
    cvn_a.put_numeral_card(c_s_4);
    cvn_a.put_numeral_card(c_s_5);
    cvn_a.put_numeral_card(c_s_6);
    cvn_a.put_numeral_card(c_s_7);
    cvn_a.put_numeral_card(c_s_8);

    ASSERT_EQ(cvn_a.get_size(), 8);

    cvn_a.put_face_card(c_s_ja, 1);

    ASSERT_EQ(cvn_a.get_size(), 7);

    ASSERT_EQ(cvn_a.get_slot(1).card.suit, c_s_2.suit);
    ASSERT_EQ(cvn_a.get_slot(1).card.rank, c_s_2.rank);

    ASSERT_EQ(cvn_a.get_slot(7).card.suit, c_s_8.suit);
    ASSERT_EQ(cvn_a.get_slot(7).card.rank, c_s_8.rank);

    try {
        Model::Slot slt = cvn_a.get_slot(8);
        FAIL();
    } catch (CaravanIllegalModelException &) {
    } catch (...) {
        FAIL();
    }
}

TEST_F(TableTest, RemoveNumeralCard_WithJack_Position5) {
    cvn_a.put_numeral_card(c_s_a);
    cvn_a.put_numeral_card(c_s_2);
    cvn_a.put_numeral_card(c_s_3);
    cvn_a.put_numeral_card(c_s_4);
    cvn_a.put_numeral_card(c_s_5);
    cvn_a.put_numeral_card(c_s_6);
    cvn_a.put_numeral_card(c_s_7);
    cvn_a.put_numeral_card(c_s_8);

    ASSERT_EQ(cvn_a.get_size(), 8);

    cvn_a.put_face_card(c_s_ja, 5);

    ASSERT_EQ(cvn_a.get_size(), 7);

    ASSERT_EQ(cvn_a.get_slot(1).card.suit, c_s_a.suit);
    ASSERT_EQ(cvn_a.get_slot(1).card.rank, c_s_a.rank);

    ASSERT_EQ(cvn_a.get_slot(5).card.suit, c_s_6.suit);
    ASSERT_EQ(cvn_a.get_slot(5).card.rank, c_s_6.rank);

    ASSERT_EQ(cvn_a.get_slot(7).card.suit, c_s_8.suit);
    ASSERT_EQ(cvn_a.get_slot(7).card.rank, c_s_8.rank);

    try {
        Model::Slot slt = cvn_a.get_slot(8);
        FAIL();
    } catch (CaravanIllegalModelException &) {
    } catch (...) {
        FAIL();
    }
}
