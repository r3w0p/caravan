add_executable(${PROJECT_NAME} main.cpp)

add_library(caravan_src STATIC
        bot_simple.cpp
        caravan.cpp
        common.cpp
        controller.cpp
        deck.cpp
        engine.cpp
        exceptions.cpp
        player.cpp
        table.cpp
        user.cpp
        view.cpp)

target_link_libraries(${PROJECT_NAME} caravan_src)
