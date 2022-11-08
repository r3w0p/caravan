add_subdirectory(googletest-release-1.12.1)

if(WIN32 OR MSVC)
    add_subdirectory(PDCurses-3.9)
    target_link_libraries(caravan_pdcurses PDCurses-3.9)
else()

endif()
