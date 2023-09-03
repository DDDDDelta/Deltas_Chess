#include "move_factory.h"
#include "code_utils.inc"

NAMESPACE_DDDELTA_START
StandardMoveFactory::StandardMoveFactory(const Board& bd, const GameStatus& status) :
    _board(bd), _status(status),
    _can_castle_long(true, true), _can_castle_short(true, true), _king_pos({ 5, 1 }, { 5, 8 }),
    _last_double_pawn_move(constant::INVALID_COOR) {}


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


static auto adap_remove_empty(const Board& board) {
    return stdvw::filter([board](BoardCoor coor) {
        return static_cast<bool>(board.get(coor));
    });
}


static auto adap_remove_pieces(const Board& board) {
    return stdvw::filter([board](BoardCoor coor) {
        return !board.get(coor);
    });
}


static auto adap_remove_color_of(const Board& board, E_Color color) {
    return stdvw::filter([board, color](BoardCoor coor) {
        return !(board.get(coor) && board.get(coor)->color == color);
    });
}


PossibleMovement* StandardMoveFactory::_knight_move(BoardCoor co) const {
    auto all_move = all_knight_movement(co);
    auto valid_move = all_move | adap_remove_off_board;

    auto move_range = valid_move
        | adap_remove_pieces(this->_board)
        | adap_to_piecemove;
    auto capture_range = valid_move
        | adap_remove_empty(this->_board)
        | adap_remove_color_of(this->_board, this->_board.get(co)->color)
        | adap_to_piecemove;
    auto protect_range = valid_move
        | adap_remove_empty(this->_board)
        | adap_remove_color_of(this->_board, !this->_board.get(co)->color)
        | adap_to_piecemove;

    return new PossibleMovement {
        { move_range.begin(), move_range.end() },
        { capture_range.begin(), capture_range.end() },
        { protect_range.begin(), protect_range.end() }
    };
}


PossibleMovement* StandardMoveFactory::_diagonal_move(PossibleMovement* p_movement, BoardCoor co) const {
    E_Color selected_color = this->_board.get(co)->color;

    auto insert_move = [this, p_movement, selected_color](BoardCoor co) mutable {
        if (!co.on_board()) {
            return false;
        } else if (this->_board.is_empty(co)) {
            p_movement->moves.emplace_back(co, E_UniqueAction::None);
            return true;
        } else if (this->_board.get(co)->color == selected_color) {
            p_movement->protects.emplace_back(co, E_UniqueAction::None);
            return false;
        } else {
            p_movement->captures.emplace_back(co, E_UniqueAction::None);
            return false;
        }
    };

    BoardCoor
        curr = co;    while (insert_move(curr += Vec2(1, 1)));
    curr = co;    while (insert_move(curr += Vec2(1, -1)));
    curr = co;    while (insert_move(curr += Vec2(-1, -1)));
    curr = co;    while (insert_move(curr += Vec2(-1, 1)));

    return p_movement;
}


PossibleMovement* StandardMoveFactory::_linear_move(PossibleMovement* p_movement, BoardCoor co) const {
    E_Color selected = this->_board.get(co)->color;

    auto insert_move = [this, p_movement, selected](BoardCoor co) mutable {
        if (!co.on_board()) {
            return false;
        } else if (this->_board.is_empty(co)) {
            p_movement->moves.emplace_back(co, E_UniqueAction::None);
            return true;
        } else if (this->_board.get(co)->color == selected) {
            p_movement->protects.emplace_back(co, E_UniqueAction::None);
            return false;
        } else {
            p_movement->captures.emplace_back(co, E_UniqueAction::None);
            return false;
        }
    };

    BoardCoor
    curr = co;     while (insert_move(curr += Vec2(1, 0)));
    curr = co;     while (insert_move(curr += Vec2(-1, 0)));
    curr = co;     while (insert_move(curr += Vec2(0, 1)));
    curr = co;     while (insert_move(curr += Vec2(0, -1)));

    return p_movement;
}


PossibleMovement* StandardMoveFactory::_bishop_move(BoardCoor co) const {
    auto p_ret = new PossibleMovement;
    return this->_diagonal_move(p_ret, co);
}


PossibleMovement* StandardMoveFactory::_rook_move(BoardCoor co) const {
    auto p_ret = new PossibleMovement;
    return this->_linear_move(p_ret, co);
}


PossibleMovement* StandardMoveFactory::_queen_move(BoardCoor co) const {
    auto p_ret = new PossibleMovement;
    this->_linear_move(p_ret, co);
    this->_diagonal_move(p_ret, co);
    return p_ret;
}


PossibleMovement* StandardMoveFactory::_pawn_move(BoardCoor co) const {
    assert(co.y != 1 && co.y != 8);

    E_Color selected_color = this->_board.get(co)->color;
    i32 direction = to_underlying(selected_color) ? 1 : -1;
    i32 promotion_rank = nth_from_last_rank<8>[selected_color];
    i32 initial_rank = nth_from_last_rank<2>[selected_color];
    i32 en_passant_rank = nth_from_last_rank<5>[selected_color];

    std::array<BoardCoor, 2> diagonal {
        BoardCoor(co.x + 1, co.y + direction),
        BoardCoor(co.x - 1, co.y + direction)
    };

    auto p_ret = new PossibleMovement;

    for (BoardCoor coor : diagonal | adap_remove_off_board) {
        if (this->_board.is_empty(coor)) {
            LOG_TO_STDOUT("last double pawn move:");
            LOG_TO_STDOUT(this->_last_double_pawn_move.x);
            if (co.y == en_passant_rank && this->_last_double_pawn_move.x == coor.x)
                p_ret->captures.emplace_back(coor, E_UniqueAction::EnPassant);

            continue;
        }
        // get rid of case of empty / En Passant

        if (this->_board.get(coor)->color != selected_color)
            if (coor.y == promotion_rank) // capturing and promoting
                p_ret->captures.emplace_back(coor, E_UniqueAction::Promote);
            else // capturing and !promoting
                p_ret->captures.emplace_back(coor, E_UniqueAction::None);
        else // protecting
            p_ret->protects.emplace_back(coor, E_UniqueAction::None);
    }

    BoardCoor single_push = co + Vec2(0, direction);
    if (single_push.on_board() && this->_board.is_empty(single_push))
        p_ret->moves.emplace_back(single_push, single_push.y == promotion_rank ?
                                               E_UniqueAction::Promote :
                                               E_UniqueAction::None);
    else
        return p_ret;

    BoardCoor double_push = co + Vec2(0, 2 * direction);
    if (co.y == initial_rank &&
        this->_board.is_empty(double_push)) // on initial rank and no blocking
        p_ret->moves.emplace_back(double_push, E_UniqueAction::DoublePawnPush);

    return p_ret;
}


// TODO: finish this
PossibleMovement* StandardMoveFactory::_king_move(BoardCoor co) const {
    E_Color selected_color = this->get_piece(co)->color;
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

    auto ret = new PossibleMovement {
        { move_range.begin(),    move_range.end()    },
        { capture_range.begin(), capture_range.end() },
        { protect_range.begin(), protect_range.end() }
    };

    BoardCoor short_castle_target = co + Vec2(2, 0);
    if (this->_can_castle_short[selected_color] &&
        this->is_empty_sqr(short_castle_target) &&
        this->is_empty_sqr(co + Vec2(1, 0)))
        ret->moves.emplace_back(short_castle_target, E_UniqueAction::ShortCastle);

    BoardCoor long_castle_target = co - Vec2(2, 0);
    if (this->_can_castle_long[selected_color] &&
        this->is_empty_sqr(co - Vec2(3, 0)) &&
        this->is_empty_sqr(short_castle_target) &&
        this->is_empty_sqr(co - Vec2(1, 0)))
        ret->moves.emplace_back(long_castle_target, E_UniqueAction::LongCastle);

    return ret;
}


bool StandardMoveFactory::is_checkmated() const {
    if (this->_attackers.attacker_count() != 0)
        return std::unique_ptr<PossibleMovement>
            (this->_king_move(this->_king_pos[this->_attacker()->color]))->unmoveable();

    return false;
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
NAMESPACE_DDDELTA_END