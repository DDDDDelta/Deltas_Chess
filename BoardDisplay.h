#ifndef BOARDDISPLAY_H
#define BOARDDISPLAY_H

#include "pieces.h"

#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

#include <array>

namespace DDDelta {
class ChessGame;
}

namespace Display {

class ChessSquare : public QObject, public QGraphicsRectItem {

    Q_OBJECT

public:
    ChessSquare(int row, int col);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void pressed(int row, int col);

private:
    int row, col;
};

class ChessBoard : public QGraphicsScene {

    Q_OBJECT

public:
    ChessBoard(QObject* parent, DDDelta::ChessGame*);

private:
    void setupBoard();
    void updatePieces();

private slots:
    void onSquarePressed(int row, int col);

private:
    DDDelta::ChessGame* game;

    const QHash<DDDelta::Piece, QPixmap> pieceMap;
    std::array<std::array<QGraphicsPixmapItem*, 8>, 8> pieceDisplay;
};

}

#endif // BOARDDISPLAY_H
