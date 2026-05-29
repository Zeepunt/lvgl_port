file(GLOB_RECURSE sources CONFIGURE_DEPENDS *.c *.cpp)

add_library(pocketpy
    OBJECT
    ${sources}
)

target_include_directories(pocketpy
    PUBLIC
    .
)

target_link_libraries(pocketpy
    PUBLIC
    lvgl::lvgl
)

target_compile_definitions(pocketpy
    PRIVATE
    PK_GC_MIN_THRESHOLD=20000
    PK_VM_STACK_SIZE=4096
)

target_compile_options(pocketpy
    PRIVATE
    -Wno-error=unused-const-variable
    -Wno-maybe-uninitialized
    -Wno-error=char-subscripts
    -Wno-error=format
    -Wno-implicit-fallthrough
    -Wno-missing-field-initializers
    -Wno-int-to-pointer-cast
    -Wno-pointer-to-int-cast
    -Wno-unused-const-variable
    -Wno-char-subscripts
    -Wno-empty-body
    -Wno-format
)
