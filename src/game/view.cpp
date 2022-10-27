// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "view.h"
#include "exceptions.h"
#include <string>
#include <iostream>


std::string direction_to_str(Direction dir) {
    switch (dir) {
        case NO_DIRECTION:
            return "ANY";
        case ASCENDING:
            return "ASC";
        case DESCENDING:
            return "DES";
        default:
            throw CaravanFatalException("Invalid direction.");
    }
}

void display_test_caravan(Table *t, CaravanName cn) {
    std::cout << caravan_name_to_str(cn) << " [";
    std::cout << "bid=" << t->get_caravan_bid(cn) << ", ";
    std::cout << "suit=" << suit_to_str(t->get_caravan_suit(cn)) << ", ";
    std::cout << "dir=" << direction_to_str(t->get_caravan_direction(cn))
              << ", ";
    int p_size = t->get_caravan_size(cn);
    std::cout << "size=" << p_size << "]: ";

    for (int pos = 1; pos <= p_size; ++pos) {
        if (pos > 1)
            std::cout << " ";

        Slot sn = t->get_caravan_cards_at(cn, pos);
        std::cout << rank_to_str(sn.card.rank) << suit_to_str(sn.card.suit)
                  << " (";

        for (int m = 0; m < sn.i_faces; ++m) {
            if (m > 0)
                std::cout << " ";

            std::cout << rank_to_str(sn.faces[m].rank)
                      << suit_to_str(sn.faces[m].suit);
        }
        std::cout << ")";
    }
    std::cout << std::endl;
}


void display_test(Engine *e) {
    Table *t = e->get_table();
    Player *pa = e->get_player(PLAYER_A);
    Player *pb = e->get_player(PLAYER_B);
    Card c;

    display_test_caravan(t, CARAVAN_A);
    display_test_caravan(t, CARAVAN_D);

    std::cout << std::endl;

    display_test_caravan(t, CARAVAN_B);
    display_test_caravan(t, CARAVAN_E);

    std::cout << std::endl;

    display_test_caravan(t, CARAVAN_C);
    display_test_caravan(t, CARAVAN_F);

    std::cout << std::endl << "PLAYER A:";
    for (int i = 0; i < pa->get_size_hand(); ++i) {
        c = pa->get_from_hand_at(i + 1);
        std::cout << " " << rank_to_str(c.rank) << suit_to_str(c.suit);
    }

    std::cout << std::endl << "PLAYER B:";
    for (int i = 0; i < pb->get_size_hand(); ++i) {
        c = pb->get_from_hand_at(i + 1);
        std::cout << " " << rank_to_str(c.rank) << suit_to_str(c.suit);
    }
    std::cout << std::endl;
}

void ViewCLI::display(Engine *g, std::string msg) {
    display_test(g);

    if (msg.size() > 0)
        std::cout << std::endl << msg << std::endl;

    std::cout << std::endl;
}
