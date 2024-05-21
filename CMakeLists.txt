cmake_minimum_required(VERSION 3.27)

project(caravan VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PROJECT_COPYRIGHT "Copyright (c) 2022-2024 r3w0p")
set(PROJECT_DESCRIPTION "A TUI version of the Caravan card game from Fallout: New Vegas.")
set(PROJECT_URL "https://github.com/r3w0p/caravan")

set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-static")

include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)


# --- FTXUI
FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
        GIT_TAG v5.0.0
)
FetchContent_MakeAvailable(ftxui)
# ---


# --- GoogleTest
FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG v1.14.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
# ---


# --- caravan libraries
include_directories(include)

add_library(controller
        "include/caravan/controller/controller.h"
        "include/caravan/controller/controller_tui.h"

        "src/caravan/controller/controller.cpp"
        "src/caravan/controller/controller_tui.cpp"
)

add_library(core
        "include/caravan/core/common.h"
        "include/caravan/core/exceptions.h"

        "src/caravan/core/common.cpp"
        "src/caravan/core/exceptions.cpp"
)

add_library(model
        "include/caravan/model/caravan.h"
        "include/caravan/model/deck.h"
        "include/caravan/model/game.h"
        "include/caravan/model/model.h"
        "include/caravan/model/model_tui.h"
        "include/caravan/model/player.h"
        "include/caravan/model/table.h"

        "src/caravan/model/caravan.cpp"
        "src/caravan/model/deck.cpp"
        "src/caravan/model/game.cpp"
        "src/caravan/model/model.cpp"
        "src/caravan/model/model_tui.cpp"
        "src/caravan/model/player.cpp"
        "src/caravan/model/table.cpp"
)

add_library(user
        "include/caravan/user/bot_easy.h"
        "include/caravan/user/user.h"

        "src/caravan/user/bot_easy.cpp"
        "src/caravan/user/user.cpp"
)

add_library(view
        "include/caravan/view/view.h"
        "include/caravan/view/view_tui.h"

        "src/caravan/view/view.cpp"
        "src/caravan/view/view_tui.cpp"
)

target_link_libraries(view
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
)
# ---


# --- caravan.exe
add_executable(caravan
        "src/caravan/main.cpp"
)

target_link_libraries(caravan
        PRIVATE controller
        PRIVATE core
        PRIVATE model
        PRIVATE user
        PRIVATE view
)
# ---


# --- test.exe
enable_testing()

add_executable(tests
        "test/caravan/model/test_caravan.cpp"
        "test/caravan/model/test_deck.cpp"
        "test/caravan/model/test_game.cpp"
        "test/caravan/model/test_player.cpp"
        "test/caravan/model/test_table.cpp"
)

target_link_libraries(tests
        PRIVATE GTest::gtest_main
        PRIVATE core
        PRIVATE model
)

include(GoogleTest)
gtest_discover_tests(tests)
# ---
