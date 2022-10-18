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
 * PILE
 */

TEST (TestGameModelPile, GetBid_ThreeNumeric) {
    Pile p = Pile(PILE_A);
    Card c_num_1 = { SPADES, ACE };
    Card c_num_2 = { SPADES, TWO };
    Card c_num_3 = { SPADES, THREE };
    PileName pn = PILE_A;

    ASSERT_EQ(p.get_bid(), 0);
    p.put_numeric_card(c_num_1);
    ASSERT_EQ(p.get_bid(), 1);
    p.put_numeric_card(c_num_2);
    ASSERT_EQ(p.get_bid(), 3);
    p.put_numeric_card(c_num_3);
    ASSERT_EQ(p.get_bid(), 6);
}

TEST (TestGameModelPile, GetName) {
    Pile p = Pile(PILE_A);
    ASSERT_EQ(p.get_name(), PILE_A);
}

TEST (TestGameModelPile, GetDirectionBeforeAfter_Ascending) {
    Pile p = Pile(PILE_A);
    Card c_num_1 = { SPADES, ACE };
    Card c_num_2 = { SPADES, TWO };

    ASSERT_EQ(p.get_direction(), NO_DIRECTION);
    p.put_numeric_card(c_num_1);
    ASSERT_EQ(p.get_direction(), NO_DIRECTION);
    p.put_numeric_card(c_num_2);
    ASSERT_EQ(p.get_direction(), ASCENDING);
}

TEST (TestGameModelPile, GetDirectionBeforeAfter_Descending) {
    Pile p = Pile(PILE_A);
    Card c_num_1 = { SPADES, TWO };
    Card c_num_2 = { SPADES, ACE };

    ASSERT_EQ(p.get_direction(), NO_DIRECTION);
    p.put_numeric_card(c_num_1);
    ASSERT_EQ(p.get_direction(), NO_DIRECTION);
    p.put_numeric_card(c_num_2);
    ASSERT_EQ(p.get_direction(), DESCENDING);
}

TEST (TestGameModelPile, GetSuitBeforeAfter) {
    Pile p = Pile(PILE_A);
    Card c_num = { SPADES, ACE };

    ASSERT_EQ(p.get_suit(), NO_SUIT);
    p.put_numeric_card(c_num);
    ASSERT_EQ(p.get_suit(), c_num.suit);
}

TEST (TestGameModelPile, PutNumeric_PutFaceNotJack_GetCardsAt) {
    Pile p = Pile(PILE_A);
    Card c_num = { SPADES, ACE };
    Card c_face = { HEARTS, KING };

    p.put_numeric_card(c_num);
    p.put_face_card(c_face, 1);

    TrackSlot ts = p.get_cards_at(1);

    ASSERT_EQ(ts.card.suit, c_num.suit);
    ASSERT_EQ(ts.card.rank, c_num.rank);

    ASSERT_EQ(ts.i_faces, 1);
    ASSERT_EQ(ts.faces[0].suit, c_face.suit);
    ASSERT_EQ(ts.faces[0].rank, c_face.rank);
}

TEST (TestGameModelPile, RemoveAllCards_TwoNumeric_OneFace) {
    Pile p = Pile(PILE_A);
    Card c_num_1 = { SPADES, ACE };
    Card c_num_2 = { SPADES, TWO };
    Card c_face = { HEARTS, KING };

    p.put_numeric_card(c_num_1);
    p.put_numeric_card(c_num_2);
    p.put_face_card(c_face, 2);
    ASSERT_EQ(p.size(), 2);

    p.remove_all_cards();
    ASSERT_EQ(p.size(), 0);
}

TEST (TestGameModelPile, RemoveRank_FiveNumeric_OneFace) {
    Pile p = Pile(PILE_A);
    Card c_num_1 = { SPADES, ACE };
    Card c_num_2 = { HEARTS, TWO };
    Card c_num_3 = { CLUBS, FIVE };
    Card c_num_4 = { CLUBS, TWO };
    Card c_num_5 = { DIAMONDS, ACE };
    Card c_face = { HEARTS, KING };

    p.put_numeric_card(c_num_1);
    p.put_numeric_card(c_num_2);
    p.put_numeric_card(c_num_3);
    p.put_numeric_card(c_num_4);
    p.put_numeric_card(c_num_5);
    p.put_face_card(c_face, 4);
    ASSERT_EQ(p.size(), 5);

    p.remove_rank(ACE, 0);
    ASSERT_EQ(p.size(), 3);

    ASSERT_EQ(p.get_cards_at(1).card.suit, c_num_2.suit);
    ASSERT_EQ(p.get_cards_at(1).card.rank, c_num_2.rank);

    ASSERT_EQ(p.get_cards_at(2).card.suit, c_num_3.suit);
    ASSERT_EQ(p.get_cards_at(2).card.rank, c_num_3.rank);

    ASSERT_EQ(p.get_cards_at(3).card.suit, c_num_4.suit);
    ASSERT_EQ(p.get_cards_at(3).card.rank, c_num_4.rank);
}

TEST (TestGameModelPile, RemoveRank_FiveNumeric_OneFace_ExcludeOne) {
    Pile p = Pile(PILE_A);
    Card c_num_1 = { SPADES, ACE };
    Card c_num_2 = { HEARTS, TWO };
    Card c_num_3 = { CLUBS, FIVE };
    Card c_num_4 = { CLUBS, TWO };
    Card c_num_5 = { DIAMONDS, ACE };
    Card c_face = { HEARTS, KING };

    p.put_numeric_card(c_num_1);
    p.put_numeric_card(c_num_2);
    p.put_numeric_card(c_num_3);
    p.put_numeric_card(c_num_4);
    p.put_numeric_card(c_num_5);
    p.put_face_card(c_face, 4);
    ASSERT_EQ(p.size(), 5);

    p.remove_rank(ACE, 5);
    ASSERT_EQ(p.size(), 4);

    ASSERT_EQ(p.get_cards_at(1).card.suit, HEARTS);
    ASSERT_EQ(p.get_cards_at(1).card.rank, TWO);

    ASSERT_EQ(p.get_cards_at(2).card.suit, CLUBS);
    ASSERT_EQ(p.get_cards_at(2).card.rank, FIVE);

    ASSERT_EQ(p.get_cards_at(3).card.suit, CLUBS);
    ASSERT_EQ(p.get_cards_at(3).card.rank, TWO);

    ASSERT_EQ(p.get_cards_at(4).card.suit, DIAMONDS);
    ASSERT_EQ(p.get_cards_at(4).card.rank, ACE);
}

TEST (TestGameModelPile, RemoveSuit_FiveNumeric_OneFace) {
    Pile p = Pile(PILE_A);
    Card c_num_1 = { SPADES, ACE };
    Card c_num_2 = { HEARTS, TWO };
    Card c_num_3 = { CLUBS, FIVE };
    Card c_num_4 = { CLUBS, TWO };
    Card c_num_5 = { DIAMONDS, ACE };
    Card c_face = { HEARTS, KING };

    p.put_numeric_card(c_num_1);
    p.put_numeric_card(c_num_2);
    p.put_numeric_card(c_num_3);
    p.put_numeric_card(c_num_4);
    p.put_numeric_card(c_num_5);
    p.put_face_card(c_face, 4);
    ASSERT_EQ(p.size(), 5);

    p.remove_suit(CLUBS, 0);
    ASSERT_EQ(p.size(), 3);

    ASSERT_EQ(p.get_cards_at(1).card.suit, c_num_1.suit);
    ASSERT_EQ(p.get_cards_at(1).card.rank, c_num_1.rank);

    ASSERT_EQ(p.get_cards_at(2).card.suit, c_num_2.suit);
    ASSERT_EQ(p.get_cards_at(2).card.rank, c_num_2.rank);

    ASSERT_EQ(p.get_cards_at(3).card.suit, c_num_5.suit);
    ASSERT_EQ(p.get_cards_at(3).card.rank, c_num_5.rank);
}


TEST (TestGameModelPile, RemoveSuit_FiveNumeric_OneFace_ExcludeOne) {
    Pile p = Pile(PILE_A);
    Card c_num_1 = { SPADES, ACE };
    Card c_num_2 = { HEARTS, TWO };
    Card c_num_3 = { CLUBS, FIVE };
    Card c_num_4 = { CLUBS, TWO };
    Card c_num_5 = { DIAMONDS, ACE };
    Card c_face = { HEARTS, KING };

    p.put_numeric_card(c_num_1);
    p.put_numeric_card(c_num_2);
    p.put_numeric_card(c_num_3);
    p.put_numeric_card(c_num_4);
    p.put_numeric_card(c_num_5);
    p.put_face_card(c_face, 4);
    ASSERT_EQ(p.size(), 5);

    p.remove_suit(CLUBS, 3);
    ASSERT_EQ(p.size(), 4);

    ASSERT_EQ(p.get_cards_at(1).card.suit, SPADES);
    ASSERT_EQ(p.get_cards_at(1).card.rank, ACE);

    ASSERT_EQ(p.get_cards_at(2).card.suit, HEARTS);
    ASSERT_EQ(p.get_cards_at(2).card.rank, TWO);

    ASSERT_EQ(p.get_cards_at(3).card.suit, CLUBS);
    ASSERT_EQ(p.get_cards_at(3).card.rank, FIVE);

    ASSERT_EQ(p.get_cards_at(4).card.suit, DIAMONDS);
    ASSERT_EQ(p.get_cards_at(4).card.rank, ACE);
}

TEST (TestGameModelPile, SizeOneBeforeAfter) {
    Pile p = Pile(PILE_A);
    Card c_num = { SPADES, ACE };

    ASSERT_EQ(p.size(), 0);
    p.put_numeric_card(c_num);
    ASSERT_EQ(p.size(), 1);
}


/*
 * PLAYER
 */

TEST (TestGameModelPlayer, PopulateHand_30CardDeck_GetHand) {
    Deck* d = DeckBuilder::build_caravan_deck(30, 1, true);
    Player pl = Player(PLAYER_YOU, d);
    Card c_back = d->back();
    Card c_hand;

    ASSERT_EQ(pl.size_deck(), 30);
    ASSERT_EQ(pl.size_hand(), 0);

    pl.populate_hand();

    ASSERT_EQ(pl.size_deck(), 22);
    ASSERT_EQ(pl.size_hand(), 8);

    c_hand = pl.get_hand()[0];

    ASSERT_EQ(c_hand.suit, c_back.suit);
    ASSERT_EQ(c_hand.rank, c_back.rank);
}

TEST (TestGameModelPlayer, PopulateHand_30CardDeck_TakeFromHand) {
    Deck* d = DeckBuilder::build_caravan_deck(30, 1, true);
    Player pl = Player(PLAYER_YOU, d);
    Card c_get;
    Card c_take;
    Card c_getagain;

    pl.populate_hand();
    ASSERT_EQ(pl.size_hand(), 8);

    c_get = pl.get_hand()[0];
    c_take = pl.take_from_hand_at(1);

    ASSERT_EQ(pl.size_hand(), 7);
    ASSERT_TRUE(c_get.suit == c_take.suit and c_get.rank == c_take.rank);

    c_getagain = pl.get_hand()[0];
    ASSERT_TRUE(c_getagain.suit != c_take.suit or c_getagain.rank != c_take.rank);
}


/*
 * TABLE
 */

TEST (TestGameModelTable, ClearPile_TwoNumeric_OneFace) {
    Table t;
    Card c_num_1 = { SPADES, ACE };
    Card c_num_2 = { SPADES, TWO };
    Card c_face = { HEARTS, KING };
    PileName pn = PILE_A;

    t.play_numeric_card(pn, c_num_1);
    t.play_numeric_card(pn, c_num_2);
    t.play_face_card(pn, c_face, 2);
    ASSERT_EQ(t.get_pile_size(pn), 2);

    t.clear_pile(pn);
    ASSERT_EQ(t.get_pile_size(pn), 0);
}

TEST (TestGameModelTable, PlayNumeric_PlayFaceNotJack_GetPileCardsAt) {
    Table t;
    Card c_num = { SPADES, ACE };
    Card c_face = { HEARTS, KING };
    PileName pn = PILE_A;

    t.play_numeric_card(pn, c_num);
    t.play_face_card(pn, c_face, 1);

    TrackSlot ts = t.get_pile_cards_at(pn, 1);

    ASSERT_EQ(ts.card.suit, c_num.suit);
    ASSERT_EQ(ts.card.rank, c_num.rank);

    ASSERT_EQ(ts.i_faces, 1);
    ASSERT_EQ(ts.faces[0].suit, c_face.suit);
    ASSERT_EQ(ts.faces[0].rank, c_face.rank);
}

TEST (TestGameModelTable, GetPileBid_ThreeNumeric) {
    Table t;
    Card c_num_1 = { SPADES, ACE };
    Card c_num_2 = { SPADES, TWO };
    Card c_num_3 = { SPADES, THREE };
    PileName pn = PILE_A;

    ASSERT_EQ(t.get_pile_bid(pn), 0);
    t.play_numeric_card(pn, c_num_1);
    ASSERT_EQ(t.get_pile_bid(pn), 1);
    t.play_numeric_card(pn, c_num_2);
    ASSERT_EQ(t.get_pile_bid(pn), 3);
    t.play_numeric_card(pn, c_num_3);
    ASSERT_EQ(t.get_pile_bid(pn), 6);
}

TEST (TestGameModelTable, GetPileBid_TwoNumeric_ThreeKings) {
    Table t;
    Card c_num_1 = { SPADES, TEN };
    Card c_num_2 = { SPADES, NINE };
    Card c_face_1 = { HEARTS, KING };
    Card c_face_2 = { CLUBS, KING };
    Card c_face_3 = { DIAMONDS, KING };
    PileName pn = PILE_A;

    ASSERT_EQ(t.get_pile_bid(pn), 0);
    t.play_numeric_card(pn, c_num_1);
    ASSERT_EQ(t.get_pile_bid(pn), 10);
    t.play_face_card(pn, c_face_1, 1);
    ASSERT_EQ(t.get_pile_bid(pn), 20);
    t.play_face_card(pn, c_face_2, 1);
    ASSERT_EQ(t.get_pile_bid(pn), 30);

    t.play_numeric_card(pn, c_num_2);
    ASSERT_EQ(t.get_pile_bid(pn), 39);
    t.play_face_card(pn, c_face_3, 2);
    ASSERT_EQ(t.get_pile_bid(pn), 48);
}

TEST (TestGameModelTable, GetPileDirectionBeforeAfter_Ascending) {
    Table t;
    Card c_num_1 = { SPADES, ACE };
    Card c_num_2 = { SPADES, TWO };
    PileName pn = PILE_A;

    ASSERT_EQ(t.get_pile_direction(pn), NO_DIRECTION);
    t.play_numeric_card(pn, c_num_1);
    ASSERT_EQ(t.get_pile_direction(pn), NO_DIRECTION);
    t.play_numeric_card(pn, c_num_2);
    ASSERT_EQ(t.get_pile_direction(pn), ASCENDING);
}

TEST (TestGameModelTable, GetPileDirectionBeforeAfter_Descending) {
    Table t;
    Card c_num_1 = { SPADES, TWO };
    Card c_num_2 = { SPADES, ACE };
    PileName pn = PILE_A;

    ASSERT_EQ(t.get_pile_direction(pn), NO_DIRECTION);
    t.play_numeric_card(pn, c_num_1);
    ASSERT_EQ(t.get_pile_direction(pn), NO_DIRECTION);
    t.play_numeric_card(pn, c_num_2);
    ASSERT_EQ(t.get_pile_direction(pn), DESCENDING);
}

TEST (TestGameModelTable, PileSizeOneBeforeAfter) {
    Table t;
    Card c_num = { SPADES, ACE };
    PileName pn = PILE_A;

    ASSERT_EQ(t.get_pile_size(pn), 0);
    t.play_numeric_card(pn, c_num);
    ASSERT_EQ(t.get_pile_size(pn), 1);
}

TEST (TestGameModelTable, GetPileSuitBeforeAfter) {
    Table t;
    Card c_num = { SPADES, ACE };
    PileName pn = PILE_A;

    ASSERT_EQ(t.get_pile_suit(pn), NO_SUIT);
    t.play_numeric_card(pn, c_num);
    ASSERT_EQ(t.get_pile_suit(pn), c_num.suit);
}

TEST (TestGameModelTable, Scenario_Joker_Ace) {
    Table t;

    t.play_numeric_card(PILE_A, { SPADES, ACE });
    t.play_numeric_card(PILE_A, { CLUBS, TWO });
    t.play_numeric_card(PILE_A, { SPADES, THREE });
    t.play_numeric_card(PILE_A, { CLUBS, FOUR });

    t.play_numeric_card(PILE_B, { CLUBS, ACE });
    t.play_numeric_card(PILE_B, { DIAMONDS, FIVE });
    t.play_numeric_card(PILE_B, { HEARTS, TEN });

    t.play_numeric_card(PILE_C, { CLUBS, FIVE });

    ASSERT_EQ(t.get_pile_size(PILE_A), 4);
    ASSERT_EQ(t.get_pile_size(PILE_B), 3);
    ASSERT_EQ(t.get_pile_size(PILE_C), 1);

    t.play_face_card(PILE_B, { NO_SUIT, JOKER }, 1);

    ASSERT_EQ(t.get_pile_size(PILE_A), 2);
    ASSERT_EQ(t.get_pile_size(PILE_B), 3);
    ASSERT_EQ(t.get_pile_size(PILE_C), 0);
}

TEST (TestGameModelTable, Scenario_Joker_2to10) {
    Table t;

    t.play_numeric_card(PILE_A, { SPADES, ACE });
    t.play_numeric_card(PILE_A, { CLUBS, TWO });
    t.play_numeric_card(PILE_A, { SPADES, THREE });
    t.play_numeric_card(PILE_A, { CLUBS, FOUR });

    t.play_numeric_card(PILE_B, { CLUBS, ACE });
    t.play_numeric_card(PILE_B, { DIAMONDS, FIVE });
    t.play_numeric_card(PILE_B, { HEARTS, TEN });

    t.play_numeric_card(PILE_C, { CLUBS, FIVE });

    ASSERT_EQ(t.get_pile_size(PILE_A), 4);
    ASSERT_EQ(t.get_pile_size(PILE_B), 3);
    ASSERT_EQ(t.get_pile_size(PILE_C), 1);

    t.play_face_card(PILE_C, { NO_SUIT, JOKER }, 1);

    ASSERT_EQ(t.get_pile_size(PILE_A), 4);
    ASSERT_EQ(t.get_pile_size(PILE_B), 2);
    ASSERT_EQ(t.get_pile_size(PILE_C), 1);
}

TEST (TestGameModelTable, Scenario_Jack) {
    Table t;

    t.play_numeric_card(PILE_A, { SPADES, ACE });
    t.play_numeric_card(PILE_A, { CLUBS, TWO });
    t.play_numeric_card(PILE_A, { SPADES, THREE });

    ASSERT_EQ(t.get_pile_size(PILE_A), 3);

    t.play_face_card(PILE_A, { SPADES, JACK }, 1);

    ASSERT_EQ(t.get_pile_size(PILE_A), 2);

    ASSERT_EQ(t.get_pile_cards_at(PILE_A, 1).card.suit, CLUBS);
    ASSERT_EQ(t.get_pile_cards_at(PILE_A, 1).card.rank, TWO);

    ASSERT_EQ(t.get_pile_cards_at(PILE_A, 2).card.suit, SPADES);
    ASSERT_EQ(t.get_pile_cards_at(PILE_A, 2).card.rank, THREE);
}
