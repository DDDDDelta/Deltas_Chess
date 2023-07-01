#include "board.h"
#include "code_utils.h"

NAMESPACE_DDDELTA_START
Board::Board() :
    _board{}, _black_lost_val(0), _white_lost_val(0) {
    Board::initialize();
}

Piece& Board::get_piece(std::uint8_t x, std::uint8_t y) {
    return STDARRAY_GET(STDARRAY_GET(this->_board, x - 1), y - 1);
}

void Board::set_up() {
    this->get_piece(1, 1) = white_rook;
    this->get_piece(2, 1) = white_knight;
    this->get_piece(3, 1) = white_bishop;
    this->get_piece(4, 1) = white_queen;
    this->get_piece(5, 1) = white_king;
    this->get_piece(6, 1) = white_bishop;
    this->get_piece(7, 1) = white_knight;
    this->get_piece(8, 1) = white_rook;
    this->get_piece(1, 8) = black_rook;
    this->get_piece(2, 8) = black_knight;
    this->get_piece(3, 8) = black_bishop;
    this->get_piece(4, 8) = black_queen;
    this->get_piece(5, 8) = black_king;
    this->get_piece(6, 8) = black_bishop;
    this->get_piece(7, 8) = black_knight;
    this->get_piece(8, 8) = black_rook;

    for (int i = 1; i <= 8; i++) {
        this->get_piece(i, 2) = white_pawn;
        this->get_piece(i, 7) = black_pawn;
    }

    for (int i = 1; i <= 8; i++)
        for (int j = 3; j <= 6; j++)
            this->get_piece(i, j) = empty;
}

void Board::initialize() {
    this->set_up();
    this->_white_lost_val = 0;
    this->_black_lost_val = 0;
    this->_captured_white.clear();
    this->_captured_white.clear();
}

NAMESPACE_DDDELTA_END