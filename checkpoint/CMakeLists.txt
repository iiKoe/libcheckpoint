# Checkpoint content configuration
set(CMAKE_CODE_ID TRUE
    CACHE BOOL "Use CMake to create the code-id (otherwise generated uing the current time during compilation)"
    )

set(CHECKPOINT_DATA TRUE
    CACHE BOOL "Checkpoint the .data section"
    )

set(CHECKPOINT_BSS TRUE
    CACHE BOOL "Checkpoint the .bss section"
    )

set(CHECKPOINT_STACK TRUE
    CACHE BOOL "Checkpoint the stack"
    )

set(CHECKPOINT_REGISTERS TRUE
    CACHE BOOL "Checkpoint registers"
    )


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

add_subdirectory(code-id)

if (CHECKPOINT_DATA)
    add_subdirectory(data)
endif()

if (CHECKPOINT_BSS)
    add_subdirectory(bss)
endif()

if (CHECKPOINT_STACK)
    add_subdirectory(stack)
endif()

if (CHECKPOINT_REGISTERS)
    add_subdirectory(arch/arm)
endif()

