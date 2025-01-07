// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <memory>
#include <random>
#include <chrono>
#include <thread>
#include "cxxopts.hpp"
#include "caravan/model/game.h"
#include "caravan/core/training.h"

int main(int argc, char *argv[]) {
    // Game and config
    std::unique_ptr<Game> game = nullptr;
    GameConfig gc;
    TrainConfig tc;
    uint8_t rand_first;

    // Training
    QTable q_table;
    ActionSpace action_space;

    // Random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint8_t> dist_first_player(
        NUM_PLAYER_ABC, NUM_PLAYER_DEF);

    uint16_t checkpoint = 1000;
    uint16_t num_wins = 0;

    // Training parameters TODO user-defined arguments
    float discount = 0.95;
    float learning = 0.7;
    uint32_t episode_max = 1000000;

    try {
        // Fill action space with all possible actions
        populate_action_space(&action_space);

        // Game config uses largest deck with most samples and balance to
        // maximise chance of encountering every player hand combination.
        // TODO random card and sample sizes
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

            if (tc.episode > (tc.episode_max / 2))
                tc.explore =
                    static_cast<float>((tc.episode_max/2) - (tc.episode - (tc.episode_max/2) - 1)) /
                    static_cast<float>(tc.episode_max/2);
            else
                tc.explore = 1.0;

            tc.learning = learning;

            if (tc.episode % checkpoint == 0) {
                float per_wins = static_cast<float>(num_wins) / static_cast<float>(checkpoint);

                printf("Episode %d\n", tc.episode);
                printf("- explore: %.2f\n", tc.explore);
                printf("- states: %llu\n", q_table.size());
                printf("- wins: %d/%d (%.2f)\n", num_wins, checkpoint, per_wins);

                num_wins = 0;
            }

            // Start a new game
            game.reset(new Game(&gc));

            // Train on game until completion
            if (train_on_game(game.get(), q_table, action_space, gc, tc, gen))
                num_wins += 1;

            //printf("Winner: %s\n", game->get_winner() == PLAYER_ABC ? "ABC" : "DEF");

            //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            //printf("\n");
        }

    } catch (CaravanException &e) {
        printf("%s\n", e.what().c_str());
        exit(EXIT_FAILURE);

    } catch (std::exception &e) {
        printf("%s\n", e.what());
        exit(EXIT_FAILURE);
    }
}
