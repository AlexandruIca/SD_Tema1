if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose type of build." FORCE)
    # Set the possible values of build type for cmake-gui, ccmake
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

find_program(CCACHE ccache)
if(CCACHE)
    message("Using ccache")
    set(CMAKE_CXX_COMPILER_LAUNCHER ${ccache})
else()
    message("CCache not found")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_LTO "Enable Link Time Optimization." OFF)

if(ENABLE_LTO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT result OUTPUT output)
    if(result)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    else()
        message(SEND_ERROR "LTO is not supported: ${output}")
    endif()
endif()
