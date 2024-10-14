#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QDomDocument>
#include <QPushButton>
#include <QIcon>



QString xmlData = R"(<game>
                            <connect>0</connect>
                            <gstarted>0</gstarted>
                            <gmode>ManVsAI</gmode>
                            <aistrat>Random</aistrat>
                            <msg></msg>
                            <board>
                                <c11>-</c11>
                                <c12>-</c12>
                                <c13>-</c13>
                                <c21>-</c21>
                                <c22>-</c22>
                                <c23>-</c23>
                                <c31>-</c31>
                                <c32>-</c32>
                                <c33>-</c33>
                            </board>
                          </game>)";



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Завантаження зображення
    // QPixmap boardPixmap(":/resources/assets/board_c.png");
    // ui->labelBoard->setPixmap(boardPixmap);
    // ui->labelBoard->setScaledContents(true);

    loadComPorts();

    connect(ui->comboBoxPorts, SIGNAL(currentTextChanged(const QString &)),
            this, SLOT(onComboBoxPortChanged(const QString &)));







    updateGameBoard(xmlData);


    ui->radioButton_mai->setChecked(true);
    ui->radioButton_ws->setChecked(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::updateGameBoard(const QString &xmlData)
{
    QDomDocument doc;
    if (!doc.setContent(xmlData)) {
        qDebug() << "Error parsing XML";
        return;
    }

    // Отримуємо кореневий елемент
    QDomElement root = doc.documentElement();
    QDomElement gameBoard = root.firstChildElement("board");

    // QDomNodeList c13List = root.elementsByTagName("c13");
    // if (!c13List.isEmpty()) {
    //     c13List.at(0).firstChild().setNodeValue("o"); // Change the value to "1"
    // }

    // Оновлюємо кнопки на основі значень з XML
    updateButtonIcon(ui->button_11, gameBoard.firstChildElement("c11").text());
    updateButtonIcon(ui->button_12, gameBoard.firstChildElement("c12").text());
    updateButtonIcon(ui->button_13, gameBoard.firstChildElement("c13").text());
    updateButtonIcon(ui->button_21, gameBoard.firstChildElement("c21").text());
    updateButtonIcon(ui->button_22, gameBoard.firstChildElement("c22").text());
    updateButtonIcon(ui->button_23, gameBoard.firstChildElement("c23").text());
    updateButtonIcon(ui->button_31, gameBoard.firstChildElement("c31").text());
    updateButtonIcon(ui->button_32, gameBoard.firstChildElement("c32").text());
    updateButtonIcon(ui->button_33, gameBoard.firstChildElement("c33").text());
}

void MainWindow::updateButtonIcon(QPushButton *button, const QString &value)
{
    if (value == "x") {
        button->setIcon(QIcon(":/resources/assets/x2c.png"));  // Іконка хрестика
    } else if (value == "o") {
        button->setIcon(QIcon(":/resources/assets/o2c.png"));  // Іконка круга
    } else {
        button->setIcon(QIcon());  // Очищаємо іконку, якщо кнопка порожня
    }
    // button->setEnabled(false);  // Робимо кнопку недоступною для натискання
}















void MainWindow::on_newButton_clicked()
{
    ui->labelPort->setText("test text");
}

void MainWindow::loadComPorts()
{
    // Список для збереження імен портів
    QStringList comPortList;

    // Буфер для збереження імен пристроїв
    char lpTargetPath[5000]; // Буфер для збереження шляху до пристрою
    DWORD result;             // Результат виклику функції

    // Додаємо елемент "None" першим у ComboBox
    //comPortList.append("None");

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

    // Якщо вибрано "None", не робимо нічого
    // if (port == "None") {
    //     ui->labelPort->setText("Please select a COM port");
    //     ui->labelPort->setStyleSheet("QLabel { color : white; }");
    //     return;
    // }


    ui->labelPort->setText(QString("Trying to connect via %1").arg(port));
    ui->labelPort->setStyleSheet("QLabel { color : yellow; }");
    QCoreApplication::processEvents(); // Дозволяємо Qt оновити інтерфейс

    if (testArduino(port)) {
        // Якщо відповідь від Arduino коректна
        ui->labelPort->setText("Arduino is connected");
        ui->labelPort->setStyleSheet("QLabel { color : green; }");

        // Активуємо кнопки
        ui->newButton->setEnabled(true);
        ui->loadButton->setEnabled(true);
        ui->saveButton->setEnabled(true);


    } else {
        // Якщо помилка при зв'язку з Arduino
        ui->labelPort->setText(QString("Failed to connect via %1").arg(port));
        ui->labelPort->setStyleSheet("QLabel { color : red; }");

        // Деактивуємо кнопки
        ui->newButton->setEnabled(false);
        ui->loadButton->setEnabled(false);
        ui->saveButton->setEnabled(false);
    }
}


// Функція для тестування зв'язку з Arduino
bool MainWindow::testArduino(const QString &portName)
{
    // Відкриваємо COM-порт
    HANDLE hSerial;
    hSerial = CreateFileA(portName.toStdString().c_str(),
                          GENERIC_READ | GENERIC_WRITE,
                          0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (hSerial == INVALID_HANDLE_VALUE) {
        return false;  // Не вдалося відкрити порт
    }

    // Налаштування порту
    DCB dcbSerialParams = {0};
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
    COMMTIMEOUTS timeouts = {0};
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

    // Відправляємо повідомлення Arduino

    QString xmlData_trim = xmlData.remove(QChar(' '));
    xmlData_trim.remove(QChar('\n'));
    xmlData_trim.remove(QChar('\t'));

    std::string xmlString = xmlData_trim.toStdString();
    const char *dataToSend = xmlString.c_str();
    qDebug() << "Sended:" << dataToSend;
    //const char *dataToSend = "0";
    DWORD bytesWritten;
    if (!WriteFile(hSerial, dataToSend, strlen(dataToSend), &bytesWritten, nullptr)) {
        CloseHandle(hSerial);
        return false;  // Помилка при відправці даних
    }

    // Чекаємо на відповідь
    char incomingData[256] = {0};
    DWORD bytesRead;
    if (!ReadFile(hSerial, incomingData, sizeof(incomingData), &bytesRead, nullptr)) {
        CloseHandle(hSerial);
        return false;  // Помилка при отриманні даних
    }

    // Закриваємо порт
    CloseHandle(hSerial);

    // Перевіряємо, чи отримали правильну відповідь
    QString response(incomingData);
    qDebug() << "Response:" << response;
    return response.trimmed() == "1";  // Порівнюємо з відповіддю Arduino
}

