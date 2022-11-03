add_library(caravan_bots STATIC
        bot_simple.cpp)

target_link_libraries(${PROJECT_NAME} caravan_bots)
