add_library(caravan_game STATIC controller.cpp model.cpp view.cpp)
target_link_libraries(${PROJECT_NAME} caravan_game)
