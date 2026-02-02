add_library(model
        "${CMAKE_SOURCE_DIR}/include/caravan/model/caravan.h"
        "${CMAKE_SOURCE_DIR}/include/caravan/model/deck.h"
        "${CMAKE_SOURCE_DIR}/include/caravan/model/game.h"
        "${CMAKE_SOURCE_DIR}/include/caravan/model/player.h"
        "${CMAKE_SOURCE_DIR}/include/caravan/model/table.h"

        "caravan.cpp"
        "deck.cpp"
        "game.cpp"
        "player.cpp"
        "table.cpp"
)

target_include_directories(model PUBLIC
        "${CMAKE_SOURCE_DIR}/include"
)
