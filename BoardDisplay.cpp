#include "BoardDisplay.h"

#include "chess_game.h"

#include <QBrush>
#include <QHash>

namespace Display {

static const int squareSize = 50;

QPixmap makePixmap(const char* path) {
    return QPixmap((QString)":board/imgs/" + path + ".png").scaled(
        50, 50,
        Qt::KeepAspectRatio, Qt::SmoothTransformation
    );
};

DDDelta::Vec2 rcToScreen(int row, int col) {
    return { row , 9 - col };
}

ChessSquare::ChessSquare(int row, int col) :
    QGraphicsRectItem(nullptr), row(row), col(col) {
    auto [x, y] = rcToScreen(row, col);

    setPen(Qt::NoPen);
    setRect((x - 1) * squareSize, (y - 1) * squareSize, squareSize, squareSize);
}

void ChessSquare::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsRectItem::mousePressEvent(event);

    emit pressed(row, col);
}

ChessBoard::ChessBoard(QObject* parent, DDDelta::ChessGame* b) :
    QGraphicsScene(parent), game(b), pieceMap {
    { DDDelta::constant::WhitePawn, makePixmap("wP") },
    { DDDelta::constant::WhiteKnight, makePixmap("wN") },
    { DDDelta::constant::WhiteBishop, makePixmap("wB") },
    { DDDelta::constant::WhiteRook, makePixmap("wR") },
    { DDDelta::constant::WhiteQueen, makePixmap("wQ") },
    { DDDelta::constant::WhiteKing, makePixmap("wK") },

    { DDDelta::constant::BlackPawn, makePixmap("bP") },
    { DDDelta::constant::BlackKnight, makePixmap("bN") },
    { DDDelta::constant::BlackBishop, makePixmap("bB") },
    { DDDelta::constant::BlackRook, makePixmap("bR") },
    { DDDelta::constant::BlackQueen, makePixmap("bQ") },
    { DDDelta::constant::BlackKing, makePixmap("bK") },
} {
    setupBoard();

    setSceneRect(0, 0, 8 * squareSize, 8 * squareSize);
}

void ChessBoard::setupBoard() {
    for (int row = 1; row <= 8; row++) {
        for (int col = 1; col <= 8; col++) {
            auto* square = new ChessSquare(row, col);
            QObject::connect(
                square, &ChessSquare::pressed,
                this, &ChessBoard::onSquarePressed
            );
            square->setZValue(3);
            addItem(square);

            auto [x, y] = rcToScreen(row, col);

            auto* piece = new QGraphicsPixmapItem(QPixmap());
            piece->setPos((x - 1) * squareSize, (y - 1) * squareSize);
            piece->setZValue(1);
            addItem(piece);
            pieceDisplay[row - 1][col - 1] = piece;

            auto* bgSquare = new QGraphicsRectItem();
            bgSquare->setRect((x - 1) * squareSize, (y - 1) * squareSize, squareSize, squareSize);
            bgSquare->setPen(Qt::NoPen);
            bgSquare->setBrush(QBrush(QColor((row + col) % 2 ? Qt::white : Qt::darkGreen)));
            bgSquare->setZValue(0);
            addItem(bgSquare);
        }
    }

    updatePieces();
}

void ChessBoard::updatePieces() {
    for (int row = 1; row <= 8; row++) {
        for (int col = 1; col <= 8; col++) {
            auto optPiece = (*game)[{ row, col }];

            if (optPiece) {
                pieceDisplay[row - 1][col - 1]->setPixmap(
                    pieceMap[*optPiece]
                );
            }
            else {
                pieceDisplay[row - 1][col - 1]->setPixmap(QPixmap());
            }
        }
    }
}

void ChessBoard::onSquarePressed(int row, int col) {
    auto optSel = game->get_selection();

    if (optSel) {
        std::optional<DDDelta::E_UniqueAction> optAction;
        try {
            optAction = game->execute_move({ row, col });
        }
        catch (DDDelta::throwable::pawn_promote& promote) {
            qDebug() << "promotion" << Qt::endl;
        }
        catch (DDDelta::throwable::game_end& end) {
            qDebug() << "ended" << Qt::endl;
        }

        updatePieces();
    }
    else {
        game->select_piece({ row, col });
    }
}

}
