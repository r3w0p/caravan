add_library(caravan_game STATIC
        caravan.cpp
        common.cpp
        controller.cpp
        deck.cpp
        engine.cpp
        exceptions.cpp
        player.cpp
        table.cpp
        view.cpp)

target_link_libraries(${PROJECT_NAME} caravan_game)
