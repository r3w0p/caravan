add_library(controller
        "${CMAKE_SOURCE_DIR}/include/caravan/controller/base_controller.h"
        "${CMAKE_SOURCE_DIR}/include/caravan/controller/controller_ftxui.h"

        "controller_ftxui.cpp"
)

target_include_directories(controller PUBLIC
        "${CMAKE_SOURCE_DIR}/include"
)
