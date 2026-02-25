add_library(caravan_controller
        "${CMAKE_SOURCE_DIR}/include/caravan/controller/base_controller.h"
        "${CMAKE_SOURCE_DIR}/include/caravan/controller/controller_ftxui.h"

        "controller_ftxui.cpp"
)

target_include_directories(caravan_controller PUBLIC
        "${CMAKE_SOURCE_DIR}/include"
)
