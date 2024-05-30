// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <iostream>
#include "cxxopts.hpp"
#include "caravan/view/view_tui.h"
#include "caravan/user/bot/factory.h"

const std::string OPTS_HELP = "h,help";
const std::string OPTS_VERSION = "v,version";
const std::string OPTS_PVP = "pvp";
const std::string OPTS_BVB = "bvb";
const std::string OPTS_BOT = "b,bot";
const std::string OPTS_DELAY = "d,delay";
const std::string OPTS_FIRST = "f,first";
const std::string OPTS_CARDS = "c,cards";
const std::string OPTS_SAMPLES = "s,samples";
const std::string OPTS_IMBALANCED = "i,imbalanced";

const std::string KEY_HELP = "help";
const std::string KEY_VERSION = "version";
const std::string KEY_PVP = "pvp";
const std::string KEY_BVB = "bvb";
const std::string KEY_BOT = "bot";
const std::string KEY_DELAY = "delay";
const std::string KEY_FIRST = "first";
const std::string KEY_CARDS = "cards";
const std::string KEY_SAMPLES = "samples";
const std::string KEY_IMBALANCED = "imbalanced";

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
            (OPTS_VERSION, "Print software version.")
            (OPTS_PVP, "A Player vs Player game.")
            (OPTS_BVB, "A Bot vs Bot game.")
            (OPTS_BOT, "Which bot to play with (normal, friendly).", cxxopts::value<std::string>()->default_value("normal"))
            (OPTS_DELAY, "Delay before bot makes its move (in seconds).", cxxopts::value<float>()->default_value("1.0"))
            (OPTS_FIRST, "Which player goes first (1 or 2).", cxxopts::value<uint8_t>()->default_value("1"))
            (OPTS_CARDS, "Number of cards for each caravan deck (30-162, inclusive).", cxxopts::value<uint8_t>()->default_value("54"))
            (OPTS_SAMPLES, "Number of traditional decks to sample when building caravan decks (1-3, inclusive).", cxxopts::value<uint8_t>()->default_value("1"))
            (OPTS_IMBALANCED,
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

        bool pvp = result[KEY_PVP].as<bool>();
        bool bots = result[KEY_BVB].as<bool>();
        std::string bot = result[KEY_BOT].as<std::string>();
        float delay = result[KEY_DELAY].as<float>();
        uint8_t first = result[KEY_FIRST].as<uint8_t>();
        uint8_t cards = result[KEY_CARDS].as<uint8_t>();
        uint8_t samples = result[KEY_SAMPLES].as<uint8_t>();
        bool imbalanced = result[KEY_IMBALANCED].as<bool>();

        if (pvp && bots) {
            printf("Game cannot be both Player vs Player and Bot vs Bot.\n");
            exit(EXIT_FAILURE);
        }

        if(first < FIRST_ABC || first > FIRST_DEF) {
            printf("First player must be either %d or %d.\n", FIRST_ABC, FIRST_DEF);
            exit(EXIT_FAILURE);
        }

        if (cards < DECK_CARAVAN_MIN || cards > DECK_CARAVAN_MAX) {
            printf("Caravan decks must have between %d and %d cards (inclusive).\n", DECK_CARAVAN_MIN, DECK_CARAVAN_MAX);
            exit(EXIT_FAILURE);
        }

        if (samples < SAMPLE_DECKS_MIN || samples > SAMPLE_DECKS_MAX) {
            printf("Number of caravan deck samples must be between %d and %d (inclusive).\n", SAMPLE_DECKS_MIN, SAMPLE_DECKS_MIN);
            exit(EXIT_FAILURE);
        }

        if(delay < 0) {
            printf("Bot delay cannot be a negative number.\n");
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
        printf("%s\n", e.what().c_str());
        exit(EXIT_FAILURE);

    } catch (std::exception &e) {
        printf("%s\n", e.what());
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
