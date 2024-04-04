#Source: https://stackoverflow.com/a/31518137
add_custom_target(
    my_custom_target_that_always_runs ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/__false_header.h
)

#Copy the PRU code folder to NFS
add_custom_command(
    OUTPUT
#Use a false header to help run the code
        ${CMAKE_CURRENT_BINARY_DIR}/____false_header.h 
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${CMAKE_SOURCE_DIR}/pru-as4"
        "~/cmpt433/public/pru/pru-as4" 
    COMMENT "Copying PRU code to public NFS directory"
    COMMENT " Use make install_PRU0 on target"
)