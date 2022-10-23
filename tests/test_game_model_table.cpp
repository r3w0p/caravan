// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "gtest/gtest.h"
#include "../src/game/model.h"


TEST (TestGameModelTable, ClearCaravan_TwoNumeric_OneFace) {
    Table t;
    Card c_num_1 = { SPADES, ACE };
    Card c_num_2 = { SPADES, TWO };
    Card c_face = { HEARTS, KING };
    CaravanName pn = CARAVAN_A;

    t.play_numeric_card(pn, c_num_1);
    t.play_numeric_card(pn, c_num_2);
    t.play_face_card(pn, c_face, 2);
    ASSERT_EQ(t.get_caravan_size(pn), 2);

    t.clear_caravan(pn);
    ASSERT_EQ(t.get_caravan_size(pn), 0);
}


TEST (TestGameModelTable, GetCaravanBid_ThreeNumeric) {
    Table t;
    Card c_num_1 = { SPADES, ACE };
    Card c_num_2 = { SPADES, TWO };
    Card c_num_3 = { SPADES, THREE };
    CaravanName pn = CARAVAN_A;

    ASSERT_EQ(t.get_caravan_bid(pn), 0);
    t.play_numeric_card(pn, c_num_1);
    ASSERT_EQ(t.get_caravan_bid(pn), 1);
    t.play_numeric_card(pn, c_num_2);
    ASSERT_EQ(t.get_caravan_bid(pn), 3);
    t.play_numeric_card(pn, c_num_3);
    ASSERT_EQ(t.get_caravan_bid(pn), 6);
}

TEST (TestGameModelTable, GetCaravanCardsAt_ThreeNumeric) {
    Table t;
    Card c_num_1 = { SPADES, ACE };
    Card c_num_2 = { CLUBS, TWO };
    Card c_num_3 = { HEARTS, THREE };
    CaravanName cn = CARAVAN_A;
    TrackSlot ts;

    t.play_numeric_card(cn, c_num_1);
    t.play_numeric_card(cn, c_num_2);
    t.play_numeric_card(cn, c_num_3);

    ts = t.get_caravan_cards_at(cn, 1);
    ASSERT_EQ(ts.card.suit, SPADES);
    ASSERT_EQ(ts.card.rank, ACE);

    ts = t.get_caravan_cards_at(cn, 2);
    ASSERT_EQ(ts.card.suit, CLUBS);
    ASSERT_EQ(ts.card.rank, TWO);

    ts = t.get_caravan_cards_at(cn, 3);
    ASSERT_EQ(ts.card.suit, HEARTS);
    ASSERT_EQ(ts.card.rank, THREE);
}

TEST (TestGameModelTable, GetCaravanDirection_BeforeAfter_Ascending) {
    Table t;
    Card c_num_1 = { SPADES, ACE };
    Card c_num_2 = { SPADES, TWO };
    CaravanName pn = CARAVAN_A;

    ASSERT_EQ(t.get_caravan_direction(pn), NO_DIRECTION);
    t.play_numeric_card(pn, c_num_1);
    ASSERT_EQ(t.get_caravan_direction(pn), NO_DIRECTION);
    t.play_numeric_card(pn, c_num_2);
    ASSERT_EQ(t.get_caravan_direction(pn), ASCENDING);
}

TEST (TestGameModelTable, GetCaravanDirection_BeforeAfter_Descending) {
    Table t;
    Card c_num_1 = { SPADES, TWO };
    Card c_num_2 = { SPADES, ACE };
    CaravanName pn = CARAVAN_A;

    ASSERT_EQ(t.get_caravan_direction(pn), NO_DIRECTION);
    t.play_numeric_card(pn, c_num_1);
    ASSERT_EQ(t.get_caravan_direction(pn), NO_DIRECTION);
    t.play_numeric_card(pn, c_num_2);
    ASSERT_EQ(t.get_caravan_direction(pn), DESCENDING);
}

TEST (TestGameModelTable, GetCaravanSize_ThreeNumeric_BeforeAfter) {
    Table t;
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {CLUBS, TWO};
    Card c_num_3 = {HEARTS, THREE};
    CaravanName cn = CARAVAN_A;

    ASSERT_EQ(t.get_caravan_size(cn), 0);

    t.play_numeric_card(cn, c_num_1);
    t.play_numeric_card(cn, c_num_2);
    t.play_numeric_card(cn, c_num_3);

    ASSERT_EQ(t.get_caravan_size(cn), 3);
}

TEST (TestGameModelTable, GetCaravanSuit_BeforeAfter) {
    Table t;
    Card c_num = { SPADES, ACE };
    CaravanName pn = CARAVAN_A;

    ASSERT_EQ(t.get_caravan_suit(pn), NO_SUIT);
    t.play_numeric_card(pn, c_num);
    ASSERT_EQ(t.get_caravan_suit(pn), SPADES);
}

TEST (TestGameModelTable, PlayFaceCard_Jack_BeforeAfter) {
    Table t;
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {CLUBS, TWO};
    Card c_num_3 = {HEARTS, THREE};
    Card c_face = { DIAMONDS, JACK };
    CaravanName cn = CARAVAN_A;
    TrackSlot ts;

    t.play_numeric_card(cn, c_num_1);
    t.play_numeric_card(cn, c_num_2);
    t.play_numeric_card(cn, c_num_3);

    ASSERT_EQ(t.get_caravan_size(cn), 3);
    ASSERT_EQ(t.get_caravan_suit(cn), HEARTS);

    t.play_face_card(cn, c_face, 2);

    ASSERT_EQ(t.get_caravan_size(cn), 2);
    ASSERT_EQ(t.get_caravan_suit(cn), HEARTS);
    ASSERT_EQ(t.get_caravan_direction(cn), ASCENDING);

    ts = t.get_caravan_cards_at(cn, 1);
    ASSERT_EQ(ts.card.suit, SPADES);
    ASSERT_EQ(ts.card.rank, ACE);

    ts = t.get_caravan_cards_at(cn, 2);
    ASSERT_EQ(ts.card.suit, HEARTS);
    ASSERT_EQ(ts.card.rank, THREE);
}

TEST (TestGameModelTable, PlayFaceCard_Queen_BeforeAfter) {
    Table t;
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {CLUBS, TWO};
    Card c_num_3 = {HEARTS, THREE};
    Card c_face = {DIAMONDS, QUEEN};
    CaravanName cn = CARAVAN_A;

    t.play_numeric_card(cn, c_num_1);
    t.play_numeric_card(cn, c_num_2);
    t.play_numeric_card(cn, c_num_3);

    ASSERT_EQ(t.get_caravan_size(cn), 3);
    ASSERT_EQ(t.get_caravan_suit(cn), HEARTS);
    ASSERT_EQ(t.get_caravan_direction(cn), ASCENDING);

    t.play_face_card(cn, c_face, 3);

    ASSERT_EQ(t.get_caravan_size(cn), 3);
    ASSERT_EQ(t.get_caravan_suit(cn), DIAMONDS);
    ASSERT_EQ(t.get_caravan_direction(cn), DESCENDING);
}

TEST (TestGameModelTable, PlayFaceCard_Error_Queen_NotPlayedOntoTopCard) {
    Table t;
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {CLUBS, TWO};
    Card c_num_3 = {HEARTS, THREE};
    Card c_face = {DIAMONDS, QUEEN};
    CaravanName cn = CARAVAN_A;

    t.play_numeric_card(cn, c_num_1);
    t.play_numeric_card(cn, c_num_2);
    t.play_numeric_card(cn, c_num_3);

    try {
        t.play_face_card(cn, c_face, 2);
        FAIL();

    } catch(CaravanGameModelException & e) {

    } catch(...) {
        FAIL();
    }
}

TEST (TestGameModelTable, PlayFaceCard_King_OneNumeric_ThreeKings_BeforeAfter) {
    Table t;
    Card c_num = {SPADES, FIVE};
    Card c_face_1 = {DIAMONDS, KING};
    Card c_face_2 = {CLUBS, KING};
    Card c_face_3 = {HEARTS, KING};
    CaravanName cn = CARAVAN_A;

    t.play_numeric_card(cn, c_num);
    ASSERT_EQ(t.get_caravan_bid(cn), 5);

    t.play_face_card(cn, c_face_1, 1);
    ASSERT_EQ(t.get_caravan_bid(cn), 10);

    t.play_face_card(cn, c_face_2, 1);
    ASSERT_EQ(t.get_caravan_bid(cn), 20);

    t.play_face_card(cn, c_face_3, 1);
    ASSERT_EQ(t.get_caravan_bid(cn), 40);
}

TEST (TestGameModelTable, PlayFaceCard_Joker_Ace) {
    Table t;
    Card c_num_a1 = {SPADES, ACE};
    Card c_num_a2 = {HEARTS, THREE};
    Card c_num_a3 = {SPADES, SEVEN};
    Card c_num_b1 = {SPADES, FIVE};
    Card c_num_c1 = {DIAMONDS, TEN};
    Card c_face = {DIAMONDS, JOKER};
    TrackSlot ts;

    t.play_numeric_card(CARAVAN_A, c_num_a1);
    t.play_numeric_card(CARAVAN_A, c_num_a2);
    t.play_numeric_card(CARAVAN_A, c_num_a3);
    t.play_numeric_card(CARAVAN_B, c_num_b1);
    t.play_numeric_card(CARAVAN_C, c_num_c1);

    ASSERT_EQ(t.get_caravan_size(CARAVAN_A), 3);
    ASSERT_EQ(t.get_caravan_size(CARAVAN_B), 1);
    ASSERT_EQ(t.get_caravan_size(CARAVAN_C), 1);

    t.play_face_card(CARAVAN_A, c_face, 1);

    ASSERT_EQ(t.get_caravan_size(CARAVAN_A), 2);
    ASSERT_EQ(t.get_caravan_size(CARAVAN_B), 0);
    ASSERT_EQ(t.get_caravan_size(CARAVAN_C), 1);

    ts = t.get_caravan_cards_at(CARAVAN_A, 1);
    ASSERT_EQ(ts.card.suit, SPADES);
    ASSERT_EQ(ts.card.rank, ACE);
}

TEST (TestGameModelTable, PlayFaceCard_Joker_2To10) {
    Table t;
    Card c_num_a1 = {SPADES, TWO};
    Card c_num_a2 = {HEARTS, THREE};
    Card c_num_a3 = {HEARTS, TWO};
    Card c_num_b1 = {SPADES, FIVE};
    Card c_num_c1 = {DIAMONDS, TWO};
    Card c_face = {DIAMONDS, JOKER};
    TrackSlot ts;

    t.play_numeric_card(CARAVAN_A, c_num_a1);
    t.play_numeric_card(CARAVAN_A, c_num_a2);
    t.play_numeric_card(CARAVAN_A, c_num_a3);
    t.play_numeric_card(CARAVAN_B, c_num_b1);
    t.play_numeric_card(CARAVAN_C, c_num_c1);

    ASSERT_EQ(t.get_caravan_size(CARAVAN_A), 3);
    ASSERT_EQ(t.get_caravan_size(CARAVAN_B), 1);
    ASSERT_EQ(t.get_caravan_size(CARAVAN_C), 1);

    t.play_face_card(CARAVAN_A, c_face, 1);

    ASSERT_EQ(t.get_caravan_size(CARAVAN_A), 2);
    ASSERT_EQ(t.get_caravan_size(CARAVAN_B), 1);
    ASSERT_EQ(t.get_caravan_size(CARAVAN_C), 0);

    ts = t.get_caravan_cards_at(CARAVAN_A, 1);
    ASSERT_EQ(ts.card.suit, SPADES);
    ASSERT_EQ(ts.card.rank, TWO);
}

TEST (TestGameModelTable, PlayNumericCard_BeforeAfter) {
    Table t;
    Card c_num_1 = { SPADES, ACE };
    Card c_num_2 = { HEARTS, THREE };
    CaravanName cn = CARAVAN_A;

    ASSERT_EQ(t.get_caravan_size(cn), 0);
    ASSERT_EQ(t.get_caravan_suit(cn), NO_SUIT);
    ASSERT_EQ(t.get_caravan_direction(cn), NO_DIRECTION);

    t.play_numeric_card(cn, c_num_1);

    ASSERT_EQ(t.get_caravan_size(cn), 1);
    ASSERT_EQ(t.get_caravan_suit(cn), SPADES);
    ASSERT_EQ(t.get_caravan_direction(cn), NO_DIRECTION);

    t.play_numeric_card(cn, c_num_2);

    ASSERT_EQ(t.get_caravan_size(cn), 2);
    ASSERT_EQ(t.get_caravan_suit(cn), HEARTS);
    ASSERT_EQ(t.get_caravan_direction(cn), ASCENDING);
}

TEST (TestGameModelTable, PlayNumericCard_Error_TwoCardsWithSameRankInSequence) {
    Table t;
    Card c_num_1 = {SPADES, THREE};
    Card c_num_2 = {DIAMONDS, THREE};
    CaravanName cn = CARAVAN_A;

    t.play_numeric_card(cn, c_num_1);

    try {
        t.play_numeric_card(cn, c_num_2);
        FAIL();

    } catch(CaravanGameModelException & e) {

    } catch(...) {
        FAIL();
    }
}

TEST (TestGameModelTable, PlayNumericCard_Error_CaravanAscending_ThirdCardDescendingDifferentSuit) {
    Table t;
    Card c_num_1 = {SPADES, FIVE};
    Card c_num_2 = {DIAMONDS, SEVEN};
    Card c_num_3 = {CLUBS, TWO};
    CaravanName cn = CARAVAN_A;

    t.play_numeric_card(cn, c_num_1);
    t.play_numeric_card(cn, c_num_2);
    ASSERT_EQ(t.get_caravan_direction(cn), ASCENDING);


    try {
        t.play_numeric_card(cn, c_num_3);
        FAIL();

    } catch(CaravanGameModelException & e) {

    } catch(...) {
        FAIL();
    }
}

TEST (TestGameModelTable, PlayNumericCard_CaravanAscending_ThirdCardDescendingSameSuit) {
    Table t;
    Card c_num_1 = {SPADES, FIVE};
    Card c_num_2 = {DIAMONDS, SEVEN};
    Card c_num_3 = {DIAMONDS, TWO};
    CaravanName cn = CARAVAN_A;

    t.play_numeric_card(cn, c_num_1);
    t.play_numeric_card(cn, c_num_2);

    ASSERT_EQ(t.get_caravan_suit(cn), DIAMONDS);
    ASSERT_EQ(t.get_caravan_direction(cn), ASCENDING);

    t.play_numeric_card(cn, c_num_3);

    ASSERT_EQ(t.get_caravan_suit(cn), DIAMONDS);
    ASSERT_EQ(t.get_caravan_direction(cn), DESCENDING);
}
