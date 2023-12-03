cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_TOOLCHAIN_FILE "D:/Projects/vcpkg/scripts/buildsystems/vcpkg.cmake")
project(chip8_emulator VERSION 0.1.0 LANGUAGES C CXX)

find_package(SDL2 CONFIG REQUIRED)

add_executable(chip8_emulator main.cpp)
target_link_libraries(chip8_emulator PRIVATE SDL2::SDL2main SDL2::SDL2)