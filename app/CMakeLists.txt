# Building app using HAL

include_directories(include)
include_directories(../pru-as4/)
file(GLOB MY_SOURCES "src/*.c")
add_executable(findTheDot ${MY_SOURCES})

#Use the HAL library and ALSA support
find_package(ALSA REQUIRED)
target_link_libraries(findTheDot LINK_PRIVATE hal asound pthread)

#create remote location

add_custom_command(TARGET findTheDot POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy
    "$<TARGET_FILE:findTheDot>"
    "~/cmpt433/public/myApps/findTheDot" 
    COMMENT "Copying ARM executable to public NFS directory"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
    "${CMAKE_SOURCE_DIR}/wave-files"
    "~/cmpt433/public/myApps/wave-files"
    COMMENT "Copying WAVE files to public NFS directory")