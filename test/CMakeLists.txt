enable_testing()

add_executable(tests
        "caravan/model/test_caravan.cpp"
        "caravan/model/test_deck.cpp"
        "caravan/model/test_game.cpp"
        "caravan/model/test_player.cpp"
        "caravan/model/test_table.cpp"
)

target_link_libraries(tests
        PRIVATE GTest::gtest_main
        PRIVATE core
        PRIVATE model
)

include(GoogleTest)
gtest_discover_tests(tests)
