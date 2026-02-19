enable_testing()

add_executable(tests
        "test_caravan/test_model/test_caravan.cpp"
        #"test_caravan/test_model/test_deck.cpp"
        #"test_caravan/test_model/test_game.cpp"
        "test_caravan/test_model/test_player.cpp"
        "test_caravan/test_model/test_table.cpp"
)

target_link_libraries(tests
        PRIVATE GTest::gtest_main
        PRIVATE core
        PRIVATE model
)

include(GoogleTest)
gtest_discover_tests(tests)
