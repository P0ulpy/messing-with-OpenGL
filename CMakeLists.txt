cmake_minimum_required(VERSION 3.24)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)

project(messing-with-OpenGL)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
find_package(GLEW REQUIRED)

configure_file(triangles.frag ${CMAKE_CURRENT_BINARY_DIR}/Debug/triangles.frag COPYONLY)
configure_file(triangles.vert ${CMAKE_CURRENT_BINARY_DIR}/Debug/triangles.vert COPYONLY)

configure_file(cube.frag ${CMAKE_CURRENT_BINARY_DIR}/Debug/cube.frag COPYONLY)
configure_file(cube.vert ${CMAKE_CURRENT_BINARY_DIR}/Debug/cube.vert COPYONLY)

configure_file(moche2.bmp ${CMAKE_CURRENT_BINARY_DIR}/Debug/moche2.bmp COPYONLY)

add_executable(messing-with-OpenGL main.cpp Shader.cpp Shader.hpp Matrix.hpp Cube.hpp Triangle.hpp Color.hpp Texture.hpp)

target_link_libraries(messing-with-OpenGL PRIVATE sfml-system sfml-network sfml-graphics sfml-window)
target_link_libraries(messing-with-OpenGL PRIVATE GLEW::GLEW)
