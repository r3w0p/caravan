include_directories(../include)
include_directories(../lib/pdcurses-3.9/include)

set(test_files
        ../src/bot_simple.cpp
        ../include/bot_simple.h

        ../src/caravan.cpp
        ../include/caravan.h

        ../src/common.cpp
        ../include/common.h

        ../src/controller.cpp
        ../include/controller.h

        ../src/deck.cpp
        ../include/deck.h

        ../src/engine.cpp
        ../include/engine.h

        ../src/exceptions.cpp
        ../include/exceptions.h

        ../src/player.cpp
        ../include/player.h

        ../src/table.cpp
        ../include/table.h

        ../src/user.cpp
        ../include/user.h

        ../src/view.cpp
        ../include/view.h)

add_executable(test_caravan ${test_files}
        test_game_caravan.cpp
        test_game_deck.cpp
        test_game_engine.cpp
        test_game_player.cpp
        test_game_table.cpp)

target_link_libraries(test_caravan gtest gtest_main pdcurses)
