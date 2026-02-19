add_executable(test_caravan
        "test_controller/test_controller_ftxui.cpp"

        "test_model/test_caravan.cpp"
        "test_model/test_deck.cpp"
        "test_model/test_game.cpp"
        "test_model/test_player.cpp"
        "test_model/test_table.cpp"
)

target_link_libraries(test_caravan
        PRIVATE GTest::gtest_main
        PRIVATE controller
        PRIVATE core
        PRIVATE model
)

gtest_discover_tests(test_caravan)
