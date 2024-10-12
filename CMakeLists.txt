cmake_minimum_required(VERSION 3.27)

project(Caravan LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PROJECT_VERSION 1.3.0)
set(PROJECT_DESCRIPTION "A command-line version of the Caravan card game from Fallout: New Vegas.")
set(PROJECT_COPYRIGHT "Copyright (c) 2022-2024 r3w0p")
set(PROJECT_URL "https://github.com/r3w0p/caravan")

IF (WIN32 OR WIN64)
set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-static")
ENDIF ()

include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
set(FETCHCONTENT_QUIET FALSE)


# --- FTXUI
FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
        GIT_TAG v5.0.0
)
FetchContent_MakeAvailable(ftxui)
# ---


# --- cxxopts
FetchContent_Declare(cxxopts
        GIT_REPOSITORY https://github.com/jarro2783/cxxopts
        GIT_TAG v3.2.0
)
FetchContent_MakeAvailable(cxxopts)
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
        "include/caravan/model/player.h"
        "include/caravan/model/table.h"

        "src/caravan/model/caravan.cpp"
        "src/caravan/model/deck.cpp"
        "src/caravan/model/game.cpp"
        "src/caravan/model/player.cpp"
        "src/caravan/model/table.cpp"
)

target_link_libraries(model core)

add_library(user
        "include/caravan/user/user.h"
        "include/caravan/user/bot/ai.h"
        "include/caravan/user/bot/factory.h"
        "include/caravan/user/bot/friendly.h"
        "include/caravan/user/bot/normal.h"

        "src/caravan/user/bot/ai.cpp"
        "src/caravan/user/bot/factory.cpp"
        "src/caravan/user/bot/friendly.cpp"
        "src/caravan/user/bot/normal.cpp"
)

add_library(view
        "include/caravan/view/view.h"
        "include/caravan/view/view_tui.h"

        "src/caravan/view/view.cpp"
        "src/caravan/view/view_tui.cpp"
)

target_link_libraries(view
    ftxui::dom
    ftxui::component
)
# ---


# --- caravan.exe
add_executable(caravan
        "src/caravan/main.cpp"
)

target_compile_definitions(caravan
        PRIVATE CARAVAN_NAME="${PROJECT_NAME}"
        PRIVATE CARAVAN_VERSION="${PROJECT_VERSION}"
        PRIVATE CARAVAN_DESCRIPTION="${PROJECT_DESCRIPTION}"
        PRIVATE CARAVAN_COPYRIGHT="${PROJECT_COPYRIGHT}"
        PRIVATE CARAVAN_URL="${PROJECT_URL}"
)

target_link_libraries(caravan
        PRIVATE core
        PRIVATE model
        PRIVATE user
        PRIVATE view
        PRIVATE cxxopts
)
# ---


# --- train.exe
add_executable(train
        "src/caravan/train.cpp"
)

target_compile_definitions(train
        PRIVATE CARAVAN_NAME="${PROJECT_NAME}"
        PRIVATE CARAVAN_VERSION="${PROJECT_VERSION}"
        PRIVATE CARAVAN_DESCRIPTION="${PROJECT_DESCRIPTION}"
        PRIVATE CARAVAN_COPYRIGHT="${PROJECT_COPYRIGHT}"
        PRIVATE CARAVAN_URL="${PROJECT_URL}"
)

target_link_libraries(train
        PRIVATE core
        PRIVATE model
        PRIVATE user
        PRIVATE view
        PRIVATE cxxopts
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
