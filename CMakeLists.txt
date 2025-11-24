cmake_minimum_required(VERSION 3.20)
project(FinalRoom LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories (your include/ folder with glad + KHR)
include_directories(include)

# Source files
set(SRC_FILES
    src/main.cpp
    src/glad.c
    src/Cube.cpp
)

add_executable(FinalRoom ${SRC_FILES})

# GLFW (installed via Homebrew)
find_package(glfw3 REQUIRED)

# GLM (header-only, no linking required)
# You may include GLM manually later if needed

# Link libraries for macOS
target_link_libraries(FinalRoom
    PRIVATE
        glfw
        "-framework OpenGL"
        "-ldl"
)