#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <windows.h>
#include <QComboBox>
#include <QStringList>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_newButton_clicked();

    void onComboBoxPortChanged(const QString &port);

private:
    Ui::MainWindow *ui;

    void loadComPorts();
    bool testArduino(const QString &port); // Функція для тестування Arduino

    void updateGameBoard(const QString &xmlData);
    void updateButtonIcon(QPushButton *button, const QString &value);
};
#endif // MAINWINDOW_H
