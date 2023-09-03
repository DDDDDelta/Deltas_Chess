#pragma once
#define DELTAS_CHESS_CHESS_GAME_INTERFACE_H

#include <memory>
#include <optional>

#include "board.h"
#include "game_status.h"
#include "code_utils.inc"

NAMESPACE_DDDELTA_START
namespace throwable {
class special_case_base;
}

class I_ChessGame {
public:
    virtual const Board& board() const = 0;
    virtual const GameStatus& status() const = 0;
    virtual std::shared_ptr<const PossibleMovement> select_piece(BoardCoor co) = 0;
    virtual std::optional<E_UniqueAction> execute_move(BoardCoor target_coor) = 0;

protected:
    virtual E_Color _next_round() = 0;

protected:
    friend class throwable::special_case_base;
};
NAMESPACE_DDDELTA_END