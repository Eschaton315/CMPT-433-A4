#Build library 'hal' exposing the header files as "hal/*.h"
#Use header as: #include "hal/button.h"

include_directories(hal/include)
include_directories(../pru-as4/)
file(GLOB MY_SOURCES "src/*.c")

add_library(hal STATIC ${MY_SOURCES})

target_include_directories(hal PUBLIC include)
