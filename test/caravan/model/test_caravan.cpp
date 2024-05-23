// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "gtest/gtest.h"
#include "caravan/model/caravan.h"
#include "caravan/core/common.h"
#include "caravan/core/exceptions.h"


TEST (TestCaravan, Clear_ThreeNumeric) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {SPADES, TWO};
    Card c_num_3 = {SPADES, THREE};

    ASSERT_EQ(cvn.get_size(), 0);

    cvn.put_numeral_card(c_num_1);
    cvn.put_numeral_card(c_num_2);
    cvn.put_numeral_card(c_num_3);
    ASSERT_EQ(cvn.get_size(), 3);

    cvn.clear();
    ASSERT_EQ(cvn.get_size(), 0);
}

TEST (TestCaravan, Clear_Error_EmptyCaravan) {
    auto cvn = Caravan(CARAVAN_D);

    try {
        cvn.clear();
        FAIL();

    } catch (CaravanGameException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestCaravan, GetBid_ThreeNumeric) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {SPADES, TWO};
    Card c_num_3 = {SPADES, THREE};

    ASSERT_EQ(cvn.get_bid(), 0);
    cvn.put_numeral_card(c_num_1);
    ASSERT_EQ(cvn.get_bid(), 1);
    cvn.put_numeral_card(c_num_2);
    ASSERT_EQ(cvn.get_bid(), 3);
    cvn.put_numeral_card(c_num_3);
    ASSERT_EQ(cvn.get_bid(), 6);
}

TEST (TestCaravan, GetBid_Value_RankAce) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, ACE};

    ASSERT_EQ(cvn.get_bid(), 0);
    cvn.put_numeral_card(c_num);
    ASSERT_EQ(cvn.get_bid(), 1);
}

TEST (TestCaravan, GetBid_Value_RankTwo) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, TWO};

    ASSERT_EQ(cvn.get_bid(), 0);
    cvn.put_numeral_card(c_num);
    ASSERT_EQ(cvn.get_bid(), 2);
}

TEST (TestCaravan, GetBid_Value_RankThree) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, THREE};

    ASSERT_EQ(cvn.get_bid(), 0);
    cvn.put_numeral_card(c_num);
    ASSERT_EQ(cvn.get_bid(), 3);
}

TEST (TestCaravan, GetBid_Value_RankFour) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, FOUR};

    ASSERT_EQ(cvn.get_bid(), 0);
    cvn.put_numeral_card(c_num);
    ASSERT_EQ(cvn.get_bid(), 4);
}

TEST (TestCaravan, GetBid_Value_RankFive) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, FIVE};

    ASSERT_EQ(cvn.get_bid(), 0);
    cvn.put_numeral_card(c_num);
    ASSERT_EQ(cvn.get_bid(), 5);
}

TEST (TestCaravan, GetBid_Value_RankSix) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, SIX};

    ASSERT_EQ(cvn.get_bid(), 0);
    cvn.put_numeral_card(c_num);
    ASSERT_EQ(cvn.get_bid(), 6);
}

TEST (TestCaravan, GetBid_Value_RankSeven) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, SEVEN};

    ASSERT_EQ(cvn.get_bid(), 0);
    cvn.put_numeral_card(c_num);
    ASSERT_EQ(cvn.get_bid(), 7);
}

TEST (TestCaravan, GetBid_Value_RankEight) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, EIGHT};

    ASSERT_EQ(cvn.get_bid(), 0);
    cvn.put_numeral_card(c_num);
    ASSERT_EQ(cvn.get_bid(), 8);
}

TEST (TestCaravan, GetBid_Value_RankNine) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, NINE};

    ASSERT_EQ(cvn.get_bid(), 0);
    cvn.put_numeral_card(c_num);
    ASSERT_EQ(cvn.get_bid(), 9);
}

TEST (TestCaravan, GetBid_Value_RankTen) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, TEN};

    ASSERT_EQ(cvn.get_bid(), 0);
    cvn.put_numeral_card(c_num);
    ASSERT_EQ(cvn.get_bid(), 10);
}

TEST (TestCaravan, GetCardsAt_TwoNumeric_OneFace) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {HEARTS, TWO};
    Card c_face_1 = {DIAMONDS, KING};
    Slot ts;

    cvn.put_numeral_card(c_num_1);
    cvn.put_numeral_card(c_num_2);
    cvn.put_face_card(c_face_1, 2);

    ts = cvn.get_slot(1);
    ASSERT_EQ(ts.card.suit, c_num_1.suit);
    ASSERT_EQ(ts.card.rank, c_num_1.rank);

    ts = cvn.get_slot(2);
    ASSERT_EQ(ts.card.suit, c_num_2.suit);
    ASSERT_EQ(ts.card.rank, c_num_2.rank);

    ASSERT_EQ(ts.i_faces, 1);
    ASSERT_EQ(ts.faces[0].suit, c_face_1.suit);
    ASSERT_EQ(ts.faces[0].rank, c_face_1.rank);
}

TEST (TestCaravan, GetCardsAt_Error_OneNumeric_OutOfRange) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, ACE};

    cvn.put_numeral_card(c_num);

    try {
        cvn.get_slot(2);
        FAIL();

    } catch (CaravanGameException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestCaravan, GetDirection_Ascending) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {SPADES, TWO};

    ASSERT_EQ(cvn.get_direction(), ANY);
    cvn.put_numeral_card(c_num_1);
    ASSERT_EQ(cvn.get_direction(), ANY);
    cvn.put_numeral_card(c_num_2);
    ASSERT_EQ(cvn.get_direction(), ASCENDING);
}

TEST (TestCaravan, GetDirection_Descending) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_num_1 = {SPADES, TWO};
    Card c_num_2 = {SPADES, ACE};

    ASSERT_EQ(cvn.get_direction(), ANY);
    cvn.put_numeral_card(c_num_1);
    ASSERT_EQ(cvn.get_direction(), ANY);
    cvn.put_numeral_card(c_num_2);
    ASSERT_EQ(cvn.get_direction(), DESCENDING);
}

TEST (TestCaravan, GetDirection_Ascending_ThreeQueens) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {HEARTS, TWO};
    Card c_face_1 = {CLUBS, QUEEN};
    Card c_face_2 = {DIAMONDS, QUEEN};
    Card c_face_3 = {HEARTS, QUEEN};

    cvn.put_numeral_card(c_num_1);
    cvn.put_numeral_card(c_num_2);
    ASSERT_EQ(cvn.get_direction(), ASCENDING);

    cvn.put_face_card(c_face_1, 2);
    ASSERT_EQ(cvn.get_direction(), DESCENDING);

    cvn.put_face_card(c_face_2, 2);
    ASSERT_EQ(cvn.get_direction(), ASCENDING);

    cvn.put_face_card(c_face_3, 2);
    ASSERT_EQ(cvn.get_direction(), DESCENDING);
}

TEST (TestCaravan, GetDirection_Descending_ThreeQueens) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_num_1 = {SPADES, TEN};
    Card c_num_2 = {HEARTS, NINE};
    Card c_face_1 = {CLUBS, QUEEN};
    Card c_face_2 = {DIAMONDS, QUEEN};
    Card c_face_3 = {HEARTS, QUEEN};

    cvn.put_numeral_card(c_num_1);
    cvn.put_numeral_card(c_num_2);
    ASSERT_EQ(cvn.get_direction(), DESCENDING);

    cvn.put_face_card(c_face_1, 2);
    ASSERT_EQ(cvn.get_direction(), ASCENDING);

    cvn.put_face_card(c_face_2, 2);
    ASSERT_EQ(cvn.get_direction(), DESCENDING);

    cvn.put_face_card(c_face_3, 2);
    ASSERT_EQ(cvn.get_direction(), ASCENDING);
}

TEST (TestCaravan, GetName) {
    auto cvn = Caravan(CARAVAN_D);
    ASSERT_EQ(cvn.get_name(), CARAVAN_D);
}

TEST (TestCaravan, GetSize_BeforeAfterNumeric) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, ACE};

    ASSERT_EQ(cvn.get_size(), 0);
    cvn.put_numeral_card(c_num);
    ASSERT_EQ(cvn.get_size(), 1);
}

TEST (TestCaravan, GetSuit) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, ACE};

    ASSERT_EQ(cvn.get_suit(), NO_SUIT);
    cvn.put_numeral_card(c_num);
    ASSERT_EQ(cvn.get_suit(), c_num.suit);
}

TEST (TestCaravan, PutNumericCard_PutFaceNotJack) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, ACE};
    Card c_face = {HEARTS, KING};
    Slot ts;

    cvn.put_numeral_card(c_num);
    cvn.put_face_card(c_face, 1);

    ts = cvn.get_slot(1);

    ASSERT_EQ(ts.card.suit, c_num.suit);
    ASSERT_EQ(ts.card.rank, c_num.rank);

    ASSERT_EQ(ts.i_faces, 1);
    ASSERT_EQ(ts.faces[0].suit, c_face.suit);
    ASSERT_EQ(ts.faces[0].rank, c_face.rank);
}

TEST (TestCaravan, PutNumericCard_PutFaceJack) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, ACE};
    Card c_face = {HEARTS, JACK};

    cvn.put_numeral_card(c_num);
    ASSERT_EQ(cvn.get_size(), 1);

    cvn.put_face_card(c_face, 1);
    ASSERT_EQ(cvn.get_size(), 0);
}

TEST (TestCaravan, PutNumericCard_Error_NotNumeric) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_face = {HEARTS, KING};

    try {
        cvn.put_numeral_card(c_face);
        FAIL();

    } catch (CaravanGameException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestCaravan, PutNumericCard_Error_CaravanFull) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {SPADES, THREE};
    Card c_num_3 = {SPADES, FIVE};
    Card c_num_4 = {SPADES, THREE};
    Card c_num_5 = {SPADES, FIVE};
    Card c_num_6 = {SPADES, THREE};
    Card c_num_7 = {SPADES, FIVE};
    Card c_num_8 = {SPADES, THREE};
    Card c_num_9 = {SPADES, FIVE};

    cvn.put_numeral_card(c_num_1);
    cvn.put_numeral_card(c_num_2);
    cvn.put_numeral_card(c_num_3);
    cvn.put_numeral_card(c_num_4);
    cvn.put_numeral_card(c_num_5);
    cvn.put_numeral_card(c_num_6);
    cvn.put_numeral_card(c_num_7);
    cvn.put_numeral_card(c_num_8);

    ASSERT_EQ(cvn.get_size(), TRACK_NUMERIC_MAX);

    try {
        cvn.put_numeral_card(c_num_9);
        FAIL();

    } catch (CaravanGameException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestCaravan, PutFaceCard) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, ACE};
    Card c_face = {HEARTS, KING};
    Slot ts;

    cvn.put_numeral_card(c_num);
    cvn.put_face_card(c_face, 1);

    ASSERT_EQ(cvn.get_size(), 1);
    ts = cvn.get_slot(1);

    ASSERT_EQ(ts.i_faces, 1);
    ASSERT_EQ(ts.faces[0].suit, c_face.suit);
    ASSERT_EQ(ts.faces[0].rank, c_face.rank);
}

TEST (TestCaravan, PutFaceCard_Error_EmptyCaravan) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_face = {HEARTS, KING};
    Slot ts;

    try {
        cvn.put_face_card(c_face, 1);
        FAIL();

    } catch (CaravanGameException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestCaravan, PutFaceCard_Error_OutOfRange) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, ACE};
    Card c_face = {HEARTS, KING};
    Slot ts;

    cvn.put_numeral_card(c_num);

    try {
        cvn.put_face_card(c_face, 2);
        FAIL();

    } catch (CaravanGameException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestCaravan, PutFaceCard_Error_NotFaceCard) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {SPADES, TWO};
    Slot ts;

    cvn.put_numeral_card(c_num_1);

    try {
        cvn.put_face_card(c_num_2, 1);
        FAIL();

    } catch (CaravanGameException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestCaravan, PutFaceCard_Error_FullFaceCardCapacity) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, ACE};
    Card c_face_1 = {HEARTS, KING};
    Card c_face_2 = {HEARTS, KING};
    Card c_face_3 = {HEARTS, KING};
    Card c_face_4 = {HEARTS, KING};
    Slot ts;

    cvn.put_numeral_card(c_num);
    cvn.put_face_card(c_face_1, 1);
    cvn.put_face_card(c_face_2, 1);
    cvn.put_face_card(c_face_3, 1);

    ASSERT_EQ(cvn.get_slot(1).faces.size(), TRACK_FACE_MAX);

    try {
        cvn.put_face_card(c_face_4, 1);
        FAIL();

    } catch (CaravanGameException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestCaravan, RemoveRank_FiveNumeric_OneFace) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {HEARTS, TWO};
    Card c_num_3 = {CLUBS, FIVE};
    Card c_num_4 = {CLUBS, TWO};
    Card c_num_5 = {DIAMONDS, ACE};
    Card c_face = {HEARTS, KING};

    cvn.put_numeral_card(c_num_1);
    cvn.put_numeral_card(c_num_2);
    cvn.put_numeral_card(c_num_3);
    cvn.put_numeral_card(c_num_4);
    cvn.put_numeral_card(c_num_5);
    cvn.put_face_card(c_face, 4);
    ASSERT_EQ(cvn.get_size(), 5);

    cvn.remove_rank(ACE, 0);
    ASSERT_EQ(cvn.get_size(), 3);

    ASSERT_EQ(cvn.get_slot(1).card.suit, c_num_2.suit);
    ASSERT_EQ(cvn.get_slot(1).card.rank, c_num_2.rank);

    ASSERT_EQ(cvn.get_slot(2).card.suit, c_num_3.suit);
    ASSERT_EQ(cvn.get_slot(2).card.rank, c_num_3.rank);

    ASSERT_EQ(cvn.get_slot(3).card.suit, c_num_4.suit);
    ASSERT_EQ(cvn.get_slot(3).card.rank, c_num_4.rank);
}

TEST (TestCaravan, RemoveRank_FiveNumeric_OneFace_ExcludeOne) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {HEARTS, TWO};
    Card c_num_3 = {CLUBS, FIVE};
    Card c_num_4 = {CLUBS, TWO};
    Card c_num_5 = {DIAMONDS, ACE};
    Card c_face = {HEARTS, KING};

    cvn.put_numeral_card(c_num_1);
    cvn.put_numeral_card(c_num_2);
    cvn.put_numeral_card(c_num_3);
    cvn.put_numeral_card(c_num_4);
    cvn.put_numeral_card(c_num_5);
    cvn.put_face_card(c_face, 4);
    ASSERT_EQ(cvn.get_size(), 5);

    cvn.remove_rank(ACE, 5);
    ASSERT_EQ(cvn.get_size(), 4);

    ASSERT_EQ(cvn.get_slot(1).card.suit, HEARTS);
    ASSERT_EQ(cvn.get_slot(1).card.rank, TWO);

    ASSERT_EQ(cvn.get_slot(2).card.suit, CLUBS);
    ASSERT_EQ(cvn.get_slot(2).card.rank, FIVE);

    ASSERT_EQ(cvn.get_slot(3).card.suit, CLUBS);
    ASSERT_EQ(cvn.get_slot(3).card.rank, TWO);

    ASSERT_EQ(cvn.get_slot(4).card.suit, DIAMONDS);
    ASSERT_EQ(cvn.get_slot(4).card.rank, ACE);
}

TEST (TestCaravan, RemoveRank_Error_ExcludeOutOfRange) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {HEARTS, TWO};
    Card c_num_3 = {CLUBS, FIVE};
    Card c_num_4 = {CLUBS, TWO};
    Card c_num_5 = {DIAMONDS, ACE};
    Card c_face = {HEARTS, KING};

    cvn.put_numeral_card(c_num_1);
    cvn.put_numeral_card(c_num_2);
    cvn.put_numeral_card(c_num_3);
    cvn.put_numeral_card(c_num_4);
    cvn.put_numeral_card(c_num_5);
    cvn.put_face_card(c_face, 4);

    try {
        cvn.remove_rank(ACE, 7);
        FAIL();

    } catch (CaravanFatalException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestCaravan, RemoveSuit_FiveNumeric_OneFace) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {HEARTS, TWO};
    Card c_num_3 = {CLUBS, FIVE};
    Card c_num_4 = {CLUBS, TWO};
    Card c_num_5 = {DIAMONDS, ACE};
    Card c_face = {HEARTS, KING};

    cvn.put_numeral_card(c_num_1);
    cvn.put_numeral_card(c_num_2);
    cvn.put_numeral_card(c_num_3);
    cvn.put_numeral_card(c_num_4);
    cvn.put_numeral_card(c_num_5);
    cvn.put_face_card(c_face, 4);
    ASSERT_EQ(cvn.get_size(), 5);

    cvn.remove_suit(CLUBS, 0);
    ASSERT_EQ(cvn.get_size(), 3);

    ASSERT_EQ(cvn.get_slot(1).card.suit, c_num_1.suit);
    ASSERT_EQ(cvn.get_slot(1).card.rank, c_num_1.rank);

    ASSERT_EQ(cvn.get_slot(2).card.suit, c_num_2.suit);
    ASSERT_EQ(cvn.get_slot(2).card.rank, c_num_2.rank);

    ASSERT_EQ(cvn.get_slot(3).card.suit, c_num_5.suit);
    ASSERT_EQ(cvn.get_slot(3).card.rank, c_num_5.rank);
}


TEST (TestCaravan, RemoveSuit_FiveNumeric_OneFace_ExcludeOne) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {HEARTS, TWO};
    Card c_num_3 = {CLUBS, FIVE};
    Card c_num_4 = {CLUBS, TWO};
    Card c_num_5 = {DIAMONDS, ACE};
    Card c_face = {HEARTS, KING};

    cvn.put_numeral_card(c_num_1);
    cvn.put_numeral_card(c_num_2);
    cvn.put_numeral_card(c_num_3);
    cvn.put_numeral_card(c_num_4);
    cvn.put_numeral_card(c_num_5);
    cvn.put_face_card(c_face, 4);
    ASSERT_EQ(cvn.get_size(), 5);

    cvn.remove_suit(CLUBS, 3);
    ASSERT_EQ(cvn.get_size(), 4);

    ASSERT_EQ(cvn.get_slot(1).card.suit, SPADES);
    ASSERT_EQ(cvn.get_slot(1).card.rank, ACE);

    ASSERT_EQ(cvn.get_slot(2).card.suit, HEARTS);
    ASSERT_EQ(cvn.get_slot(2).card.rank, TWO);

    ASSERT_EQ(cvn.get_slot(3).card.suit, CLUBS);
    ASSERT_EQ(cvn.get_slot(3).card.rank, FIVE);

    ASSERT_EQ(cvn.get_slot(4).card.suit, DIAMONDS);
    ASSERT_EQ(cvn.get_slot(4).card.rank, ACE);
}

TEST (TestCaravan, RemoveSuit_Error_ExcludeOutOfRange) {
    auto cvn = Caravan(CARAVAN_D);
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {HEARTS, TWO};
    Card c_num_3 = {CLUBS, FIVE};
    Card c_num_4 = {CLUBS, TWO};
    Card c_num_5 = {DIAMONDS, ACE};
    Card c_face = {HEARTS, KING};

    cvn.put_numeral_card(c_num_1);
    cvn.put_numeral_card(c_num_2);
    cvn.put_numeral_card(c_num_3);
    cvn.put_numeral_card(c_num_4);
    cvn.put_numeral_card(c_num_5);
    cvn.put_face_card(c_face, 4);

    try {
        cvn.remove_suit(CLUBS, 7);
        FAIL();

    } catch (CaravanFatalException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestCaravan, RemoveNumericCard_WithJack_Position8) {
    auto cvn = Caravan(CARAVAN_D);

    cvn.put_numeral_card({SPADES, ACE});
    cvn.put_numeral_card({SPADES, TWO});
    cvn.put_numeral_card({SPADES, THREE});
    cvn.put_numeral_card({SPADES, FOUR});
    cvn.put_numeral_card({SPADES, FIVE});
    cvn.put_numeral_card({SPADES, SIX});
    cvn.put_numeral_card({SPADES, SEVEN});
    cvn.put_numeral_card({SPADES, EIGHT});

    ASSERT_EQ(cvn.get_size(), 8);

    cvn.put_face_card({SPADES, JACK}, 8);

    ASSERT_EQ(cvn.get_size(), 7);

    ASSERT_EQ(cvn.get_slot(1).card.suit, SPADES);
    ASSERT_EQ(cvn.get_slot(1).card.rank, ACE);

    ASSERT_EQ(cvn.get_slot(7).card.suit, SPADES);
    ASSERT_EQ(cvn.get_slot(7).card.rank, SEVEN);

    try {
        cvn.get_slot(8);
        FAIL();

    } catch (CaravanGameException &e) {

    } catch (...) {
        FAIL();
    }
}


TEST (TestCaravan, RemoveNumericCard_WithJack_Position1) {
    auto cvn = Caravan(CARAVAN_D);

    cvn.put_numeral_card({SPADES, ACE});
    cvn.put_numeral_card({SPADES, TWO});
    cvn.put_numeral_card({SPADES, THREE});
    cvn.put_numeral_card({SPADES, FOUR});
    cvn.put_numeral_card({SPADES, FIVE});
    cvn.put_numeral_card({SPADES, SIX});
    cvn.put_numeral_card({SPADES, SEVEN});
    cvn.put_numeral_card({SPADES, EIGHT});

    ASSERT_EQ(cvn.get_size(), 8);

    cvn.put_face_card({SPADES, JACK}, 1);

    ASSERT_EQ(cvn.get_size(), 7);

    ASSERT_EQ(cvn.get_slot(1).card.suit, SPADES);
    ASSERT_EQ(cvn.get_slot(1).card.rank, TWO);

    ASSERT_EQ(cvn.get_slot(7).card.suit, SPADES);
    ASSERT_EQ(cvn.get_slot(7).card.rank, EIGHT);

    try {
        cvn.get_slot(8);
        FAIL();

    } catch (CaravanGameException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestCaravan, RemoveNumericCard_WithJack_Position5) {
    auto cvn = Caravan(CARAVAN_D);

    cvn.put_numeral_card({SPADES, ACE});
    cvn.put_numeral_card({SPADES, TWO});
    cvn.put_numeral_card({SPADES, THREE});
    cvn.put_numeral_card({SPADES, FOUR});
    cvn.put_numeral_card({SPADES, FIVE});
    cvn.put_numeral_card({SPADES, SIX});
    cvn.put_numeral_card({SPADES, SEVEN});
    cvn.put_numeral_card({SPADES, EIGHT});

    ASSERT_EQ(cvn.get_size(), 8);

    cvn.put_face_card({SPADES, JACK}, 5);

    ASSERT_EQ(cvn.get_size(), 7);

    ASSERT_EQ(cvn.get_slot(1).card.suit, SPADES);
    ASSERT_EQ(cvn.get_slot(1).card.rank, ACE);

    ASSERT_EQ(cvn.get_slot(5).card.suit, SPADES);
    ASSERT_EQ(cvn.get_slot(5).card.rank, SIX);

    ASSERT_EQ(cvn.get_slot(7).card.suit, SPADES);
    ASSERT_EQ(cvn.get_slot(7).card.rank, EIGHT);

    try {
        cvn.get_slot(8);
        FAIL();

    } catch (CaravanGameException &e) {

    } catch (...) {
        FAIL();
    }
}
