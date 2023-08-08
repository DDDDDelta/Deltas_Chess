#pragma once
#ifndef DELTAS_CHESS_GAME_RECORD_H
#define DELTAS_CHESS_GAME_RECORD_H

#include <cstddef>
#include <vector>
#include <tuple>

#include "code_utils.inc"
#include "pieces.h"
#include "moves.h"

NAMESPACE_DDDELTA_START
enum class E_Result : std::uint8_t {
    WHITE_WIN,
    BLACK_WIN,
    DRAW,
    UNFINISHED
};


class GameRecord {
public:
    struct MovePair;
    using size_type = std::vector<MovePair>::size_type;

    GameRecord();
    GameRecord(const std::string& pgn);
    GameRecord(const GameRecord&) = delete;
    GameRecord(GameRecord&&) = default;

    inline
    MovePair& operator [](size_type index) { return this->_moves[index - 1]; }
    inline
    size_type size() { return this->_moves.size(); }
public:
    E_Result res;
public:
    struct MovePair {
        RecordedMovement white_move;
        RecordedMovement black_move;
    };
private:
    std::vector<MovePair> _moves;
};
NAMESPACE_DDDELTA_END
#endif //DELTAS_CHESS_GAME_RECORD_H
