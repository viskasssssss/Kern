cmake_minimum_required(VERSION 3.15)
project(Kern LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =========================
# KERN SOURCES
# =========================

add_library(kern STATIC
    src/utils/shaders.cpp
    src/utils/stb.cpp
    src/utils/vectors.cpp
    src/backends/OpenGL/openglrenderer.cpp
    src/utils/vertexlayout.cpp
)

# =========================
# INCLUDE PATHS
# =========================

target_include_directories(kern
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/libraries/include
)

# =========================
# GLAD (C FILE)
# =========================

add_library(glad STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c
)

target_include_directories(glad
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/libraries/include
)

# =========================
# LINK
# =========================

target_link_libraries(kern
    PUBLIC glad
)

# =========================
# SHADERS
# =========================

set(SHADER_DEST_DIR "${CMAKE_BINARY_DIR}/src/shaders/OpenGL")

# Shaders List
file(GLOB KERN_SHADERS "src/shaders/OpenGL/*")

foreach(shader ${KERN_SHADERS})
    add_custom_command(
        TARGET kern POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADER_DEST_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy ${shader} ${SHADER_DEST_DIR}
    )
endforeach()