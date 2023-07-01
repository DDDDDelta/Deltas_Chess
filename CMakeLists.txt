cmake_minimum_required(VERSION 3.25)
project(Deltas_Chess)

set(CMAKE_CXX_STANDARD 20)

add_executable(Deltas_Chess main.cpp chess_game.cpp chess_game.h board.cpp board.h pieces.cpp pieces.h code_utils.h chess_macros.h player.h board_operator.cpp board_operator.h)
