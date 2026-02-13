// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <iostream>
#include "cxxopts.hpp"
#include "caravan/controller/controller_ftxui.h"
#include "caravan/view/view_ftxui.h"
#include "caravan/user/bot_factory.h"
#include "caravan/user/user_human_ftxui.h"

namespace Caravan {
    const std::string OPTS_HELP = "h,help";
    const std::string OPTS_VERSION = "v,version";
    const std::string OPTS_PVP = "pvp";
    const std::string OPTS_BVB = "bvb";
    const std::string OPTS_BOT1 = "b,bot,bot1";
    const std::string OPTS_BOT2 = "bot2";
    const std::string OPTS_DELAY = "d,delay";
    const std::string OPTS_FIRST = "f,first";
    const std::string OPTS_CARDS = "c,cards";
    const std::string OPTS_SAMPLES = "s,samples";
    const std::string OPTS_IMBALANCED = "i,imbalanced";
    const std::string OPTS_CHEAT = "cheat";
    const std::string OPTS_NOCOL = "nocol";

    const std::string ALIAS_BOT = "bot";

    const std::string KEY_HELP = "help";
    const std::string KEY_VERSION = "version";
    const std::string KEY_PVP = "pvp";
    const std::string KEY_BVB = "bvb";
    const std::string KEY_BOT1 = "bot1";
    const std::string KEY_BOT2 = "bot2";
    const std::string KEY_DELAY = "delay";
    const std::string KEY_FIRST = "first";
    const std::string KEY_CARDS = "cards";
    const std::string KEY_SAMPLES = "samples";
    const std::string KEY_IMBALANCED = "imbalanced";
    const std::string KEY_CHEAT = "cheat";
    const std::string KEY_NOCOL = "nocol";

    const std::string DEFAULT_BOT1 = "random";
    const std::string DEFAULT_BOT2 = "random";
    const std::string DEFAULT_DELAY = "1000";
    const std::string DEFAULT_FIRST = "1";
    const std::string DEFAULT_CARDS = "54";
    const std::string DEFAULT_SAMPLES = "1";

    constexpr uint8_t FIRST_ABC = 1;
    constexpr uint8_t FIRST_DEF = 2;


    int run(int argc, char **argv) {
        std::unique_ptr<User::BaseUser<std::string>> user_abc;
        std::unique_ptr<User::BaseUser<std::string>> user_def;
        std::unique_ptr<Model::Game> game;
        std::unique_ptr<Controller::ControllerFTXUI> ctrl;
        std::unique_ptr<View::ViewFTXUI> view;

        try {
            cxxopts::Options options(CARAVAN_NAME);

            auto get_option = options.add_options();

            get_option(OPTS_HELP, "Print help instructions.");
            get_option(OPTS_VERSION, "Print Caravan version.");
            get_option(OPTS_PVP, "A Player vs Player game.");
            get_option(OPTS_BVB, "A Bot vs Bot game.");
            get_option(
                OPTS_BOT1,
                "Alias: --" + ALIAS_BOT + ". " +
                "Which bot to play with in a PvB game (normal, friendly).",
                cxxopts::value<std::string>()->default_value(DEFAULT_BOT1)
            );

            get_option(
                OPTS_BOT2,
                "Which bot for the bot to play with in a BvB game (normal, friendly).",
                cxxopts::value<std::string>()->default_value(DEFAULT_BOT2)
            );

            get_option(
                OPTS_DELAY,
                "Delay before bot makes its move (in milliseconds).",
                cxxopts::value<uint16_t>()->default_value(DEFAULT_DELAY)
            );

            get_option(
                OPTS_FIRST,
                "Which player goes first (1 or 2).",
                cxxopts::value<uint8_t>()->default_value(DEFAULT_FIRST)
            );

            get_option(
                OPTS_CARDS,
                "Number of cards for each caravan deck (30-162, inclusive).",
                cxxopts::value<uint8_t>()->default_value(DEFAULT_CARDS)
            );

            get_option(
                OPTS_SAMPLES,
                "Number of traditional decks to sample when building caravan decks (1-3, inclusive).",
                cxxopts::value<uint8_t>()->default_value(DEFAULT_SAMPLES)
            );

            get_option(
                OPTS_IMBALANCED,
                "An imbalanced caravan deck is built by taking as many "
                "cards from one shuffled sample deck before moving to the next. "
                "A balanced deck randomly samples cards across all sample decks."
            );

            get_option(OPTS_CHEAT, "Always show both players' hands.");
            get_option(
                OPTS_NOCOL,
                "Remove colour from the game interface, if it is available."
            );

            auto result = options.parse(argc, argv);

            // Print help instructions
            if (result.count(KEY_HELP)) {
                printf("%s v%s\n\n", CARAVAN_NAME, CARAVAN_VERSION);
                printf("%s\n", CARAVAN_DESCRIPTION);
                printf("%s\n", CARAVAN_COPYRIGHT);
                printf("%s\n", CARAVAN_URL);
                printf("%s", options.help().c_str());
                exit(EXIT_SUCCESS);
            }

            // Print version
            if (result.count(KEY_VERSION)) {
                printf("%s\n", CARAVAN_VERSION);
                exit(EXIT_SUCCESS);
            }

            // Get option results
            bool pvp = result[KEY_PVP].as<bool>();
            bool bvb = result[KEY_BVB].as<bool>();

            std::string bot1 = result[KEY_BOT1].as<std::string>();
            std::string bot2 = result[KEY_BOT2].as<std::string>();

            uint16_t delay = result[KEY_DELAY].as<uint16_t>();
            uint8_t first = result[KEY_FIRST].as<uint8_t>();
            uint8_t cards = result[KEY_CARDS].as<uint8_t>();
            uint8_t samples = result[KEY_SAMPLES].as<uint8_t>();

            bool imbalanced = result[KEY_IMBALANCED].as<bool>();
            bool cheat = result[KEY_CHEAT].as<bool>();
            bool nocol = result[KEY_NOCOL].as<bool>();

            // Validate options
            if (pvp && bvb) {
                printf(
                    "Game cannot be both Player vs Player and Bot vs Bot.\n"
                );
                exit(EXIT_FAILURE);
            }

            if (first < FIRST_ABC || first > FIRST_DEF) {
                printf(
                    "First player must be either %d or %d, not %d.\n",
                    FIRST_ABC,
                    FIRST_DEF,
                    first
                );
                exit(EXIT_FAILURE);
            }

            if (cards < Model::DECK_CARAVAN_MIN || cards >
                Model::DECK_CARAVAN_MAX) {
                printf(
                    "Caravan decks must have between %d and %d cards (inclusive), not %d.\n",
                    Model::DECK_CARAVAN_MIN,
                    Model::DECK_CARAVAN_MAX,
                    cards
                );
                exit(EXIT_FAILURE);
            }

            if (samples < Model::SAMPLE_DECKS_MIN || samples >
                Model::SAMPLE_DECKS_MAX) {
                printf(
                    "Number of caravan deck samples must be between %d and %d (inclusive), not %d.\n",
                    Model::SAMPLE_DECKS_MIN,
                    Model::SAMPLE_DECKS_MIN,
                    samples
                );
                exit(EXIT_FAILURE);
            }

            if (delay < 0) {
                printf("Bot delay cannot be a negative number.\n");
                exit(EXIT_FAILURE);
            }

            // Determine users
            if (pvp) {
                // human vs human
                user_abc = std::make_unique<User::UserHumanFTXUI>(
                    Model::PLAYER_ABC
                );
                user_def = std::make_unique<User::UserHumanFTXUI>(
                    Model::PLAYER_DEF
                );
            } else if (bvb) {
                // bot vs bot
                user_abc = std::unique_ptr<User::BaseUserBot<std::string>>(
                    User::BotFactory::get(bot1, Model::PLAYER_ABC)
                );
                user_def = std::unique_ptr<User::BaseUserBot<std::string>>(
                    User::BotFactory::get(bot2, Model::PLAYER_DEF)
                );
            } else {
                // humans vs bot
                user_abc = std::make_unique<User::UserHumanFTXUI>(
                    Model::PLAYER_ABC
                );
                user_def = std::unique_ptr<User::BaseUserBot<std::string>>(
                    User::BotFactory::get(bot1, Model::PLAYER_DEF)
                );
            }

            // Build decks for each player
            std::unique_ptr<Model::Deck> deck_abc(
                Model::DeckBuilder::build_caravan_deck(
                    cards,
                    samples,
                    !imbalanced
                )
            );

            std::unique_ptr<Model::Deck> deck_def(
                Model::DeckBuilder::build_caravan_deck(
                    cards,
                    samples,
                    !imbalanced
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
                first == FIRST_ABC ? Model::PLAYER_ABC : Model::PLAYER_DEF
            );

            // Link game to FTXUI controller
            ctrl = std::make_unique<Controller::ControllerFTXUI>(*game);

            // Link game and controller to FTXUI view
            view = std::make_unique<View::ViewFTXUI>(
                *game,
                *ctrl,
                *user_abc,
                *user_def,
                delay,
                cheat,
                !nocol
            );

            // Run the FTXUI view
            view->run();
        } catch (std::exception &e) {
            printf("%s\n", e.what());
            exit(EXIT_FAILURE);
        }

        return EXIT_SUCCESS;
    }
}

int main(int argc, char **argv) {
    Caravan::run(argc, argv);
}
