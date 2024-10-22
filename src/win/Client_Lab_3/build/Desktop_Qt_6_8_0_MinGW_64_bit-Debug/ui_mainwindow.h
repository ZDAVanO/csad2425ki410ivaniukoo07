/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QFrame *frame_main_buttons;
    QPushButton *newButton;
    QPushButton *loadButton;
    QPushButton *saveButton;
    QFrame *frame_port;
    QComboBox *comboBoxPorts;
    QLabel *labelPort;
    QPushButton *refreshButton;
    QFrame *frame_stats;
    QLabel *label_pc_msg;
    QLabel *label_info_icon;
    QFrame *frame_game_modes;
    QRadioButton *radioButton_mai;
    QRadioButton *radioButton_mvm;
    QRadioButton *radioButton_ava;
    QFrame *frame_board;
    QLabel *labelBoard;
    QPushButton *button_22;
    QPushButton *button_12;
    QPushButton *button_32;
    QPushButton *button_11;
    QPushButton *button_21;
    QPushButton *button_31;
    QPushButton *button_13;
    QPushButton *button_23;
    QPushButton *button_33;
    QFrame *frame_ai_modes;
    QRadioButton *radioButton_ws;
    QRadioButton *radioButton_rm;
    QLabel *label;
    QFrame *frame_2;
    QLabel *label_arduino_msg;
    QLabel *label_arduino_icon;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(772, 530);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        frame_main_buttons = new QFrame(centralwidget);
        frame_main_buttons->setObjectName("frame_main_buttons");
        frame_main_buttons->setEnabled(true);
        frame_main_buttons->setGeometry(QRect(10, 70, 331, 51));
        frame_main_buttons->setFrameShape(QFrame::Shape::StyledPanel);
        frame_main_buttons->setFrameShadow(QFrame::Shadow::Raised);
        newButton = new QPushButton(frame_main_buttons);
        newButton->setObjectName("newButton");
        newButton->setEnabled(true);
        newButton->setGeometry(QRect(10, 10, 121, 31));
        loadButton = new QPushButton(frame_main_buttons);
        loadButton->setObjectName("loadButton");
        loadButton->setEnabled(true);
        loadButton->setGeometry(QRect(140, 10, 86, 31));
        saveButton = new QPushButton(frame_main_buttons);
        saveButton->setObjectName("saveButton");
        saveButton->setEnabled(true);
        saveButton->setGeometry(QRect(235, 10, 86, 31));
        frame_port = new QFrame(centralwidget);
        frame_port->setObjectName("frame_port");
        frame_port->setEnabled(true);
        frame_port->setGeometry(QRect(10, 10, 331, 51));
        frame_port->setFrameShape(QFrame::Shape::StyledPanel);
        frame_port->setFrameShadow(QFrame::Shadow::Raised);
        comboBoxPorts = new QComboBox(frame_port);
        comboBoxPorts->setObjectName("comboBoxPorts");
        comboBoxPorts->setGeometry(QRect(10, 10, 91, 31));
        comboBoxPorts->setModelColumn(0);
        labelPort = new QLabel(frame_port);
        labelPort->setObjectName("labelPort");
        labelPort->setGeometry(QRect(140, 10, 181, 31));
        labelPort->setAutoFillBackground(false);
        labelPort->setAlignment(Qt::AlignmentFlag::AlignCenter);
        refreshButton = new QPushButton(frame_port);
        refreshButton->setObjectName("refreshButton");
        refreshButton->setGeometry(QRect(100, 10, 31, 31));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resources/assets/refresh.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        refreshButton->setIcon(icon);
        refreshButton->setIconSize(QSize(25, 25));
        frame_stats = new QFrame(centralwidget);
        frame_stats->setObjectName("frame_stats");
        frame_stats->setGeometry(QRect(350, 10, 201, 51));
        frame_stats->setFrameShape(QFrame::Shape::StyledPanel);
        frame_stats->setFrameShadow(QFrame::Shadow::Raised);
        label_pc_msg = new QLabel(frame_stats);
        label_pc_msg->setObjectName("label_pc_msg");
        label_pc_msg->setGeometry(QRect(10, 10, 141, 31));
        label_info_icon = new QLabel(frame_stats);
        label_info_icon->setObjectName("label_info_icon");
        label_info_icon->setGeometry(QRect(160, 10, 31, 31));
        label_info_icon->setPixmap(QPixmap(QString::fromUtf8(":/resources/assets/info.png")));
        label_info_icon->setScaledContents(true);
        frame_game_modes = new QFrame(centralwidget);
        frame_game_modes->setObjectName("frame_game_modes");
        frame_game_modes->setEnabled(true);
        frame_game_modes->setGeometry(QRect(10, 130, 161, 101));
        frame_game_modes->setFrameShape(QFrame::Shape::StyledPanel);
        frame_game_modes->setFrameShadow(QFrame::Shadow::Raised);
        radioButton_mai = new QRadioButton(frame_game_modes);
        radioButton_mai->setObjectName("radioButton_mai");
        radioButton_mai->setGeometry(QRect(10, 10, 141, 22));
        radioButton_mai->setAutoFillBackground(true);
        radioButton_mvm = new QRadioButton(frame_game_modes);
        radioButton_mvm->setObjectName("radioButton_mvm");
        radioButton_mvm->setGeometry(QRect(10, 40, 141, 22));
        radioButton_mvm->setAutoFillBackground(true);
        radioButton_ava = new QRadioButton(frame_game_modes);
        radioButton_ava->setObjectName("radioButton_ava");
        radioButton_ava->setGeometry(QRect(10, 70, 141, 22));
        radioButton_ava->setAutoFillBackground(true);
        frame_board = new QFrame(centralwidget);
        frame_board->setObjectName("frame_board");
        frame_board->setGeometry(QRect(350, 70, 411, 411));
        frame_board->setFrameShape(QFrame::Shape::StyledPanel);
        frame_board->setFrameShadow(QFrame::Shadow::Raised);
        labelBoard = new QLabel(frame_board);
        labelBoard->setObjectName("labelBoard");
        labelBoard->setGeometry(QRect(10, 10, 394, 394));
        labelBoard->setAutoFillBackground(false);
        labelBoard->setPixmap(QPixmap(QString::fromUtf8(":/resources/assets/board_c.png")));
        labelBoard->setScaledContents(true);
        button_22 = new QPushButton(frame_board);
        button_22->setObjectName("button_22");
        button_22->setGeometry(QRect(150, 150, 111, 114));
        button_22->setIconSize(QSize(100, 100));
        button_12 = new QPushButton(frame_board);
        button_12->setObjectName("button_12");
        button_12->setGeometry(QRect(150, 16, 111, 114));
        button_12->setIconSize(QSize(100, 100));
        button_32 = new QPushButton(frame_board);
        button_32->setObjectName("button_32");
        button_32->setGeometry(QRect(150, 284, 111, 114));
        button_32->setIconSize(QSize(100, 100));
        button_11 = new QPushButton(frame_board);
        button_11->setObjectName("button_11");
        button_11->setGeometry(QRect(17, 16, 111, 114));
        button_11->setIconSize(QSize(100, 100));
        button_21 = new QPushButton(frame_board);
        button_21->setObjectName("button_21");
        button_21->setGeometry(QRect(17, 150, 111, 114));
        button_21->setIconSize(QSize(100, 100));
        button_31 = new QPushButton(frame_board);
        button_31->setObjectName("button_31");
        button_31->setGeometry(QRect(17, 284, 111, 114));
        button_31->setIconSize(QSize(100, 100));
        button_13 = new QPushButton(frame_board);
        button_13->setObjectName("button_13");
        button_13->setGeometry(QRect(283, 16, 111, 114));
        button_13->setIconSize(QSize(100, 100));
        button_23 = new QPushButton(frame_board);
        button_23->setObjectName("button_23");
        button_23->setGeometry(QRect(283, 150, 111, 114));
        button_23->setIconSize(QSize(100, 100));
        button_33 = new QPushButton(frame_board);
        button_33->setObjectName("button_33");
        button_33->setGeometry(QRect(283, 284, 111, 114));
        button_33->setIconSize(QSize(100, 100));
        frame_ai_modes = new QFrame(centralwidget);
        frame_ai_modes->setObjectName("frame_ai_modes");
        frame_ai_modes->setEnabled(true);
        frame_ai_modes->setGeometry(QRect(180, 130, 161, 101));
        frame_ai_modes->setFrameShape(QFrame::Shape::StyledPanel);
        frame_ai_modes->setFrameShadow(QFrame::Shadow::Raised);
        radioButton_ws = new QRadioButton(frame_ai_modes);
        radioButton_ws->setObjectName("radioButton_ws");
        radioButton_ws->setGeometry(QRect(10, 70, 141, 22));
        radioButton_ws->setAutoFillBackground(true);
        radioButton_rm = new QRadioButton(frame_ai_modes);
        radioButton_rm->setObjectName("radioButton_rm");
        radioButton_rm->setGeometry(QRect(10, 40, 141, 22));
        radioButton_rm->setAutoFillBackground(true);
        label = new QLabel(frame_ai_modes);
        label->setObjectName("label");
        label->setGeometry(QRect(10, 10, 141, 21));
        label->setTextFormat(Qt::TextFormat::AutoText);
        label->setScaledContents(false);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label->setWordWrap(false);
        frame_2 = new QFrame(centralwidget);
        frame_2->setObjectName("frame_2");
        frame_2->setGeometry(QRect(560, 10, 201, 51));
        frame_2->setAutoFillBackground(false);
        frame_2->setFrameShape(QFrame::Shape::StyledPanel);
        frame_2->setFrameShadow(QFrame::Shadow::Raised);
        label_arduino_msg = new QLabel(frame_2);
        label_arduino_msg->setObjectName("label_arduino_msg");
        label_arduino_msg->setGeometry(QRect(10, 10, 141, 31));
        label_arduino_icon = new QLabel(frame_2);
        label_arduino_icon->setObjectName("label_arduino_icon");
        label_arduino_icon->setGeometry(QRect(160, 10, 31, 31));
        label_arduino_icon->setPixmap(QPixmap(QString::fromUtf8(":/resources/assets/arduino.png")));
        label_arduino_icon->setScaledContents(true);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 772, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        newButton->setText(QCoreApplication::translate("MainWindow", "New Game", nullptr));
        loadButton->setText(QCoreApplication::translate("MainWindow", "Load", nullptr));
        saveButton->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
        comboBoxPorts->setPlaceholderText(QCoreApplication::translate("MainWindow", "Select Port", nullptr));
        labelPort->setText(QCoreApplication::translate("MainWindow", "Select COM Port with Arduino", nullptr));
        refreshButton->setText(QString());
        label_pc_msg->setText(QCoreApplication::translate("MainWindow", "Info here", nullptr));
        label_info_icon->setText(QString());
        radioButton_mai->setText(QCoreApplication::translate("MainWindow", "Man vs AI", nullptr));
        radioButton_mvm->setText(QCoreApplication::translate("MainWindow", "Man vs Man", nullptr));
        radioButton_ava->setText(QCoreApplication::translate("MainWindow", "AI vs AI", nullptr));
        labelBoard->setText(QString());
        button_22->setText(QString());
        button_12->setText(QString());
        button_32->setText(QString());
        button_11->setText(QString());
        button_21->setText(QString());
        button_31->setText(QString());
        button_13->setText(QString());
        button_23->setText(QString());
        button_33->setText(QString());
        radioButton_ws->setText(QCoreApplication::translate("MainWindow", "Win strategy", nullptr));
        radioButton_rm->setText(QCoreApplication::translate("MainWindow", "Random move", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "AI Mode", nullptr));
        label_arduino_msg->setText(QCoreApplication::translate("MainWindow", "Arduino messages here", nullptr));
        label_arduino_icon->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
