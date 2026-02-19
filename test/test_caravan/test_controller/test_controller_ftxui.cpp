// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "gtest/gtest.h"
#include "caravan/model/caravan.h"
#include "caravan/controller/controller_ftxui.h"
#include "caravan/core/exceptions.h"
#include "caravan/core/functions.h"

using namespace Caravan;


class ControllerTest : public testing::Test {
    protected:
        const uint8_t F_NUM_CARDS = 54;
        const uint8_t F_NUM_SAMPLES = 1;
        const bool F_BALANCED_SAMPLE = true;

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

        Controller::ControllerFTXUI ctrl = Controller::ControllerFTXUI(game);

        explicit ControllerTest() = default;
};


TEST_F(ControllerTest, Play_Numeral_Unconfirmed) {
    std::string raw_command = "P1A";
    bool confirmed = false;

    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    auto [gm, err] = ctrl.on_user_input(raw_command, confirmed);

    ASSERT_TRUE(err.empty());
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    ASSERT_EQ(cvn_a.get_size(), 0);

    ASSERT_EQ(gm.option, Model::OPTION_PLAY);
    ASSERT_EQ(gm.pos_hand, 1);
    ASSERT_EQ(gm.caravan_name, Model::CARAVAN_A);
    ASSERT_EQ(gm.pos_caravan, 0);
    ASSERT_EQ(gm.hand.rank, Model::NO_RANK);
    ASSERT_EQ(gm.hand.suit, Model::NO_RANK);
    ASSERT_EQ(gm.board.rank, Model::NO_RANK);
    ASSERT_EQ(gm.board.suit, Model::NO_RANK);
}

TEST_F(ControllerTest, Play_Numeral_Confirmed) {
    std::string raw_command = "P1A";
    bool confirmed = true;

    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    auto [gm, err] = ctrl.on_user_input(raw_command, confirmed);

    ASSERT_TRUE(err.empty());
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_DEF);

    ASSERT_EQ(cvn_a.get_size(), 1);

    ASSERT_EQ(gm.option, Model::OPTION_PLAY);
    ASSERT_EQ(gm.pos_hand, 1);
    ASSERT_EQ(gm.caravan_name, Model::CARAVAN_A);
    ASSERT_EQ(gm.pos_caravan, 0);
    ASSERT_EQ(gm.hand.rank, Model::ACE);
    ASSERT_NE(gm.hand.suit, Model::NO_RANK);
    ASSERT_EQ(gm.board.rank, Model::NO_RANK);
    ASSERT_EQ(gm.board.suit, Model::NO_RANK);
}

TEST_F(ControllerTest, Play_Numeral_AllValid_Unconfirmed) {
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    for (int pos = 1; pos <= Model::HAND_SIZE_MAX_START; pos++) {
        for (int i = Model::CARAVAN_A; i <= Model::CARAVAN_F; i++) {
            std::string raw_command =
                "P" +
                std::to_string(pos) +
                caravan_name_to_str(
                    static_cast<Model::CaravanName>(i), true
                );
            bool confirmed = false;

            auto [gm, err] = ctrl.on_user_input(raw_command, confirmed);

            ASSERT_TRUE(err.empty());
            ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);
        }
    }
}

TEST_F(ControllerTest, Error_Play_Numeral_NoHandPosition_Unconfirmed) {
    std::string raw_command = "P";
    bool confirmed = false;

    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    auto [gm, err] = ctrl.on_user_input(raw_command, confirmed);

    ASSERT_FALSE(err.empty());
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);
}

TEST_F(ControllerTest, Error_Play_Numeral_NoCaravanLetter_Unconfirmed) {
    std::string raw_command = "P1";
    bool confirmed = false;

    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    auto [gm, err] = ctrl.on_user_input(raw_command, confirmed);

    ASSERT_FALSE(err.empty());
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);
}

TEST_F(ControllerTest, Error_Play_Numeral_InvalidCaravanLetter_Unconfirmed) {
    std::string raw_command = "P1X";
    bool confirmed = false;

    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    auto [gm, err] = ctrl.on_user_input(raw_command, confirmed);

    ASSERT_FALSE(err.empty());
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);
}

TEST_F(ControllerTest, Play_Face_Unconfirmed) {
    std::string raw_command = "P1A5";
    bool confirmed = false;

    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    auto [gm, err] = ctrl.on_user_input(raw_command, confirmed);

    ASSERT_TRUE(err.empty());
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    ASSERT_EQ(cvn_a.get_size(), 0);

    ASSERT_EQ(gm.option, Model::OPTION_PLAY);
    ASSERT_EQ(gm.pos_hand, 1);
    ASSERT_EQ(gm.caravan_name, Model::CARAVAN_A);
    ASSERT_EQ(gm.pos_caravan, 5);
    ASSERT_EQ(gm.hand.rank, Model::NO_RANK);
    ASSERT_EQ(gm.hand.suit, Model::NO_RANK);
    ASSERT_EQ(gm.board.rank, Model::NO_RANK);
    ASSERT_EQ(gm.board.suit, Model::NO_RANK);
}

TEST_F(ControllerTest, Play_Face_AllValid_Unconfirmed) {
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    for (int pos = 1; pos <= Model::HAND_SIZE_MAX_START; pos++) {
        for (int i = Model::CARAVAN_A; i <= Model::CARAVAN_F; i++) {
            for (int t = 1; t <= Model::TRACK_NUMERIC_MAX; t++) {
                std::string raw_command =
                    "P" +
                    std::to_string(pos) +
                    caravan_name_to_str(
                        static_cast<Model::CaravanName>(i), true
                    ) +
                    std::to_string(t);
                bool confirmed = false;

                auto [gm, err] = ctrl.on_user_input(raw_command, confirmed);

                ASSERT_TRUE(err.empty());
                ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);
            }
        }
    }
}

TEST_F(ControllerTest, Error_Play_Face_CaravanPositionTooLow_Unconfirmed) {
    std::string raw_command = "P1A0";
    bool confirmed = false;

    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    auto [gm, err] = ctrl.on_user_input(raw_command, confirmed);

    ASSERT_FALSE(err.empty());
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);
}

TEST_F(ControllerTest, Error_Play_Face_CaravanPositionTooHigh_Unconfirmed) {
    std::string raw_command = "P1A9";
    bool confirmed = false;

    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    auto [gm, err] = ctrl.on_user_input(raw_command, confirmed);

    ASSERT_FALSE(err.empty());
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);
}

TEST_F(ControllerTest, Clear_Unconfirmed) {
    std::string raw_command = "CA";
    bool confirmed = false;

    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    auto [gm, err] = ctrl.on_user_input(raw_command, confirmed);

    ASSERT_TRUE(err.empty());
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    ASSERT_EQ(gm.option, Model::OPTION_CLEAR);
    ASSERT_EQ(gm.pos_hand, 0);
    ASSERT_EQ(gm.caravan_name, Model::CARAVAN_A);
    ASSERT_EQ(gm.pos_caravan, 0);
    ASSERT_EQ(gm.hand.rank, Model::NO_RANK);
    ASSERT_EQ(gm.hand.suit, Model::NO_RANK);
    ASSERT_EQ(gm.board.rank, Model::NO_RANK);
    ASSERT_EQ(gm.board.suit, Model::NO_RANK);
}

TEST_F(ControllerTest, Clear_AllValid_Unconfirmed) {
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    for (int i = Model::CARAVAN_A; i <= Model::CARAVAN_F; i++) {
        std::string raw_command =
            "C" +
            caravan_name_to_str(
                static_cast<Model::CaravanName>(i), true
            );
        bool confirmed = false;

        auto [gm, err] = ctrl.on_user_input(raw_command, confirmed);

        ASSERT_TRUE(err.empty());
        ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);
    }
}

TEST_F(ControllerTest, Error_Clear_UnknownCaravanLetter_Unconfirmed) {
    std::string raw_command = "CX";
    bool confirmed = false;

    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    auto [gm, err] = ctrl.on_user_input(raw_command, confirmed);

    ASSERT_FALSE(err.empty());
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);
}

TEST_F(ControllerTest, Error_Clear_NoCaravanLetter_Unconfirmed) {
    std::string raw_command = "C";
    bool confirmed = false;

    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    auto [gm, err] = ctrl.on_user_input(raw_command, confirmed);

    ASSERT_FALSE(err.empty());
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);
}

TEST_F(ControllerTest, Discard_Unconfirmed) {
    std::string raw_command = "D1";
    bool confirmed = false;

    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    auto [gm, err] = ctrl.on_user_input(raw_command, confirmed);

    ASSERT_TRUE(err.empty());
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    ASSERT_EQ(gm.option, Model::OPTION_DISCARD);
    ASSERT_EQ(gm.pos_hand, 1);
    ASSERT_EQ(gm.caravan_name, Model::NO_CARAVAN);
    ASSERT_EQ(gm.pos_caravan, 0);
    ASSERT_EQ(gm.hand.rank, Model::NO_RANK);
    ASSERT_EQ(gm.hand.suit, Model::NO_RANK);
    ASSERT_EQ(gm.board.rank, Model::NO_RANK);
    ASSERT_EQ(gm.board.suit, Model::NO_RANK);
}

TEST_F(ControllerTest, Discard_AllValid_Unconfirmed) {
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    for (int pos = 1; pos <= Model::HAND_SIZE_MAX_START; pos++) {
        std::string raw_command = "D" + std::to_string(pos);
        bool confirmed = false;

        auto [gm, err] = ctrl.on_user_input(raw_command, confirmed);

        ASSERT_TRUE(err.empty());
        ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);
    }
}

TEST_F(ControllerTest, Error_Discard_TooLow_Unconfirmed) {
    std::string raw_command = "D0";
    bool confirmed = false;

    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    auto [gm, err] = ctrl.on_user_input(raw_command, confirmed);

    ASSERT_FALSE(err.empty());
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);
}

TEST_F(ControllerTest, Error_Discard_TooHigh_Unconfirmed) {
    std::string raw_command = "D9";
    bool confirmed = false;

    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    auto [gm, err] = ctrl.on_user_input(raw_command, confirmed);

    ASSERT_FALSE(err.empty());
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);
}

TEST_F(ControllerTest, Error_Discard_NoHandPosition_Unconfirmed) {
    std::string raw_command = "D";
    bool confirmed = false;

    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    auto [gm, err] = ctrl.on_user_input(raw_command, confirmed);

    ASSERT_FALSE(err.empty());
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);
}

TEST_F(ControllerTest, Error_InvalidInput_Unconfirmed) {
    std::string raw_command = "BAD";
    bool confirmed = false;

    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    auto [gm, err] = ctrl.on_user_input(raw_command, confirmed);

    ASSERT_FALSE(err.empty());
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    ASSERT_EQ(gm.option, Model::NO_OPTION);
}

TEST_F(ControllerTest, Error_InvalidInput_Confirmed) {
    std::string raw_command = "BAD";
    bool confirmed = true;

    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    auto [gm, err] = ctrl.on_user_input(raw_command, confirmed);

    ASSERT_FALSE(err.empty());
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    ASSERT_EQ(gm.option, Model::NO_OPTION);
}

TEST_F(ControllerTest, EmptyInput_Unconfirmed) {
    std::string raw_command;
    bool confirmed = false;

    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    auto [gm, err] = ctrl.on_user_input(raw_command, confirmed);

    ASSERT_TRUE(err.empty());
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    ASSERT_EQ(gm.option, Model::NO_OPTION);
}

TEST_F(ControllerTest, EmptyInput_Confirmed) {
    std::string raw_command;
    bool confirmed = true;

    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    auto [gm, err] = ctrl.on_user_input(raw_command, confirmed);

    ASSERT_TRUE(err.empty());
    ASSERT_EQ(game.get_player_turn(), Model::PLAYER_ABC);

    ASSERT_EQ(gm.option, Model::NO_OPTION);
}
