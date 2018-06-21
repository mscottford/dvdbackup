set(DVDBACKUP_SOURCE_FILES
        dvdbackup-main.cpp
        dvdbackup.cpp
        config.h
)

add_executable(dvdbackup ${DVDBACKUP_SOURCE_FILES})
target_link_libraries(dvdbackup
        PRIVATE
        dvdread
)

set(READTITLE_SOURCE_FILES
        dvdreadtitle-main.cpp
        dvdbackup.cpp
        config.h
)

add_executable(dvdreadtitle ${READTITLE_SOURCE_FILES})
target_link_libraries(dvdreadtitle
        PRIVATE
        dvdread
)

set(OPTICAL_DRIVE_FINDER_SOURCE_FILES
        optical-drive-finder-main.cpp
)

add_executable(optical-drive-finder ${OPTICAL_DRIVE_FINDER_SOURCE_FILES})
target_link_libraries(optical-drive-finder
        PRIVATE
        ${IOKit_LIBRARY}
        ${CoreFoundation_LIBRARY}
)

