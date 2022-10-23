project(tests)

set(files_game_model
    ../src/game/model.h
    ../src/game/model.cpp)

add_executable(tests_run ${files_game_model}
        test_game_model_caravan.cpp
        test_game_model_deck_builder.cpp
        test_game_model_game.cpp
        test_game_model_player.cpp
        test_game_model_table.cpp)

target_link_libraries(tests_run gtest gtest_main)

add_subdirectory(lib)
