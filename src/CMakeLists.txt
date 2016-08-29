set(SOURCE_FILES
        main.cpp
        dvdbackup.cpp
        config.h
        optical_drive_finder.cpp
)

add_executable(dvdbackup ${SOURCE_FILES})
target_link_libraries(dvdbackup
        PRIVATE
        dvdread
        ${IOKit_LIBRARY}
        ${CoreFoundation_LIBRARY}
)
