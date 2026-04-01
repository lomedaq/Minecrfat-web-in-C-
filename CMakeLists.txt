cmake_minimum_required(VERSION 3.22.1)
project(minecraft VERSION 1.0)

cmake_policy(SET CMP0135 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

include(FetchContent)
FetchContent_Declare(
  raylib
  URL https://github.com/raysan5/raylib/archive/refs/tags/5.0.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP ON
)
FetchContent_MakeAvailable(raylib)

set(SOURCES
    src/main.cpp
    src/world/world.cpp
    src/world/chunk/chunk.cpp
    src/utils/SimplexNoise.cpp
    src/generation/surface.cpp
    src/generation/cave.cpp
    src/player/physics.cpp
)

add_executable(minecraft ${SOURCES})

if(PLATFORM STREQUAL "Web")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")

    set(SHADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shaders")

    target_link_options(${PROJECT_NAME} PRIVATE
        "-sUSE_GLFW=3"
        "-sWASM=1"
        "-sALLOW_MEMORY_GROWTH=1"
        "-sASYNCIFY"
        "-sFORCE_FILESYSTEM=1"
        "-sSTACK_SIZE=4194304"
        "--preload-file" "${SHADER_DIR}@/shaders"
    )
    target_compile_definitions(minecraft PUBLIC PLATFORM_WEB)
endif()

target_include_directories(minecraft PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(minecraft raylib)