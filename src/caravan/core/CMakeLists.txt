add_library(core
        "${CMAKE_SOURCE_DIR}/include/caravan/core/exceptions.h"
        "${CMAKE_SOURCE_DIR}/include/caravan/core/functions.h"

        "exceptions.cpp"
        "functions.cpp"
)

target_include_directories(core PUBLIC
        "${CMAKE_SOURCE_DIR}/include"
)
