
target_include_directories(${PROJECT_NAME}
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/reg"
    )

target_sources(${PROJECT_NAME}
    PUBLIC
    "checkpoint_setup.c"
    "reg/checkpoint_registers.c"
    "reg/checkpoint_registers_nvm.c"
    #"reg/checkpoint_svc.s"
    )
