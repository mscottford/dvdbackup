set(SOURCE_FILES
        main.c
        dvdbackup.c
)
add_executable(dvdbackup ${SOURCE_FILES})
target_link_libraries(dvdbackup
        PRIVATE
        dvdread
        )

