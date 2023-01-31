// Copyright (c) 2022-2023 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "gtest/gtest.h"
#include "../include/table.h"
#include "../include/exceptions.h"


TEST (TestGameTable, ClearCaravan_TwoNumeric_OneFace) {
    Table t;
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {SPADES, TWO};
    Card c_face = {HEARTS, KING};
    CaravanName pn = CARAVAN_D;

    t.play_numeral_card(pn, c_num_1);
    t.play_numeral_card(pn, c_num_2);
    t.play_face_card(pn, c_face, 2);
    ASSERT_EQ(t.get_caravan_size(pn), 2);

    t.clear_caravan(pn);
    ASSERT_EQ(t.get_caravan_size(pn), 0);
}


TEST (TestGameTable, GetCaravanBid_ThreeNumeric) {
    Table t;
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {SPADES, TWO};
    Card c_num_3 = {SPADES, THREE};
    CaravanName pn = CARAVAN_E;

    ASSERT_EQ(t.get_caravan_bid(pn), 0);
    t.play_numeral_card(pn, c_num_1);
    ASSERT_EQ(t.get_caravan_bid(pn), 1);
    t.play_numeral_card(pn, c_num_2);
    ASSERT_EQ(t.get_caravan_bid(pn), 3);
    t.play_numeral_card(pn, c_num_3);
    ASSERT_EQ(t.get_caravan_bid(pn), 6);
}

TEST (TestGameTable, GetCaravanCardsAt_ThreeNumeric) {
    Table t;
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {CLUBS, TWO};
    Card c_num_3 = {HEARTS, THREE};
    CaravanName cn = CARAVAN_F;
    Slot ts;

    t.play_numeral_card(cn, c_num_1);
    t.play_numeral_card(cn, c_num_2);
    t.play_numeral_card(cn, c_num_3);

    ts = t.get_slot_at(cn, 1);
    ASSERT_EQ(ts.card.suit, SPADES);
    ASSERT_EQ(ts.card.rank, ACE);

    ts = t.get_slot_at(cn, 2);
    ASSERT_EQ(ts.card.suit, CLUBS);
    ASSERT_EQ(ts.card.rank, TWO);

    ts = t.get_slot_at(cn, 3);
    ASSERT_EQ(ts.card.suit, HEARTS);
    ASSERT_EQ(ts.card.rank, THREE);
}

TEST (TestGameTable, GetCaravanDirection_Ascending) {
    Table t;
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {SPADES, TWO};
    CaravanName pn = CARAVAN_A;

    ASSERT_EQ(t.get_caravan_direction(pn), ANY);
    t.play_numeral_card(pn, c_num_1);
    ASSERT_EQ(t.get_caravan_direction(pn), ANY);
    t.play_numeral_card(pn, c_num_2);
    ASSERT_EQ(t.get_caravan_direction(pn), ASCENDING);
}

TEST (TestGameTable, GetCaravanDirection_Descending) {
    Table t;
    Card c_num_1 = {SPADES, TWO};
    Card c_num_2 = {SPADES, ACE};
    CaravanName pn = CARAVAN_B;

    ASSERT_EQ(t.get_caravan_direction(pn), ANY);
    t.play_numeral_card(pn, c_num_1);
    ASSERT_EQ(t.get_caravan_direction(pn), ANY);
    t.play_numeral_card(pn, c_num_2);
    ASSERT_EQ(t.get_caravan_direction(pn), DESCENDING);
}

TEST (TestGameTable, GetCaravanSize_ThreeNumeric) {
    Table t;
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {CLUBS, TWO};
    Card c_num_3 = {HEARTS, THREE};
    CaravanName cn = CARAVAN_C;

    ASSERT_EQ(t.get_caravan_size(cn), 0);

    t.play_numeral_card(cn, c_num_1);
    t.play_numeral_card(cn, c_num_2);
    t.play_numeral_card(cn, c_num_3);

    ASSERT_EQ(t.get_caravan_size(cn), 3);
}

TEST (TestGameTable, GetCaravanSuit_BeforeAfter) {
    Table t;
    Card c_num = {SPADES, ACE};
    CaravanName pn = CARAVAN_D;

    ASSERT_EQ(t.get_caravan_suit(pn), NO_SUIT);
    t.play_numeral_card(pn, c_num);
    ASSERT_EQ(t.get_caravan_suit(pn), SPADES);
}

TEST (TestGameTable, PlayFaceCard_Jack) {
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

    ASSERT_EQ(t.get_caravan_size(cn), 3);
    ASSERT_EQ(t.get_caravan_suit(cn), HEARTS);

    t.play_face_card(cn, c_face, 2);

    ASSERT_EQ(t.get_caravan_size(cn), 2);
    ASSERT_EQ(t.get_caravan_suit(cn), HEARTS);
    ASSERT_EQ(t.get_caravan_direction(cn), ASCENDING);

    ts = t.get_slot_at(cn, 1);
    ASSERT_EQ(ts.card.suit, SPADES);
    ASSERT_EQ(ts.card.rank, ACE);

    ts = t.get_slot_at(cn, 2);
    ASSERT_EQ(ts.card.suit, HEARTS);
    ASSERT_EQ(ts.card.rank, THREE);
}

TEST (TestGameTable, PlayFaceCard_Queen) {
    Table t;
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {CLUBS, TWO};
    Card c_num_3 = {HEARTS, THREE};
    Card c_face = {DIAMONDS, QUEEN};
    CaravanName cn = CARAVAN_D;

    t.play_numeral_card(cn, c_num_1);
    t.play_numeral_card(cn, c_num_2);
    t.play_numeral_card(cn, c_num_3);

    ASSERT_EQ(t.get_caravan_size(cn), 3);
    ASSERT_EQ(t.get_caravan_suit(cn), HEARTS);
    ASSERT_EQ(t.get_caravan_direction(cn), ASCENDING);

    t.play_face_card(cn, c_face, 3);

    ASSERT_EQ(t.get_caravan_size(cn), 3);
    ASSERT_EQ(t.get_caravan_suit(cn), DIAMONDS);
    ASSERT_EQ(t.get_caravan_direction(cn), DESCENDING);
}

TEST (TestGameTable, PlayFaceCard_Error_Queen_NotPlayedOnTopCard) {
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

TEST (TestGameTable, PlayFaceCard_King_OneNumeric_ThreeKings) {
    Table t;
    Card c_num = {SPADES, FIVE};
    Card c_face_1 = {DIAMONDS, KING};
    Card c_face_2 = {CLUBS, KING};
    Card c_face_3 = {HEARTS, KING};
    CaravanName cn = CARAVAN_D;

    t.play_numeral_card(cn, c_num);
    ASSERT_EQ(t.get_caravan_bid(cn), 5);

    t.play_face_card(cn, c_face_1, 1);
    ASSERT_EQ(t.get_caravan_bid(cn), 10);

    t.play_face_card(cn, c_face_2, 1);
    ASSERT_EQ(t.get_caravan_bid(cn), 20);

    t.play_face_card(cn, c_face_3, 1);
    ASSERT_EQ(t.get_caravan_bid(cn), 40);
}

TEST (TestGameTable, PlayFaceCard_Joker_Ace) {
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

    ASSERT_EQ(t.get_caravan_size(CARAVAN_D), 3);
    ASSERT_EQ(t.get_caravan_size(CARAVAN_E), 1);
    ASSERT_EQ(t.get_caravan_size(CARAVAN_F), 1);

    t.play_face_card(CARAVAN_D, c_face, 1);

    ASSERT_EQ(t.get_caravan_size(CARAVAN_D), 2);
    ASSERT_EQ(t.get_caravan_size(CARAVAN_E), 0);
    ASSERT_EQ(t.get_caravan_size(CARAVAN_F), 1);

    ts = t.get_slot_at(CARAVAN_D, 1);
    ASSERT_EQ(ts.card.suit, SPADES);
    ASSERT_EQ(ts.card.rank, ACE);
}

TEST (TestGameTable, PlayFaceCard_Joker_2To10) {
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

    ASSERT_EQ(t.get_caravan_size(CARAVAN_D), 3);
    ASSERT_EQ(t.get_caravan_size(CARAVAN_E), 1);
    ASSERT_EQ(t.get_caravan_size(CARAVAN_F), 1);

    t.play_face_card(CARAVAN_D, c_face, 1);

    ASSERT_EQ(t.get_caravan_size(CARAVAN_D), 2);
    ASSERT_EQ(t.get_caravan_size(CARAVAN_E), 1);
    ASSERT_EQ(t.get_caravan_size(CARAVAN_F), 0);

    ts = t.get_slot_at(CARAVAN_D, 1);
    ASSERT_EQ(ts.card.suit, SPADES);
    ASSERT_EQ(ts.card.rank, TWO);
}

TEST (TestGameTable, PlayNumericCard) {
    Table t;
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {HEARTS, THREE};
    CaravanName cn = CARAVAN_D;

    ASSERT_EQ(t.get_caravan_size(cn), 0);
    ASSERT_EQ(t.get_caravan_suit(cn), NO_SUIT);
    ASSERT_EQ(t.get_caravan_direction(cn), ANY);

    t.play_numeral_card(cn, c_num_1);

    ASSERT_EQ(t.get_caravan_size(cn), 1);
    ASSERT_EQ(t.get_caravan_suit(cn), SPADES);
    ASSERT_EQ(t.get_caravan_direction(cn), ANY);

    t.play_numeral_card(cn, c_num_2);

    ASSERT_EQ(t.get_caravan_size(cn), 2);
    ASSERT_EQ(t.get_caravan_suit(cn), HEARTS);
    ASSERT_EQ(t.get_caravan_direction(cn), ASCENDING);
}

TEST (TestGameTable, PlayNumericCard_Error_TwoCards_SameRank_InSequence) {
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

TEST (TestGameTable, PlayNumericCard_Error_OppositeDirection_DifferentSuit) {
    Table t;
    Card c_num_1 = {SPADES, FIVE};
    Card c_num_2 = {DIAMONDS, SEVEN};
    Card c_num_3 = {CLUBS, TWO};
    CaravanName cn = CARAVAN_D;

    t.play_numeral_card(cn, c_num_1);
    t.play_numeral_card(cn, c_num_2);
    ASSERT_EQ(t.get_caravan_direction(cn), ASCENDING);


    try {
        t.play_numeral_card(cn, c_num_3);
        FAIL();

    } catch (CaravanGameException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestGameTable, PlayNumericCard_OppositeDirection_SameSuit) {
    Table t;
    Card c_num_1 = {SPADES, FIVE};
    Card c_num_2 = {DIAMONDS, SEVEN};
    Card c_num_3 = {DIAMONDS, TWO};
    CaravanName cn = CARAVAN_D;

    t.play_numeral_card(cn, c_num_1);
    t.play_numeral_card(cn, c_num_2);

    ASSERT_EQ(t.get_caravan_suit(cn), DIAMONDS);
    ASSERT_EQ(t.get_caravan_direction(cn), ASCENDING);

    t.play_numeral_card(cn, c_num_3);

    ASSERT_EQ(t.get_caravan_suit(cn), DIAMONDS);
    ASSERT_EQ(t.get_caravan_direction(cn), DESCENDING);
}
