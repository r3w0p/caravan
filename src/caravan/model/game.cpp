// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/model/game.h"
#include "caravan/core/common.h"


/*
 * PUBLIC
 */

Game::Game(GameConfig *gc) {
    if (gc->player_first == NO_PLAYER) {
        throw CaravanFatalException(
            "Invalid first player in game configuration.");
    }

    Deck *deck_top = DeckBuilder::build_caravan_deck(
        gc->player_abc_cards,
        gc->player_abc_samples,
        gc->player_abc_balanced);

    Deck *deck_bottom = DeckBuilder::build_caravan_deck(
        gc->player_def_cards,
        gc->player_def_samples,
        gc->player_def_balanced);

    table = std::make_unique<Table>();
    player_a = std::make_unique<Player>(PLAYER_ABC, deck_bottom);
    player_b = std::make_unique<Player>(PLAYER_DEF, deck_top);

    player_turn = gc->player_first == player_a->get_name() ? player_a.get() : player_b.get();
}

CaravanName Game::get_opposite_caravan_name(CaravanName cvname) {
    switch (cvname) {
        case CARAVAN_A:
            return CARAVAN_D;
        case CARAVAN_B:
            return CARAVAN_E;
        case CARAVAN_C:
            return CARAVAN_F;
        case CARAVAN_D:
            return CARAVAN_A;
        case CARAVAN_E:
            return CARAVAN_B;
        case CARAVAN_F:
            return CARAVAN_C;
        default:
            return NO_CARAVAN;
    }
}

Player *Game::get_player(PlayerName pname) {
    if (player_a->get_name() == pname) {
        return player_a.get();
    }

    if (player_b->get_name() == pname) {
        return player_b.get();
    }

    throw CaravanFatalException("Invalid player name.");
}

PlayerCaravanNames Game::get_player_caravan_names(PlayerName pname) {
    if (player_a->get_name() == pname) {
        return PlayerCaravanNames{CARAVAN_A, CARAVAN_B, CARAVAN_C};
    }

    if (player_b->get_name() == pname) {
        return PlayerCaravanNames{CARAVAN_D, CARAVAN_E, CARAVAN_F};
    }

    throw CaravanFatalException("Invalid player name.");
}

PlayerName Game::get_player_turn() {
    return player_turn->get_name();
}

Table* Game::get_table() {
    return table.get();
}

PlayerName Game::get_winner() {
    uint8_t won_pa = 0;
    uint8_t won_pb = 0;
    int8_t comp[3];

    // Check if all three caravans have been sold...

    comp[0] = compare_bids(CARAVAN_A, CARAVAN_D);
    comp[1] = compare_bids(CARAVAN_B, CARAVAN_E);
    comp[2] = compare_bids(CARAVAN_C, CARAVAN_F);

    for (signed char n : comp) {
        if (n < 0) {
            won_pa += 1;
        } else if (n > 0) {
            won_pb += 1;
        } else {
            // All three must be sold for there to be a winner
            break;
        }
    }

    // Winner is whoever won at least 2 out of the 3 bids
    if(won_pa + won_pb == 3) {
        if (won_pa >= 2) {
            return player_a->get_name();
        }

        if (won_pb >= 2) {
            return player_b->get_name();
        }
    }

    // Neither player has outbid the other

    // Check if players have empty hands...

    if (player_a->get_size_hand() > 0 and player_b->get_size_hand() == 0) {
        return player_a->get_name();
    }

    if (player_a->get_size_hand() == 0 and player_b->get_size_hand() > 0) {
        return player_b->get_name();
    }

    // Neither player has an empty hand

    // Nobody has won yet...

    return NO_PLAYER;
}

bool Game::is_caravan_bust(CaravanName cvname) {
    if (cvname == NO_CARAVAN) return false;

    return table->get_caravan(cvname)->get_bid() > CARAVAN_SOLD_MAX;
}

bool Game::is_caravan_winning(CaravanName cvname) {
    if (cvname == NO_CARAVAN) return false;

    return winning_bid(cvname, get_opposite_caravan_name(cvname)) == cvname;
}

void Game::play_option(GameCommand *command) {
    if (get_winner() != NO_PLAYER) {
        throw CaravanFatalException(
            "The game has already been won.");
    }

    switch (command->option) {
        case OPTION_PLAY:
            option_play(command, false);
            break;

        case OPTION_DISCARD:
            option_discard(command, false);
            break;

        case OPTION_CLEAR:
            option_clear(command, false);
            break;

        default:
            throw CaravanFatalException("Invalid play option.");
    }

    player_turn->increment_moves();
    player_turn->maybe_add_card_to_hand();

    if (player_a->get_name() == player_turn->get_name()) {
        player_turn = player_b.get();
    } else {
        player_turn = player_a.get();
    }
}

bool Game::check_option(GameCommand *command) {
    if (get_winner() != NO_PLAYER) {
        throw CaravanFatalException(
            "The game has already been won.");
    }

    switch (command->option) {
        case OPTION_PLAY:
            return option_play(command, true);

        case OPTION_DISCARD:
            return option_discard(command, true);

        case OPTION_CLEAR:
            return option_clear(command, true);

        default:
            throw CaravanFatalException("Invalid play option.");
    }
}


/*
 * PROTECTED
 */

int8_t Game::compare_bids(CaravanName cvname1, CaravanName cvname2) {
    uint8_t bid_cn1;
    uint8_t bid_cn2;

    if (has_sold(cvname1)) {
        if (has_sold(cvname2)) {
            bid_cn1 = table->get_caravan(cvname1)->get_bid();
            bid_cn2 = table->get_caravan(cvname2)->get_bid();

            if (bid_cn1 > bid_cn2) {
                return -1; // CN1 sold; CN2 sold; CN1 highest bid
            } else if (bid_cn1 < bid_cn2) {
                return 1; // CN1 sold; CN2 sold; CN2 highest bid
            } else {
                return 0;
            } // CN1 sold; CN2 sold; matching bids
        } else {
            return -1;
        } // CN1 sold; CN2 unsold
    } else if (has_sold(cvname2)) {
        return 1; // CN1 unsold; CN2 sold
    } else {
        return 0;
    } // CN1 unsold; CN2 unsold
}

CaravanName Game::winning_bid(CaravanName cvname1, CaravanName cvname2) {
    int8_t bidcomp = compare_bids(cvname1, cvname2);

    if (bidcomp < 0) {
        return cvname1;
    } else if (bidcomp > 0) {
        return cvname2;
    } else {
        return NO_CARAVAN;
    }
}

bool Game::has_sold(CaravanName cvname) {
    uint8_t bid = table->get_caravan(cvname)->get_bid();
    return bid >= CARAVAN_SOLD_MIN and bid <= CARAVAN_SOLD_MAX;
}

bool Game::option_clear(GameCommand *command, bool check_only) {
    if (player_turn->get_moves_count() < MOVES_START_ROUND) {
        if (check_only) {
            return false;
        } else {
            throw CaravanGameException(
                "A player cannot clear a caravan during "
                "the Start round.");
        }
    }

    // Intentionally not catching fatal exception if player is not ABC or DEF
    PlayerCaravanNames pcns = get_player_caravan_names(player_turn->get_name());

    // Invalid for a player to clear their opponent's caravans
    if (pcns[0] != command->caravan_name and
        pcns[1] != command->caravan_name and
        pcns[2] != command->caravan_name) {
        if (check_only) {
            return false;
        } else {
            throw CaravanGameException(
                "A player cannot clear their opponent's caravans.");
        }
    }

    // Clear the caravan
    return table->clear_caravan(command->caravan_name, check_only);
}

bool Game::option_discard(GameCommand *command, bool check_only) {
    Card c_discarded;
    bool result;

    if (player_turn->get_moves_count() < MOVES_START_ROUND) {
        if (check_only) {
            return false;
        } else {
            throw CaravanGameException(
                "A player cannot discard a card during "
                "the Start round.");
        }
    }

    result = player_turn->discard_from_hand_at(
        command->pos_hand, &c_discarded, check_only);

    if (!check_only) {
        // Log discarded card to command
        command->hand = c_discarded;
    }

    return result;
}

bool Game::option_play(GameCommand *command, bool check_only) {
    Card c_hand;

    // Intentionally not catching fatal exception if player hand is empty
    try {
        c_hand = player_turn->get_from_hand_at(command->pos_hand);
    } catch (CaravanGameException &e) {
        if (check_only) return false;

        throw;
    }

    bool in_start_stage = player_turn->get_moves_count() < MOVES_START_ROUND;

    if (is_numeral_card(c_hand)) {
        bool pa_playing_num_onto_pa_caravans =
            player_turn->get_name() == player_a->get_name() and
            (command->caravan_name == CARAVAN_A or
             command->caravan_name == CARAVAN_B or
             command->caravan_name == CARAVAN_C);

        bool pb_playing_num_onto_pb_caravans =
            player_turn->get_name() == player_b->get_name() and
            (command->caravan_name == CARAVAN_D or
             command->caravan_name == CARAVAN_E or
             command->caravan_name == CARAVAN_F);

        if (!(pa_playing_num_onto_pa_caravans or
              pb_playing_num_onto_pb_caravans)) {
            if (check_only) {
                return false;
            } else {
                throw CaravanGameException(
                    "A numeral card can only be played on "
                    "a player's own caravan.");
            }
        }

        if (in_start_stage and table->get_caravan(
                command->caravan_name)->get_size() > 0) {
            if (check_only) {
                return false;
            } else {
                throw CaravanGameException(
                    "A numeral card must be played on an empty caravan "
                    "during the Start round.");
            }
        }

        if (command->pos_caravan > 0) {
            if (check_only) {
                return false;
            } else {
                throw CaravanGameException(
                    "A numeral card must not have a specified caravan position.");
            }
        }

        if (!table->play_numeral_card(
            command->caravan_name,
            c_hand,
            check_only)) {
            return false;
        }
    } else {
        // is a face card
        if (in_start_stage) {
            if (check_only) {
                return false;
            } else {
                throw CaravanGameException(
                    "A face card cannot be played during the Start round.");
            }
        }

        if (!check_only) {
            // Log to command
            command->board = table->get_caravan(
                command->caravan_name)->get_slot(
                command->pos_caravan).card;
        }

        if (!table->play_face_card(
            command->caravan_name,
            c_hand,
            command->pos_caravan,
            check_only)) {
            return false;
        }
    }

    if (!check_only) {
        player_turn->discard_from_hand_at(
            command->pos_hand, &command->hand, check_only);
    }

    return true;
}
