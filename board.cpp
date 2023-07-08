#include "board.h"
#include "code_utils.h"

NAMESPACE_DDDELTA_START
Board::Board() :
    _board{}, _black_lost_val(0), _white_lost_val(0) {
    this->set_up();
}

Piece& Board::get_piece(const BoardCoor co) {
    return this->_board[co.x - 1][co.y - 1];
}

const Board::ChessBoard& Board::get_board() const {
    return this->_board;
}

void Board::set_up() {
    this->get_piece({ 1, 1 }) = WhiteRook;
    this->get_piece({ 2, 1 }) = WhiteKnight;
    this->get_piece({ 3, 1 }) = WhiteBishop;
    this->get_piece({ 4, 1 }) = WhiteQueen;
    this->get_piece({ 5, 1 }) = WhiteKing;
    this->get_piece({ 6, 1 }) = WhiteBishop;
    this->get_piece({ 7, 1 }) = WhiteKnight;
    this->get_piece({ 8, 1 }) = WhiteRook;
    this->get_piece({ 1, 8 }) = BlackRook;
    this->get_piece({ 2, 8 }) = BlackKnight;
    this->get_piece({ 3, 8 }) = BlackBishop;
    this->get_piece({ 4, 8 }) = BlackQueen;
    this->get_piece({ 5, 8 }) = BlackKing;
    this->get_piece({ 6, 8 }) = BlackBishop;
    this->get_piece({ 7, 8 }) = BlackKnight;
    this->get_piece({ 8, 8 }) = BlackRook;

    for (std::uint8_t i = 1; i <= 8; i++) {
        this->get_piece({ i, 2 }) = WhitePawn;
        this->get_piece({ i, 7 }) = BlackPawn;
    }

    for (std::uint8_t i = 1; i <= 8; i++)
        for (std::uint8_t j = 3; j <= 6; j++)
            this->get_piece({ i, j }) = Empty;
}

void Board::reset() {
    this->set_up();
    this->_white_lost_val = 0;
    this->_black_lost_val = 0;
    this->_captured_white.clear();
    this->_captured_white.clear();
}

NAMESPACE_DDDELTA_END