// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/model/game.h"
#include "caravan/core/common.h"

/**
 * @param config Game configuration.
 * 
 * @throws CaravanFatalException Invalid player names.
 */
Game::Game(GameConfig *gc) {
    if (gc->player_first == NO_PLAYER) {
        throw CaravanFatalException("Invalid player name for first player in game configuration.");
    }

    Deck *deck_top = DeckBuilder::build_caravan_deck(
        gc->player_abc_cards,
        gc->player_abc_samples,
        gc->player_abc_balanced);

    Deck *deck_bottom = DeckBuilder::build_caravan_deck(
        gc->player_def_cards,
        gc->player_def_samples,
        gc->player_def_balanced);

    table_ptr = new Table();
    pa_ptr = new Player(PLAYER_ABC, deck_bottom);
    pb_ptr = new Player(PLAYER_DEF, deck_top);

    p_turn = gc->player_first == pa_ptr->get_name() ? pa_ptr : pb_ptr;
}

Game::~Game() {
    delete table_ptr;
    delete pa_ptr;
    delete pb_ptr;
}

Player *Game::get_player(PlayerName pname) {
    if (pa_ptr->get_name() == pname) {
        return pa_ptr;
    }

    if (pb_ptr->get_name() == pname) {
        return pb_ptr;
    }

    throw CaravanFatalException("Invalid player name.");
}

PlayerCaravanNames Game::get_player_caravan_names(PlayerName pname) {
    if (pa_ptr->get_name() == pname) {
        return PlayerCaravanNames{CARAVAN_A, CARAVAN_B, CARAVAN_C};
    }

    if (pb_ptr->get_name() == pname) {
        return PlayerCaravanNames{CARAVAN_D, CARAVAN_E, CARAVAN_F};
    }

    throw CaravanFatalException("Invalid player name.");
}

PlayerName Game::get_player_turn() {
    return p_turn->get_name();
}

Table *Game::get_table() {
    return table_ptr;
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
            return pa_ptr->get_name();

        } else if (won_pb >= 2) {
            return pb_ptr->get_name();
        }
    }

    // Neither player has outbid the other

    // Check if players have empty hands...

    if (pa_ptr->get_size_hand() > 0 and pb_ptr->get_size_hand() == 0) {
        return pa_ptr->get_name();

    } else if (pa_ptr->get_size_hand() == 0 and pb_ptr->get_size_hand() > 0) {
        return pb_ptr->get_name();
    }

    // Neither player has an empty hand

    // Nobody has won yet...

    return NO_PLAYER;
}

void Game::play_option(GameCommand *command) {
    if (get_winner() != NO_PLAYER) {
        throw CaravanFatalException(
            "The game has already been won.");
    }

    switch (command->option) {
        case OPTION_PLAY:
            option_play(p_turn, command, false);
            break;

        case OPTION_DISCARD:
            if (p_turn->get_moves_count() < MOVES_START_ROUND) {
                throw CaravanGameException(
                    "A player cannot discard a card during "
                    "the Start round.");
            }

            option_discard(p_turn, command, false);
            break;

        case OPTION_CLEAR:
            if (p_turn->get_moves_count() < MOVES_START_ROUND) {
                throw CaravanGameException(
                    "A player cannot clear a caravan during "
                    "the Start round.");
            }

            option_clear(p_turn, command, false);
            break;

        default:
            throw CaravanFatalException("Invalid play option.");
    }

    p_turn->increment_moves();
    p_turn->maybe_add_card_to_hand();

    if (pa_ptr->get_name() == p_turn->get_name()) {
        p_turn = pb_ptr;
    } else {
        p_turn = pa_ptr;
    }
}

bool Game::is_caravan_winning(CaravanName cvname) {
    if(cvname == NO_CARAVAN) {
        return false;
    } else {
        return winning_bid(cvname, get_opposite_caravan_name(cvname)) == cvname;
    }
}

bool Game::is_caravan_bust(CaravanName cvname) {
    if (cvname == NO_CARAVAN) {
        return false;
    } else {
        return table_ptr->get_caravan(cvname)->get_bid() > CARAVAN_SOLD_MAX;
    }
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
    uint8_t bid_cn1;
    uint8_t bid_cn2;

    if (has_sold(cvname1)) {
        if (has_sold(cvname2)) {
            bid_cn1 = table_ptr->get_caravan(cvname1)->get_bid();
            bid_cn2 = table_ptr->get_caravan(cvname2)->get_bid();

            if (bid_cn1 > bid_cn2) {
                return -1;  // CN1 sold; CN2 sold; CN1 highest bid
            } else if (bid_cn1 < bid_cn2) {
                return 1;  // CN1 sold; CN2 sold; CN2 highest bid
            } else {
                return 0;
            }  // CN1 sold; CN2 sold; matching bids

        } else {
            return -1;
        }  // CN1 sold; CN2 unsold

    } else if (has_sold(cvname2)) {
        return 1;  // CN1 unsold; CN2 sold

    } else {
        return 0;
    }  // CN1 unsold; CN2 unsold
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
    uint8_t bid = table_ptr->get_caravan(cvname)->get_bid();
    return bid >= CARAVAN_SOLD_MIN and bid <= CARAVAN_SOLD_MAX;
}

bool Game::option_clear(Player *pptr, GameCommand *command, bool check) {
    PlayerCaravanNames pcns = get_player_caravan_names(pptr->get_name());

    // Invalid for a player to clear their opponent's caravans
    if (pcns[0] != command->caravan_name and
        pcns[1] != command->caravan_name and
        pcns[2] != command->caravan_name) {

        if(check) {
            return false;

        } else {
            throw CaravanGameException(
                "A player cannot clear their opponent's caravans.");
        }
    }

    // If player's caravan is not empty, then it is valid to clear it
    if(check) {
        return table_ptr->get_caravan(
            command->caravan_name)->get_size() > 0;
    }

    // Clear the caravan
    table_ptr->clear_caravan(command->caravan_name);
    return true;
}

bool Game::option_discard(Player *pptr, GameCommand *command, bool check) {
    Card c_hand;
    uint8_t size_hand;

    if(check) {
        size_hand = pptr->get_size_hand();
        return size_hand > 0 and command->pos_hand <= size_hand;
    }

    c_hand = pptr->discard_from_hand_at(command->pos_hand);

    command->hand = c_hand;  // Log to command
    return true;
}

bool Game::option_play(Player *pptr, GameCommand *command, bool check) {  // TODO check
    Card c_hand = pptr->get_from_hand_at(command->pos_hand);

    command->hand = c_hand;  // Log to command

    bool in_start_stage = pptr->get_moves_count() < MOVES_START_ROUND;
    bool pa_playing_num_onto_pa_caravans;
    bool pb_playing_num_onto_pb_caravans;

    if (is_numeral_card(c_hand)) {
        pa_playing_num_onto_pa_caravans =
            pptr->get_name() == pa_ptr->get_name() and
            (command->caravan_name == CARAVAN_A or
             command->caravan_name == CARAVAN_B or
             command->caravan_name == CARAVAN_C);

        pb_playing_num_onto_pb_caravans =
            pptr->get_name() == pb_ptr->get_name() and
            (command->caravan_name == CARAVAN_D or
             command->caravan_name == CARAVAN_E or
             command->caravan_name == CARAVAN_F);

        if (!(pa_playing_num_onto_pa_caravans or
              pb_playing_num_onto_pb_caravans)) {
            throw CaravanGameException(
                "A numeral card can only be played on "
                "a player's own caravan.");
        }

        if (in_start_stage and
            table_ptr->get_caravan(command->caravan_name)->get_size() > 0) {
            throw CaravanGameException(
                "A numeral card must be played on an empty caravan "
                "during the Start round.");
        }

        table_ptr->play_numeral_card(command->caravan_name, c_hand);

    } else {  // is a face card
        if (in_start_stage) {
            throw CaravanGameException(
                "A face card cannot be played during the "
                "Start round.");
        }

        // Log to command
        command->board = table_ptr->get_caravan(command->caravan_name)->get_slot(command->pos_caravan).card;
        table_ptr->play_face_card(
            command->caravan_name,
            c_hand,
            command->pos_caravan);
    }

    pptr->discard_from_hand_at(command->pos_hand);
}
