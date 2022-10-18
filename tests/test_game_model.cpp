// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "gtest/gtest.h"
#include "../src/game/model.h"

/*
 * DECK BUILDER
 */

TEST (TestGameModelDeckBuilder, CaravanDeck_30Cards_2SampleDecks_Balanced) {
    Deck* d = DeckBuilder::build_caravan_deck(30, 2, true);

    ASSERT_EQ(d->size(), 30);
}


/*
 * CARAVAN
 */

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


/*
 * PLAYER
 */

TEST (TestGameModelPlayer, Deck30_Init) {
    Deck* d = DeckBuilder::build_caravan_deck(30, 1, true);
    Player pl = Player(PLAYER_YOU, d);
    Card c_back = d->back();
    Card c_hand;

    ASSERT_EQ(pl.size_deck(), 22);
    ASSERT_EQ(pl.size_hand(), 8);
}

TEST (TestGameModelPlayer, Deck30_TakeFromHand) {
    Deck* d = DeckBuilder::build_caravan_deck(30, 1, true);
    Player pl = Player(PLAYER_YOU, d);
    Card c_get;
    Card c_take;
    Card c_getagain;

    ASSERT_EQ(pl.size_hand(), 8);

    c_get = pl.get_hand()[0];
    c_take = pl.remove_from_hand_at(1);

    ASSERT_EQ(pl.size_hand(), 8);
    ASSERT_TRUE(c_get.suit == c_take.suit and c_get.rank == c_take.rank);

    c_getagain = pl.get_hand()[0];
    ASSERT_TRUE(c_getagain.suit != c_take.suit or c_getagain.rank != c_take.rank);
}


/*
 * TABLE
 */

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

TEST (TestGameModelTable, PlayNumeric_PlayFaceNotJack_GetCaravanCardsAt) {
    Table t;
    Card c_num = { SPADES, ACE };
    Card c_face = { HEARTS, KING };
    CaravanName pn = CARAVAN_A;

    t.play_numeric_card(pn, c_num);
    t.play_face_card(pn, c_face, 1);

    TrackSlot ts = t.get_caravan_cards_at(pn, 1);

    ASSERT_EQ(ts.card.suit, c_num.suit);
    ASSERT_EQ(ts.card.rank, c_num.rank);

    ASSERT_EQ(ts.i_faces, 1);
    ASSERT_EQ(ts.faces[0].suit, c_face.suit);
    ASSERT_EQ(ts.faces[0].rank, c_face.rank);
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

TEST (TestGameModelTable, GetCaravanBid_TwoNumeric_ThreeKings) {
    Table t;
    Card c_num_1 = { SPADES, TEN };
    Card c_num_2 = { SPADES, NINE };
    Card c_face_1 = { HEARTS, KING };
    Card c_face_2 = { CLUBS, KING };
    Card c_face_3 = { DIAMONDS, KING };
    CaravanName pn = CARAVAN_A;

    ASSERT_EQ(t.get_caravan_bid(pn), 0);
    t.play_numeric_card(pn, c_num_1);
    ASSERT_EQ(t.get_caravan_bid(pn), 10);
    t.play_face_card(pn, c_face_1, 1);
    ASSERT_EQ(t.get_caravan_bid(pn), 20);
    t.play_face_card(pn, c_face_2, 1);
    ASSERT_EQ(t.get_caravan_bid(pn), 30);

    t.play_numeric_card(pn, c_num_2);
    ASSERT_EQ(t.get_caravan_bid(pn), 39);
    t.play_face_card(pn, c_face_3, 2);
    ASSERT_EQ(t.get_caravan_bid(pn), 48);
}

TEST (TestGameModelTable, GetCaravanDirectionBeforeAfter_Ascending) {
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

TEST (TestGameModelTable, GetCaravanDirectionBeforeAfter_Descending) {
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

TEST (TestGameModelTable, CaravanSizeOneBeforeAfter) {
    Table t;
    Card c_num = { SPADES, ACE };
    CaravanName pn = CARAVAN_A;

    ASSERT_EQ(t.get_caravan_size(pn), 0);
    t.play_numeric_card(pn, c_num);
    ASSERT_EQ(t.get_caravan_size(pn), 1);
}

TEST (TestGameModelTable, GetCaravanSuitBeforeAfter) {
    Table t;
    Card c_num = { SPADES, ACE };
    CaravanName pn = CARAVAN_A;

    ASSERT_EQ(t.get_caravan_suit(pn), NO_SUIT);
    t.play_numeric_card(pn, c_num);
    ASSERT_EQ(t.get_caravan_suit(pn), c_num.suit);
}

TEST (TestGameModelTable, Scenario_Joker_Ace) {
    Table t;

    t.play_numeric_card(CARAVAN_A, {SPADES, ACE });
    t.play_numeric_card(CARAVAN_A, {CLUBS, TWO });
    t.play_numeric_card(CARAVAN_A, {SPADES, THREE });
    t.play_numeric_card(CARAVAN_A, {CLUBS, FOUR });

    t.play_numeric_card(CARAVAN_B, {CLUBS, ACE });
    t.play_numeric_card(CARAVAN_B, {DIAMONDS, FIVE });
    t.play_numeric_card(CARAVAN_B, {HEARTS, TEN });

    t.play_numeric_card(CARAVAN_C, {CLUBS, FIVE });

    ASSERT_EQ(t.get_caravan_size(CARAVAN_A), 4);
    ASSERT_EQ(t.get_caravan_size(CARAVAN_B), 3);
    ASSERT_EQ(t.get_caravan_size(CARAVAN_C), 1);

    t.play_face_card(CARAVAN_B, {NO_SUIT, JOKER }, 1);

    ASSERT_EQ(t.get_caravan_size(CARAVAN_A), 2);
    ASSERT_EQ(t.get_caravan_size(CARAVAN_B), 3);
    ASSERT_EQ(t.get_caravan_size(CARAVAN_C), 0);
}

TEST (TestGameModelTable, Scenario_Joker_2to10) {
    Table t;

    t.play_numeric_card(CARAVAN_A, {SPADES, ACE });
    t.play_numeric_card(CARAVAN_A, {CLUBS, TWO });
    t.play_numeric_card(CARAVAN_A, {SPADES, THREE });
    t.play_numeric_card(CARAVAN_A, {CLUBS, FOUR });

    t.play_numeric_card(CARAVAN_B, {CLUBS, ACE });
    t.play_numeric_card(CARAVAN_B, {DIAMONDS, FIVE });
    t.play_numeric_card(CARAVAN_B, {HEARTS, TEN });

    t.play_numeric_card(CARAVAN_C, {CLUBS, FIVE });

    ASSERT_EQ(t.get_caravan_size(CARAVAN_A), 4);
    ASSERT_EQ(t.get_caravan_size(CARAVAN_B), 3);
    ASSERT_EQ(t.get_caravan_size(CARAVAN_C), 1);

    t.play_face_card(CARAVAN_C, {NO_SUIT, JOKER }, 1);

    ASSERT_EQ(t.get_caravan_size(CARAVAN_A), 4);
    ASSERT_EQ(t.get_caravan_size(CARAVAN_B), 2);
    ASSERT_EQ(t.get_caravan_size(CARAVAN_C), 1);
}

TEST (TestGameModelTable, Scenario_Jack) {
    Table t;

    t.play_numeric_card(CARAVAN_A, {SPADES, ACE });
    t.play_numeric_card(CARAVAN_A, {CLUBS, TWO });
    t.play_numeric_card(CARAVAN_A, {SPADES, THREE });

    ASSERT_EQ(t.get_caravan_size(CARAVAN_A), 3);

    t.play_face_card(CARAVAN_A, {SPADES, JACK }, 1);

    ASSERT_EQ(t.get_caravan_size(CARAVAN_A), 2);

    ASSERT_EQ(t.get_caravan_cards_at(CARAVAN_A, 1).card.suit, CLUBS);
    ASSERT_EQ(t.get_caravan_cards_at(CARAVAN_A, 1).card.rank, TWO);

    ASSERT_EQ(t.get_caravan_cards_at(CARAVAN_A, 2).card.suit, SPADES);
    ASSERT_EQ(t.get_caravan_cards_at(CARAVAN_A, 2).card.rank, THREE);
}
