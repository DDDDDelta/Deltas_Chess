#include "board.h"
#include "code_utils.h"

NAMESPACE_DDDELTA_START
Board::Board() :
    _up_board(std::make_unique<ChessBoard>()),_up_movement_map(std::make_unique<MovementMap>()), _black_lost_val(0), _white_lost_val(0) {
    this->set_up();
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

    for (std::uint8_t i : std::views::iota(1, 9)) {
        this->get_piece(i, 2 ) = WhitePawn;
        this->get_piece(i, 7) = BlackPawn;
    }

    for (std::uint8_t i : stdvw::iota(1,9))
        for (std::uint8_t j : stdvw::iota(3, 7))
            this->get_piece(i, j) = Empty;
}

void Board::reset() {
    this->set_up();
    this->_white_lost_val = 0;
    this->_black_lost_val = 0;
    this->_captured_white.clear();
    this->_captured_white.clear();
}

PossibleMovement Board::_pawn_move(BoardCoor co) const {
    PossibleMovement
    if (this->get_piece(co).color == E_Color::White) {
        if (co + { 0, 1 })
    }

    return std::move(ret);
}

NAMESPACE_DDDELTA_END