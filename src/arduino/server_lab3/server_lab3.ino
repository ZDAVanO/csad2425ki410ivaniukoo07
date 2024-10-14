



String receivedMessage = ""; // Змінна для зберігання отриманих даних
String connectionStatus = "0";
String gameStarted = "0";
String gameMode = "ManVsAI";
String aiStrategy = "Random";
String message = "You win!";
String gameBoard[3][3];




String testData = "<game>"
                     "<connect>0</connect>"
                     "<gstarted>0</gstarted>"
                     "<gmode>ManVsAI</gmode>"
                     "<aistrat>Random</aistrat>"
                     "<msg>You win!</msg>"
                     "<board>"
                     "<c11>x</c11>"
                     "<c12>-</c12>"
                     "<c13>-</c13>"
                     "<c21>o</c21>"
                     "<c22>-</c22>"
                     "<c23>-</c23>"
                     "<c31>-</c31>"
                     "<c32>-</c32>"
                     "<c33>o</c33>"
                     "</board>"
                     "</game>";



void setup() {
  pinMode(13, OUTPUT); // Ініціалізація піну для лампочки
  digitalWrite(13, HIGH);

  Serial.begin(9600);  // Налаштування серійного зв'язку зі швидкістю 9600 біт/с
  // while (!Serial);     // Очікуємо поки порт стане доступним (тільки для Leonardo і схожих плат)
  // delay(2000);    
  digitalWrite(13, LOW);

  gameBoard[0][0] = '-';
  gameBoard[0][1] = '-';
  gameBoard[0][2] = '-';
  gameBoard[1][0] = '-';
  gameBoard[1][1] = '-';
  gameBoard[1][2] = '-';
  gameBoard[2][0] = '-';
  gameBoard[2][1] = '-';
  gameBoard[2][2] = '-';

  // parseXML(testData);
  // Serial.print(generateXML());
}




void loop() {
  if (Serial.available() > 0) {




    String receivedMessage = Serial.readString();  // Читаємо дані з серійного порту
    digitalWrite(13, HIGH);

    // String parsedMessage = parseXML(receivedMessage);
    
    // // Перетворюємо всі літери в верхній регістр
    // for (int i = 0; i < parsedMessage.length(); i++) {
    //   parsedMessage[i] = toupper(parsedMessage[i]);
    // }

    // String xmlToSend = createXML(parsedMessage);
    parseXML(receivedMessage);

    digitalWrite(13, LOW);

    connectionStatus = "1";

    String myString = generateXML();
    Serial.println(myString);



    // String incomingMessage = Serial.readString();  // Читання повідомлення
    // if (incomingMessage == "0") {
    //   Serial.println("1");  // Відповідь, якщо отримали правильне повідомлення
    // } else {
    //   Serial.println("ERROR");  // Інша відповідь
    // }


  }
}




void parseXML(String xml) {
    // Парсинг XML для отримання даних
    connectionStatus = getValue(xml, "connect");
    gameStarted = getValue(xml, "gstarted");
    gameMode = getValue(xml, "gmode");
    aiStrategy = getValue(xml, "aistrat");
    message = getValue(xml, "msg");
    
    // Парсинг ігрової дошки
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            String cell = "c" + String(row + 1) + String(col + 1);
            gameBoard[row][col] = getValue(xml, cell);
        }
    }

    // Для демонстрації виведемо отримані дані
    // Serial.println("ConnectionStatus: " + connectionStatus);
    // Serial.println("GameStarted: " + gameStarted);
    // Serial.println("GameMode: " + gameMode);
    // Serial.println("AIStrategy: " + aiStrategy);
    // Serial.println("Message: " + message);

}


void printBoard() {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            Serial.print(gameBoard[row][col] + " ");
        }
        Serial.println();
    }
}



String getValue(String xml, String tag) {
    // Отримуємо значення між тегами
    String startTag = "<" + tag + ">";
    String endTag = "</" + tag + ">";
    int startIndex = xml.indexOf(startTag) + startTag.length();
    int endIndex = xml.indexOf(endTag);
    
    if (startIndex >= 0 && endIndex > startIndex) {
        return xml.substring(startIndex, endIndex);
    }
    return "-"; // Повертаємо порожнє, якщо не знайдено
}





String generateXML() {
    // Генерація нового XML на основі змінених змінних
    String xml = "<game>\n";
    xml += "    <connect>" + connectionStatus + "</connect>\n";
    xml += "    <gstarted>" + gameStarted + "</gstarted>\n";
    xml += "    <gmode>" + gameMode + "</gmode>\n";
    xml += "    <aistrat>" + aiStrategy + "</aistrat>\n";
    xml += "    <msg>" + message + "</msg>\n";
    xml += "    <board>\n";
    
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            xml += "        <c" + String(row + 1) + String(col + 1) + ">" + gameBoard[row][col] + "</c" + String(row + 1) + String(col + 1) + ">\n";
        }
    }
    
    xml += "    </board>\n";
    xml += "</game>";
    return xml;
}
