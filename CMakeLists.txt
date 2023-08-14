cmake_minimum_required(VERSION 3.25)
project(Deltas_Chess)

set(CMAKE_CXX_STANDARD 20)

add_executable(Deltas_Chess main.cpp chess_game.cpp chess_game.h pieces.cpp pieces.h code_utils.inc player.h moves.cpp moves.h GUI.h GUI.cpp board.cpp board.h)
