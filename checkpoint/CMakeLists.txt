target_compile_definitions(${PROJECT_NAME}
    PUBLIC
    "__CHECKPOINT__"
    )

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

target_sources(${PROJECT_NAME}
    PUBLIC
    "checkpoint.c"
    "checkpoint_logical_clock_nvm.c"
    )

set(CMAKE_CODE_ID TRUE
    CACHE BOOL "Use CMake to create the code-id (otherwise generated uing the current time during compilation)"
    )
add_subdirectory(code-id)

