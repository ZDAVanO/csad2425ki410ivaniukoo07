#include <QApplication>
#include <gtest/gtest.h>
#include "mainwindow.h"

class MainWindowTest : public ::testing::Test {
protected:
    MainWindow *mainWindow; // Pointer to MainWindow instance
    QApplication *app;      // Pointer to QApplication instance

    void SetUp() override {
        // Create a QApplication instance
        int argc = 0;
        char *argv[] = { nullptr };
        app = new QApplication(argc, argv);

        // Create an instance of MainWindow
        mainWindow = new MainWindow();
    }

    void TearDown() override {
        // Clean up
        delete mainWindow;
        delete app;  // Delete QApplication
    }
};

// Тест для getTagValue
TEST_F(MainWindowTest, GetTagValue_ValidTag) {
    QString xml = "<g><tag>Hello</tag></g>";
    QString value = mainWindow->getTagValue(xml, "tag");
    EXPECT_EQ(value, "Hello");
}
TEST_F(MainWindowTest, GetTagValue_ValidTag_2) {
    QString xmlInput = "<g><con>1</con><gs>1</gs><gm>mva</gm><ais>rand</ais><msg>Hello</msg><nt>x</nt></g>";
    QString expected = "1";
    QString actual = mainWindow->getTagValue(xmlInput, "con");
    EXPECT_EQ(actual, expected);
}
TEST_F(MainWindowTest, GetTagValue_MissingTag) {
    QString xml = "<g><tag>Hello</tag></g>";
    QString value = mainWindow->getTagValue(xml, "missing");
    EXPECT_EQ(value, "");
}
TEST_F(MainWindowTest, GetTagValue_InvalidTag) {
    QString xmlInput = "<g><con>1</con><gs>1</gs><gm>mva</gm><ais>rand</ais><msg>Hello</msg><nt>x</nt></g>";
    QString actual = mainWindow->getTagValue(xmlInput, "invalid");
    EXPECT_EQ(actual, "");  // Expecting an empty string
}



// Тест для buildXML
TEST_F(MainWindowTest, BuildXML_ProducesCorrectOutput) {
    mainWindow->game_started = "1";
    mainWindow->connect_arduino = "1";
    mainWindow->game_mode = "mva";
    mainWindow->ai_strategy = "rand";
    mainWindow->message = "Game in progress";
    mainWindow->next_turn = "x";
    mainWindow->board[0][0] = "x";
    mainWindow->board[1][1] = "o";
    mainWindow->board[2][2] = "-";

    QString expectedXml = "<g>\n<con>1</con>\n<gs>1</gs>\n<gm>mva</gm>\n<ais>rand</ais>\n<msg>Game in progress</msg>\n<nt>x</nt>\n<brd>\n<c11>x</c11>\n<c12></c12>\n<c13></c13>\n<c21></c21>\n<c22>o</c22>\n<c23></c23>\n<c31></c31>\n<c32></c32>\n<c33>-</c33>\n</brd>\n</g>\n";
    QString actualXml = mainWindow->buildXML();
    // qDebug() << "Expected XML: " << expectedXml;
    // qDebug() << "Actual XML: " << actualXml;
    EXPECT_EQ(actualXml.trimmed(), expectedXml.trimmed());
}

// Тест для parseXML
TEST_F(MainWindowTest, ParseXML_CorrectlyUpdatesValues) {
    QString xml = "<g>\n<con>0</con>\n<gs>1</gs>\n<gm>ava</gm>\n<ais>win</ais>\n<msg>test_msg</msg>\n<nt>o</nt>\n<brd>\n<c11>o</c11>\n<c12></c12>\n<c13></c13>\n<c21></c21>\n<c22>o</c22>\n<c23></c23>\n<c31></c31>\n<c32></c32>\n<c33>-</c33>\n</brd>\n</g>\n";

    mainWindow->parseXML(xml);

    EXPECT_EQ(mainWindow->connect_arduino, "0");
    EXPECT_EQ(mainWindow->game_started, "1");
    EXPECT_EQ(mainWindow->game_mode, "ava");
    EXPECT_EQ(mainWindow->ai_strategy, "win");
    EXPECT_EQ(mainWindow->message, "test_msg");
    EXPECT_EQ(mainWindow->next_turn, "o");
    EXPECT_EQ(mainWindow->board[0][0], "o");
    EXPECT_EQ(mainWindow->board[1][1], "o");
}

// Тест для resetValues
TEST_F(MainWindowTest, ResetValues_SetsToInitialState) {
    mainWindow->game_started = "1";
    mainWindow->message = "msg here";
    mainWindow->resetValues();

    EXPECT_EQ(mainWindow->game_started, "0");
    EXPECT_EQ(mainWindow->message, "");
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            EXPECT_EQ(mainWindow->board[i][j], "-");
        }
    }
}





// // Тест для add_player_turn
// TEST_F(MainWindowTest, AddPlayerTurn_UpdatesBoardCorrectly) {
//     mainWindow->connect_arduino = "1";
//     mainWindow->game_started = "1";
//     mainWindow->game_mode = "mva";


//     mainWindow->board[0][0] = "";


//     qDebug() << "board[0][0]: " << mainWindow->board[0][0];
//     mainWindow->next_turn = "x";
//     mainWindow->add_player_turn(0, 0);  // Додаємо хрестик

//     qDebug() << "board[0][0]: " << mainWindow->board[0][0];

//     mainWindow->board[0][0] = "x";
//     EXPECT_EQ(mainWindow->board[0][0], QString("x"));

// }

// TEST_F(MainWindowTest, AddPlayerTurn_RejectsInvalidMove) {
//     mainWindow->connect_arduino = "1";
//     mainWindow->game_started = "1";
//     mainWindow->next_turn = "x";
//     mainWindow->game_mode = "mva";
//     mainWindow->add_player_turn(0, 0);  // Додаємо хрестик
//     EXPECT_EQ(mainWindow->board[0][0], "x");

//     // Спроба поставити хрестик на вже зайняту клітинку
//     mainWindow->add_player_turn(0, 0);
//     EXPECT_EQ(mainWindow->board[0][0], "x");  // Клітинка має залишатися без змін
// }

