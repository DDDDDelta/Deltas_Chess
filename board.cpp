#include "board.h"
#include "code_utils.inc"

NAMESPACE_DDDELTA_START
template <E_PieceType Type>
static inline constexpr std::array<std::optional<Piece>, 8> board_file {
    Piece(E_Color::White, Type), constant::WhitePawn, nullopt, nullopt,
    nullopt, nullopt, constant::BlackPawn, Piece(E_Color::Black, Type)
};


static inline constexpr Board::ChessBoard_t starting_position {
    board_file<E_PieceType::Rook>,
    board_file<E_PieceType::Knight>,
    board_file<E_PieceType::Bishop>,
    board_file<E_PieceType::Queen>,
    board_file<E_PieceType::King>,
    board_file<E_PieceType::Bishop>,
    board_file<E_PieceType::Knight>,
    board_file<E_PieceType::Rook>
};


Board::Board() :
    _board(starting_position), _can_castle_long(true, true), _can_castle_short(true, true),
    _in_check(constant::INVALID_COOR), _last_double_pawn_move(constant::INVALID_COOR) {}


static inline std::array<BoardCoor, 8> all_king_movement(BoardCoor co) {
    return {
        co + Vec2(1, 0),  co + Vec2(1, 1),  co + Vec2(1, -1),
        co + Vec2(0, 1),                    co + Vec2(0, -1),
        co + Vec2(-1, 0), co + Vec2(-1, 1), co + Vec2(-1, -1)
    };
}


static inline std::array<BoardCoor, 8> all_knight_movement(BoardCoor co) {
    return {
        co + Vec2(-1, 2),  co + Vec2(1, 2),
        co + Vec2(-2, 1),  co + Vec2(2, 1),
        co + Vec2(-2, -1), co + Vec2(2, -1),
        co + Vec2(-1, -2), co + Vec2(1, -2)
    };
}


static inline constexpr auto adap_to_piecemove = stdvw::transform([](BoardCoor coor) -> PieceMove {
    return { coor, E_UniqueAction::None };
});


static inline constexpr auto adap_remove_off_board = stdvw::filter([](BoardCoor coor) {
    return coor.on_board();
});


static auto adap_remove_empty(const Board* const self) {
    return stdvw::filter([self](BoardCoor coor) {
        return static_cast<bool>(self->get_piece(coor));
    });
}


static auto adap_remove_pieces(const Board* const self) {
    return stdvw::filter([self](BoardCoor coor) {
        return !self->get_piece(coor);
    });
}


static auto adap_remove_color_of(const Board* const self, E_Color color) {
    return stdvw::filter([self, color](BoardCoor coor) {
        return !(self->get_piece(coor) && self->get_piece(coor)->color == color);
    });
}


// TODO: finish this
static auto adap_remove_attacked(Board* self, E_Color color) {
    return stdvw::filter([color](BoardCoor co) {

        return true;
    });
}


PossibleMovement* Board::_knight_move(BoardCoor co) const {
    assert(this->get_piece(co) != nullopt);

    auto all_move = all_knight_movement(co);
    auto valid_move = all_move | adap_remove_off_board;

    auto move_range = valid_move
        | adap_remove_pieces(this)
        | adap_to_piecemove;
    auto capture_range = valid_move
        | adap_remove_empty(this)
        | adap_remove_color_of(this, this->get_piece(co)->color)
        | adap_to_piecemove;
    auto protect_range = valid_move
        | adap_remove_empty(this)
        | adap_remove_color_of(this, !this->get_piece(co)->color)
        | adap_to_piecemove;

    return new PossibleMovement {
        { move_range.begin(), move_range.end() },
        { capture_range.begin(), capture_range.end() },
        { protect_range.begin(), protect_range.end() }
    };
}


PossibleMovement* Board::_diagonal_move(PossibleMovement* p_movement, BoardCoor co) const {
    assert(this->get_piece(co) != nullopt);

    E_Color curr_color = this->get_piece(co)->color;

    auto insert_move = [this, p_movement, curr_color](BoardCoor co) mutable {
        if (!co.on_board()) {
            return false;
        } else if (this->get_piece(co)) {
            p_movement->moves.emplace_back(co, E_UniqueAction::None);
            return true;
        } else if (this->get_piece(co)->color == curr_color) {
            p_movement->protects.emplace_back(co, E_UniqueAction::None);
            return false;
        } else {
            p_movement->captures.emplace_back(co, E_UniqueAction::None);
            return false;
        }
    };

    BoardCoor
    curr = co;    while (insert_move(curr += Vec2(1,1)));
    curr = co;    while (insert_move(curr += Vec2(1,-1)));
    curr = co;    while (insert_move(curr += Vec2(-1,-1)));
    curr = co;    while (insert_move(curr += Vec2(-1,1)));

    return p_movement;
}


PossibleMovement* Board::_linear_move(PossibleMovement* p_movement, BoardCoor co) const {
    assert(this->get_piece(co) != nullopt);

    E_Color curr_color = this->get_piece(co)->color;

    auto insert_move = [this, p_movement, curr_color](BoardCoor co) mutable {
        if (!co.on_board()) {
            return false;
        } else if (this->get_piece(co)) {
            p_movement->moves.emplace_back(co, E_UniqueAction::None);
            return true;
        } else if (this->get_piece(co)->color == curr_color) {
            p_movement->protects.emplace_back(co, E_UniqueAction::None);
            return false;
        } else {
            p_movement->captures.emplace_back(co, E_UniqueAction::None);
            return false;
        }
    };

    BoardCoor
    curr = co;     while (insert_move(curr += Vec2(1, 0)));
    curr = co;     while (insert_move(curr += Vec2(-1,0)));
    curr = co;     while (insert_move(curr += Vec2(0, 1)));
    curr = co;     while (insert_move(curr += Vec2(0,-1)));

    return p_movement;
}


PossibleMovement* Board::_bishop_move(BoardCoor co) const {
    assert(this->get_piece(co) != nullopt);

    auto p_ret = new PossibleMovement;
    return this->_diagonal_move(p_ret, co);
}


PossibleMovement* Board::_rook_move(BoardCoor co) const {
    assert(this->get_piece(co) != nullopt);

    auto p_ret = new PossibleMovement;
    return this->_linear_move(p_ret, co);
}


PossibleMovement* Board::_queen_move(BoardCoor co) const {
    assert(this->get_piece(co) != nullopt);

    auto p_ret = new PossibleMovement;
    this->_linear_move(p_ret, co);
    this->_diagonal_move(p_ret, co);
    return p_ret;
}


PossibleMovement* Board::_pawn_move(BoardCoor co) const {
    assert(this->get_piece(co) != nullopt);
    assert(co.y != 1 && co.y != 8);

    E_Color selected_color = this->get_piece(co)->color;
    i32 direction = to_underlying(selected_color) ? 1 : -1;
    i32 promotion_rank = to_underlying(selected_color) ? 8 : 1;
    i32 initial_rank = to_underlying(selected_color) ? 2 : 7;

    std::array<BoardCoor, 2> diagonal {
        BoardCoor(co.x + 1, co.y + direction),
        BoardCoor(co.x - 1, co.y + direction)
    };

    auto p_ret = new PossibleMovement;

    for (BoardCoor coor : diagonal | adap_remove_off_board) {
        if (!this->get_piece(coor)) {
            if (this->_last_double_pawn_move.x == coor.x)
                p_ret->captures.emplace_back(coor, E_UniqueAction::EnPassant);

            continue;
        }
        // get rid of case of empty / En Passant

        if (this->get_piece(coor)->color != selected_color)
            if (coor.y == promotion_rank) // capturing and promoting
                p_ret->captures.emplace_back(coor, E_UniqueAction::Promote);
            else // capturing and !promoting
                p_ret->captures.emplace_back(coor, E_UniqueAction::None);
        else // protecting
            p_ret->protects.emplace_back(coor, E_UniqueAction::None);
    }

    BoardCoor single_push = co + Vec2(0, direction);
    if (single_push.on_board() && !this->get_piece(single_push))
        p_ret->moves.emplace_back(co, E_UniqueAction::None);
    else
        return p_ret;

    BoardCoor double_push = co + Vec2(0, 2 * direction);
    if (co.y == initial_rank &&
        !this->get_piece(double_push)) // on initial rank and no blocking
        p_ret->moves.emplace_back(co, E_UniqueAction::DoublePawnPush);

    return p_ret;
}


// TODO: finish this
PossibleMovement* Board::_king_move(BoardCoor co) const {
    assert(this->get_piece(co) != nullopt);

    auto all_move = all_king_movement(co);
    auto valid_move = all_move | adap_remove_off_board;

    auto move_range = valid_move
        | adap_remove_pieces(this)
        | adap_to_piecemove;
    auto capture_range = valid_move
        | adap_remove_empty(this)
        | adap_remove_color_of(this, this->get_piece(co)->color)
        | adap_to_piecemove;
    auto protect_range = valid_move
        | adap_remove_empty(this)
        | adap_remove_color_of(this, !this->get_piece(co)->color)
        | adap_to_piecemove;

    return new PossibleMovement {
        { move_range.begin(), move_range.end() },
        { capture_range.begin(), capture_range.end() },
        { protect_range.begin(), protect_range.end() }
    };
}


PossibleMovement* Board::get_move(BoardCoor coor) const {
    assert_on_board_coor(coor);

    if (!this->get_piece(coor))
        return nullptr;

    E_PieceType selected_type = this->get_piece(coor)->type;
    switch (selected_type) {
        case E_PieceType::Pawn:
            return this->_pawn_move(coor);
        case E_PieceType::Knight:
            return this->_knight_move(coor);
        case E_PieceType::Bishop:
            return this->_bishop_move(coor);
        case E_PieceType::Rook:
            return this->_rook_move(coor);
        case E_PieceType::Queen:
            return this->_queen_move(coor);
        case E_PieceType::King:
            return this->_king_move(coor);
        default:
            UNREACHABLE();
    }

    UNREACHABLE();
}


PossibleMovement* Board::get_move(i32 x, i32 y) const {
    assert_on_board_xy(x, y);

    return this->get_move({ x, y });
}
NAMESPACE_DDDELTA_END