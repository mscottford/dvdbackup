set(SOURCE_FILES
        main.c
        dvdbackup.c
        config.h
        optical_drive_finder.c
)

add_executable(dvdbackup ${SOURCE_FILES})
target_link_libraries(dvdbackup
        PRIVATE
        dvdread
        ${IOKit_LIBRARY}
        ${CoreFoundation_LIBRARY}
)
