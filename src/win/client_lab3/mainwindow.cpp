/**
@file mainwindow.cpp
*/


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <windows.h>

/**
 * @brief A QString variable to store the port information for Arduino.
 */
QString portArduino;

/**
 * @brief A handle to the serial port for communication with Arduino.
 */
HANDLE hSerial;

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


/**
 * @brief Resets the game values to their initial state.
 * 
 * This function performs the following actions:
 * - Sets the game_started flag to "0".
 * - Clears the message string.
 * - Initializes the board array with the value "-".
 * 
 * Note: Some variables such as connect_arduino, game_mode, and ai_strategy are commented out and not reset by this function.
 */
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


/**
 * @brief Constructs an XML representation of the current game state.
 *
 * This function builds an XML string that represents the current state of the game,
 * including connection status, game status, game mode, AI strategy, messages, next turn,
 * and the game board configuration.
 *
 * @return QString The XML string representing the current game state.
 */
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



/**
 * @brief Extracts the value enclosed within a specified XML tag from a given response string.
 *
 * This function searches for the specified opening and closing tags within the response string
 * and extracts the text that lies between these tags. If the tags are not found, it returns an empty string.
 *
 * @param response The input string containing the XML-like content.
 * @param tagName The name of the tag whose value needs to be extracted.
 * @return A QString containing the text between the specified tags, or an empty QString if the tags are not found.
 */
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



/**
 * @brief Parses the provided XML input and updates the game state.
 *
 * This function extracts game-related information from the XML input string
 * and updates the corresponding member variables in the MainWindow class.
 * It looks for various tags that represent the game state and updates the
 * UI accordingly.
 *
 * @param input A QString containing the XML input with game state information.
 *
 * The expected tags in the XML input include:
 * - "con": Connection status with Arduino.
 * - "gs": Game started status.
 * - "gm": Game mode.
 * - "ais": AI strategy.
 * - "msg": Message to display.
 * - "nt": Next turn indicator.
 * - "c11" to "c33": Board cell values.
 */
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



/**
 * @brief Saves the current game state to a specified file.
 *
 * This function attempts to open the specified file for writing. If the file
 * cannot be opened, it updates the UI to indicate the failure. If the file
 * is successfully opened, it writes the game state in XML format to the file
 * and then closes the file. Finally, it updates the UI to indicate that the
 * game has been saved.
 *
 * @param filePath The path to the file where the game state should be saved.
 */
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

/**
 * @brief Loads the game state from an XML file.
 *
 * This function attempts to open the specified file in read-only mode.
 * If the file cannot be opened, it updates the UI to indicate the failure.
 * If the file is successfully opened, it reads the entire content of the file
 * into a QString, closes the file, and then parses the XML data to restore
 * the game state. Upon successful loading, it updates the UI to indicate
 * completion.
 *
 * @param filePath The path to the XML file containing the game state.
 */
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



/**
 * @brief Constructs a new MainWindow object.
 * 
 * This constructor initializes the MainWindow object, sets up the UI, loads available COM ports,
 * connects the comboBoxPorts signal to the appropriate slot, and disables the main buttons frame.
 * 
 * @param parent The parent widget, if any.
 */
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



/**
 * @brief Updates the game board UI with the current state of the board.
 * 
 * This function updates the icons of the buttons on the game board to reflect
 * the current state of the game. It iterates through each button and sets its
 * icon based on the corresponding value in the board array.
 * 
 * @note This function assumes that the board is a 3x3 array and that the UI
 *       elements (buttons) are named in a specific pattern (button_ij).
 */
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

/**
 * @brief Updates the icon of a QPushButton based on the provided value.
 *
 * This function sets the icon of the given QPushButton to either an "x" icon,
 * an "o" icon, or clears the icon based on the value provided.
 *
 * @param button A pointer to the QPushButton whose icon is to be updated.
 * @param value A QString that determines which icon to set. If the value is "x",
 *              the button will be set with an "x" icon. If the value is "o",
 *              the button will be set with an "o" icon. If the value is neither,
 *              the icon will be cleared.
 */
void MainWindow::updateButtonIcon(QPushButton *button, const QString &value){
    if (value == "x") {
        button->setIcon(QIcon(":/resources/assets/x2c.png"));  // Іконка хрестика
    } else if (value == "o") {
        button->setIcon(QIcon(":/resources/assets/o2c.png"));  // Іконка круга
    } else {
        button->setIcon(QIcon());  // Очищаємо іконку, якщо кнопка порожня
    }
}


/**
 * @brief Slot function that handles the click event of the new button.
 * 
 * This function resets the game values and starts the game based on the selected game mode.
 * If the game mode is "ava" (AI vs AI), it disables the main buttons, game modes, and AI modes frames,
 * updates the game board in a loop until the game is over, and then re-enables the frames.
 * If the game mode is not "ava", it starts the game and updates the game board once.
 * 
 * @note The function uses QCoreApplication::processEvents() to allow Qt to update the interface during the loop.
 */
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
/**
 * @brief Slot function that handles the load button click event.
 *
 * This function is triggered when the load button is clicked. It performs the following actions:
 * - Loads the game state from the "game_state.xml" file.
 * - Updates the game board based on the loaded state.
 * - Sets the appropriate radio buttons based on the game mode and AI strategy.
 *
 * The game mode can be one of the following:
 * - "mva": Man vs AI
 * - "mvm": Man vs Man
 * - "ava": AI vs AI
 *
 * The AI strategy can be one of the following:
 * - "rand": Random strategy
 * - "win": Winning strategy
 */
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
/**
 * @brief Slot function that handles the save button click event.
 *
 * This function is triggered when the save button is clicked. It calls the
 * saveGameState function to save the current game state to an XML file.
 */
void MainWindow::on_saveButton_clicked(){
    saveGameState("game_state.xml");
}


/**
 * @brief Loads available COM ports into the comboBoxPorts widget.
 *
 * This function scans for available COM ports on the system by iterating
 * through port names from COM1 to COM255. For each port, it queries the
 * system to check if the port exists. If the port exists, it is added to
 * a list of available COM ports. Finally, the list of available COM ports
 * is added to the comboBoxPorts widget in the UI.
 *
 * @note This function uses the QueryDosDeviceA function to query the system
 * for available COM ports.
 *
 * @details
 * - Clears the comboBoxPorts widget before adding new items.
 * - Uses a buffer to store the device path information.
 * - Iterates through port names from COM1 to COM255.
 * - Checks if each port exists using QueryDosDeviceA.
 * - Adds existing ports to a QStringList.
 * - Updates the comboBoxPorts widget with the list of available ports.
 */
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
/**
 * @brief Slot function that handles changes in the combo box for selecting the port.
 * 
 * This function updates the UI to reflect the attempt to connect to an Arduino device via the selected port.
 * It changes the label text and color to indicate the connection status and enables or disables the main buttons
 * based on the success of the connection. It also updates the game board after attempting to connect.
 * 
 * @param port The name of the port selected from the combo box.
 * 
 * @note This function allows Qt to update the interface by calling QCoreApplication::processEvents().
 * 
 * @see connectArduino()
 * @see updateGameBoard()
 */
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

/**
 * @brief Slot function that gets called when the refresh button is clicked.
 * 
 * This function triggers the loading of available COM ports by calling the 
 * loadComPorts() method.
 */
void MainWindow::on_refreshButton_clicked()
{
    loadComPorts();
}

/**
 * @brief Sends XML data to an Arduino device and waits for a response.
 *
 * This function builds an XML string, removes whitespace characters, and sends it to an Arduino device
 * via a serial port. It then waits for a response from the Arduino and returns the response as a QString.
 *
 * @return QString The response received from the Arduino device. Returns an empty QString if there is an error
 *         during sending or receiving data.
 */
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
/**
 * @brief Connects to an Arduino device via the specified COM port.
 *
 * This function attempts to open a COM port and configure it for communication
 * with an Arduino device. It sets the baud rate, byte size, stop bits, and parity.
 * It also configures the communication timeouts and waits for 2 seconds before
 * attempting to communicate with the Arduino.
 *
 * @param portName The name of the COM port to connect to (e.g., "COM3").
 * @return true if the connection to the Arduino was successful, false otherwise.
 */
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


/**
 * @brief Adds a player's turn to the game board.
 * 
 * This function handles the logic for adding a player's turn to the game board.
 * It checks if the game is connected to Arduino, if the game has started, and if the game mode is not "ava".
 * If these conditions are met, it updates the game board with the player's move.
 * 
 * @param row The row index of the cell where the player wants to place their mark.
 * @param col The column index of the cell where the player wants to place their mark.
 * 
 * @note The function updates the game board and processes Qt events to refresh the UI.
 * It also sends data to Arduino and parses the received XML response.
 */
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

/**
 * @brief Slot function triggered when button 11 is clicked.
 * 
 * This function is connected to the clicked signal of button 11.
 * It calls the add_player_turn function with parameters (0, 0).
 */
void MainWindow::on_button_11_clicked() { add_player_turn(0, 0); }

/**
 * @brief Slot function triggered when button 12 is clicked.
 * 
 * This function is connected to the clicked signal of button 12.
 * It calls the add_player_turn function with parameters (0, 1).
 */
void MainWindow::on_button_12_clicked() { add_player_turn(0, 1); }

/**
 * @brief Slot function triggered when button 13 is clicked.
 * 
 * This function is connected to the clicked signal of button 13.
 * It calls the add_player_turn function with parameters (0, 2).
 */
void MainWindow::on_button_13_clicked() { add_player_turn(0, 2); }

/**
 * @brief Slot function triggered when button 21 is clicked.
 * 
 * This function is connected to the clicked signal of button 21.
 * It calls the add_player_turn function with parameters (1, 0).
 */
void MainWindow::on_button_21_clicked() { add_player_turn(1, 0); }

/**
 * @brief Slot function triggered when button 22 is clicked.
 * 
 * This function is connected to the clicked signal of button 22.
 * It calls the add_player_turn function with parameters (1, 1).
 */
void MainWindow::on_button_22_clicked() { add_player_turn(1, 1); }

/**
 * @brief Slot function triggered when button 23 is clicked.
 * 
 * This function is connected to the clicked signal of button 23.
 * It calls the add_player_turn function with parameters (1, 2).
 */
void MainWindow::on_button_23_clicked() { add_player_turn(1, 2); }

/**
 * @brief Slot function triggered when button 31 is clicked.
 * 
 * This function is connected to the clicked signal of button 31.
 * It calls the add_player_turn function with parameters (2, 0).
 */
void MainWindow::on_button_31_clicked() { add_player_turn(2, 0); }

/**
 * @brief Slot function triggered when button 32 is clicked.
 * 
 * This function is connected to the clicked signal of button 32.
 * It calls the add_player_turn function with parameters (2, 1).
 */
void MainWindow::on_button_32_clicked() { add_player_turn(2, 1); }

/**
 * @brief Slot function triggered when button 33 is clicked.
 * 
 * This function is connected to the clicked signal of button 33.
 * It calls the add_player_turn function with parameters (2, 2).
 */
void MainWindow::on_button_33_clicked() { add_player_turn(2, 2); }



/**
 * @brief Slot function triggered when the "MAI" radio button is clicked.
 *
 * This function sets the game mode to "mva" (Man vs AI) when the "MAI" radio button is selected.
 */
void MainWindow::on_radioButton_mai_clicked(){
    game_mode = "mva";
    // ui->frame_ai_modes->setEnabled(true);
    // qDebug() << "game_mode:" << game_mode;
}

/**
 * @brief Slot function triggered when the "MVM" radio button is clicked.
 *
 * This function sets the game mode to "mvm" (Man vs Man) when the "MVM" radio button is selected.
 */
void MainWindow::on_radioButton_mvm_clicked(){
    game_mode = "mvm";
    // ui->frame_ai_modes->setEnabled(false);
    // qDebug() << "game_mode:" << game_mode;
}

/**
 * @brief Slot function triggered when the "AVA" radio button is clicked.
 *
 * This function sets the game mode to "ava" (AI vs AI) when the "AVA" radio button is selected.
 */
void MainWindow::on_radioButton_ava_clicked(){
    game_mode = "ava";
    // ui->frame_ai_modes->setEnabled(true);
    // qDebug() << "game_mode:" << game_mode;
}

/**
 * @brief Slot function triggered when the "RM" radio button is clicked.
 *
 * This function sets the AI strategy to "rand" (random moves) when the "RM" radio button is selected.
 */
void MainWindow::on_radioButton_rm_clicked(){
    ai_strategy = "rand";
    // qDebug() << "ai_strategy:" << ai_strategy;
}

/**
 * @brief Slot function triggered when the "WS" radio button is clicked.
 *
 * This function sets the AI strategy to "win" (best move to win) when the "WS" radio button is selected.
 */
void MainWindow::on_radioButton_ws_clicked(){
    ai_strategy = "win";
    // qDebug() << "ai_strategy:" << ai_strategy;
}





