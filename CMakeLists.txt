cmake_minimum_required(VERSION 3.31)
project(volumetric-display)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wall -Wextra -Werror)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_link_options(-stdlib=libstdc++)
    add_compile_options(-Wall -Wextra -Werror)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/W4 /WX /EHsc)
endif()

find_package(glm)
find_package(raylib)
add_executable(grid_to_polar src/grid_to_polar.cpp)
target_link_libraries(grid_to_polar glm::glm raylib)

add_executable(rotating_cube src/rotating_cube_voxel.cpp)
target_link_libraries(rotating_cube glm::glm raylib)
