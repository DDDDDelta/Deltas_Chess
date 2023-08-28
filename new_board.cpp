#include "new_board.h"
#include "code_utils.inc"

NAMESPACE_DDDELTA_START
template <E_PieceType Type>
static inline constexpr std::array<std::optional<Piece>, 8> init_board_file {
    Piece(E_Color::White, Type), constant::WhitePawn, nullopt, nullopt,
    nullopt, nullopt, constant::BlackPawn, Piece(E_Color::Black, Type)
};


static inline constexpr Board::ChessBoard_t starting_position {
    init_board_file<E_PieceType::Rook>,
    init_board_file<E_PieceType::Knight>,
    init_board_file<E_PieceType::Bishop>,
    init_board_file<E_PieceType::Queen>,
    init_board_file<E_PieceType::King>,
    init_board_file<E_PieceType::Bishop>,
    init_board_file<E_PieceType::Knight>,
    init_board_file<E_PieceType::Rook>
};


Board::Board() :
    _board(starting_position) {}
NAMESPACE_DDDELTA_END
