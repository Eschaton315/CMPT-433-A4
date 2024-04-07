# Building app using HAL

include_directories(include)
include_directories(../pru-as4/)
file(GLOB MY_SOURCES "src/*.c")
add_executable(findTheDot ${MY_SOURCES})

#Use the HAL library a

target_link_libraries(findTheDot LINK_PRIVATE hal pthread)

#create remote location

add_custom_command(TARGET findTheDot POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy
    "$<TARGET_FILE:findTheDot>"
    "~/cmpt433/public/myApps/findTheDot" 
    COMMENT "Copying ARM executable to public NFS directory")
    