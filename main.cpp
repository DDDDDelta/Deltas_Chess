#include "mainwindow.h"
#include "chess_game.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DDDelta::ChessGame game;

    MainWindow w(&game);
    w.setWindowTitle("DDDelta's Chess");
    w.show();

    return a.exec();
}
