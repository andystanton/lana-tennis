cmake_minimum_required(VERSION 2.8)
project(lana-tennis)

set(lana-tennis_VERSION_MAJOR 1)
set(lana-tennis_VERSION_MINOR 0)

set (
    HEADERS
    headers/System.h
)

set (
    SOURCES
    src/main.cpp
    src/System.cpp
)

# lana-tennis
add_executable(
    lana-tennis
    ${HEADERS}
    ${SOURCES}
)

# enable testing - not currently in use but required for 'make test' that travis-ci runs.
enable_testing()

# glfw
add_subdirectory(lib/glfw)
include_directories(lib/glfw/include)
target_link_libraries(lana-tennis glfw ${GLFW_LIBRARIES})
target_link_libraries(lana-tennis glfw ${OPENGL_glu_LIBRARY} ${GLFW_LIBRARIES})

