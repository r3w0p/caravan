include_directories(../include)
include_directories(../lib/pdcurses-3.9/include)

add_executable(tests
        test_caravan.cpp
        test_deck.cpp
        test_engine.cpp
        test_player.cpp
        test_table.cpp)

target_link_libraries(
        tests
        GTest::gtest_main
        caravan_src
        pdcurses)

include(GoogleTest)
gtest_discover_tests(tests)
