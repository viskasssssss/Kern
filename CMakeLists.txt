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

file(GLOB_RECURSE SHADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/*"
)

foreach(SHADER_FILE ${SHADER_FILES})
    file(COPY ${SHADER_FILE} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/src/shaders")
endforeach()
