// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "gtest/gtest.h"
#include "caravan/core/exceptions.h"
#include "caravan/model/game.h"

using namespace Caravan;

class GameTest : public testing::Test {
    protected:
        const uint8_t F_NUM_CARDS = 54;
        const uint8_t F_NUM_SAMPLES = 1;
        const bool F_BALANCED_SAMPLE = true;

        // Unrelated to game below
        // Contains shuffled deck
        std::unique_ptr<Model::Player> plr_abc = std::make_unique<
            Model::Player>(
            Model::PLAYER_ABC,
            std::unique_ptr<Model::Deck>(
                Model::DeckBuilder::build_caravan_deck(
                    F_NUM_CARDS,
                    F_NUM_SAMPLES,
                    F_BALANCED_SAMPLE
                )
            )
        );

        // Unrelated to game below
        // Contains shuffled deck
        std::unique_ptr<Model::Player> plr_def = std::make_unique<
            Model::Player>(
            Model::PLAYER_DEF,
            std::unique_ptr<Model::Deck>(
                Model::DeckBuilder::build_caravan_deck(
                    F_NUM_CARDS,
                    F_NUM_SAMPLES,
                    F_BALANCED_SAMPLE
                )
            )
        );

        // Has its own two players
        // Both have unshuffled decks for deterministic gameplay testing
        Model::Game game = Model::Game(
            std::make_unique<
                Model::Player>(
                Model::PLAYER_ABC,
                std::unique_ptr<Model::Deck>(
                    Model::DeckBuilder::build_caravan_deck(
                        F_NUM_CARDS,
                        F_NUM_SAMPLES,
                        F_BALANCED_SAMPLE,
                        false
                    )
                )
            ),
            std::make_unique<
                Model::Player>(
                Model::PLAYER_DEF,
                std::unique_ptr<Model::Deck>(
                    Model::DeckBuilder::build_caravan_deck(
                        F_NUM_CARDS,
                        F_NUM_SAMPLES,
                        F_BALANCED_SAMPLE,
                        false
                    )

                )
            ),
            Model::PLAYER_ABC
        );

        Model::Table &tbl = game.get_table();

        Model::Caravan &cvn_a = tbl.get_caravan(Model::CARAVAN_A);
        Model::Caravan &cvn_b = tbl.get_caravan(Model::CARAVAN_B);
        Model::Caravan &cvn_c = tbl.get_caravan(Model::CARAVAN_C);
        Model::Caravan &cvn_d = tbl.get_caravan(Model::CARAVAN_D);
        Model::Caravan &cvn_e = tbl.get_caravan(Model::CARAVAN_E);
        Model::Caravan &cvn_f = tbl.get_caravan(Model::CARAVAN_F);

        Model::GameMove move_p_1_a = {
            Model::OPTION_PLAY,
            1,
            Model::CARAVAN_A,
            0
        };

        Model::GameMove move_p_2_a = {
            Model::OPTION_PLAY,
            2,
            Model::CARAVAN_A,
            0
        };

        Model::GameMove move_p_1_b = {
            Model::OPTION_PLAY,
            1,
            Model::CARAVAN_B,
            0
        };

        Model::GameMove move_p_4_b_1 = {
            Model::OPTION_PLAY,
            4,
            Model::CARAVAN_B,
            1
        };

        Model::GameMove move_p_1_c = {
            Model::OPTION_PLAY,
            1,
            Model::CARAVAN_C,
            0
        };

        Model::GameMove move_p_1_d = {
            Model::OPTION_PLAY,
            1,
            Model::CARAVAN_D,
            0
        };

        Model::GameMove move_p_2_d = {
            Model::OPTION_PLAY,
            2,
            Model::CARAVAN_D,
            0
        };

        Model::GameMove move_p_1_e = {
            Model::OPTION_PLAY,
            1,
            Model::CARAVAN_E,
            0
        };

        Model::GameMove move_p_4_e_1 = {
            Model::OPTION_PLAY,
            4,
            Model::CARAVAN_E,
            1
        };

        Model::GameMove move_p_1_f = {
            Model::OPTION_PLAY,
            1,
            Model::CARAVAN_F,
            0
        };

        Model::GameMove move_d_1 = {
            Model::OPTION_DISCARD,
            1,
            Model::NO_CARAVAN,
            0
        };

        Model::GameMove move_c_a = {
            Model::OPTION_CLEAR,
            0,
            Model::CARAVAN_A,
            0
        };

        Model::GameMove move_c_b = {
            Model::OPTION_CLEAR,
            0,
            Model::CARAVAN_B,
            0
        };

        Model::GameMove move_c_c = {
            Model::OPTION_CLEAR,
            0,
            Model::CARAVAN_C,
            0
        };

        Model::GameMove move_c_d = {
            Model::OPTION_CLEAR,
            0,
            Model::CARAVAN_D,
            0
        };

        Model::GameMove move_c_e = {
            Model::OPTION_CLEAR,
            0,
            Model::CARAVAN_E,
            0
        };

        Model::GameMove move_c_f = {
            Model::OPTION_CLEAR,
            0,
            Model::CARAVAN_F,
            0
        };

        explicit GameTest() = default;
};

TEST_F(GameTest, Error_Constructor_InvalidPlayerNameABC) {
    std::unique_ptr<Model::Player> err_plr_abc = std::make_unique<
        Model::Player>(
        Model::NO_PLAYER,
        std::unique_ptr<Model::Deck>(
            Model::DeckBuilder::build_caravan_deck(
                F_NUM_CARDS,
                F_NUM_SAMPLES,
                F_BALANCED_SAMPLE
            )
        )
    );

    try {
        Model::Game game = Model::Game(
            std::move(err_plr_abc),
            std::move(plr_def),
            Model::PLAYER_ABC
        );
        FAIL();
    } catch (CaravanFatalModelException &) {
        SUCCEED();
    } catch (...) {
        FAIL();
    }
}

TEST_F(GameTest, Error_Constructor_InvalidPlayerNameDEF) {
    std::unique_ptr<Model::Player> err_plr_def = std::make_unique<
        Model::Player>(
        Model::NO_PLAYER,
        std::unique_ptr<Model::Deck>(
            Model::DeckBuilder::build_caravan_deck(
                F_NUM_CARDS,
                F_NUM_SAMPLES,
                F_BALANCED_SAMPLE
            )
        )
    );

    try {
        Model::Game game = Model::Game(
            std::move(plr_abc),
            std::move(err_plr_def),
            Model::PLAYER_ABC
        );
        FAIL();
    } catch (CaravanFatalModelException &) {
        SUCCEED();
    } catch (...) {
        FAIL();
    }
}

TEST_F(GameTest, Error_Constructor_InvalidPlayerFirst) {
    try {
        Model::Game game = Model::Game(
            std::move(plr_abc),
            std::move(plr_def),
            Model::NO_PLAYER
        );
        FAIL();
    } catch (CaravanFatalModelException &) {
        SUCCEED();
    } catch (...) {
        FAIL();
    }
}

TEST_F(GameTest, Constructor_PlayerFirstABC) {
    Model::Game game = Model::Game(
        std::move(plr_abc),
        std::move(plr_def),
        Model::PLAYER_ABC
    );

    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);
}

TEST_F(GameTest, Constructor_PlayerFirstDEF) {
    Model::Game game = Model::Game(
        std::move(plr_abc),
        std::move(plr_def),
        Model::PLAYER_DEF
    );

    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_DEF);
}

TEST_F(GameTest, GetPlayer_Both) {
    ASSERT_EQ(game.get_player(Model::PLAYER_ABC).get_name(), Model::PLAYER_ABC);
    ASSERT_EQ(game.get_player(Model::PLAYER_DEF).get_name(), Model::PLAYER_DEF);
}

TEST_F(GameTest, Error_GetPlayer_InvalidPlayerName) {
    try {
        Model::Player &plr = game.get_player(Model::NO_PLAYER);
        FAIL();
    } catch (CaravanFatalModelException &) {
        SUCCEED();
    } catch (...) {
        FAIL();
    }
}

TEST_F(GameTest, GetPlayerCaravanNames_Both) {
    Model::PlayerCaravanNames pcvnames_abc = game.get_player_caravan_names(
        Model::PLAYER_ABC
    );

    ASSERT_EQ(pcvnames_abc.size(), 3);
    ASSERT_EQ(pcvnames_abc[0], Model::CARAVAN_A);
    ASSERT_EQ(pcvnames_abc[1], Model::CARAVAN_B);
    ASSERT_EQ(pcvnames_abc[2], Model::CARAVAN_C);

    Model::PlayerCaravanNames pcvnames_def = game.get_player_caravan_names(
        Model::PLAYER_DEF
    );

    ASSERT_EQ(pcvnames_def.size(), 3);
    ASSERT_EQ(pcvnames_def[0], Model::CARAVAN_D);
    ASSERT_EQ(pcvnames_def[1], Model::CARAVAN_E);
    ASSERT_EQ(pcvnames_def[2], Model::CARAVAN_F);
}

TEST_F(GameTest, Error_GetPlayerCaravanNames_InvalidPlayerName) {
    try {
        Model::PlayerCaravanNames pcvnames = game.get_player_caravan_names(
            Model::NO_PLAYER
        );
        FAIL();
    } catch (CaravanFatalModelException &) {
        SUCCEED();
    } catch (...) {
        FAIL();
    }
}

TEST_F(GameTest, IsCaravanBust_NoMoves) {
    ASSERT_FALSE(game.is_caravan_bust(Model::CARAVAN_A));
    ASSERT_FALSE(game.is_caravan_bust(Model::CARAVAN_B));
    ASSERT_FALSE(game.is_caravan_bust(Model::CARAVAN_C));
    ASSERT_FALSE(game.is_caravan_bust(Model::CARAVAN_D));
    ASSERT_FALSE(game.is_caravan_bust(Model::CARAVAN_E));
    ASSERT_FALSE(game.is_caravan_bust(Model::CARAVAN_F));
}

TEST_F(GameTest, Error_IsCaravanBust_InvalidCaravanName) {
    try {
        bool bust = game.is_caravan_bust(Model::NO_CARAVAN);
        FAIL();
    } catch (CaravanFatalModelException &) {
        SUCCEED();
    } catch (...) {
        FAIL();
    }
}

TEST_F(GameTest, IsCaravanWinning_NoMoves) {
    ASSERT_FALSE(game.is_caravan_winning(Model::CARAVAN_A));
    ASSERT_FALSE(game.is_caravan_winning(Model::CARAVAN_B));
    ASSERT_FALSE(game.is_caravan_winning(Model::CARAVAN_C));
    ASSERT_FALSE(game.is_caravan_winning(Model::CARAVAN_D));
    ASSERT_FALSE(game.is_caravan_winning(Model::CARAVAN_E));
    ASSERT_FALSE(game.is_caravan_winning(Model::CARAVAN_F));
}

TEST_F(GameTest, Error_IsCaravanWinning_InvalidCaravanName) {
    try {
        bool winning = game.is_caravan_winning(Model::NO_CARAVAN);
        FAIL();
    } catch (CaravanFatalModelException &) {
        SUCCEED();
    } catch (...) {
        FAIL();
    }
}

TEST_F(GameTest, GetWinner_NoMoves) {
    ASSERT_EQ(game.get_winner(), Model::NO_PLAYER);
}

TEST_F(GameTest, MakeMove_StartRound_FirstMove) {
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);
    ASSERT_EQ(cvn_a.get_size(), 0);

    game.make_move(move_p_1_a);

    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_DEF);
    ASSERT_EQ(cvn_a.get_size(), 1);
    ASSERT_EQ(cvn_a.get_bid(), 1);

    ASSERT_EQ(cvn_a.get_slot(1).card.rank, Model::ACE);
    ASSERT_EQ(game.get_winner(), Model::NO_PLAYER);
}

TEST_F(GameTest, MakeMove_StartRound_ToPost) {
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    // Make moves until post-start
    game.make_move(move_p_1_a);
    game.make_move(move_p_1_d);

    game.make_move(move_p_1_b);
    game.make_move(move_p_1_e);

    game.make_move(move_p_1_c);
    game.make_move(move_p_1_f);

    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    // A: ACE
    ASSERT_EQ(cvn_a.get_size(), 1);
    ASSERT_EQ(cvn_a.get_bid(), 1);

    // B: TWO
    ASSERT_EQ(cvn_b.get_size(), 1);
    ASSERT_EQ(cvn_b.get_bid(), 2);

    // C: THREE
    ASSERT_EQ(cvn_c.get_size(), 1);
    ASSERT_EQ(cvn_c.get_bid(), 3);

    // D: ACE
    ASSERT_EQ(cvn_d.get_size(), 1);
    ASSERT_EQ(cvn_d.get_bid(), 1);

    // E: TWO
    ASSERT_EQ(cvn_e.get_size(), 1);
    ASSERT_EQ(cvn_e.get_bid(), 2);

    // F: THREE
    ASSERT_EQ(cvn_f.get_size(), 1);
    ASSERT_EQ(cvn_f.get_bid(), 3);
}

TEST_F(GameTest, MakeMove_GetWinner_FirstPlayer) {
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    // Make moves until post-start
    game.make_move(move_p_1_a);
    game.make_move(move_p_1_d); // D: ACE (1)

    game.make_move(move_p_1_b); // B: TWO (2)
    game.make_move(move_p_1_e);

    game.make_move(move_p_1_c); // C: THREE (3)
    game.make_move(move_p_1_f);

    // ABC to win B and C, DEF to win D

    game.make_move(move_p_1_b); // B: TWO, FOUR (6)
    game.make_move(move_p_1_d); // D: ACE, FOUR (5)

    game.make_move(move_p_1_c); // C: THREE, FIVE (8)
    game.make_move(move_p_1_d); // D: ACE, FOUR, FIVE (10)

    game.make_move(move_p_1_b); // B: TWO, FOUR, SIX (12)
    game.make_move(move_p_1_d); // D: ACE, FOUR, FIVE, SIX (16)

    game.make_move(move_p_1_c); // C: THREE, FIVE, SEVEN (15)
    game.make_move(move_p_1_d); // D: ACE, FOUR, FIVE, SIX, SEVEN (23)

    ASSERT_TRUE(game.is_caravan_winning(Model::CARAVAN_D));
    ASSERT_FALSE(game.is_caravan_winning(Model::CARAVAN_A));
    ASSERT_EQ(game.get_winner(), Model::NO_PLAYER);

    game.make_move(move_p_1_b); // B: TWO, FOUR, SIX, EIGHT (20)
    game.make_move(move_d_1);

    game.make_move(move_p_1_c); // C: THREE, FIVE, SEVEN, NINE (24)
    game.make_move(move_d_1);

    ASSERT_TRUE(game.is_caravan_winning(Model::CARAVAN_C));
    ASSERT_FALSE(game.is_caravan_winning(Model::CARAVAN_F));
    ASSERT_EQ(game.get_winner(), Model::NO_PLAYER);

    game.make_move(move_p_4_b_1); // B: TWO [K], FOUR, SIX, EIGHT (22)

    ASSERT_TRUE(game.is_caravan_winning(Model::CARAVAN_B));
    ASSERT_FALSE(game.is_caravan_winning(Model::CARAVAN_E));
    ASSERT_EQ(game.get_winner(), Model::PLAYER_ABC);
}

TEST_F(GameTest, MakeMove_GetWinner_SecondPlayer) {
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    // Make moves until post-start
    game.make_move(move_p_1_a); // A: ACE (1)
    game.make_move(move_p_1_d);

    game.make_move(move_p_1_b);
    game.make_move(move_p_1_e); // B: TWO (2)

    game.make_move(move_p_1_c);
    game.make_move(move_p_1_f); // C: THREE (3)

    // ABC to win A, DEF to win E and F

    game.make_move(move_p_1_a); // A: ACE, FOUR (5)
    game.make_move(move_p_1_e); // E: TWO, FOUR (6)

    game.make_move(move_p_1_a); // A: ACE, FOUR, FIVE (10)
    game.make_move(move_p_1_f); // F: THREE, FIVE (8)

    game.make_move(move_p_1_a); // A: ACE, FOUR, FIVE, SIX (16)
    game.make_move(move_p_1_e); // E: TWO, FOUR, SIX (12)

    game.make_move(move_p_1_a); // A: ACE, FOUR, FIVE, SIX, SEVEN (23)
    game.make_move(move_p_1_f); // F: THREE, FIVE, SEVEN (15)

    ASSERT_TRUE(game.is_caravan_winning(Model::CARAVAN_A));
    ASSERT_FALSE(game.is_caravan_winning(Model::CARAVAN_D));
    ASSERT_EQ(game.get_winner(), Model::NO_PLAYER);

    game.make_move(move_d_1);
    game.make_move(move_p_1_e); // E: TWO, FOUR, SIX, EIGHT (20)

    game.make_move(move_d_1);
    game.make_move(move_p_1_f); // F: THREE, FIVE, SEVEN, NINE (24)

    ASSERT_TRUE(game.is_caravan_winning(Model::CARAVAN_F));
    ASSERT_FALSE(game.is_caravan_winning(Model::CARAVAN_C));
    ASSERT_EQ(game.get_winner(), Model::NO_PLAYER);

    game.make_move(move_p_4_e_1); // E: TWO [K], FOUR, SIX, EIGHT (22)

    ASSERT_TRUE(game.is_caravan_winning(Model::CARAVAN_E));
    ASSERT_FALSE(game.is_caravan_winning(Model::CARAVAN_B));
    ASSERT_EQ(game.get_winner(), Model::PLAYER_DEF);
}

TEST_F(GameTest, MakeMove_BothCaravansSold_FirstPlayerHigher) {
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    // Make moves until post-start
    game.make_move(move_p_1_a); // A: ACE (1)
    game.make_move(move_p_1_d); // D: ACE (1)

    game.make_move(move_p_1_b);
    game.make_move(move_p_1_e);

    game.make_move(move_p_1_c);
    game.make_move(move_p_1_f);

    // ABC to win A, DEF to win E and F

    game.make_move(move_p_1_a); // A: ACE, FOUR (5)
    game.make_move(move_p_1_d); // D: ACE, FOUR (5)

    game.make_move(move_p_1_a); // A: ACE, FOUR, FIVE (10)
    game.make_move(move_p_1_d); // D: ACE, FOUR, FIVE (10)

    game.make_move(move_p_1_a); // A: ACE, FOUR, FIVE, SIX (16)
    game.make_move(move_p_1_d); // D: ACE, FOUR, FIVE, SIX (16)

    game.make_move(move_p_2_a); // A: ACE, FOUR, FIVE, SIX, EIGHT (24)
    game.make_move(move_p_1_d); // D: ACE, FOUR, FIVE, SIX, SEVEN (23)

    ASSERT_TRUE(game.is_caravan_winning(Model::CARAVAN_A));
    ASSERT_FALSE(game.is_caravan_winning(Model::CARAVAN_D));
    ASSERT_EQ(game.get_winner(), Model::NO_PLAYER);
}

TEST_F(GameTest, MakeMove_BothCaravansSold_SecondPlayerHigher) {
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    // Make moves until post-start
    game.make_move(move_p_1_a); // A: ACE (1)
    game.make_move(move_p_1_d); // D: ACE (1)

    game.make_move(move_p_1_b);
    game.make_move(move_p_1_e);

    game.make_move(move_p_1_c);
    game.make_move(move_p_1_f);

    // ABC to win A, DEF to win E and F

    game.make_move(move_p_1_a); // A: ACE, FOUR (5)
    game.make_move(move_p_1_d); // D: ACE, FOUR (5)

    game.make_move(move_p_1_a); // A: ACE, FOUR, FIVE (10)
    game.make_move(move_p_1_d); // D: ACE, FOUR, FIVE (10)

    game.make_move(move_p_1_a); // A: ACE, FOUR, FIVE, SIX (16)
    game.make_move(move_p_1_d); // D: ACE, FOUR, FIVE, SIX (16)

    game.make_move(move_p_1_a); // A: ACE, FOUR, FIVE, SIX, SEVEN (23)
    game.make_move(move_p_2_d); // D: ACE, FOUR, FIVE, SIX, EIGHT (24)

    ASSERT_TRUE(game.is_caravan_winning(Model::CARAVAN_D));
    ASSERT_FALSE(game.is_caravan_winning(Model::CARAVAN_A));
    ASSERT_EQ(game.get_winner(), Model::NO_PLAYER);
}

TEST_F(GameTest, MakeMove_GetWinner_FirstPlayer_SecondPlayerEmptyHand) {
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    // Make moves until post-start
    game.make_move(move_p_1_a);
    game.make_move(move_p_1_d);

    game.make_move(move_p_1_b);
    game.make_move(move_p_1_e);

    game.make_move(move_p_1_c);
    game.make_move(move_p_1_f);

    // Winner clears caravans while loser discards cards
    game.make_move(move_c_a);
    game.make_move(move_d_1);

    game.make_move(move_c_b);
    game.make_move(move_d_1);

    game.make_move(move_c_c);
    game.make_move(move_d_1);

    // Discard the rest of the loser's cards
    for (int i = 0; i < F_NUM_CARDS-6; i++) {
        game.make_move(move_d_1);
        game.make_move(move_d_1);
    }

    ASSERT_EQ(game.get_winner(), Model::PLAYER_ABC);
}

TEST_F(GameTest, Error_MakeMove_GameAlreadyWon) {
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    // Make moves until post-start
    game.make_move(move_p_1_a);
    game.make_move(move_p_1_d);

    game.make_move(move_p_1_b);
    game.make_move(move_p_1_e);

    game.make_move(move_p_1_c);
    game.make_move(move_p_1_f);

    // Winner clears caravans while loser discards cards
    game.make_move(move_c_a);
    game.make_move(move_d_1);

    game.make_move(move_c_b);
    game.make_move(move_d_1);

    game.make_move(move_c_c);
    game.make_move(move_d_1);

    // Discard the rest of the loser's cards
    for (int i = 0; i < F_NUM_CARDS-6; i++) {
        game.make_move(move_d_1);
        game.make_move(move_d_1);
    }

    ASSERT_EQ(game.get_winner(), Model::PLAYER_ABC);

    try {
        game.make_move(move_d_1);
        FAIL();
    } catch (CaravanFatalModelException &) {
        SUCCEED();
    } catch (...) {
        FAIL();
    }
}

TEST_F(GameTest, Error_MakeMove_ClearOpponentCaravan) {
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    // Make moves until post-start
    game.make_move(move_p_1_a);
    game.make_move(move_p_1_d);

    game.make_move(move_p_1_b);
    game.make_move(move_p_1_e);

    game.make_move(move_p_1_c);
    game.make_move(move_p_1_f);

    // Try to clear opponent's caravan
    try {
        game.make_move(move_c_d);
        FAIL();
    } catch (CaravanIllegalModelException &) {
        SUCCEED();
    } catch (...) {
        FAIL();
    }
}

TEST_F(GameTest, Error_MakeMove_PlayNumeralOnOpponentCaravan) {
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    // Make moves until post-start
    game.make_move(move_p_1_a);
    game.make_move(move_p_1_d);

    game.make_move(move_p_1_b);
    game.make_move(move_p_1_e);

    game.make_move(move_p_1_c);
    game.make_move(move_p_1_f);

    // Try to play a numeral card onto an opponent's caravan
    try {
        game.make_move(move_p_1_d);
        FAIL();
    } catch (CaravanIllegalModelException &) {
        SUCCEED();
    } catch (...) {
        FAIL();
    }
}

TEST_F(GameTest, Error_MakeMove_StartRound_TwoNumeralsOnSameCaravan) {
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    // Make moves until post-start
    game.make_move(move_p_1_a);
    game.make_move(move_p_1_d);

    // Try to play another numeral card on same caravan during start round
    try {
        game.make_move(move_p_1_a);
        FAIL();
    } catch (CaravanIllegalModelException &) {
        SUCCEED();
    } catch (...) {
        FAIL();
    }
}

TEST_F(GameTest, Error_MakeMove_StartRound_FaceCard) {
    Model::Deck *face_deck{};
    uint8_t face_pos{};
    bool found = false;

    // Generate a deck with a guaranteed face card in opening hand
    while (!found) {
        face_deck = Model::DeckBuilder::build_caravan_deck(
            F_NUM_CARDS,
            F_NUM_SAMPLES,
            F_BALANCED_SAMPLE
        );

        for (int pos = 1; pos <= Model::HAND_SIZE_MAX_START; pos++) {
            // Cards are added to the player's hand from the back of the deck
            if (face_deck->at(F_NUM_CARDS-pos).is_face_card()) {
                face_pos = pos;
                found = true;
                break;
            }
        }
    }

    // Put face deck into first player
    std::unique_ptr<Model::Player> face_plr_abc = std::make_unique<
        Model::Player>(
        Model::PLAYER_ABC,
        std::unique_ptr<Model::Deck>(face_deck)
    );

    Model::Game face_game = Model::Game(
        std::move(face_plr_abc),
        std::move(plr_def),
        Model::PLAYER_ABC
    );

    // First move will be the face card
    Model::GameMove face_move = {
        Model::OPTION_PLAY,
        face_pos,
        Model::CARAVAN_A,
        0
    };

    // Try to play face card in start round
    try {
        face_game.make_move(face_move);
        FAIL();
    } catch (CaravanIllegalModelException &) {
        SUCCEED();
    } catch (...) {
        FAIL();
    }
}

TEST_F(GameTest, MakeMove_GetWinner_FirstPlayer_FirstPlayerEmptyHand) {
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    // Make moves until post-start
    game.make_move(move_p_1_a);
    game.make_move(move_p_1_d);

    game.make_move(move_p_1_b);
    game.make_move(move_p_1_e);

    game.make_move(move_p_1_c);
    game.make_move(move_p_1_f);

    // Winner clears caravans while loser discards cards
    game.make_move(move_d_1);
    game.make_move(move_c_d);

    game.make_move(move_d_1);
    game.make_move(move_c_e);

    game.make_move(move_d_1);
    game.make_move(move_c_f);

    // Discard the rest of the loser's cards
    for (int i = 0; i < F_NUM_CARDS-7; i++) {
        game.make_move(move_d_1);
        game.make_move(move_d_1);
    }
    game.make_move(move_d_1);

    ASSERT_EQ(game.get_winner(), Model::PLAYER_DEF);
}

TEST_F(GameTest, Error_MakeMove_NoOption) {
    Model::GameMove move = {
        Model::NO_OPTION,
        1,
        Model::CARAVAN_A,
        0
    };

    try {
        game.make_move(move);
        FAIL();
    } catch (CaravanFatalModelException &) {
        SUCCEED();
    } catch (...) {
        FAIL();
    }
}

TEST_F(GameTest, Error_MakeMove_StartRound_Remove) {
    try {
        game.make_move(move_d_1);
        FAIL();
    } catch (CaravanIllegalModelException &) {
        SUCCEED();
    } catch (...) {
        FAIL();
    }
}

TEST_F(GameTest, Error_MakeMove_StartRound_Clear) {
    try {
        game.make_move(move_c_a);
        FAIL();
    } catch (CaravanIllegalModelException &) {
        SUCCEED();
    } catch (...) {
        FAIL();
    }
}
