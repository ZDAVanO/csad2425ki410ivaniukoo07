﻿#include <windows.h>
#include <iostream>
#include <string>

#include "xml_utils.h"

using namespace std;

int main() {

    int portNumber;
    cout << "Enter COM port number (e.g., 6 for COM6): ";
    cin >> portNumber;

    string portName_s = "COM" + to_string(portNumber);
    const char* portName = portName_s.c_str();

    /*const char* portName = "COM6";*/

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
        system("pause");
        return 1;
    }

    // Налаштування параметрів порту
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        cerr << "Error getting port state" << endl;
        system("pause");
        return 1;
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        cerr << "Error setting port parameters" << endl;
        system("pause");
        return 1;
    }

    // Налаштування таймаутів
    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        cerr << "Error setting timeouts!" << endl;
        system("pause");
        return 1;
    }

    Sleep(2000);

    // Цикл для надсилання повідомлень і читання відповідей
    while (true) {
        string message = "hello from pc";
        string messageXML = createXML(message);
        DWORD bytesWritten;

        // Надсилання повідомлення до Arduino
        if (!WriteFile(hSerial, messageXML.c_str(), messageXML.size(), &bytesWritten, NULL)) {
            cerr << "Error writing to port" << endl;
            break;
        }
        cout << "Sent: " << message << " (" << messageXML << ")" << endl;

        // Читання відповіді з Arduino
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

        Sleep(1000);
    }

    // Закриття COM-порту
    CloseHandle(hSerial);
    return 0;
}
