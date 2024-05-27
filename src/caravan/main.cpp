// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <iostream>
#include <cxxopts.hpp>
#include "caravan/view/view_tui.h"
#include "caravan/user/bot/factory.h"

const std::string ARG_PVP = "pvp";
const std::string ARG_BVB = "bvb";
const std::string ARG_FIRST = "first";
const std::string ARG_CARDS = "cards";
const std::string ARG_SAMPLES = "samples";
const std::string ARG_BALANCED = "balanced";
const std::string ARG_BOT = "bot";
const std::string ARG_DELAY = "delay";

const uint8_t FIRST_ABC = 1;
const uint8_t FIRST_DEF = 2;


int main(int argc, char *argv[]) {
    User *user_abc;
    User *user_def;
    Game *game;
    ViewTUI *view;

    try {
        cxxopts::Options options(
            "Caravan",
            "A command-line version of the Caravan card game "
            "from Fallout: New Vegas.");

        options.add_options()
            (ARG_PVP, "Player vs player.", cxxopts::value<bool>()->default_value("false"))
            (ARG_BVB, "Bot vs bot.", cxxopts::value<bool>()->default_value("false"))
            (ARG_FIRST, "Which player goes first.", cxxopts::value<uint8_t>()->default_value("1"))
            (ARG_CARDS, "Number of cards for each caravan deck.", cxxopts::value<uint8_t>()->default_value("54"))
            (ARG_SAMPLES, "Number of traditional decks to sample when building caravan decks.", cxxopts::value<uint8_t>()->default_value("1"))
            (ARG_BALANCED, "Whether caravan decks should contain a balanced number of cards across all sampled traditional decks.", cxxopts::value<bool>()->default_value("true"))
            (ARG_BOT, "Which bot to play against.", cxxopts::value<std::string>()->default_value("normal"))
            (ARG_DELAY, "Delay before bot makes its move (in seconds).", cxxopts::value<float>()->default_value("1.0"))
        ;

        auto result = options.parse(argc, argv);

        bool pvp = result[ARG_PVP].as<bool>();
        bool bots = result[ARG_BVB].as<bool>();
        uint8_t first = result[ARG_FIRST].as<uint8_t>();
        uint8_t cards = result[ARG_CARDS].as<uint8_t>();
        uint8_t samples = result[ARG_SAMPLES].as<uint8_t>();
        bool balanced = result[ARG_BALANCED].as<bool>();
        std::string bot = result[ARG_BOT].as<std::string>();
        float delay = result[ARG_DELAY].as<float>();

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
            cards, samples, balanced,
            cards, samples, balanced,
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
