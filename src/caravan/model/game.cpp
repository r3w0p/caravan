// Copyright (c) 2022-2025 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/model/game.h"

/**
 * @param gc Game configuration.
 * 
 * @throws CaravanFatalModelException Invalid name for first player.
 */
Game::Game(const GameConfig *gc) {
    if (gc->player_first == NO_PLAYER) {
        throw CaravanFatalModelException(
            "Invalid player name for first player "
            "in game configuration.");
    }

    // Generate decks for each player
    std::unique_ptr<Deck> deck_abc(
        DeckBuilder::build_caravan_deck(
            gc->player_abc_cards,
            gc->player_abc_samples,
            gc->player_abc_balanced));

    std::unique_ptr<Deck> deck_def(
        DeckBuilder::build_caravan_deck(
        gc->player_def_cards,
        gc->player_def_samples,
        gc->player_def_balanced));

    // Create game table
    table = std::make_unique<Table>();

    // Create players and assign their decks to them
    player_abc = std::make_unique<Player>(PLAYER_ABC, std::move(deck_abc));
    player_def = std::make_unique<Player>(PLAYER_DEF, std::move(deck_def));

    // Determine which player moves first
    player_turn = gc->player_first == player_abc->get_name() ?
        player_abc.get() : player_def.get();
}

Player *Game::get_player(PlayerName pname) const {
    if (player_abc->get_name() == pname) {
        return player_abc.get();
    }

    if (player_def->get_name() == pname) {
        return player_def.get();
    }

    throw CaravanFatalModelException("Invalid player name.");
}

PlayerCaravanNames Game::get_player_caravan_names(PlayerName pname) const {
    if (player_abc->get_name() == pname) {
        return PlayerCaravanNames{CARAVAN_A, CARAVAN_B, CARAVAN_C};
    }

    if (player_def->get_name() == pname) {
        return PlayerCaravanNames{CARAVAN_D, CARAVAN_E, CARAVAN_F};
    }

    throw CaravanFatalModelException("Invalid player name.");
}

PlayerName Game::get_player_turn() const {
    return player_turn->get_name();
}

Table *Game::get_table() const {
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

    for (int i = 0; i < 3; ++i) {
        if (comp[i] < 0) {
            won_pa += 1;
        } else if (comp[i] > 0) {
            won_pb += 1;
        } else {
            // All three must be sold for there to be a winner
            break;
        }
    }

    // Winner is whoever won at least 2 out of the 3 bids
    if(won_pa + won_pb == 3) {
        if (won_pa >= 2) {
            return player_abc->get_name();
        }

        if (won_pb >= 2) {
            return player_def->get_name();
        }
    }

    // Neither player has outbid the other

    // Check if players have empty hands...

    if (player_abc->get_size_hand() > 0 and player_def->get_size_hand() == 0) {
        return player_abc->get_name();
    }

    if (player_abc->get_size_hand() == 0 and player_def->get_size_hand() > 0) {
        return player_def->get_name();
    }

    // Neither player has an empty hand

    // Nobody has won yet...

    return NO_PLAYER;
}

void Game::play_option(GameCommand *command) {
    if (get_winner() != NO_PLAYER) {
        throw CaravanFatalModelException(
            "The game has already been won.");
    }

    switch (command->option) {
        case OPTION_PLAY:
            option_play(player_turn, command);
            break;

        case OPTION_DISCARD:
            if (player_turn->get_moves_count() < MOVES_START_ROUND) {
                throw CaravanIllegalModelException(
                    "A player cannot discard a card during "
                    "the Start round.");
            }

            option_discard(player_turn, command);
            break;

        case OPTION_CLEAR:
            if (player_turn->get_moves_count() < MOVES_START_ROUND) {
                throw CaravanIllegalModelException(
                    "A player cannot clear a caravan during "
                    "the Start round.");
            }

            option_clear(player_turn, command);
            break;

        default:
            throw CaravanFatalModelException("Invalid play option.");
    }

    player_turn->increment_moves();
    player_turn->maybe_add_card_to_hand_from_deck();

    if (player_abc->get_name() == player_turn->get_name()) {
        player_turn = player_def.get();
    } else {
        player_turn = player_abc.get();
    }
}

bool Game::is_caravan_winning(CaravanName cvname) {
    if(cvname == NO_CARAVAN) {
        return false;
    }

    return winning_bid(cvname, get_opposite_caravan_name(cvname)) == cvname;
}

bool Game::is_caravan_bust(CaravanName cvname) {
    if (cvname == NO_CARAVAN) {
        return false;
    }

    return table->get_caravan(cvname)->get_bid() > CARAVAN_SOLD_MAX;
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

/*
 * PROTECTED
 */

int8_t Game::compare_bids(CaravanName cvname1, CaravanName cvname2) {
    if (has_sold(cvname1)) {
        if (has_sold(cvname2)) {
            uint8_t bid_cn1 = table->get_caravan(cvname1)->get_bid();
            uint8_t bid_cn2 = table->get_caravan(cvname2)->get_bid();

            if (bid_cn1 > bid_cn2) {
                return -1;  // CN1 sold; CN2 sold; CN1 highest bid
            }

            if (bid_cn1 < bid_cn2) {
                return 1;  // CN1 sold; CN2 sold; CN2 highest bid
            }

            return 0;  // CN1 sold; CN2 sold; matching bids
        }

        return -1;  // CN1 sold; CN2 unsold
    }

    if (has_sold(cvname2)) {
        return 1;  // CN1 unsold; CN2 sold
    }

    return 0;  // CN1 unsold; CN2 unsold
}

CaravanName Game::winning_bid(CaravanName cvname1, CaravanName cvname2) {
    int8_t bidcomp = compare_bids(cvname1, cvname2);

    if (bidcomp < 0) return cvname1;
    if (bidcomp > 0) return cvname2;
    return NO_CARAVAN;
}

bool Game::has_sold(CaravanName cvname) {
    uint8_t bid = table->get_caravan(cvname)->get_bid();
    return bid >= CARAVAN_SOLD_MIN and bid <= CARAVAN_SOLD_MAX;
}

void Game::option_clear(const Player *pptr, GameCommand *command) {
    PlayerCaravanNames pcns = get_player_caravan_names(pptr->get_name());

    if (pcns[0] != command->caravan_name and
        pcns[1] != command->caravan_name and
        pcns[2] != command->caravan_name) {
        throw CaravanIllegalModelException(
            "A player cannot clear their opponent's caravans.");
    }

    table->clear_caravan(command->caravan_name);
}

void Game::option_discard(Player *pptr, GameCommand *command) {
    Card c_hand;
    c_hand = pptr->discard_from_hand_at(command->pos_hand);

    command->hand = c_hand;  // Log to command
}

void Game::option_play(Player *pptr, GameCommand *command) {
    Card c_hand = pptr->get_from_hand_at(command->pos_hand);

    command->hand = c_hand;  // Log to command

    bool in_start_stage = pptr->get_moves_count() < MOVES_START_ROUND;
    bool pa_playing_num_onto_pa_caravans;
    bool pb_playing_num_onto_pb_caravans;

    if (c_hand.is_numeral_card()) {
        pa_playing_num_onto_pa_caravans =
            pptr->get_name() == player_abc->get_name() and
            (command->caravan_name == CARAVAN_A or
             command->caravan_name == CARAVAN_B or
             command->caravan_name == CARAVAN_C);

        pb_playing_num_onto_pb_caravans =
            pptr->get_name() == player_def->get_name() and
            (command->caravan_name == CARAVAN_D or
             command->caravan_name == CARAVAN_E or
             command->caravan_name == CARAVAN_F);

        if (!(pa_playing_num_onto_pa_caravans or
              pb_playing_num_onto_pb_caravans)) {
            throw CaravanIllegalModelException(
                "A numeral card can only be played on "
                "a player's own caravan.");
        }

        if (in_start_stage and
            table->get_caravan(command->caravan_name)->get_size() > 0) {
            throw CaravanIllegalModelException(
                "A numeral card must be played on an empty caravan "
                "during the Start round.");
        }

        table->play_numeral_card(command->caravan_name, c_hand);

    } else {  // is a face card
        if (in_start_stage) {
            throw CaravanIllegalModelException(
                "A face card cannot be played during the "
                "Start round.");
        }

        // Log to command
        command->board = table->get_caravan(command->caravan_name)->get_slot(command->pos_caravan).card;
        table->play_face_card(
            command->caravan_name,
            c_hand,
            command->pos_caravan);
    }

    pptr->discard_from_hand_at(command->pos_hand);
}
