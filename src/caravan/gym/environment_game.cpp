// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/gym/environment_game.h"
#include "caravan/core/exceptions.h"
#include "caravan/user/user_bot_agent.h"
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

    std::array<uint8_t, SIZE_OBSERVATION> EnvironmentGame::make_observation() {
        std::array<uint8_t, SIZE_OBSERVATION> observation{};
        uint8_t i = 0;

        // Get agent hand as unique, sorted card numbers
        std::set<uint8_t> hand_set{};

        Model::PlayerName name_agent = user_agent.get_name();
        Model::PlayerName name_random = user_random.get_name();

        Model::Player &player = game->get_player(name_agent);
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
        Model::PlayerCaravanNames cvnames_abc = game->get_player_caravan_names(name_agent);
        Model::PlayerCaravanNames cvnames_def = game->get_player_caravan_names(name_random);

        std::array<Model::CaravanName, Model::TABLE_CARAVANS_MAX> cvnames_all{};

        // Agent's caravans are first
        for (uint8_t i_cvname = 0; i_cvname < Model::PLAYER_CARAVANS_MAX; i_cvname++) {
            cvnames_all[i_cvname] = cvnames_abc[i_cvname];
        }

        // Then random's caravans
        for (uint8_t i_cvname = 0; i_cvname < Model::PLAYER_CARAVANS_MAX; i_cvname++) {
            cvnames_all[Model::PLAYER_CARAVANS_MAX + i_cvname] = cvnames_def[i_cvname];
        }

        Model::Table& table = game->get_table();
        for (uint8_t i_cvname = 0; i_cvname < Model::TABLE_CARAVANS_MAX; i_cvname++) {
            Model::Caravan& cvn = table.get_caravan(cvnames_all[i_cvname]);

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

                    for (uint8_t i_face = 0; i_face < slot.i_faces; i_face++) {
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

    /*
     * PUBLIC
     */

    EnvironmentGame::EnvironmentGame(
        User::BaseUser<std::string> &user_abc,
        User::BaseUser<std::string> &user_def
    ) : user_agent(user_abc),
        user_random(user_def) {

        if (user_abc.get_name() != Model::PLAYER_ABC) {
            throw CaravanFatalGymException(
                "User ABC must have player name ABC.");
        }

        if (user_def.get_name() != Model::PLAYER_DEF) {
            throw CaravanFatalGymException(
                "User DEF must have player name DEF.");
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

        first = Model::PLAYER_ABC;

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
            first
        );

        reset_never_called = false;
        environment_is_done = false;

        // TODO observation, info
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
