cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 20)
add_definitions(-DPDC_WIDE)

project(caravan VERSION 0.1)

set(PROJECT_COPYRIGHT "Copyright (c) 2022-2023 r3w0p")
set(PROJECT_DESCRIPTION "A CLI version of the Caravan card game from Fallout: New Vegas.")
set(PROJECT_URL "https://github.com/r3w0p/caravan")

add_subdirectory(src)
add_subdirectory(lib)
add_subdirectory(include)
add_subdirectory(test)

include_directories(include)

set_target_properties(caravan PROPERTIES LINK_SEARCH_START_STATIC ON)
set_target_properties(caravan PROPERTIES LINK_SEARCH_END_STATIC ON)

target_link_options(caravan PRIVATE -static-libgcc -static-libstdc++ -static)
