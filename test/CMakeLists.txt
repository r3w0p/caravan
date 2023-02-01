include_directories(../include)
include_directories(../lib/pdcurses-3.9/include)

add_executable(test_caravan
        test_game_caravan.cpp
        test_game_deck.cpp
        test_game_engine.cpp
        test_game_player.cpp
        test_game_table.cpp)

target_link_libraries(
        test_caravan
        GTest::gtest_main
        caravan_src
        pdcurses)

include(GoogleTest)
gtest_discover_tests(test_caravan)
