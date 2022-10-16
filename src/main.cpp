// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <iostream>
#include "game/model.h"
#include "game/view.h"

int main() {
    Deck d = DeckBuilder::build_caravan_deck(30, 2, false);
    Table table = Table();
    ViewCLI cli = ViewCLI();

    table.play_numeric_card(PILE_A, { SPADES, TWO });
    table.play_numeric_card(PILE_A, { HEARTS, FOUR });
    table.play_numeric_card(PILE_A, { HEARTS, ACE });
    table.play_numeric_card(PILE_A, { HEARTS, TWO });
    table.play_numeric_card(PILE_A, { HEARTS, ACE });
    table.play_numeric_card(PILE_A, { HEARTS, TWO });
    table.play_numeric_card(PILE_A, { HEARTS, ACE });
    table.play_numeric_card(PILE_A, { HEARTS, TWO });
    table.play_numeric_card(PILE_A, { HEARTS, ACE });
    table.play_numeric_card(PILE_A, { HEARTS, TWO });
    table.play_face_card(PILE_A, { SPADES, QUEEN }, 10);
    table.play_face_card(PILE_A, { SPADES, QUEEN }, 10);
    table.play_face_card(PILE_A, { SPADES, QUEEN }, 10);
    table.play_face_card(PILE_A, { SPADES, QUEEN }, 10);
    table.play_face_card(PILE_A, { SPADES, QUEEN }, 10);
    table.play_face_card(PILE_A, { SPADES, QUEEN }, 9);
    table.play_face_card(PILE_A, { SPADES, QUEEN }, 9);

    table.play_numeric_card(PILE_D, { CLUBS, TEN });
    table.play_numeric_card(PILE_D, { DIAMONDS, NINE });

    cli.display(table); std::cout << "\n";

    return 0;
}
