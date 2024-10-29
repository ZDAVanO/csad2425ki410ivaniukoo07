/**
@file mainwindow.h
*/

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

    /**
 * @brief Variables used in the main window of the client application.
 *
 * @var connect_arduino Indicates the connection status with the Arduino device.
 * @var game_started Indicates whether the game has started.
 * @var game_mode Represents the current game mode.
 * @var ai_strategy Represents the AI strategy being used.
 * @var message Stores messages to be displayed or processed.
 * @var next_turn Indicates whose turn is next in the game.
 */
    QString connect_arduino, game_started, game_mode, ai_strategy, message, next_turn;
    QString board[3][3];

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void loadComPorts();
    bool connectArduino(const QString &port); // Функція для тестування Arduino
    QString sendArduino();

    void updateGameBoard();
    void updateButtonIcon(QPushButton *button, const QString &value);

    void parseXML(QString input);

    void add_player_turn(int row, int col);
    void add_player_turn_cb(QString board[3][3], int row, int col);

    void saveGameState(const QString& filePath);
    void loadGameState(const QString& filePath);
    // QString getTagValue(const QString& response, const QString& tagName);

    QString getTagValue(const QString& response, const QString& tagName);
    QString buildXML();
    void resetValues();

private slots:
    void on_newButton_clicked();
    void on_loadButton_clicked();
    void on_saveButton_clicked();

    void onComboBoxPortChanged(const QString &port);

    void on_button_11_clicked();
    void on_button_12_clicked();
    void on_button_13_clicked();
    void on_button_21_clicked();
    void on_button_22_clicked();
    void on_button_23_clicked();
    void on_button_31_clicked();
    void on_button_32_clicked();
    void on_button_33_clicked();

    void on_radioButton_mai_clicked();
    void on_radioButton_mvm_clicked();
    void on_radioButton_ava_clicked();

    void on_radioButton_rm_clicked();
    void on_radioButton_ws_clicked();

    void on_refreshButton_clicked();

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
