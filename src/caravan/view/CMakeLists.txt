add_library(caravan_view
        "${CMAKE_SOURCE_DIR}/include/caravan/view/base_view.h"
        "${CMAKE_SOURCE_DIR}/include/caravan/view/view_ftxui.h"

        "view_ftxui.cpp"
)

target_include_directories(caravan_view PUBLIC
        "${CMAKE_SOURCE_DIR}/include"
)

target_link_libraries(caravan_view
        ftxui::dom
        ftxui::component
)
