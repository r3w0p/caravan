// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/gym/environment_game.h"

#include <set>

#include "caravan/core/exceptions.h"
#include "caravan/user/user_bot_random.h"

namespace Caravan::Gym {

    const std::string ERR_RESET = "Environment must be reset before first use.";
    const std::string ERR_DONE = "Environment episode is already done.";

    constexpr uint8_t CARD_NUM_RANK_MIN = 0; // 0-9, 10-19, 20-29, 30-39
    constexpr uint8_t CARD_NUM_NO_RANK = 40;
    constexpr uint8_t CARD_NUM_JACK = 41;
    constexpr uint8_t CARD_NUM_KING = 42;
    constexpr uint8_t CARD_NUM_JOKER = 43;
    constexpr uint8_t CARD_NUM_QUEEN = 44;
    constexpr uint8_t CARD_NUM_QUEEN_MAX = 47;

    /*
     * PROTECTED
     */

    uint8_t EnvironmentGame::card_to_uint8_t(Model::Card card) {

        // Empty card space
        if (card.rank == Model::NO_RANK) {
            return CARD_NUM_NO_RANK;
        }

        // Suit of JACK, KING, JOKER does not matter
        // Not specifying it reduces action space
        if (card.rank == Model::JACK) {
            return CARD_NUM_JACK;
        }

        if (card.rank == Model::KING) {
            return CARD_NUM_KING;
        }

        if (card.rank == Model::JOKER) {
            return CARD_NUM_JOKER;
        }

        // Error if numeral or QUEEN card without suit
        if (card.suit == Model::NO_SUIT) {
            throw CaravanFatalGymException(
                "A suit must be specified for a numeral or QUEEN card."
            );
        }

        uint8_t suit_offset = card.suit - 1;

        // QUEEN suit important
        if (card.rank == Model::QUEEN) {
            return CARD_NUM_QUEEN + suit_offset;
        }

        // Numeral card
        return suit_offset * 10 + (card.rank - 1);
    }

    Model::Card EnvironmentGame::uint8_t_to_card(uint8_t card_num) {

        // Error if card number out of bounds
        if (card_num < CARD_NUM_RANK_MIN || card_num > CARD_NUM_QUEEN_MAX) {
            throw CaravanFatalGymException(
                "Card number " +
                std::to_string(card_num) +
                " out of bounds. Must be between " +
                std::to_string(CARD_NUM_RANK_MIN) +
                " and " +
                std::to_string(CARD_NUM_QUEEN_MAX) +
                " (inclusive)."
            );
        }

        // Empty card space
        if (card_num == CARD_NUM_NO_RANK) {
            return {.suit = Model::NO_SUIT, .rank = Model::NO_RANK};
        }

        // Suit of JACK, KING, JOKER not important
        // Not specifying it reduces state and action space
        if (card_num == CARD_NUM_JACK) {
            return {.suit = Model::NO_SUIT, .rank = Model::JACK};
        }

        if (card_num == CARD_NUM_KING) {
            return {.suit = Model::NO_SUIT, .rank = Model::KING};
        }

        if (card_num == CARD_NUM_JOKER) {
            return {.suit = Model::NO_SUIT, .rank = Model::JOKER};
        }

        // QUEEN suit important
        if (card_num >= CARD_NUM_QUEEN) {
            uint8_t suit_num = card_num - CARD_NUM_QUEEN + 1;

            return {
                .suit = static_cast<Model::Suit>(suit_num),
                .rank = Model::QUEEN
            };
        }

        // Numeral card
        return {
            .suit = static_cast<Model::Suit>(card_num / 10 + 1),
            .rank = static_cast<Model::Rank>(card_num % 10 + 1),
        };
    }

    Observation EnvironmentGame::make_observation() {
        Observation observation{};
        uint8_t i = 0;

        // Get agent hand as unique, sorted card numbers
        std::set<uint8_t> hand_set{};

        Model::Player &player = game->get_player(pname_agent);
        uint8_t hand_size = player.get_size_hand();
        const Model::Hand &hand = player.get_hand();

        for (int i_hand = 0; i_hand < hand_size; i_hand++) {
            hand_set.insert(card_to_uint8_t(hand[i_hand]));
        }

        // Add hand to observation
        for (uint8_t card_num : hand_set) {
            observation[i++] = card_num;
        }

        // Pad to max hand size, if needed
        for (uint8_t _ = 0; _ < Model::HAND_SIZE_MAX_START - hand_set.size(); _++) {
            observation[i++] = CARD_NUM_NO_RANK;
        }

        // Add caravan state to observation

        Model::Table& table = game->get_table();
        for (uint8_t i_cvname = 0; i_cvname < Model::TABLE_CARAVANS_MAX; i_cvname++) {
            Model::Caravan& cvn = table.get_caravan(cvnames_all_ordered[i_cvname]);

            observation[i++] = cvn.get_direction();
            observation[i++] = cvn.get_suit();

            uint8_t cvn_size = cvn.get_size();

            for (uint8_t i_track = 0; i_track < Model::TRACK_NUMERIC_MAX; i_track++) {
                // There is a numeral card in this slot
                if (i_track < cvn_size) {
                    Model::Slot slot = cvn.get_slot(i_track + 1);

                    // Add numeral card num
                    observation[i++] = slot.card.rank;

                    // Add face card info
                    uint8_t num_queens = 0;
                    uint8_t num_kings = 0;
                    uint8_t num_jokers = 0;

                    uint8_t i_last_queen = 0;
                    uint8_t last_queen = Model::NO_RANK;

                    for (uint8_t i_face = 0; i_face < slot.n_faces; i_face++) {
                        switch (slot.faces[i_face].rank) {
                            case Model::QUEEN:
                                num_queens++;
                                i_last_queen = i_face;
                                break;
                            case Model::KING:
                                num_kings++;
                                break;
                            case Model::JOKER:
                                num_jokers++;
                                break;
                            default:
                                break;
                        }
                    }

                    if (num_queens > 0) {
                        last_queen = card_to_uint8_t(slot.faces[i_last_queen]);
                    }

                    observation[i++] = num_queens;
                    observation[i++] = num_kings;
                    observation[i++] = num_jokers;
                    observation[i++] = last_queen;

                } else {  // Empty slot
                    // Pad for numeral card
                    observation[i++] = Model::NO_RANK;

                    // Pad for # QUEENS, # KINGS, # JOKERS, last QUEEN
                    observation[i++] = Model::NO_RANK;
                    observation[i++] = Model::NO_RANK;
                    observation[i++] = Model::NO_RANK;
                    observation[i++] = Model::NO_RANK;
                }
            }
        }

        return observation;
    }

    // TODO GameMove to unique Action string
    std::string EnvironmentGame::game_move_to_action_key(
        Model::GameMove move
    ) {
        std::string key{};

        // Option
        switch (move.option) {
            case Model::OPTION_PLAY:
                key += "P";
                break;
            case Model::OPTION_DISCARD:
                key += "D";
                break;
            case Model::OPTION_CLEAR:
                key += "C";
                break;
            default:
                break;
        }

        // Card in hand
        if (move.pos_hand > 0) {
            key += std::to_string(card_to_uint8_t(
                game->get_player(pname_agent).get_from_hand_at(move.pos_hand)
            ));
        }

        // Caravan name
        if (move.caravan_name != Model::NO_CARAVAN) {
            key += caravan_name_to_str(move.caravan_name, true);
        }

        // Position in caravan
        if (move.pos_caravan > 0) {
            key += std::to_string(move.pos_caravan);
        }

        return key;
    }

    // Get agent hand as unique, sorted card numbers
    std::vector<ActionMove> EnvironmentGame::get_valid_moves() {
        std::set<uint8_t> hand_set{};

        std::vector<ActionMove> moves{};

        Model::Player &player = game->get_player(pname_agent);
        uint8_t hand_size = player.get_size_hand();
        const Model::Hand &hand = player.get_hand();

        // Clear commands for own non-empty caravans
        for (uint8_t i_cvname = 0; i_cvname < Model::PLAYER_CARAVANS_MAX; i_cvname++) {
            Model::CaravanName cvname = cvnames_agent[i_cvname];
            Model::Caravan cvn = game->get_table().get_caravan(cvname);

            if (cvn.get_size() > 0) {
                moves.push_back({
                    .option = Model::OPTION_CLEAR,
                    .caravan_name = cvname
                });
            }
        }

        // Commands involving cards in own hand
        for (int i_hand = 0; i_hand < hand_size; i_hand++) {
            Model::Card card = hand[i_hand];
            uint8_t card_num = card_to_uint8_t(card);

            // Skip duplicate cards
            if (hand_set.contains(card_num)) {
                continue;
            }

            hand_set.insert(card_num);

            // Discard command
            moves.push_back({
                .option = Model::OPTION_DISCARD,
                .card_hand = card
            });

            // Play commands, numeral and face
            if (card.is_numeral_card()) {

                for (uint8_t i_cvname = 0; i_cvname < Model::PLAYER_CARAVANS_MAX; i_cvname++) {
                    Model::CaravanName cvname = cvnames_agent[i_cvname];
                    Model::Caravan cvn = game->get_table().get_caravan(cvname);

                    if (cvn.check_card(card)) {
                        moves.push_back({
                            .option = Model::OPTION_PLAY,
                            .card_hand = card,
                            .caravan_name = cvname
                        });
                    }
                }

            } else {

                for (uint8_t i_cvname = 0; i_cvname < Model::TABLE_CARAVANS_MAX; i_cvname++) {
                    Model::CaravanName cvname = cvnames_all_ordered[i_cvname];
                    Model::Caravan cvn = game->get_table().get_caravan(cvname);
                    uint8_t cvn_size = cvn.get_size();

                    for (uint8_t pos = 1; pos <= cvn_size; pos++) {

                        // Check face card can be applied at this position
                        if (cvn.check_card(card, pos)) {
                            moves.push_back({
                                .option = Model::OPTION_PLAY,
                                .card_hand = card,
                                .caravan_name = cvname,
                                .pos_caravan = pos
                            });
                        }

                    }
                }

            }
        }
    }

    /*
     * PUBLIC
     */

    EnvironmentGame::EnvironmentGame(
        User::BaseUser<std::string> &user_random
    ) : user_random(user_random) {

        pname_random = user_random.get_name();

        if (pname_random == Model::NO_PLAYER) {
            throw CaravanFatalGymException(
                "User random must have a player name.");
        }

        if (pname_random == Model::PLAYER_ABC) {
            pname_agent = Model::PLAYER_DEF;
        } else {
            pname_agent = Model::PLAYER_ABC;
        }

        // Get caravan names, and an array of them ordered by agent
        cvnames_agent = game->get_player_caravan_names(pname_agent);
        cvnames_random = game->get_player_caravan_names(pname_random);

        // Agent's caravans are first
        for (uint8_t i_cvname = 0; i_cvname < Model::PLAYER_CARAVANS_MAX; i_cvname++) {
            cvnames_all_ordered[i_cvname] = cvnames_agent[i_cvname];
        }

        // Then random's caravans
        for (uint8_t i_cvname = 0; i_cvname < Model::PLAYER_CARAVANS_MAX; i_cvname++) {
            cvnames_all_ordered[Model::PLAYER_CARAVANS_MAX + i_cvname] = cvnames_random[i_cvname];
        }
    }

    std::tuple<Observation, Info> EnvironmentGame::reset() {
        // TODO Random start conditions for new game
        uint8_t abc_cards = 54;
        uint8_t abc_samples = 1;
        bool abc_imbalanced = false;

        uint8_t def_cards = 54;
        uint8_t def_samples = 1;
        bool def_imbalanced = false;

        pname_first = Model::PLAYER_ABC;

        // Build decks for each player
        std::unique_ptr<Model::Deck> deck_abc(
            Model::DeckBuilder::build_caravan_deck(
                abc_cards,
                abc_samples,
                !abc_imbalanced
            )
        );

        std::unique_ptr<Model::Deck> deck_def(
            Model::DeckBuilder::build_caravan_deck(
                def_cards,
                def_samples,
                !def_imbalanced
            )
        );

        // Create players and assign their decks to them
        std::unique_ptr<Model::Player> player_abc = std::make_unique<
            Model::Player>(
            Model::PLAYER_ABC,
            std::move(deck_abc)
        );

        std::unique_ptr<Model::Player> player_def = std::make_unique<
            Model::Player>(
            Model::PLAYER_DEF,
            std::move(deck_def)
        );

        // Create new game with players
        game = std::make_unique<Model::Game>(
            std::move(player_abc),
            std::move(player_def),
            pname_first
        );

        reset_never_called = false;
        environment_is_done = false;

        // TODO info
        return std::make_tuple(make_observation(), Info{});
    }

    std::tuple<Action, Info> EnvironmentGame::sample() {
        if (reset_never_called) {
            throw CaravanFatalGymException(ERR_RESET);
        }

        if (environment_is_done) {
            throw CaravanFatalGymException(ERR_DONE);
        }

        // TODO action, info
    }

    std::tuple<Observation, Reward, bool, Info> EnvironmentGame::step(
        Action &action
    ) {
        if (reset_never_called) {
            throw CaravanFatalGymException(ERR_RESET);
        }

        if (environment_is_done) {
            throw CaravanFatalGymException(ERR_DONE);
        }

        // TODO environment_is_done = true
        // TODO observation, reward, done, info
    }
}
