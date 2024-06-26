// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "gtest/gtest.h"
#include "caravan/model/table.h"
#include "caravan/core/exceptions.h"


TEST (TestTable, ClearCaravan_TwoNumeric_OneFace) {
    Table t;
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {SPADES, TWO};
    Card c_face = {HEARTS, KING};
    CaravanName pn = CARAVAN_D;

    t.play_numeral_card(pn, c_num_1);
    t.play_numeral_card(pn, c_num_2);
    t.play_face_card(pn, c_face, 2);
    ASSERT_EQ(t.get_caravan(pn)->get_size(), 2);

    t.clear_caravan(pn);
    ASSERT_EQ(t.get_caravan(pn)->get_size(), 0);
}


TEST (TestTable, GetCaravanBid_ThreeNumeric) {
    Table t;
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {SPADES, TWO};
    Card c_num_3 = {SPADES, THREE};
    CaravanName pn = CARAVAN_E;

    ASSERT_EQ(t.get_caravan(pn)->get_bid(), 0);
    t.play_numeral_card(pn, c_num_1);
    ASSERT_EQ(t.get_caravan(pn)->get_bid(), 1);
    t.play_numeral_card(pn, c_num_2);
    ASSERT_EQ(t.get_caravan(pn)->get_bid(), 3);
    t.play_numeral_card(pn, c_num_3);
    ASSERT_EQ(t.get_caravan(pn)->get_bid(), 6);
}

TEST (TestTable, GetCaravanCardsAt_ThreeNumeric) {
    Table t;
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {CLUBS, TWO};
    Card c_num_3 = {HEARTS, THREE};
    CaravanName cn = CARAVAN_F;
    Slot ts;

    t.play_numeral_card(cn, c_num_1);
    t.play_numeral_card(cn, c_num_2);
    t.play_numeral_card(cn, c_num_3);

    ts = t.get_caravan(cn)->get_slot(1);
    ASSERT_EQ(ts.card.suit, SPADES);
    ASSERT_EQ(ts.card.rank, ACE);

    ts = t.get_caravan(cn)->get_slot(2);
    ASSERT_EQ(ts.card.suit, CLUBS);
    ASSERT_EQ(ts.card.rank, TWO);

    ts = t.get_caravan(cn)->get_slot(3);
    ASSERT_EQ(ts.card.suit, HEARTS);
    ASSERT_EQ(ts.card.rank, THREE);
}

TEST (TestTable, GetCaravanDirection_Ascending) {
    Table t;
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {SPADES, TWO};
    CaravanName pn = CARAVAN_A;

    ASSERT_EQ(t.get_caravan(pn)->get_direction(), ANY);
    t.play_numeral_card(pn, c_num_1);
    ASSERT_EQ(t.get_caravan(pn)->get_direction(), ANY);
    t.play_numeral_card(pn, c_num_2);
    ASSERT_EQ(t.get_caravan(pn)->get_direction(), ASCENDING);
}

TEST (TestTable, GetCaravanDirection_Descending) {
    Table t;
    Card c_num_1 = {SPADES, TWO};
    Card c_num_2 = {SPADES, ACE};
    CaravanName pn = CARAVAN_B;

    ASSERT_EQ(t.get_caravan(pn)->get_direction(), ANY);
    t.play_numeral_card(pn, c_num_1);
    ASSERT_EQ(t.get_caravan(pn)->get_direction(), ANY);
    t.play_numeral_card(pn, c_num_2);
    ASSERT_EQ(t.get_caravan(pn)->get_direction(), DESCENDING);
}

TEST (TestTable, GetCaravanSize_ThreeNumeric) {
    Table t;
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {CLUBS, TWO};
    Card c_num_3 = {HEARTS, THREE};
    CaravanName cn = CARAVAN_C;

    ASSERT_EQ(t.get_caravan(cn)->get_size(), 0);

    t.play_numeral_card(cn, c_num_1);
    t.play_numeral_card(cn, c_num_2);
    t.play_numeral_card(cn, c_num_3);

    ASSERT_EQ(t.get_caravan(cn)->get_size(), 3);
}

TEST (TestTable, GetCaravanSuit_BeforeAfter) {
    Table t;
    Card c_num = {SPADES, ACE};
    CaravanName pn = CARAVAN_D;

    ASSERT_EQ(t.get_caravan(pn)->get_suit(), NO_SUIT);
    t.play_numeral_card(pn, c_num);
    ASSERT_EQ(t.get_caravan(pn)->get_suit(), SPADES);
}

TEST (TestTable, PlayFaceCard_Jack) {
    Table t;
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {CLUBS, TWO};
    Card c_num_3 = {HEARTS, THREE};
    Card c_face = {DIAMONDS, JACK};
    CaravanName cn = CARAVAN_D;
    Slot ts;

    t.play_numeral_card(cn, c_num_1);
    t.play_numeral_card(cn, c_num_2);
    t.play_numeral_card(cn, c_num_3);

    ASSERT_EQ(t.get_caravan(cn)->get_size(), 3);
    ASSERT_EQ(t.get_caravan(cn)->get_suit(), HEARTS);

    t.play_face_card(cn, c_face, 2);

    ASSERT_EQ(t.get_caravan(cn)->get_size(), 2);
    ASSERT_EQ(t.get_caravan(cn)->get_suit(), HEARTS);
    ASSERT_EQ(t.get_caravan(cn)->get_direction(), ASCENDING);

    ts = t.get_caravan(cn)->get_slot(1);
    ASSERT_EQ(ts.card.suit, SPADES);
    ASSERT_EQ(ts.card.rank, ACE);

    ts = t.get_caravan(cn)->get_slot(2);
    ASSERT_EQ(ts.card.suit, HEARTS);
    ASSERT_EQ(ts.card.rank, THREE);
}

TEST (TestTable, PlayFaceCard_Queen) {
    Table t;
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {CLUBS, TWO};
    Card c_num_3 = {HEARTS, THREE};
    Card c_face = {DIAMONDS, QUEEN};
    CaravanName cn = CARAVAN_D;

    t.play_numeral_card(cn, c_num_1);
    t.play_numeral_card(cn, c_num_2);
    t.play_numeral_card(cn, c_num_3);

    ASSERT_EQ(t.get_caravan(cn)->get_size(), 3);
    ASSERT_EQ(t.get_caravan(cn)->get_suit(), HEARTS);
    ASSERT_EQ(t.get_caravan(cn)->get_direction(), ASCENDING);

    t.play_face_card(cn, c_face, 3);

    ASSERT_EQ(t.get_caravan(cn)->get_size(), 3);
    ASSERT_EQ(t.get_caravan(cn)->get_suit(), DIAMONDS);
    ASSERT_EQ(t.get_caravan(cn)->get_direction(), DESCENDING);
}

TEST (TestTable, PlayFaceCard_Error_Queen_NotPlayedOnTopCard) {
    Table t;
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {CLUBS, TWO};
    Card c_num_3 = {HEARTS, THREE};
    Card c_face = {DIAMONDS, QUEEN};
    CaravanName cn = CARAVAN_D;

    t.play_numeral_card(cn, c_num_1);
    t.play_numeral_card(cn, c_num_2);
    t.play_numeral_card(cn, c_num_3);

    try {
        t.play_face_card(cn, c_face, 2);
        FAIL();

    } catch (CaravanGameException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestTable, PlayFaceCard_King_OneNumeric_ThreeKings) {
    Table t;
    Card c_num = {SPADES, FIVE};
    Card c_face_1 = {DIAMONDS, KING};
    Card c_face_2 = {CLUBS, KING};
    Card c_face_3 = {HEARTS, KING};
    CaravanName cn = CARAVAN_D;

    t.play_numeral_card(cn, c_num);
    ASSERT_EQ(t.get_caravan(cn)->get_bid(), 5);

    t.play_face_card(cn, c_face_1, 1);
    ASSERT_EQ(t.get_caravan(cn)->get_bid(), 10);

    t.play_face_card(cn, c_face_2, 1);
    ASSERT_EQ(t.get_caravan(cn)->get_bid(), 20);

    t.play_face_card(cn, c_face_3, 1);
    ASSERT_EQ(t.get_caravan(cn)->get_bid(), 40);
}

TEST (TestTable, PlayFaceCard_Joker_Ace) {
    Table t;
    Card c_num_a1 = {SPADES, ACE};
    Card c_num_a2 = {HEARTS, THREE};
    Card c_num_a3 = {SPADES, SEVEN};
    Card c_num_b1 = {SPADES, FIVE};
    Card c_num_c1 = {DIAMONDS, TEN};
    Card c_face = {DIAMONDS, JOKER};
    Slot ts;

    t.play_numeral_card(CARAVAN_D, c_num_a1);
    t.play_numeral_card(CARAVAN_D, c_num_a2);
    t.play_numeral_card(CARAVAN_D, c_num_a3);
    t.play_numeral_card(CARAVAN_E, c_num_b1);
    t.play_numeral_card(CARAVAN_F, c_num_c1);

    ASSERT_EQ(t.get_caravan(CARAVAN_D)->get_size(), 3);
    ASSERT_EQ(t.get_caravan(CARAVAN_E)->get_size(), 1);
    ASSERT_EQ(t.get_caravan(CARAVAN_F)->get_size(), 1);

    t.play_face_card(CARAVAN_D, c_face, 1);

    ASSERT_EQ(t.get_caravan(CARAVAN_D)->get_size(), 2);
    ASSERT_EQ(t.get_caravan(CARAVAN_E)->get_size(), 0);
    ASSERT_EQ(t.get_caravan(CARAVAN_F)->get_size(), 1);

    ts = t.get_caravan(CARAVAN_D)->get_slot(1);
    ASSERT_EQ(ts.card.suit, SPADES);
    ASSERT_EQ(ts.card.rank, ACE);
}

TEST (TestTable, PlayFaceCard_Joker_2To10) {
    Table t;
    Card c_num_a1 = {SPADES, TWO};
    Card c_num_a2 = {HEARTS, THREE};
    Card c_num_a3 = {HEARTS, TWO};
    Card c_num_b1 = {SPADES, FIVE};
    Card c_num_c1 = {DIAMONDS, TWO};
    Card c_face = {DIAMONDS, JOKER};
    Slot ts;

    t.play_numeral_card(CARAVAN_D, c_num_a1);
    t.play_numeral_card(CARAVAN_D, c_num_a2);
    t.play_numeral_card(CARAVAN_D, c_num_a3);
    t.play_numeral_card(CARAVAN_E, c_num_b1);
    t.play_numeral_card(CARAVAN_F, c_num_c1);

    ASSERT_EQ(t.get_caravan(CARAVAN_D)->get_size(), 3);
    ASSERT_EQ(t.get_caravan(CARAVAN_E)->get_size(), 1);
    ASSERT_EQ(t.get_caravan(CARAVAN_F)->get_size(), 1);

    t.play_face_card(CARAVAN_D, c_face, 1);

    ASSERT_EQ(t.get_caravan(CARAVAN_D)->get_size(), 2);
    ASSERT_EQ(t.get_caravan(CARAVAN_E)->get_size(), 1);
    ASSERT_EQ(t.get_caravan(CARAVAN_F)->get_size(), 0);

    ts = t.get_caravan(CARAVAN_D)->get_slot(1);
    ASSERT_EQ(ts.card.suit, SPADES);
    ASSERT_EQ(ts.card.rank, TWO);
}

TEST (TestTable, PlayNumericCard) {
    Table t;
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {HEARTS, THREE};
    CaravanName cn = CARAVAN_D;

    ASSERT_EQ(t.get_caravan(cn)->get_size(), 0);
    ASSERT_EQ(t.get_caravan(cn)->get_suit(), NO_SUIT);
    ASSERT_EQ(t.get_caravan(cn)->get_direction(), ANY);

    t.play_numeral_card(cn, c_num_1);

    ASSERT_EQ(t.get_caravan(cn)->get_size(), 1);
    ASSERT_EQ(t.get_caravan(cn)->get_suit(), SPADES);
    ASSERT_EQ(t.get_caravan(cn)->get_direction(), ANY);

    t.play_numeral_card(cn, c_num_2);

    ASSERT_EQ(t.get_caravan(cn)->get_size(), 2);
    ASSERT_EQ(t.get_caravan(cn)->get_suit(), HEARTS);
    ASSERT_EQ(t.get_caravan(cn)->get_direction(), ASCENDING);
}

TEST (TestTable, PlayNumericCard_Error_TwoCards_SameRank_InSequence) {
    Table t;
    Card c_num_1 = {SPADES, THREE};
    Card c_num_2 = {DIAMONDS, THREE};
    CaravanName cn = CARAVAN_D;

    t.play_numeral_card(cn, c_num_1);

    try {
        t.play_numeral_card(cn, c_num_2);
        FAIL();

    } catch (CaravanGameException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestTable, PlayNumericCard_Error_OppositeDirection_DifferentSuit) {
    Table t;
    Card c_num_1 = {SPADES, FIVE};
    Card c_num_2 = {DIAMONDS, SEVEN};
    Card c_num_3 = {CLUBS, TWO};
    CaravanName cn = CARAVAN_D;

    t.play_numeral_card(cn, c_num_1);
    t.play_numeral_card(cn, c_num_2);
    ASSERT_EQ(t.get_caravan(cn)->get_direction(), ASCENDING);


    try {
        t.play_numeral_card(cn, c_num_3);
        FAIL();

    } catch (CaravanGameException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestTable, PlayNumericCard_OppositeDirection_SameSuit) {
    Table t;
    Card c_num_1 = {SPADES, FIVE};
    Card c_num_2 = {DIAMONDS, SEVEN};
    Card c_num_3 = {DIAMONDS, TWO};
    CaravanName cn = CARAVAN_D;

    t.play_numeral_card(cn, c_num_1);
    t.play_numeral_card(cn, c_num_2);

    ASSERT_EQ(t.get_caravan(cn)->get_suit(), DIAMONDS);
    ASSERT_EQ(t.get_caravan(cn)->get_direction(), ASCENDING);

    t.play_numeral_card(cn, c_num_3);

    ASSERT_EQ(t.get_caravan(cn)->get_suit(), DIAMONDS);
    ASSERT_EQ(t.get_caravan(cn)->get_direction(), DESCENDING);
}
