cmake_minimum_required(VERSION 3.20)
project(FinalRoom LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(FinalRoom
    src/main.cpp
    src/Cube.cpp
    src/stb_image_imp.cpp   # exactly once
    # headers are optional in the list; keeping them here is fine
    src/Cube.h
    src/Furniture.h
    src/Room.h
    src/shader.h
    src/stb_image.h
)

# include path AFTER the target exists
target_include_directories(FinalRoom PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(FinalRoom PRIVATE
    glfw
    glad::glad
    glm::glm
    opengl32
)

# (Optional) make debugging paths sane when launched from VS
set_property(TARGET FinalRoom PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
