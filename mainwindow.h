#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace Display {
class ChessBoard;
}

namespace DDDelta {
class ChessGame;
}

class QGraphicsView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(DDDelta::ChessGame*, QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    DDDelta::ChessGame* game;
    Display::ChessBoard* board;
    QGraphicsView* boardView;
};

#endif // MAINWINDOW_H
