project(tests)

set(test_files_game
        ../src/game/caravan.h
        ../src/game/caravan.cpp
        ../src/game/common.h
        ../src/game/controller.h
        ../src/game/controller.cpp
        ../src/game/deck.h
        ../src/game/deck.cpp
        ../src/game/engine.h
        ../src/game/engine.cpp
        ../src/game/exceptions.h
        ../src/game/player.h
        ../src/game/player.cpp
        ../src/game/table.h
        ../src/game/table.cpp
        ../src/game/view.h
        ../src/game/view.cpp)

add_executable(tests_run ${test_files_game}
        test_game_caravan.cpp
        test_game_deck.cpp
        test_game_engine.cpp
        test_game_player.cpp
        test_game_table.cpp)

target_link_libraries(tests_run gtest gtest_main)

add_subdirectory(lib)
