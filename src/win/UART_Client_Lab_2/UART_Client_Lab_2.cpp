#include <windows.h>
#include <iostream>
#include <string>

using namespace std;


string createXML(const string& message) {
    return "<message>" + message + "</message>";
}

string parseXML(const string& xml) {
    size_t start = xml.find("<message>") + 9; // 9 - це довжина тегу <message>
    size_t end = xml.find("</message>");

    if (start != string::npos && end != string::npos) {
        return xml.substr(start, end - start);
    }

    return ""; // Якщо XML некоректний
}



int main() {
    // Запит номера COM-порту у користувача
    //int portNumber;
    //cout << "Enter COM port number (e.g., 6 for COM6): ";
    //cin >> portNumber;

    //// Формування назви порту
    //string portName = "COM" + to_string(portNumber);

    const char* portName = "COM6";

    // Відкриття COM-порту
    HANDLE hSerial = CreateFileA(portName, // portName.c_str()
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hSerial == INVALID_HANDLE_VALUE) {
        cerr << "Error opening port" << endl;
        return 1;
    }

    // Налаштування параметрів порту
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        cerr << "Error getting port state" << endl;
        return 1;
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        cerr << "Error setting port parameters" << endl;
        return 1;
    }

    // Налаштування тайм-аутів
    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        cerr << "Error setting timeouts!" << endl;
        return 1;
    }

    Sleep(2000);

    // Цикл для безкінечної відправки повідомлень
    while (true) {
        string message = "hello from pc";  // Повідомлення для відправки
        string messageXML = createXML(message);
        DWORD bytesWritten;

        

        // Відправка повідомлення до Arduino
        if (!WriteFile(hSerial, messageXML.c_str(), messageXML.size(), &bytesWritten, NULL)) {
            cerr << "Error writing to port" << endl;
            break;
        }
        cout << "Sent: " << message << " (" << messageXML << ")" << endl;

        // Читання відповіді від Arduino
        char buffer[128];
        DWORD bytesRead;

        if (ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
            buffer[bytesRead] = '\0';
            if (bytesRead > 0) {
                cout << "Received from Arduino: " << parseXML(buffer) << " (" << buffer << ")" << endl << endl;
            }
        }
        else {
            cerr << "Error reading from port" << endl;
            break;
        }

        // Затримка перед наступним запитом
        Sleep(1000);
    }

    // Закриття COM-порту
    CloseHandle(hSerial);
    return 0;
}
