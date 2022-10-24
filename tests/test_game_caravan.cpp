// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "gtest/gtest.h"
#include "../src/game/model.h"


TEST (TestGameModelCaravan, GetBid_ThreeNumeric) {
    Caravan cvn = Caravan(CARAVAN_A);
    Card c_num_1 = { SPADES, ACE };
    Card c_num_2 = { SPADES, TWO };
    Card c_num_3 = { SPADES, THREE };
    CaravanName pn = CARAVAN_A;

    ASSERT_EQ(cvn.get_bid(), 0);
    cvn.put_numeric_card(c_num_1);
    ASSERT_EQ(cvn.get_bid(), 1);
    cvn.put_numeric_card(c_num_2);
    ASSERT_EQ(cvn.get_bid(), 3);
    cvn.put_numeric_card(c_num_3);
    ASSERT_EQ(cvn.get_bid(), 6);
}

TEST (TestGameModelCaravan, GetName) {
    Caravan cvn = Caravan(CARAVAN_A);
    ASSERT_EQ(cvn.get_name(), CARAVAN_A);
}

TEST (TestGameModelCaravan, GetDirectionBeforeAfter_Ascending) {
    Caravan cvn = Caravan(CARAVAN_A);
    Card c_num_1 = { SPADES, ACE };
    Card c_num_2 = { SPADES, TWO };

    ASSERT_EQ(cvn.get_direction(), NO_DIRECTION);
    cvn.put_numeric_card(c_num_1);
    ASSERT_EQ(cvn.get_direction(), NO_DIRECTION);
    cvn.put_numeric_card(c_num_2);
    ASSERT_EQ(cvn.get_direction(), ASCENDING);
}

TEST (TestGameModelCaravan, GetDirectionBeforeAfter_Descending) {
    Caravan cvn = Caravan(CARAVAN_A);
    Card c_num_1 = { SPADES, TWO };
    Card c_num_2 = { SPADES, ACE };

    ASSERT_EQ(cvn.get_direction(), NO_DIRECTION);
    cvn.put_numeric_card(c_num_1);
    ASSERT_EQ(cvn.get_direction(), NO_DIRECTION);
    cvn.put_numeric_card(c_num_2);
    ASSERT_EQ(cvn.get_direction(), DESCENDING);
}

TEST (TestGameModelCaravan, GetSuitBeforeAfter) {
    Caravan cvn = Caravan(CARAVAN_A);
    Card c_num = { SPADES, ACE };

    ASSERT_EQ(cvn.get_suit(), NO_SUIT);
    cvn.put_numeric_card(c_num);
    ASSERT_EQ(cvn.get_suit(), c_num.suit);
}

TEST (TestGameModelCaravan, PutNumeric_PutFaceNotJack_GetCardsAt) {
    Caravan cvn = Caravan(CARAVAN_A);
    Card c_num = { SPADES, ACE };
    Card c_face = { HEARTS, KING };

    cvn.put_numeric_card(c_num);
    cvn.put_face_card(c_face, 1);

    TrackSlot ts = cvn.get_cards_at(1);

    ASSERT_EQ(ts.card.suit, c_num.suit);
    ASSERT_EQ(ts.card.rank, c_num.rank);

    ASSERT_EQ(ts.i_faces, 1);
    ASSERT_EQ(ts.faces[0].suit, c_face.suit);
    ASSERT_EQ(ts.faces[0].rank, c_face.rank);
}

TEST (TestGameModelCaravan, RemoveAllCards_TwoNumeric_OneFace) {
    Caravan cvn = Caravan(CARAVAN_A);
    Card c_num_1 = { SPADES, ACE };
    Card c_num_2 = { SPADES, TWO };
    Card c_face = { HEARTS, KING };

    cvn.put_numeric_card(c_num_1);
    cvn.put_numeric_card(c_num_2);
    cvn.put_face_card(c_face, 2);
    ASSERT_EQ(cvn.size(), 2);

    cvn.clear();
    ASSERT_EQ(cvn.size(), 0);
}

TEST (TestGameModelCaravan, RemoveRank_FiveNumeric_OneFace) {
    Caravan cvn = Caravan(CARAVAN_A);
    Card c_num_1 = { SPADES, ACE };
    Card c_num_2 = { HEARTS, TWO };
    Card c_num_3 = { CLUBS, FIVE };
    Card c_num_4 = { CLUBS, TWO };
    Card c_num_5 = { DIAMONDS, ACE };
    Card c_face = { HEARTS, KING };

    cvn.put_numeric_card(c_num_1);
    cvn.put_numeric_card(c_num_2);
    cvn.put_numeric_card(c_num_3);
    cvn.put_numeric_card(c_num_4);
    cvn.put_numeric_card(c_num_5);
    cvn.put_face_card(c_face, 4);
    ASSERT_EQ(cvn.size(), 5);

    cvn.remove_rank(ACE, 0);
    ASSERT_EQ(cvn.size(), 3);

    ASSERT_EQ(cvn.get_cards_at(1).card.suit, c_num_2.suit);
    ASSERT_EQ(cvn.get_cards_at(1).card.rank, c_num_2.rank);

    ASSERT_EQ(cvn.get_cards_at(2).card.suit, c_num_3.suit);
    ASSERT_EQ(cvn.get_cards_at(2).card.rank, c_num_3.rank);

    ASSERT_EQ(cvn.get_cards_at(3).card.suit, c_num_4.suit);
    ASSERT_EQ(cvn.get_cards_at(3).card.rank, c_num_4.rank);
}

TEST (TestGameModelCaravan, RemoveRank_FiveNumeric_OneFace_ExcludeOne) {
    Caravan cvn = Caravan(CARAVAN_A);
    Card c_num_1 = { SPADES, ACE };
    Card c_num_2 = { HEARTS, TWO };
    Card c_num_3 = { CLUBS, FIVE };
    Card c_num_4 = { CLUBS, TWO };
    Card c_num_5 = { DIAMONDS, ACE };
    Card c_face = { HEARTS, KING };

    cvn.put_numeric_card(c_num_1);
    cvn.put_numeric_card(c_num_2);
    cvn.put_numeric_card(c_num_3);
    cvn.put_numeric_card(c_num_4);
    cvn.put_numeric_card(c_num_5);
    cvn.put_face_card(c_face, 4);
    ASSERT_EQ(cvn.size(), 5);

    cvn.remove_rank(ACE, 5);
    ASSERT_EQ(cvn.size(), 4);

    ASSERT_EQ(cvn.get_cards_at(1).card.suit, HEARTS);
    ASSERT_EQ(cvn.get_cards_at(1).card.rank, TWO);

    ASSERT_EQ(cvn.get_cards_at(2).card.suit, CLUBS);
    ASSERT_EQ(cvn.get_cards_at(2).card.rank, FIVE);

    ASSERT_EQ(cvn.get_cards_at(3).card.suit, CLUBS);
    ASSERT_EQ(cvn.get_cards_at(3).card.rank, TWO);

    ASSERT_EQ(cvn.get_cards_at(4).card.suit, DIAMONDS);
    ASSERT_EQ(cvn.get_cards_at(4).card.rank, ACE);
}

TEST (TestGameModelCaravan, RemoveSuit_FiveNumeric_OneFace) {
    Caravan cvn = Caravan(CARAVAN_A);
    Card c_num_1 = { SPADES, ACE };
    Card c_num_2 = { HEARTS, TWO };
    Card c_num_3 = { CLUBS, FIVE };
    Card c_num_4 = { CLUBS, TWO };
    Card c_num_5 = { DIAMONDS, ACE };
    Card c_face = { HEARTS, KING };

    cvn.put_numeric_card(c_num_1);
    cvn.put_numeric_card(c_num_2);
    cvn.put_numeric_card(c_num_3);
    cvn.put_numeric_card(c_num_4);
    cvn.put_numeric_card(c_num_5);
    cvn.put_face_card(c_face, 4);
    ASSERT_EQ(cvn.size(), 5);

    cvn.remove_suit(CLUBS, 0);
    ASSERT_EQ(cvn.size(), 3);

    ASSERT_EQ(cvn.get_cards_at(1).card.suit, c_num_1.suit);
    ASSERT_EQ(cvn.get_cards_at(1).card.rank, c_num_1.rank);

    ASSERT_EQ(cvn.get_cards_at(2).card.suit, c_num_2.suit);
    ASSERT_EQ(cvn.get_cards_at(2).card.rank, c_num_2.rank);

    ASSERT_EQ(cvn.get_cards_at(3).card.suit, c_num_5.suit);
    ASSERT_EQ(cvn.get_cards_at(3).card.rank, c_num_5.rank);
}


TEST (TestGameModelCaravan, RemoveSuit_FiveNumeric_OneFace_ExcludeOne) {
    Caravan cvn = Caravan(CARAVAN_A);
    Card c_num_1 = { SPADES, ACE };
    Card c_num_2 = { HEARTS, TWO };
    Card c_num_3 = { CLUBS, FIVE };
    Card c_num_4 = { CLUBS, TWO };
    Card c_num_5 = { DIAMONDS, ACE };
    Card c_face = { HEARTS, KING };

    cvn.put_numeric_card(c_num_1);
    cvn.put_numeric_card(c_num_2);
    cvn.put_numeric_card(c_num_3);
    cvn.put_numeric_card(c_num_4);
    cvn.put_numeric_card(c_num_5);
    cvn.put_face_card(c_face, 4);
    ASSERT_EQ(cvn.size(), 5);

    cvn.remove_suit(CLUBS, 3);
    ASSERT_EQ(cvn.size(), 4);

    ASSERT_EQ(cvn.get_cards_at(1).card.suit, SPADES);
    ASSERT_EQ(cvn.get_cards_at(1).card.rank, ACE);

    ASSERT_EQ(cvn.get_cards_at(2).card.suit, HEARTS);
    ASSERT_EQ(cvn.get_cards_at(2).card.rank, TWO);

    ASSERT_EQ(cvn.get_cards_at(3).card.suit, CLUBS);
    ASSERT_EQ(cvn.get_cards_at(3).card.rank, FIVE);

    ASSERT_EQ(cvn.get_cards_at(4).card.suit, DIAMONDS);
    ASSERT_EQ(cvn.get_cards_at(4).card.rank, ACE);
}

TEST (TestGameModelCaravan, SizeOneBeforeAfter) {
    Caravan cvn = Caravan(CARAVAN_A);
    Card c_num = { SPADES, ACE };

    ASSERT_EQ(cvn.size(), 0);
    cvn.put_numeric_card(c_num);
    ASSERT_EQ(cvn.size(), 1);
}
