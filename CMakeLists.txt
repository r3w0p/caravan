cmake_minimum_required(VERSION 3.27)

project(caravan VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PROJECT_COPYRIGHT "Copyright (c) 2022-2024 r3w0p")
set(PROJECT_DESCRIPTION "A TUI version of the Caravan card game from Fallout: New Vegas.")
set(PROJECT_URL "https://github.com/r3w0p/caravan")

include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)

# FTXUI
FetchContent_Declare(ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
    GIT_TAG v5.0.0
)
FetchContent_MakeAvailable(ftxui)

# GoogleTest
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG v1.14.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


include_directories(include)

add_library(model
    include/model/deck.h

    src/model/deck.cpp
)



add_executable(caravan src/main1.cpp)

target_link_libraries(caravan
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component # Not needed for this example.
)
