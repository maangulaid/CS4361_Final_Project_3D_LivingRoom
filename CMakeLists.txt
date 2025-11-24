cmake_minimum_required(VERSION 3.20)
project(FinalRoom LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# If you used vcpkg at C:\dev\vcpkg
if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()
set(VCPKG_TARGET_TRIPLET "x64-mingw-dynamic" CACHE STRING "")

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(FinalRoom
    src/main.cpp
    src/shader.h
)

target_link_libraries(FinalRoom
    PRIVATE
        glfw
        glad::glad
        glm::glm
        opengl32
)
