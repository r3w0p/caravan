// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <random>
#include "caravan/core/exceptions.h"
#include "caravan/core/functions.h"
#include "caravan/user/bot/random.h"
#include "caravan/user/constants.h"

namespace Caravan::User {
    uint16_t UserBotRandom::check_index(const uint16_t &index) const {
        for (uint16_t i = 0; i < i_cache; ++i) {
            if (cache_moves[i] == index) {
                return false;
            }
        }

        return true;
    }

    void UserBotRandom::populate_moves(Model::PlayerCaravanNames pcvnames) {
        // Generate all moves
        uint16_t i_move = 0;

        // Clear moves (3)
        for (int i = 0; i < Model::PLAYER_CARAVANS_MAX; ++i) {
            all_moves[i_move++] = LETTER_CLEAR + caravan_name_to_str(pcvnames[i], true);
        }

        // Discard moves (5)
        for (int pos = Model::HAND_POS_MIN;
             pos <= Model::HAND_SIZE_MAX_POST_START; ++pos) {
            all_moves[i_move++] = LETTER_DISCARD + std::to_string(pos);
        }

        // Play (Face) moves (5 x 6 x 8 = 240)
        for (int pos1 = Model::HAND_POS_MIN;
             pos1 <= Model::HAND_SIZE_MAX_POST_START; ++pos1) {
            for (int cvn = Model::CARAVAN_A; cvn <= Model::CARAVAN_F; ++cvn) {
                for (int pos2 = Model::TRACK_NUMERIC_MIN;
                     pos2 <= Model::TRACK_NUMERIC_MAX; ++pos2) {
                    all_moves[i_move++] =
                        LETTER_PLAY +
                        std::to_string(pos1) +
                        caravan_name_to_str(static_cast<Model::CaravanName>(cvn), true) +
                        std::to_string(pos2);
                }
            }
        }

        // Play (Numeral) moves (8 x 3 = 24)
        for (int pos1 = Model::HAND_POS_MIN; pos1 <= Model::HAND_SIZE_MAX_START;
             ++pos1) {
            for (int i = 0; i < Model::PLAYER_CARAVANS_MAX; ++i) {
                all_moves[i_move++] =
                    LETTER_PLAY +
                    std::to_string(pos1) +
                    caravan_name_to_str(pcvnames[i], true);
            }
        }
    }

    std::string UserBotRandom::request_move(Model::Game *game) {
        Model::Player *player = game->get_player(name);
        uint16_t current_move_count = player->get_moves_count();

        // Player has never moved before
        // Generate all possible moves
        if (current_move_count == 0) {
            populate_moves(game->get_player_caravan_names(name));
        }

        // Player's last move was successful
        // Clear cache of previously attempted illegal moves
        if (current_move_count > last_move_count) {
            i_cache = 0;
        }

        bool index_ok = false;
        uint16_t index;

        // Randomly select a new move to make
        while (!index_ok) {
            // Select a new index at random
            index = distr(gen);

            // Make sure it hasn't already been attempted
            index_ok = check_index(index);
        }

        // Add selected move to cache
        cache_moves[i_cache++] = index;
        last_move_count = current_move_count;

        return all_moves[index];
    }
}