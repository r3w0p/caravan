cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 20)

project(caravan VERSION 0.1)

set(PROJECT_NAME "caravan")
set(PROJECT_COPYRIGHT "Copyright (c) 2022 r3w0p")
set(PROJECT_DESCRIPTION "A CLI version of the Caravan card game from Fallout: New Vegas.")
set(PROJECT_URL "https://github.com/r3w0p/caravan")

add_subdirectory(src)
add_subdirectory(tests)
