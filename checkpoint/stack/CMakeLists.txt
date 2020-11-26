target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

target_sources(${PROJECT_NAME}
    PUBLIC
    "checkpoint_stack.c"
    "checkpoint_stack_nvm.c"
    )
