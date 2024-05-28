// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <iostream>
#include "cxxopts.hpp"
#include "caravan/view/view_tui.h"
#include "caravan/user/bot/factory.h"

const std::string OPTS_HELP = "h,help";
const std::string OPTS_VERSION = "v,version";

const std::string KEY_HELP = "help";
const std::string KEY_VERSION = "version";

const uint8_t FIRST_ABC = 1;
const uint8_t FIRST_DEF = 2;


int main(int argc, char *argv[]) {
    User *user_abc;
    User *user_def;
    Game *game;
    ViewTUI *view;

    try {
        cxxopts::Options options(CARAVAN_NAME);

        options.add_options()
            (OPTS_HELP, "Print help instructions.")
            (OPTS_VERSION, "Print help instructions.")
            ("pvp", "A Player vs Player game.")
            ("bvb", "A Bot vs Bot game.")
            ("b,bot", "Which bot to play with.", cxxopts::value<std::string>()->default_value("normal"))
            ("d,delay", "Delay before bot makes its move (in seconds).", cxxopts::value<float>()->default_value("1.0"))
            ("f,first", "Which player goes first (1 or 2).", cxxopts::value<uint8_t>()->default_value("1"))
            ("c,cards", "Number of cards for each caravan deck (30-162, inclusive).", cxxopts::value<uint8_t>()->default_value("54"))
            ("s,samples", "Number of traditional decks to sample when building caravan decks (1-3, inclusive).", cxxopts::value<uint8_t>()->default_value("1"))
            ("i,imbalanced",
             "An imbalanced caravan deck is built by taking as many "
             "cards from one shuffled sample deck before moving to the next. "
             "A balanced deck randomly samples cards across all sample decks.")
        ;

        auto result = options.parse(argc, argv);

        // Print help instructions.
        if (result.count(KEY_HELP)) {
            printf("%s v%s\n\n", CARAVAN_NAME, CARAVAN_VERSION);
            printf("%s\n", CARAVAN_DESCRIPTION);
            printf("%s\n", CARAVAN_COPYRIGHT);
            printf("%s\n", CARAVAN_URL);
            printf("%s", options.help().c_str());
            exit(EXIT_SUCCESS);
        }

        if (result.count(KEY_VERSION)) {
            printf("%s\n", CARAVAN_VERSION);
            exit(EXIT_SUCCESS);
        }

        bool pvp = result["pvp"].as<bool>();
        bool bots = result["bvb"].as<bool>();
        std::string bot = result["bot"].as<std::string>();
        float delay = result["delay"].as<float>();
        uint8_t first = result["first"].as<uint8_t>();
        uint8_t cards = result["cards"].as<uint8_t>();
        uint8_t samples = result["samples"].as<uint8_t>();
        bool imbalanced = result["imbalanced"].as<bool>();

        if (pvp && bots) {
            printf("Game cannot be both Player vs Player and Bot vs Bot.");
            exit(EXIT_FAILURE);
        }

        if(first < FIRST_ABC || first > FIRST_DEF) {
            printf("First player must be either %d or %d.", FIRST_ABC, FIRST_DEF);
            exit(EXIT_FAILURE);
        }

        if (cards < DECK_CARAVAN_MIN || cards > DECK_CARAVAN_MAX) {
            printf("Caravan decks must have between %d and %d cards (inclusive).", DECK_CARAVAN_MIN, DECK_CARAVAN_MAX);
            exit(EXIT_FAILURE);
        }

        if (samples < SAMPLE_DECKS_MIN || samples > SAMPLE_DECKS_MAX) {
            printf("Number of caravan deck samples must be between %d and %d (inclusive).", SAMPLE_DECKS_MIN, SAMPLE_DECKS_MIN);
            exit(EXIT_FAILURE);
        }

        if(delay < 0) {
            printf("Bot delay cannot be a negative number.");
            exit(EXIT_FAILURE);
        }

        if(pvp) {  // human vs human
            user_abc = new UserHuman(PLAYER_ABC);
            user_def = new UserHuman(PLAYER_DEF);

        } else if (bots) {  // bot vs bot
            user_abc = BotFactory::get(bot, PLAYER_ABC);
            user_def = BotFactory::get(bot, PLAYER_DEF);

        } else {  // humans vs bot
            user_abc = new UserHuman(PLAYER_ABC);
            user_def = BotFactory::get(bot, PLAYER_DEF);
        }

        GameConfig gc = {
            cards, samples, !imbalanced,
            cards, samples, !imbalanced,
            first == FIRST_ABC ? PLAYER_ABC : PLAYER_DEF
        };

        ViewConfig vc = {
            .user_abc=user_abc,
            .user_def=user_def,
            .bot_delay_sec=delay
        };

        game = new Game(&gc);
        view = new ViewTUI(&vc, game);

    } catch (CaravanException &e) {
        printf("%s", e.what().c_str());
        exit(EXIT_FAILURE);

    } catch (std::exception &e) {
        printf("%s", e.what());
        exit(EXIT_FAILURE);
    }

    view->run();

    view->close();
    user_abc->close();
    user_def->close();
    game->close();

    delete view;
    delete user_abc;
    delete user_def;
    delete game;
}
