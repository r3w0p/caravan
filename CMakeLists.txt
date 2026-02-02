cmake_minimum_required(VERSION 3.20...4.2)

project(
        Caravan
        VERSION 2.0.0
        DESCRIPTION "A command-line version of the Caravan card game from Fallout: New Vegas."
        LANGUAGES C CXX
        HOMEPAGE_URL https://github.com/r3w0p/caravan
)

set(PROJECT_COPYRIGHT "Copyright (c) 2022-2026 r3w0p")

# ISO C++ standard mode without vendor-specific language extensions
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
# FetchContent fails gracefully when offline
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
# Suppresses output from FetchContent during the content fetching process
set(FETCHCONTENT_QUIET FALSE)


# FTXUI
FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
        GIT_TAG v5.0.0
)
FetchContent_MakeAvailable(ftxui)


# cxxopts
FetchContent_Declare(cxxopts
        GIT_REPOSITORY https://github.com/jarro2783/cxxopts
        GIT_TAG v3.3.1
)
FetchContent_MakeAvailable(cxxopts)


# GoogleTest
FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG v1.17.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


add_subdirectory("src")
add_subdirectory("test")
