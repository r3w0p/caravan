// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <iostream>
#include "game/table.h"

int main() {
    Deck d = DeckBuilder::build_caravan_deck(30, 2, true);

    for(auto & c : d) {
        std::cout << c.rank << ", " << c.suit << std::endl;
    }
    std::cout << "total: " << d.size() << std::endl;

    // std::cout << "Hello, World!" << std::endl;
    return 0;
}
