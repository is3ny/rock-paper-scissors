project(rps CXX)
cmake_minimum_required(VERSION 3.8)


# The code snippet is taken from StackOverflow
# Enables color output from ninja
option (FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." FALSE)
if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
       add_compile_options (-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
       add_compile_options (-fcolor-diagnostics)
    endif ()
endif ()


file(GLOB SRC ${CMAKE_SOURCE_DIR}/src/*)

add_executable(${PROJECT_NAME} ${SRC})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
