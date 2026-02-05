add_library(user
        "${CMAKE_SOURCE_DIR}/include/caravan/user/constants.h"
        "${CMAKE_SOURCE_DIR}/include/caravan/user/user.h"

        "${CMAKE_SOURCE_DIR}/include/caravan/user/bot/factory.h"
        "${CMAKE_SOURCE_DIR}/include/caravan/user/bot/random.h"

        "bot/factory.cpp"
        "bot/random.cpp"
)

target_include_directories(user PUBLIC
        "${CMAKE_SOURCE_DIR}/include"
)
