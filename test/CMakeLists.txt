include_directories(../include)
include_directories(../lib/googletest-release-1.12.1/googletest/include)
include_directories(../lib/pdcurses-3.9/include)

set(test_caravan_src
        ../src/bot_simple.cpp
        ../src/caravan.cpp
        ../src/common.cpp
        ../src/controller.cpp
        ../src/deck.cpp
        ../src/engine.cpp
        ../src/exceptions.cpp
        ../src/player.cpp
        ../src/table.cpp
        ../src/user.cpp
        ../src/view.cpp)

set(test_caravan_tests
        test_game_caravan.cpp
        test_game_deck.cpp
        test_game_engine.cpp
        test_game_player.cpp
        test_game_table.cpp)

add_executable(test_caravan
        test_main.cpp
        ${test_caravan_src}
        ${test_caravan_tests})

target_link_libraries(test_caravan gtest pdcurses)

include(GoogleTest)
gtest_discover_tests(test_caravan)
