// Copyright (c) 2022-2023 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "gtest/gtest.h"
#include "../include/caravan.h"
#include "../include/exceptions.h"


TEST (TestGameCaravan, Clear_ThreeNumeric) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {SPADES, TWO};
    Card c_num_3 = {SPADES, THREE};

    ASSERT_EQ(cvn.get_size(), 0);

    cvn.put_numeric_card(c_num_1);
    cvn.put_numeric_card(c_num_2);
    cvn.put_numeric_card(c_num_3);
    ASSERT_EQ(cvn.get_size(), 3);

    cvn.clear();
    ASSERT_EQ(cvn.get_size(), 0);
}

TEST (TestGameCaravan, Clear_Error_EmptyCaravan) {
    Caravan cvn = Caravan(CARAVAN_D);

    try {
        cvn.clear();
        FAIL();

    } catch (CaravanGameException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestGameCaravan, GetBid_ThreeNumeric) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {SPADES, TWO};
    Card c_num_3 = {SPADES, THREE};

    ASSERT_EQ(cvn.get_bid(), 0);
    cvn.put_numeric_card(c_num_1);
    ASSERT_EQ(cvn.get_bid(), 1);
    cvn.put_numeric_card(c_num_2);
    ASSERT_EQ(cvn.get_bid(), 3);
    cvn.put_numeric_card(c_num_3);
    ASSERT_EQ(cvn.get_bid(), 6);
}

TEST (TestGameCaravan, GetBid_Value_RankAce) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, ACE};

    ASSERT_EQ(cvn.get_bid(), 0);
    cvn.put_numeric_card(c_num);
    ASSERT_EQ(cvn.get_bid(), 1);
}

TEST (TestGameCaravan, GetBid_Value_RankTwo) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, TWO};

    ASSERT_EQ(cvn.get_bid(), 0);
    cvn.put_numeric_card(c_num);
    ASSERT_EQ(cvn.get_bid(), 2);
}

TEST (TestGameCaravan, GetBid_Value_RankThree) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, THREE};

    ASSERT_EQ(cvn.get_bid(), 0);
    cvn.put_numeric_card(c_num);
    ASSERT_EQ(cvn.get_bid(), 3);
}

TEST (TestGameCaravan, GetBid_Value_RankFour) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, FOUR};

    ASSERT_EQ(cvn.get_bid(), 0);
    cvn.put_numeric_card(c_num);
    ASSERT_EQ(cvn.get_bid(), 4);
}

TEST (TestGameCaravan, GetBid_Value_RankFive) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, FIVE};

    ASSERT_EQ(cvn.get_bid(), 0);
    cvn.put_numeric_card(c_num);
    ASSERT_EQ(cvn.get_bid(), 5);
}

TEST (TestGameCaravan, GetBid_Value_RankSix) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, SIX};

    ASSERT_EQ(cvn.get_bid(), 0);
    cvn.put_numeric_card(c_num);
    ASSERT_EQ(cvn.get_bid(), 6);
}

TEST (TestGameCaravan, GetBid_Value_RankSeven) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, SEVEN};

    ASSERT_EQ(cvn.get_bid(), 0);
    cvn.put_numeric_card(c_num);
    ASSERT_EQ(cvn.get_bid(), 7);
}

TEST (TestGameCaravan, GetBid_Value_RankEight) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, EIGHT};

    ASSERT_EQ(cvn.get_bid(), 0);
    cvn.put_numeric_card(c_num);
    ASSERT_EQ(cvn.get_bid(), 8);
}

TEST (TestGameCaravan, GetBid_Value_RankNine) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, NINE};

    ASSERT_EQ(cvn.get_bid(), 0);
    cvn.put_numeric_card(c_num);
    ASSERT_EQ(cvn.get_bid(), 9);
}

TEST (TestGameCaravan, GetBid_Value_RankTen) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, TEN};

    ASSERT_EQ(cvn.get_bid(), 0);
    cvn.put_numeric_card(c_num);
    ASSERT_EQ(cvn.get_bid(), 10);
}

TEST (TestGameCaravan, GetCardsAt_TwoNumeric_OneFace) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {HEARTS, TWO};
    Card c_face_1 = {DIAMONDS, KING};
    Slot ts;

    cvn.put_numeric_card(c_num_1);
    cvn.put_numeric_card(c_num_2);
    cvn.put_face_card(c_face_1, 2);

    ts = cvn.get_cards_at(1);
    ASSERT_EQ(ts.card.suit, c_num_1.suit);
    ASSERT_EQ(ts.card.rank, c_num_1.rank);

    ts = cvn.get_cards_at(2);
    ASSERT_EQ(ts.card.suit, c_num_2.suit);
    ASSERT_EQ(ts.card.rank, c_num_2.rank);

    ASSERT_EQ(ts.i_faces, 1);
    ASSERT_EQ(ts.faces[0].suit, c_face_1.suit);
    ASSERT_EQ(ts.faces[0].rank, c_face_1.rank);
}

TEST (TestGameCaravan, GetCardsAt_Error_OneNumeric_OutOfRange) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, ACE};

    cvn.put_numeric_card(c_num);

    try {
        cvn.get_cards_at(2);
        FAIL();

    } catch (CaravanGameException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestGameCaravan, GetDirection_Ascending) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {SPADES, TWO};

    ASSERT_EQ(cvn.get_direction(), ANY);
    cvn.put_numeric_card(c_num_1);
    ASSERT_EQ(cvn.get_direction(), ANY);
    cvn.put_numeric_card(c_num_2);
    ASSERT_EQ(cvn.get_direction(), ASCENDING);
}

TEST (TestGameCaravan, GetDirection_Descending) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_num_1 = {SPADES, TWO};
    Card c_num_2 = {SPADES, ACE};

    ASSERT_EQ(cvn.get_direction(), ANY);
    cvn.put_numeric_card(c_num_1);
    ASSERT_EQ(cvn.get_direction(), ANY);
    cvn.put_numeric_card(c_num_2);
    ASSERT_EQ(cvn.get_direction(), DESCENDING);
}

TEST (TestGameCaravan, GetDirection_Ascending_ThreeQueens) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {HEARTS, TWO};
    Card c_face_1 = {CLUBS, QUEEN};
    Card c_face_2 = {DIAMONDS, QUEEN};
    Card c_face_3 = {HEARTS, QUEEN};

    cvn.put_numeric_card(c_num_1);
    cvn.put_numeric_card(c_num_2);
    ASSERT_EQ(cvn.get_direction(), ASCENDING);

    cvn.put_face_card(c_face_1, 2);
    ASSERT_EQ(cvn.get_direction(), DESCENDING);

    cvn.put_face_card(c_face_2, 2);
    ASSERT_EQ(cvn.get_direction(), ASCENDING);

    cvn.put_face_card(c_face_3, 2);
    ASSERT_EQ(cvn.get_direction(), DESCENDING);
}

TEST (TestGameCaravan, GetDirection_Descending_ThreeQueens) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_num_1 = {SPADES, TEN};
    Card c_num_2 = {HEARTS, NINE};
    Card c_face_1 = {CLUBS, QUEEN};
    Card c_face_2 = {DIAMONDS, QUEEN};
    Card c_face_3 = {HEARTS, QUEEN};

    cvn.put_numeric_card(c_num_1);
    cvn.put_numeric_card(c_num_2);
    ASSERT_EQ(cvn.get_direction(), DESCENDING);

    cvn.put_face_card(c_face_1, 2);
    ASSERT_EQ(cvn.get_direction(), ASCENDING);

    cvn.put_face_card(c_face_2, 2);
    ASSERT_EQ(cvn.get_direction(), DESCENDING);

    cvn.put_face_card(c_face_3, 2);
    ASSERT_EQ(cvn.get_direction(), ASCENDING);
}

TEST (TestGameCaravan, GetName) {
    Caravan cvn = Caravan(CARAVAN_D);
    ASSERT_EQ(cvn.get_name(), CARAVAN_D);
}

TEST (TestGameCaravan, GetSize_BeforeAfterNumeric) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, ACE};

    ASSERT_EQ(cvn.get_size(), 0);
    cvn.put_numeric_card(c_num);
    ASSERT_EQ(cvn.get_size(), 1);
}

TEST (TestGameCaravan, GetSuit) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, ACE};

    ASSERT_EQ(cvn.get_suit(), NO_SUIT);
    cvn.put_numeric_card(c_num);
    ASSERT_EQ(cvn.get_suit(), c_num.suit);
}

TEST (TestGameCaravan, PutNumericCard_PutFaceNotJack) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, ACE};
    Card c_face = {HEARTS, KING};
    Slot ts;

    cvn.put_numeric_card(c_num);
    cvn.put_face_card(c_face, 1);

    ts = cvn.get_cards_at(1);

    ASSERT_EQ(ts.card.suit, c_num.suit);
    ASSERT_EQ(ts.card.rank, c_num.rank);

    ASSERT_EQ(ts.i_faces, 1);
    ASSERT_EQ(ts.faces[0].suit, c_face.suit);
    ASSERT_EQ(ts.faces[0].rank, c_face.rank);
}

TEST (TestGameCaravan, PutNumericCard_PutFaceJack) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, ACE};
    Card c_face = {HEARTS, JACK};

    cvn.put_numeric_card(c_num);
    ASSERT_EQ(cvn.get_size(), 1);

    cvn.put_face_card(c_face, 1);
    ASSERT_EQ(cvn.get_size(), 0);
}

TEST (TestGameCaravan, PutNumericCard_Error_NotNumeric) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_face = {HEARTS, KING};

    try {
        cvn.put_numeric_card(c_face);
        FAIL();

    } catch (CaravanGameException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestGameCaravan, PutNumericCard_Error_CaravanFull) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {SPADES, THREE};
    Card c_num_3 = {SPADES, FIVE};
    Card c_num_4 = {SPADES, THREE};
    Card c_num_5 = {SPADES, FIVE};
    Card c_num_6 = {SPADES, THREE};
    Card c_num_7 = {SPADES, FIVE};
    Card c_num_8 = {SPADES, THREE};
    Card c_num_9 = {SPADES, FIVE};
    Card c_num_10 = {SPADES, THREE};
    Card c_num_11 = {SPADES, NINE};

    cvn.put_numeric_card(c_num_1);
    cvn.put_numeric_card(c_num_2);
    cvn.put_numeric_card(c_num_3);
    cvn.put_numeric_card(c_num_4);
    cvn.put_numeric_card(c_num_5);
    cvn.put_numeric_card(c_num_6);
    cvn.put_numeric_card(c_num_7);
    cvn.put_numeric_card(c_num_8);
    cvn.put_numeric_card(c_num_9);
    cvn.put_numeric_card(c_num_10);

    ASSERT_EQ(cvn.get_size(), 10);

    try {
        cvn.put_numeric_card(c_num_11);
        FAIL();

    } catch (CaravanGameException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestGameCaravan, PutFaceCard) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, ACE};
    Card c_face = {HEARTS, KING};
    Slot ts;

    cvn.put_numeric_card(c_num);
    cvn.put_face_card(c_face, 1);

    ASSERT_EQ(cvn.get_size(), 1);
    ts = cvn.get_cards_at(1);

    ASSERT_EQ(ts.i_faces, 1);
    ASSERT_EQ(ts.faces[0].suit, c_face.suit);
    ASSERT_EQ(ts.faces[0].rank, c_face.rank);
}

TEST (TestGameCaravan, PutFaceCard_Error_EmptyCaravan) {
    Caravan cvn = Caravan(CARAVAN_D);
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

TEST (TestGameCaravan, PutFaceCard_Error_OutOfRange) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, ACE};
    Card c_face = {HEARTS, KING};
    Slot ts;

    cvn.put_numeric_card(c_num);

    try {
        cvn.put_face_card(c_face, 2);
        FAIL();

    } catch (CaravanGameException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestGameCaravan, PutFaceCard_Error_NotFaceCard) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {SPADES, TWO};
    Slot ts;

    cvn.put_numeric_card(c_num_1);

    try {
        cvn.put_face_card(c_num_2, 1);
        FAIL();

    } catch (CaravanGameException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestGameCaravan, PutFaceCard_Error_FullFaceCardCapacity) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_num = {SPADES, ACE};
    Card c_face_1 = {HEARTS, KING};
    Card c_face_2 = {HEARTS, KING};
    Card c_face_3 = {HEARTS, KING};
    Card c_face_4 = {HEARTS, KING};
    Card c_face_5 = {HEARTS, KING};
    Card c_face_6 = {HEARTS, KING};
    Slot ts;

    cvn.put_numeric_card(c_num);
    cvn.put_face_card(c_face_1, 1);
    cvn.put_face_card(c_face_2, 1);
    cvn.put_face_card(c_face_3, 1);
    cvn.put_face_card(c_face_4, 1);
    cvn.put_face_card(c_face_5, 1);

    try {
        cvn.put_face_card(c_face_6, 1);
        FAIL();

    } catch (CaravanGameException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestGameCaravan, RemoveRank_FiveNumeric_OneFace) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {HEARTS, TWO};
    Card c_num_3 = {CLUBS, FIVE};
    Card c_num_4 = {CLUBS, TWO};
    Card c_num_5 = {DIAMONDS, ACE};
    Card c_face = {HEARTS, KING};

    cvn.put_numeric_card(c_num_1);
    cvn.put_numeric_card(c_num_2);
    cvn.put_numeric_card(c_num_3);
    cvn.put_numeric_card(c_num_4);
    cvn.put_numeric_card(c_num_5);
    cvn.put_face_card(c_face, 4);
    ASSERT_EQ(cvn.get_size(), 5);

    cvn.remove_rank(ACE, 0);
    ASSERT_EQ(cvn.get_size(), 3);

    ASSERT_EQ(cvn.get_cards_at(1).card.suit, c_num_2.suit);
    ASSERT_EQ(cvn.get_cards_at(1).card.rank, c_num_2.rank);

    ASSERT_EQ(cvn.get_cards_at(2).card.suit, c_num_3.suit);
    ASSERT_EQ(cvn.get_cards_at(2).card.rank, c_num_3.rank);

    ASSERT_EQ(cvn.get_cards_at(3).card.suit, c_num_4.suit);
    ASSERT_EQ(cvn.get_cards_at(3).card.rank, c_num_4.rank);
}

TEST (TestGameCaravan, RemoveRank_FiveNumeric_OneFace_ExcludeOne) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {HEARTS, TWO};
    Card c_num_3 = {CLUBS, FIVE};
    Card c_num_4 = {CLUBS, TWO};
    Card c_num_5 = {DIAMONDS, ACE};
    Card c_face = {HEARTS, KING};

    cvn.put_numeric_card(c_num_1);
    cvn.put_numeric_card(c_num_2);
    cvn.put_numeric_card(c_num_3);
    cvn.put_numeric_card(c_num_4);
    cvn.put_numeric_card(c_num_5);
    cvn.put_face_card(c_face, 4);
    ASSERT_EQ(cvn.get_size(), 5);

    cvn.remove_rank(ACE, 5);
    ASSERT_EQ(cvn.get_size(), 4);

    ASSERT_EQ(cvn.get_cards_at(1).card.suit, HEARTS);
    ASSERT_EQ(cvn.get_cards_at(1).card.rank, TWO);

    ASSERT_EQ(cvn.get_cards_at(2).card.suit, CLUBS);
    ASSERT_EQ(cvn.get_cards_at(2).card.rank, FIVE);

    ASSERT_EQ(cvn.get_cards_at(3).card.suit, CLUBS);
    ASSERT_EQ(cvn.get_cards_at(3).card.rank, TWO);

    ASSERT_EQ(cvn.get_cards_at(4).card.suit, DIAMONDS);
    ASSERT_EQ(cvn.get_cards_at(4).card.rank, ACE);
}

TEST (TestGameCaravan, RemoveRank_Error_ExcludeOutOfRange) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {HEARTS, TWO};
    Card c_num_3 = {CLUBS, FIVE};
    Card c_num_4 = {CLUBS, TWO};
    Card c_num_5 = {DIAMONDS, ACE};
    Card c_face = {HEARTS, KING};

    cvn.put_numeric_card(c_num_1);
    cvn.put_numeric_card(c_num_2);
    cvn.put_numeric_card(c_num_3);
    cvn.put_numeric_card(c_num_4);
    cvn.put_numeric_card(c_num_5);
    cvn.put_face_card(c_face, 4);

    try {
        cvn.remove_rank(ACE, 7);
        FAIL();

    } catch (CaravanFatalException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestGameCaravan, RemoveSuit_FiveNumeric_OneFace) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {HEARTS, TWO};
    Card c_num_3 = {CLUBS, FIVE};
    Card c_num_4 = {CLUBS, TWO};
    Card c_num_5 = {DIAMONDS, ACE};
    Card c_face = {HEARTS, KING};

    cvn.put_numeric_card(c_num_1);
    cvn.put_numeric_card(c_num_2);
    cvn.put_numeric_card(c_num_3);
    cvn.put_numeric_card(c_num_4);
    cvn.put_numeric_card(c_num_5);
    cvn.put_face_card(c_face, 4);
    ASSERT_EQ(cvn.get_size(), 5);

    cvn.remove_suit(CLUBS, 0);
    ASSERT_EQ(cvn.get_size(), 3);

    ASSERT_EQ(cvn.get_cards_at(1).card.suit, c_num_1.suit);
    ASSERT_EQ(cvn.get_cards_at(1).card.rank, c_num_1.rank);

    ASSERT_EQ(cvn.get_cards_at(2).card.suit, c_num_2.suit);
    ASSERT_EQ(cvn.get_cards_at(2).card.rank, c_num_2.rank);

    ASSERT_EQ(cvn.get_cards_at(3).card.suit, c_num_5.suit);
    ASSERT_EQ(cvn.get_cards_at(3).card.rank, c_num_5.rank);
}


TEST (TestGameCaravan, RemoveSuit_FiveNumeric_OneFace_ExcludeOne) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {HEARTS, TWO};
    Card c_num_3 = {CLUBS, FIVE};
    Card c_num_4 = {CLUBS, TWO};
    Card c_num_5 = {DIAMONDS, ACE};
    Card c_face = {HEARTS, KING};

    cvn.put_numeric_card(c_num_1);
    cvn.put_numeric_card(c_num_2);
    cvn.put_numeric_card(c_num_3);
    cvn.put_numeric_card(c_num_4);
    cvn.put_numeric_card(c_num_5);
    cvn.put_face_card(c_face, 4);
    ASSERT_EQ(cvn.get_size(), 5);

    cvn.remove_suit(CLUBS, 3);
    ASSERT_EQ(cvn.get_size(), 4);

    ASSERT_EQ(cvn.get_cards_at(1).card.suit, SPADES);
    ASSERT_EQ(cvn.get_cards_at(1).card.rank, ACE);

    ASSERT_EQ(cvn.get_cards_at(2).card.suit, HEARTS);
    ASSERT_EQ(cvn.get_cards_at(2).card.rank, TWO);

    ASSERT_EQ(cvn.get_cards_at(3).card.suit, CLUBS);
    ASSERT_EQ(cvn.get_cards_at(3).card.rank, FIVE);

    ASSERT_EQ(cvn.get_cards_at(4).card.suit, DIAMONDS);
    ASSERT_EQ(cvn.get_cards_at(4).card.rank, ACE);
}

TEST (TestGameCaravan, RemoveSuit_Error_ExcludeOutOfRange) {
    Caravan cvn = Caravan(CARAVAN_D);
    Card c_num_1 = {SPADES, ACE};
    Card c_num_2 = {HEARTS, TWO};
    Card c_num_3 = {CLUBS, FIVE};
    Card c_num_4 = {CLUBS, TWO};
    Card c_num_5 = {DIAMONDS, ACE};
    Card c_face = {HEARTS, KING};

    cvn.put_numeric_card(c_num_1);
    cvn.put_numeric_card(c_num_2);
    cvn.put_numeric_card(c_num_3);
    cvn.put_numeric_card(c_num_4);
    cvn.put_numeric_card(c_num_5);
    cvn.put_face_card(c_face, 4);

    try {
        cvn.remove_suit(CLUBS, 7);
        FAIL();

    } catch (CaravanFatalException &e) {

    } catch (...) {
        FAIL();
    }
}

