add_library(core
        "${CMAKE_SOURCE_DIR}/include/caravan/core/exceptions.h"

        "exceptions.cpp"
)

target_include_directories(core PUBLIC
        "${CMAKE_SOURCE_DIR}/include"
)
