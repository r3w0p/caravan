// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <algorithm>
#include "cxxopts.hpp"
#include "caravan/user/bot/ai_train.h"

const uint8_t FIRST_ABC = 1;
const uint8_t FIRST_DEF = 2;

int main(int argc, char *argv[]) {
    UserBotAITrain *user_abc;
    UserBotAITrain *user_def;
    UserBotAITrain *user_turn;
    Game *game;
    GameConfig gc;
    TrainConfig tc;
    uint8_t rand_first;

    // Random player selector
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr_first(FIRST_ABC, FIRST_DEF);

    try {
        // Training parameters
        // TODO user-defined arguments for these
        float discount = 0.95;
        float learning = 0.75;
        uint32_t episode_max = 10;

        // Game config uses largest deck with most samples and balance to
        // maximise chance of encountering every player hand combination.
        gc = {
            .player_abc_cards = DECK_CARAVAN_MAX,
            .player_abc_samples = SAMPLE_DECKS_MAX,
            .player_abc_balanced = true,
            .player_def_cards = DECK_CARAVAN_MAX,
            .player_def_samples = SAMPLE_DECKS_MAX,
            .player_def_balanced = true
        };

        // Train config is passed to bots to manage their training.
        tc = {
            .episode_max = episode_max,
            .episode = 1
        };

        user_abc = new UserBotAITrain(PLAYER_ABC);
        user_def = new UserBotAITrain(PLAYER_DEF);

        for(; tc.episode <= tc.episode_max; tc.episode++) {
            // Random first player
            rand_first = distr_first(gen);
            gc.player_first = rand_first == FIRST_ABC ? PLAYER_ABC : PLAYER_DEF;
            user_turn = rand_first == FIRST_ABC ? user_abc : user_def;

            // Set training parameters
            tc.discount = discount;
            tc.explore = (float) (tc.episode_max - (tc.episode - 1)) / (float) tc.episode_max;
            tc.learning = learning;

            // Start a new game
            game = new Game(&gc);

            // Take turns until a winner is declared
            while(game->get_winner() != NO_PLAYER) {
                // TODO borrow logic from other bot to determine if move is
                //  valid or not; use this to narrow down possible moves that
                //  can be made per game state; any issues with move when
                //  passing to game should result in fatal exception
                user_turn->make_move_train(game, &tc);

                // TODO convert string move to command: take functions to do
                //  this out of view tui; have single function to make this
                //  conversion and then pass it to the game

                if(user_turn->get_name() == PLAYER_ABC) {
                    user_turn = user_def;
                } else {
                    user_turn = user_abc;
                }
            }

            // TODO reward winner here?
            //  (or maybe bots should figure that out for themselves...)

            // Finish game
            game->close();
            delete game;
        }

    } catch (CaravanException &e) {
        printf("%s\n", e.what().c_str());
        exit(EXIT_FAILURE);

    } catch (std::exception &e) {
        printf("%s\n", e.what());
        exit(EXIT_FAILURE);
    }

    user_abc->close();
    user_def->close();

    delete user_abc;
    delete user_def;
}
