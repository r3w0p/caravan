// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>
#include "cxxopts.hpp"
#include "caravan/user/bot/train.h"

const uint8_t FIRST_ABC = 1;
const uint8_t FIRST_DEF = 2;

int main(int argc, char *argv[]) {
    UserBotTrain *user_train;
    PlayerName player_turn;
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

        // Single bot plays as both players and is trained on both.
        user_train = new UserBotTrain();

        for(; tc.episode <= tc.episode_max; tc.episode++) {
            // Random first player
            rand_first = distr_first(gen);
            gc.player_first = rand_first == FIRST_ABC ? PLAYER_ABC : PLAYER_DEF;

            // Set training parameters
            tc.discount = discount;
            tc.explore =
                (float) (tc.episode_max - (tc.episode - 1)) /
                (float) tc.episode_max;
            tc.learning = learning;

            // Start a new game
            game = new Game(&gc);

            // Take turns until a winner is declared
            while(game->get_winner() != NO_PLAYER) {
                user_train->make_move_train(game, &tc);
            }

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

    user_train->close();

    delete user_train;
}
