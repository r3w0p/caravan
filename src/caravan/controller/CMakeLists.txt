add_library(controller
        "${CMAKE_SOURCE_DIR}/include/caravan/controller/controller.h"
        "${CMAKE_SOURCE_DIR}/include/caravan/controller/controller_str_to_move.h"

        "controller_str_to_move.cpp"
)

target_include_directories(controller PUBLIC
        "${CMAKE_SOURCE_DIR}/include"
)
