#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <windows.h>

HANDLE hSerial;
QString portArduino;

QString connect_arduino, game_started, game_mode, ai_strategy, message, next_turn;
QString board[3][3];


void resetValues() {
    // connect_arduino = "0";
    game_started = "0";
    // game_mode = "mva";
    // ai_strategy = "rand";
    message = "";

    // Заповнюємо масив board значенням "-"
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            board[i][j] = "-";
        }
    }
}


QString buildXML() {
    QString output = "<g>\n";
    output += "<con>" + connect_arduino + "</con>\n";
    output += "<gs>" + game_started + "</gs>\n";
    output += "<gm>" + game_mode + "</gm>\n";
    output += "<ais>" + ai_strategy + "</ais>\n";
    output += "<msg>" + message + "</msg>\n";
    output += "<nt>" + next_turn + "</nt>\n";
    output += "<brd>\n";
    output += "<c11>" + board[0][0] + "</c11>\n";
    output += "<c12>" + board[0][1] + "</c12>\n";
    output += "<c13>" + board[0][2] + "</c13>\n";
    output += "<c21>" + board[1][0] + "</c21>\n";
    output += "<c22>" + board[1][1] + "</c22>\n";
    output += "<c23>" + board[1][2] + "</c23>\n";
    output += "<c31>" + board[2][0] + "</c31>\n";
    output += "<c32>" + board[2][1] + "</c32>\n";
    output += "<c33>" + board[2][2] + "</c33>\n";
    output += "</brd>\n";
    output += "</g>\n";
    return output;
}



QString getTagValue(const QString& response, const QString& tagName) {
    // Формуємо строки для відкриваючого та закриваючого тегів
    QString openTag = "<" + tagName + ">";
    QString closeTag = "</" + tagName + ">";

    // Знаходимо позиції відкриваючого та закриваючого тегів
    int startIndex = response.indexOf(openTag);
    int endIndex = response.indexOf(closeTag, startIndex);

    // Якщо тег не знайдено, повертаємо пустий рядок
    if (startIndex == -1 || endIndex == -1) {
        return QString();
    }

    // Вираховуємо позицію, з якої починається текст між тегами
    startIndex += openTag.length();

    // Витягуємо текст між тегами
    return response.mid(startIndex, endIndex - startIndex);
}



void MainWindow::parseXML(QString input)
{
    connect_arduino = getTagValue(input, "con");
    game_started = getTagValue(input, "gs");
    game_mode = getTagValue(input, "gm");
    ai_strategy = getTagValue(input, "ais");

    message = getTagValue(input, "msg");
    ui->label_arduino_msg->setText(QString(message));

    next_turn  = getTagValue(input, "nt");

    board[0][0] = getTagValue(input, "c11");
    board[0][1] = getTagValue(input, "c12");
    board[0][2] = getTagValue(input, "c13");
    board[1][0] = getTagValue(input, "c21");
    board[1][1] = getTagValue(input, "c22");
    board[1][2] = getTagValue(input, "c23");
    board[2][0] = getTagValue(input, "c31");
    board[2][1] = getTagValue(input, "c32");
    board[2][2] = getTagValue(input, "c33");
}



void MainWindow::saveGameState(const QString& filePath) {
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        ui->label_pc_msg->setText("Can't open file for write");
        return;
    }

    QTextStream out(&file);
    QString xmlData = buildXML();  // Викликаємо функцію, яка формує XML
    out << xmlData;

    file.close();

    ui->label_pc_msg->setText("Game Saved");
}

void MainWindow::loadGameState(const QString& filePath) {
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->label_pc_msg->setText("Can't open file for read");
        return;
    }

    QTextStream in(&file);
    QString xmlData = in.readAll();  // Читаємо весь файл у строку
    file.close();

    parseXML(xmlData);  // Парсимо XML та відновлюємо стан гри
    ui->label_pc_msg->setText("Load Complete");
}



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadComPorts();

    connect(ui->comboBoxPorts,
            SIGNAL(currentTextChanged(const QString &)),
            this,
            SLOT(onComboBoxPortChanged(const QString &)));

    ui->frame_main_buttons->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::updateGameBoard(){
    updateButtonIcon(ui->button_11, board[0][0]);
    updateButtonIcon(ui->button_12, board[0][1]);
    updateButtonIcon(ui->button_13, board[0][2]);
    updateButtonIcon(ui->button_21, board[1][0]);
    updateButtonIcon(ui->button_22, board[1][1]);
    updateButtonIcon(ui->button_23, board[1][2]);
    updateButtonIcon(ui->button_31, board[2][0]);
    updateButtonIcon(ui->button_32, board[2][1]);
    updateButtonIcon(ui->button_33, board[2][2]);
}

void MainWindow::updateButtonIcon(QPushButton *button, const QString &value){
    if (value == "x") {
        button->setIcon(QIcon(":/resources/assets/x2c.png"));  // Іконка хрестика
    } else if (value == "o") {
        button->setIcon(QIcon(":/resources/assets/o2c.png"));  // Іконка круга
    } else {
        button->setIcon(QIcon());  // Очищаємо іконку, якщо кнопка порожня
    }
}


void MainWindow::on_newButton_clicked()
{
    resetValues();
    game_started = "1";

    if (game_mode == "ava")
    {
        ui->frame_main_buttons->setEnabled(false);
        ui->frame_game_modes->setEnabled(false);
        ui->frame_ai_modes->setEnabled(false);

        ui->label_pc_msg->setText("AI vs AI mode started");

        while (game_started == "1")
        {
            // QString receivedXml = sendArduino();
            // parseXML(receivedXml);
            parseXML(sendArduino());
            updateGameBoard();

            QCoreApplication::processEvents(); // Дозволяємо Qt оновити інтерфейс

            //Sleep(150);
        }

        ui->frame_main_buttons->setEnabled(true);
        ui->frame_game_modes->setEnabled(true);
        ui->frame_ai_modes->setEnabled(true);

        ui->label_pc_msg->setText("Game is over");
    }
    else
    {
        // QString receivedXml = sendArduino();
        // parseXML(receivedXml);
        parseXML(sendArduino());

        if (game_started == "1"){
            ui->label_pc_msg->setText("Game started");
        }

        updateGameBoard();
    }
}
void MainWindow::on_loadButton_clicked()
{
    loadGameState("game_state.xml");
    updateGameBoard();

    if (game_mode == "mva") ui->radioButton_mai->setChecked(true);
    if (game_mode == "mvm") ui->radioButton_mvm->setChecked(true);
    if (game_mode == "ava") ui->radioButton_ava->setChecked(true);

    if (ai_strategy == "rand") ui->radioButton_rm->setChecked(true);
    if (ai_strategy == "win") ui->radioButton_ws->setChecked(true);

}
void MainWindow::on_saveButton_clicked(){
    saveGameState("game_state.xml");
}


void MainWindow::loadComPorts()
{
    ui->comboBoxPorts->clear();

    // Список для збереження імен портів
    QStringList comPortList;

    // Буфер для збереження імен пристроїв
    char lpTargetPath[5000]; // Буфер для збереження шляху до пристрою
    DWORD result;             // Результат виклику функції

    for (int i = 1; i <= 255; ++i) {
        // Формуємо ім'я порту: COM1, COM2, ..., COM255
        QString portName = QString("COM%1").arg(i);

        // Отримуємо інформацію про порт
        result = QueryDosDeviceA(portName.toStdString().c_str(), lpTargetPath, 5000);

        // Якщо порт існує, додаємо його до списку
        if (result != 0) {
            comPortList.append(portName);
        }
    }

    // Додаємо отримані порти в comboBox
    ui->comboBoxPorts->addItems(comPortList);
}

// Слот, який викликається при зміні вибраного порту
void MainWindow::onComboBoxPortChanged(const QString &port)
{
    ui->labelPort->setText(QString("Trying to connect via %1").arg(port));
    ui->labelPort->setStyleSheet("QLabel { color : lightblue; }");
    QCoreApplication::processEvents(); // Дозволяємо Qt оновити інтерфейс

    if (connectArduino(port)) {
        // Якщо відповідь від Arduino коректна
        ui->labelPort->setText("Arduino is connected");
        ui->labelPort->setStyleSheet("QLabel { color : lightgreen; }");

        // Активуємо кнопки
        ui->frame_main_buttons->setEnabled(true);

        portArduino = port;
    }
    else {
        if (port.length() > 0) {
            // Якщо помилка при зв'язку з Arduino
            ui->labelPort->setText(QString("Failed to connect via %1").arg(port));
            ui->labelPort->setStyleSheet("QLabel { color : red; }");
        }
        else {
            ui->labelPort->setText(QString("Select COM Port with Arduino"));
            ui->labelPort->setStyleSheet("QLabel { color : lightblue; }");
        }

        // Деактивуємо кнопки
        ui->frame_main_buttons->setEnabled(false);
    }

    updateGameBoard();

}

void MainWindow::on_refreshButton_clicked()
{
    loadComPorts();
}

QString MainWindow::sendArduino() {

    QString xmlData = buildXML();

    QString xmlData_trim = xmlData.remove(QChar(' '));
    xmlData_trim.remove(QChar('\n'));
    xmlData_trim.remove(QChar('\t'));

    std::string xmlString = xmlData_trim.toStdString();
    const char *dataToSend = xmlString.c_str();

    // qDebug() << "Sended  :" << dataToSend;

    DWORD bytesWritten;
    if (!WriteFile(hSerial, dataToSend, strlen(dataToSend), &bytesWritten, nullptr)) {
        CloseHandle(hSerial);
        return "";  // Помилка при відправці даних
    }

    // Чекаємо на відповідь
    char incomingData[256] = {0};
    DWORD bytesRead;
    if (!ReadFile(hSerial, incomingData, sizeof(incomingData), &bytesRead, nullptr)) {
        CloseHandle(hSerial);
        return "";  // Помилка при отриманні даних
    }

    // Закриваємо порт
    // CloseHandle(hSerial);

    // Перевіряємо, чи отримали правильну відповідь
    QString response(incomingData);
    // qDebug() << "Response:" << response;

    return response;
}


// Функція для тестування зв'язку з Arduino
bool MainWindow::connectArduino(const QString &portName)
{
    CloseHandle(hSerial);

    // Відкриваємо COM-порт
    hSerial = CreateFileA(portName.toStdString().c_str(),
                          GENERIC_READ | GENERIC_WRITE,
                          0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (hSerial == INVALID_HANDLE_VALUE) {
        return false;  // Не вдалося відкрити порт
    }

    // Налаштування порту
    DCB dcbSerialParams = {};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        CloseHandle(hSerial);
        return false;  // Помилка отримання стану порту
    }

    dcbSerialParams.BaudRate = CBR_9600;  // Налаштування швидкості
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        CloseHandle(hSerial);
        return false;  // Помилка налаштування порту
    }

    // Налаштування тайм-аутів
    COMMTIMEOUTS timeouts = {};
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        CloseHandle(hSerial);
        return false;  // Помилка налаштування тайм-аутів
    }

    Sleep(2000);

    resetValues();
    connect_arduino = "0";
    QString receivedXml = sendArduino();

    // QString conValue = getTagValue(receivedXml, "con");
    // qDebug() << "conValue:" << conValue;

    parseXML(receivedXml);

    return connect_arduino.trimmed() == "1";  // Порівнюємо з відповіддю Arduino
}


void MainWindow::add_player_turn(int row, int col)
{
    // connect_arduino, game_started, game_mode, ai_strategy, message, next_turn;
    if (connect_arduino != "1" || game_started != "1" || game_mode == "ava") {
        ui->label_pc_msg->setText("Game not started");
        return;
    }

    if (board[row][col] != "x" && board[row][col] != "o")
    {
        if (next_turn == "x") { board[row][col] = "x"; }
        else { board[row][col] = "o"; }

        ui->label_pc_msg->setText("");
    }
    else {
        ui->label_pc_msg->setText("Select empty cell");
        return;
    }

    updateGameBoard();
    QCoreApplication::processEvents(); // Дозволяємо Qt оновити інтерфейс

    QString receivedXml = sendArduino();
    parseXML(receivedXml);

    updateGameBoard();
}

void MainWindow::on_button_11_clicked() { add_player_turn(0, 0); }
void MainWindow::on_button_12_clicked() { add_player_turn(0, 1); }
void MainWindow::on_button_13_clicked() { add_player_turn(0, 2); }
void MainWindow::on_button_21_clicked() { add_player_turn(1, 0); }
void MainWindow::on_button_22_clicked() { add_player_turn(1, 1); }
void MainWindow::on_button_23_clicked() { add_player_turn(1, 2); }
void MainWindow::on_button_31_clicked() { add_player_turn(2, 0); }
void MainWindow::on_button_32_clicked() { add_player_turn(2, 1); }
void MainWindow::on_button_33_clicked() { add_player_turn(2, 2); }



void MainWindow::on_radioButton_mai_clicked(){
    game_mode = "mva";
    // ui->frame_ai_modes->setEnabled(true);
    // qDebug() << "game_mode:" << game_mode;
}
void MainWindow::on_radioButton_mvm_clicked(){
    game_mode = "mvm";
    // ui->frame_ai_modes->setEnabled(false);
    // qDebug() << "game_mode:" << game_mode;
}
void MainWindow::on_radioButton_ava_clicked(){
    game_mode = "ava";
    // ui->frame_ai_modes->setEnabled(true);
    // qDebug() << "game_mode:" << game_mode;
}

void MainWindow::on_radioButton_rm_clicked(){
    ai_strategy = "rand";
    // qDebug() << "ai_strategy:" << ai_strategy;
}
void MainWindow::on_radioButton_ws_clicked(){
    ai_strategy = "win";
    // qDebug() << "ai_strategy:" << ai_strategy;
}





