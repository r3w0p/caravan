cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(caravan VERSION 0.1)

set(PROJECT_NAME "Caravan")
set(PROJECT_COPYRIGHT "Copyright (c) 2022 r3w0p")
set(PROJECT_DESCRIPTION "A CLI version of the Caravan card game from Fallout: New Vegas.")
set(PROJECT_URL "https://github.com/r3w0p/caravan")

add_executable(caravan src/main.cpp src/game/table.cpp)
