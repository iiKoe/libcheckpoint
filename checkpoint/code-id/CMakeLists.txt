if (CMAKE_CODE_ID)
    string(RANDOM
        LENGTH 8
        ALPHABET "0123456789ABCDEF"
        COMPILE_TIME_CODE_ID
        )

    add_compile_definitions(
        "COMPILE_TIME_CODE_ID=0x${COMPILE_TIME_CODE_ID}"
        )

    message("COMPILE TIME CODE ID = 0x${COMPILE_TIME_CODE_ID}")
endif()

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

target_sources(${PROJECT_NAME}
    PUBLIC
    "code_id.c"
    "code_id_nvm.c"
    )

# Always force the code-id file to be rebuild
add_custom_target(rebuild-code-id ALL
                  COMMAND ${CMAKE_COMMAND} -E touch
                  "${CMAKE_CURRENT_SOURCE_DIR}/code_id.c")
add_dependencies(${PROJECT_NAME} rebuild-code-id)

