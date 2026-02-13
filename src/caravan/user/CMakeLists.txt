add_library(user
        "${CMAKE_SOURCE_DIR}/include/caravan/user/base_user.h"
        "${CMAKE_SOURCE_DIR}/include/caravan/user/base_user_bot.h"
        "${CMAKE_SOURCE_DIR}/include/caravan/user/base_user_human.h"
        "${CMAKE_SOURCE_DIR}/include/caravan/user/bot_factory.h"
        "${CMAKE_SOURCE_DIR}/include/caravan/user/user_bot_random.h"
        "${CMAKE_SOURCE_DIR}/include/caravan/user/user_human_ftxui.h"

        "bot_factory.cpp"
        "user_bot_random.cpp"
)

target_include_directories(user PUBLIC
        "${CMAKE_SOURCE_DIR}/include"
)
