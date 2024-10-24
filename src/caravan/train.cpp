// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <iostream>
#include <chrono>
#include <memory>
#include <random>
#include <algorithm>
#include "cxxopts.hpp"
#include "caravan/core/common.h"
#include "caravan/model/game.h"
#include "caravan/core/training.h"

int main(int argc, char *argv[]) {
    // Game and config
    std::unique_ptr<Game> game;
    GameConfig gc;
    TrainConfig tc;
    uint8_t rand_first;

    // Training
    QTable q_table;
    ActionSpace action_space;

    // Random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    try {
        // Fill action space with all possible actions
        populate_action_space(&action_space);

        // Training parameters TODO user-defined arguments
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

        for(; tc.episode <= tc.episode_max; tc.episode++) {
            // Random first player
            rand_first = dist_first_player(gen);
            gc.player_first = rand_first == NUM_PLAYER_ABC ?
                PLAYER_ABC : PLAYER_DEF;

            // Set training parameters
            tc.discount = discount;
            tc.explore =
                (float) (tc.episode_max - (tc.episode - 1)) /
                (float) tc.episode_max;
            tc.learning = learning;

            // Start a new game
            game = std::make_unique<Game>(&gc);

            // Train on game until completion
            train_on_game(game.get(), q_table, action_space, tc, gen);

            // Close game
            game.reset();
        }

    } catch (CaravanException &e) {
        printf("%s\n", e.what().c_str());
        exit(EXIT_FAILURE);

    } catch (std::exception &e) {
        printf("%s\n", e.what());
        exit(EXIT_FAILURE);
    }
}
