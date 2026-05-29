option(ENABLE_COMPONENT_POCKETPY "Enable Component Pocketpy" OFF)

add_library(components INTERFACE)

if(ENABLE_COMPONENT_POCKETPY)
    add_subdirectory(pocketpy)

    target_sources(components
        INTERFACE
        $<TARGET_OBJECTS:pocketpy>
    )

    target_link_libraries(components
        INTERFACE
        pocketpy
    )

    target_compile_definitions(components
        INTERFACE
        ENABLE_COMPONENT_POCKETPY
    )
endif()