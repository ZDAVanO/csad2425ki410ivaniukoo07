/**
@file main.cpp
*/

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("TicTacToe");
    QIcon icon(":/icons/ttt_icon.ico");
    w.setWindowIcon(icon);
    w.show();
    return a.exec();
}
