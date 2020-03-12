function(set_project_warnings project_name)
    option(WARNINGS_AS_ERRORS "Treat compiler warnings as errors." TRUE)

    set(MSVC_WARNINGS
        /W4 # Reasonable, /Wall is madness
        /analyze
        /permissive-)

    set(CLANG_WARNINGS
        -Wall
        -Wextra
        -Wshadow
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wcast-align
        -Wunused
        -Woverloaded-virtual
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wnull-dereference)

    if(WARNINGS_AS_ERRORS)
        set(CLANG_WARNINGS ${CLANG_WARNINGS} -Werror)
        set(MSVC_WARNINGS ${MSVC_WARNINGS} /WX)
    endif()

    set(GCC_WARNINGS
        ${CLANG_WARNINGS}
        -Wmisleading-indentation
        -Wduplicated-cond
        -Wuseless-cast)


    if(MSVC)
        set(PROJECT_WARNINGS ${MSVC_WARNINGS})
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(PROJECT_WARNINGS ${CLANG_WARNINGS})
    else()
        set(PROJECT_WARNINGS ${GCC_WARNINGS})
    endif()

    target_compile_options(${project_name} INTERFACE ${PROJECT_WARNINGS})
endfunction()
