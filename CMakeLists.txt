project(rps CXX C)
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

set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/glfw)

set(BUILD_STATIC_LIBS ON)
add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/glm/glm)

file(GLOB SRC ${CMAKE_SOURCE_DIR}/src/*)

add_executable(${PROJECT_NAME} ${SRC})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_FLAGS -O2)
target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES} glm_static)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/third_party/glfw/include/GLFW)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/third_party/glm/glm)
