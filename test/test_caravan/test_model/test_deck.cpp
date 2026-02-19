// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "gtest/gtest.h"
#include "caravan/model/deck.h"
#include "caravan/core/exceptions.h"

using namespace Caravan;

TEST(DeckTest, CaravanDeck_30Cards_1SampleDeck_Balanced_Shuffled) {
    Model::Deck *d = Model::DeckBuilder::build_caravan_deck(30, 1, true, true);
    uint8_t sum_num = 0;

    ASSERT_EQ(d->size(), 30);

    for (int i = 0; i < 8; ++i) {
        Model::Card c_back = d->back();

        if (c_back.is_numeral_card()) {
            sum_num += 1;
        }

        d->pop_back();
    }

    ASSERT_GE(sum_num, 3);
}

TEST(DeckTest, CaravanDeck_30Cards_1SampleDeck_Balanced_Unshuffled) {
    Model::Deck *d = Model::DeckBuilder::build_caravan_deck(30, 1, true, false);
    uint8_t sum_num = 0;

    ASSERT_EQ(d->size(), 30);

    for (int i = 0; i < 8; ++i) {
        Model::Card c_back = d->back();

        if (c_back.is_numeral_card()) {
            sum_num += 1;
        }

        d->pop_back();
    }

    ASSERT_GE(sum_num, 3);
}

TEST(DeckTest, CaravanDeck_30Cards_1SampleDeck_Balanced) {
    Model::Deck *d = Model::DeckBuilder::build_caravan_deck(30, 1, true);
    uint8_t sum_num = 0;

    ASSERT_EQ(d->size(), 30);

    for (int i = 0; i < 8; ++i) {
        Model::Card c_back = d->back();

        if (c_back.is_numeral_card()) {
            sum_num += 1;
        }

        d->pop_back();
    }

    ASSERT_GE(sum_num, 3);
}

TEST(DeckTest, CaravanDeck_30Cards_1SampleDeck_NotBalanced) {
    Model::Deck *d = Model::DeckBuilder::build_caravan_deck(30, 1, false);
    uint8_t sum_num = 0;

    ASSERT_EQ(d->size(), 30);

    for (int i = 0; i < 8; ++i) {
        Model::Card c_back = d->back();

        if (c_back.is_numeral_card()) {
            sum_num += 1;
        }

        d->pop_back();
    }

    ASSERT_GE(sum_num, 3);
}

TEST(DeckTest, CaravanDeck_90Cards_2SampleDecks_Balanced) {
    Model::Deck *d = Model::DeckBuilder::build_caravan_deck(90, 2, true);
    uint8_t sum_num = 0;

    ASSERT_EQ(d->size(), 90);

    for (int i = 0; i < 8; ++i) {
        Model::Card c_back = d->back();

        if (c_back.is_numeral_card()) {
            sum_num += 1;
        }

        d->pop_back();
    }

    ASSERT_GE(sum_num, 3);
}

TEST(DeckTest, CaravanDeck_90Cards_2SampleDecks_NotBalanced) {
    Model::Deck *d = Model::DeckBuilder::build_caravan_deck(90, 2, false);
    uint8_t sum_num = 0;

    ASSERT_EQ(d->size(), 90);

    for (int i = 0; i < 8; ++i) {
        Model::Card c_back = d->back();

        if (c_back.is_numeral_card()) {
            sum_num += 1;
        }

        d->pop_back();
    }

    ASSERT_GE(sum_num, 3);
}

TEST(DeckTest, CaravanDeck_162Cards_3SampleDecks_Balanced) {
    Model::Deck *d = Model::DeckBuilder::build_caravan_deck(162, 3, true);
    uint8_t sum_num = 0;

    ASSERT_EQ(d->size(), 162);

    for (int i = 0; i < 8; ++i) {
        Model::Card c_back = d->back();

        if (c_back.is_numeral_card()) {
            sum_num += 1;
        }

        d->pop_back();
    }

    ASSERT_GE(sum_num, 3);
}

TEST(DeckTest, CaravanDeck_162Cards_3SampleDecks_NotBalanced) {
    Model::Deck *d = Model::DeckBuilder::build_caravan_deck(162, 3, false);
    uint8_t sum_num = 0;

    ASSERT_EQ(d->size(), 162);

    for (int i = 0; i < 8; ++i) {
        Model::Card c_back = d->back();

        if (c_back.is_numeral_card()) {
            sum_num += 1;
        }

        d->pop_back();
    }

    ASSERT_GE(sum_num, 3);
}

TEST(DeckTest, Error_CaravanDeck_TooFewCards) {
    try {
        Model::DeckBuilder::build_caravan_deck(29, 1, false);
        FAIL();
    } catch (CaravanFatalModelException &) {
        SUCCEED();
    } catch (...) {
        FAIL();
    }
}

TEST(DeckTest, Error_CaravanDeck_TooManyCards) {
    try {
        Model::DeckBuilder::build_caravan_deck(157, 1, false);
        FAIL();
    } catch (CaravanFatalModelException &) {
        SUCCEED();
    } catch (...) {
        FAIL();
    }
}

TEST(DeckTest, Error_CaravanDeck_TooFewSampleDecks) {
    try {
        Model::DeckBuilder::build_caravan_deck(60, 0, false);
        FAIL();
    } catch (CaravanFatalModelException &) {
        SUCCEED();
    } catch (...) {
        FAIL();
    }
}

TEST(DeckTest, Error_CaravanDeck_TooManySampleDecks) {
    try {
        Model::DeckBuilder::build_caravan_deck(60, 4, false);
        FAIL();
    } catch (CaravanFatalModelException &) {
        SUCCEED();
    } catch (...) {
        FAIL();
    }
}

TEST(DeckTest, Error_CaravanDeck_InsufficientSampleCards) {
    try {
        Model::DeckBuilder::build_caravan_deck(120, 1, false);
        FAIL();
    } catch (CaravanFatalModelException &) {
        SUCCEED();
    } catch (...) {
        FAIL();
    }
}
