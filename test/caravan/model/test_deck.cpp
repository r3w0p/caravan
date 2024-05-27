// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "gtest/gtest.h"
#include "caravan/model/deck.h"
#include "caravan/core/exceptions.h"


TEST (TestDeck, CaravanDeck_30Cards_1SampleDeck_Balanced) {
    Deck *d = DeckBuilder::build_caravan_deck(30, 1, true);
    uint8_t sum_num = 0;
    Card c_back;

    ASSERT_EQ(d->size(), 30);

    for (int i = 0; i < 8; ++i) {
        c_back = d->back();

        if (is_numeral_card(c_back)) {
            sum_num += 1;
        }

        d->pop_back();
    }

    ASSERT_TRUE(sum_num >= 3);
}

TEST (TestDeck, CaravanDeck_30Cards_1SampleDeck_NotBalanced) {
    Deck *d = DeckBuilder::build_caravan_deck(30, 1, false);
    uint8_t sum_num = 0;
    Card c_back;

    ASSERT_EQ(d->size(), 30);

    for (int i = 0; i < 8; ++i) {
        c_back = d->back();

        if (is_numeral_card(c_back)) {
            sum_num += 1;
        }

        d->pop_back();
    }

    ASSERT_TRUE(sum_num >= 3);
}

TEST (TestDeck, CaravanDeck_90Cards_2SampleDecks_Balanced) {
    Deck *d = DeckBuilder::build_caravan_deck(90, 2, true);
    uint8_t sum_num = 0;
    Card c_back;

    ASSERT_EQ(d->size(), 90);

    for (int i = 0; i < 8; ++i) {
        c_back = d->back();

        if (is_numeral_card(c_back)) {
            sum_num += 1;
        }

        d->pop_back();
    }

    ASSERT_TRUE(sum_num >= 3);
}

TEST (TestDeck, CaravanDeck_90Cards_2SampleDecks_NotBalanced) {
    Deck *d = DeckBuilder::build_caravan_deck(90, 2, false);
    uint8_t sum_num = 0;
    Card c_back;

    ASSERT_EQ(d->size(), 90);

    for (int i = 0; i < 8; ++i) {
        c_back = d->back();

        if (is_numeral_card(c_back)) {
            sum_num += 1;
        }

        d->pop_back();
    }

    ASSERT_TRUE(sum_num >= 3);
}

TEST (TestDeck, CaravanDeck_162Cards_3SampleDecks_Balanced) {
    Deck *d = DeckBuilder::build_caravan_deck(162, 3, true);
    uint8_t sum_num = 0;
    Card c_back;

    ASSERT_EQ(d->size(), 162);

    for (int i = 0; i < 8; ++i) {
        c_back = d->back();

        if (is_numeral_card(c_back)) {
            sum_num += 1;
        }

        d->pop_back();
    }

    ASSERT_TRUE(sum_num >= 3);
}

TEST (TestDeck, CaravanDeck_162Cards_3SampleDecks_NotBalanced) {
    Deck *d = DeckBuilder::build_caravan_deck(162, 3, false);
    uint8_t sum_num = 0;
    Card c_back;

    ASSERT_EQ(d->size(), 162);

    for (int i = 0; i < 8; ++i) {
        c_back = d->back();

        if (is_numeral_card(c_back)) {
            sum_num += 1;
        }

        d->pop_back();
    }

    ASSERT_TRUE(sum_num >= 3);
}

TEST (TestDeck, CaravanDeck_Error_TooFewCards) {
    try {
        DeckBuilder::build_caravan_deck(29, 1, false);
        FAIL();

    } catch (CaravanFatalException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestDeck, CaravanDeck_Error_TooManyCards) {
    try {
        DeckBuilder::build_caravan_deck(157, 1, false);
        FAIL();

    } catch (CaravanFatalException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestDeck, CaravanDeck_Error_TooFewSampleDecks) {
    try {
        DeckBuilder::build_caravan_deck(60, 0, false);
        FAIL();

    } catch (CaravanFatalException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestDeck, CaravanDeck_Error_TooManySampleDecks) {
    try {
        DeckBuilder::build_caravan_deck(60, 4, false);
        FAIL();

    } catch (CaravanFatalException &e) {

    } catch (...) {
        FAIL();
    }
}

TEST (TestDeck, CaravanDeck_Error_InsufficientSampleCards) {
    try {
        DeckBuilder::build_caravan_deck(120, 1, false);
        FAIL();

    } catch (CaravanFatalException &e) {

    } catch (...) {
        FAIL();
    }
}