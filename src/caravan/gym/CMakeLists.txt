add_library(caravan_gym
        "${CMAKE_SOURCE_DIR}/include/caravan/gym/base_environment.h"
        "${CMAKE_SOURCE_DIR}/include/caravan/gym/constants.h"
        "${CMAKE_SOURCE_DIR}/include/caravan/gym/environment_game.h"
        "${CMAKE_SOURCE_DIR}/include/caravan/gym/types.h"

        "environment_game.cpp"
)

target_include_directories(caravan_gym PUBLIC
        "${CMAKE_SOURCE_DIR}/include"
)
