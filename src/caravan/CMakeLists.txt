add_subdirectory("core")
add_subdirectory("gym")
add_subdirectory("model")
add_subdirectory("user")
add_subdirectory("controller")
add_subdirectory("view")

add_executable(caravan "main.cpp")

target_compile_definitions(caravan
        PRIVATE CARAVAN_NAME="${PROJECT_NAME}"
        PRIVATE CARAVAN_VERSION="${PROJECT_VERSION}"
        PRIVATE CARAVAN_DESCRIPTION="${PROJECT_DESCRIPTION}"
        PRIVATE CARAVAN_URL="${PROJECT_HOMEPAGE_URL}"
        PRIVATE CARAVAN_COPYRIGHT="${PROJECT_COPYRIGHT}"
)

target_link_libraries(caravan_model PUBLIC caravan_core)

target_link_libraries(caravan
        PRIVATE caravan_core
        PRIVATE caravan_gym
        PRIVATE caravan_model
        PRIVATE caravan_user
        PRIVATE caravan_controller
        PRIVATE caravan_view
        PRIVATE cxxopts
)
