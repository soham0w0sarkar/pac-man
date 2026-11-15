cmake_minimum_required(VERSION 3.20)
project(PacMan LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

add_executable(PacMan
  src/main.cpp
  src/Game.cpp
  src/Berry.cpp
  src/Ghost.cpp
  src/GhostManager.cpp
  src/Maze.cpp
  src/Pathfinding.cpp
  src/Pellet.cpp
  src/Pacman.cpp
)

target_include_directories(PacMan PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(PacMan PRIVATE SFML::Graphics SFML::Window SFML::System)


add_custom_target(copy_assets ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/assets
          $<TARGET_FILE_DIR:PacMan>/assets
  COMMENT "Copying assets to build directory"
  VERBATIM
)
add_dependencies(copy_assets PacMan)
