project(tests)

set(tests_files
        ../src/controller.cpp
        ../include/controller.h

        ../src/caravan.cpp
        ../include/caravan.h

        ../src/deck.cpp
        ../include/deck.h

        ../src/engine.cpp
        ../include/engine.h

        ../src/player.cpp
        ../include/player.h

        ../src/table.cpp
        ../include/table.h

        ../src/common.cpp
        ../include/common.h

        ../src/exceptions.cpp
        ../include/exceptions.h

        ../src/view.cpp
        ../include/view.h)

add_executable(tests_run ${tests_files}
        test_game_caravan.cpp
        test_game_deck.cpp
        test_game_engine.cpp
        test_game_player.cpp
        test_game_table.cpp)

target_link_libraries(tests_run gtest gtest_main)
