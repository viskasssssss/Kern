cmake_minimum_required(VERSION 3.15)
project(Kern)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Пути к include
include_directories(src)
include_directories(src/utils)
include_directories(src/backends)
include_directories(src/backends/OpenGL)  # только OpenGL бэкенд на старте

# Список исходников
set(SRC_FILES
    src/utils/shaders.cpp
    src/utils/stb.cpp
    src/utils/vectors.cpp
    src/backends/OpenGL/openglrenderer.cpp
)

# Собираем статическую библиотеку
add_library(kern STATIC ${SRC_FILES})

# Заголовки для пользователя
target_include_directories(kern PUBLIC src)
