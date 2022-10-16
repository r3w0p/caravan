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

    return 0;
}
