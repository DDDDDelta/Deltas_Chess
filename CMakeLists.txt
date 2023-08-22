cmake_minimum_required(VERSION 3.25)
project(Deltas_Chess)

set(CMAKE_CXX_STANDARD 20)

add_executable(Deltas_Chess main.cpp chess_game.cpp chess_game.h pieces.cpp pieces.h code_utils.inc player.h moves.cpp moves.h GUI.h GUI.cpp board.cpp board.h controller.cpp controller.h)

set(SDL2_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDL2/include)
set(SDL2_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDL2/lib/x64)
include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})
target_link_libraries(Deltas_Chess ${CMAKE_CURRENT_SOURCE_DIR}/SDL2/lib/x64/SDL2main.lib ${CMAKE_CURRENT_SOURCE_DIR}/SDL2/lib/x64/SDL2.lib ${CMAKE_CURRENT_SOURCE_DIR}/SDL2/lib/x64/SDL2.dll)


set(SDL2_ttf_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDL2_ttf/include)
set(SDL2_ttf_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDL2_ttf/lib/x64)
include_directories(${SDL2_ttf_INCLUDE_DIR})
link_directories(${SDL2_ttf_LIB_DIR})
target_link_libraries(Deltas_Chess ${CMAKE_CURRENT_SOURCE_DIR}/SDL2_ttf/lib/x64/SDL2_ttf.lib ${CMAKE_CURRENT_SOURCE_DIR}/SDL2_ttf/lib/x64/SDL2_ttf.dll)


set(SDL2_image_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDL2_image/include)
set(SDL2_image_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDL2_image/lib/x64)
include_directories(${SDL2_image_INCLUDE_DIR})
link_directories(${SDL2_image_LIB_DIR})
target_link_libraries(Deltas_Chess ${CMAKE_CURRENT_SOURCE_DIR}/SDL2_image/lib/x64/SDL2_image.lib ${CMAKE_CURRENT_SOURCE_DIR}/SDL2_image/lib/x64/SDL2_image.dll)
