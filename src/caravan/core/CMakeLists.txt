add_library(caravan_core
        "${CMAKE_SOURCE_DIR}/include/caravan/core/exceptions.h"
        "${CMAKE_SOURCE_DIR}/include/caravan/core/functions.h"

        "exceptions.cpp"
        "functions.cpp"
)

target_include_directories(caravan_core PUBLIC
        "${CMAKE_SOURCE_DIR}/include"
)
