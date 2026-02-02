add_library(user
        "${CMAKE_SOURCE_DIR}/include/caravan/user/constants.h"
        "${CMAKE_SOURCE_DIR}/include/caravan/user/functions.h"
        "${CMAKE_SOURCE_DIR}/include/caravan/user/user.h"

        "${CMAKE_SOURCE_DIR}/include/caravan/user/bot/factory.h"
        "${CMAKE_SOURCE_DIR}/include/caravan/user/bot/friendly.h"
        "${CMAKE_SOURCE_DIR}/include/caravan/user/bot/normal.h"
        "${CMAKE_SOURCE_DIR}/include/caravan/user/bot/random.h"

        "functions.cpp"

        "bot/factory.cpp"
        "bot/friendly.cpp"
        "bot/normal.cpp"
        "bot/random.cpp"
)

target_include_directories(user PUBLIC
        "${CMAKE_SOURCE_DIR}/include"
)
