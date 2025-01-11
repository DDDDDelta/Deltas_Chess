#include "mainwindow.h"

#include "ui_mainwindow.h"
#include "BoardDisplay.h"

#include <QGraphicsView>

MainWindow::MainWindow(DDDelta::ChessGame* game, QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), game(game),
    board(new Display::ChessBoard(this, game)), boardView(new QGraphicsView(board)) {
    ui->setupUi(this);
    boardView->setRenderHint(QPainter::Antialiasing);
    boardView->setAlignment(Qt::AlignCenter);

    setCentralWidget(boardView);
}

MainWindow::~MainWindow()
{
    delete ui;
}
