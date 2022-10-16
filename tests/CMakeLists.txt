project(tests)

set(files_game_model
    ../src/game/model.h
    ../src/game/model.cpp)

add_executable(tests_run test_game_model.cpp ${files_game_model})
target_link_libraries(tests_run gtest gtest_main)

add_subdirectory(lib)