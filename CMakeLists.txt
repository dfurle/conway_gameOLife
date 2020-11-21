cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 11)

project(game VERSION 1.0)

file(GLOB GAME_SRC "src/*")

add_executable(game ${GAME_SRC})

target_include_directories(game PRIVATE include)

target_include_directories(game PRIVATE /opt/local/include/)

target_link_directories(game PRIVATE /opt/local/lib/)

TARGET_LINK_LIBRARIES(game sfml-graphics sfml-window sfml-system sfml-audio)