include_directories(../include)
include_directories(../lib/pdcurses-3.9/include)

add_library(caravan_src STATIC
        bot_simple.cpp
        caravan.cpp
        common.cpp
        controller.cpp
        deck.cpp
        engine.cpp
        exceptions.cpp
        player.cpp
        table.cpp
        user.cpp
        view.cpp)

add_executable(caravan main.cpp)

target_link_libraries(caravan caravan_src)
