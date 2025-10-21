cmake_minimum_required(VERSION 4.0)
project(raycaster)
file(GLOB_RECURSE sources src/*.cpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(raycaster ${sources})

target_compile_definitions(raycaster PRIVATE UNICODE _UNICODE _CRT_SECURE_NO_WARNINGS YAML_CPP_STATIC_DEFINE)
target_compile_options(raycaster PRIVATE "/utf-8")

target_include_directories(raycaster PRIVATE include include/opengl include/glm)

target_link_libraries(raycaster PRIVATE ${CMAKE_SOURCE_DIR}/lib/Gdi32.lib ${CMAKE_SOURCE_DIR}/lib/user32.lib ${CMAKE_SOURCE_DIR}/lib/opengl32.lib ${CMAKE_SOURCE_DIR}/lib/spdlogd.lib ${CMAKE_SOURCE_DIR}/lib/yaml-cppd.lib)

set_target_properties(raycaster PROPERTIES WIN32_EXECUTABLE True)

