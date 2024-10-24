void setup() {
  pinMode(13, OUTPUT); // Ініціалізація піну для лампочки
  digitalWrite(13, HIGH);

  Serial.begin(9600);  // Налаштування серійного зв'язку зі швидкістю 9600 біт/с
  while (!Serial);     // Очікуємо поки порт стане доступним (тільки для Leonardo і схожих плат)
  // delay(2000);
  digitalWrite(13, LOW);
}

void loop() {
  if (Serial.available() > 0) {
    String receivedMessage = Serial.readString();  // Читаємо дані з серійного порту
    digitalWrite(13, HIGH);

    
    String parsedMessage = parseXML(receivedMessage);
    
    // Перетворюємо всі літери в верхній регістр
    for (int i = 0; i < parsedMessage.length(); i++) {
      parsedMessage[i] = toupper(parsedMessage[i]);
    }

    String xmlToSend = createXML(parsedMessage);


    digitalWrite(13, LOW);
    // Відправляємо змінене повідомлення назад
    Serial.print(xmlToSend);
  }
}

// Функція створення XML
String createXML(String message) {
  String xml = "<message>";
  xml += message;
  xml += "</message>";
  return xml;
}

// Функція парсингу XML
String parseXML(String xml) {
  int start = xml.indexOf("<message>") + 9; // 9 - це довжина тегу <message>
  int end = xml.indexOf("</message>");
  
  if (start != -1 && end != -1) {
    return xml.substring(start, end);
  }
  
  return ""; // Якщо XML некоректний
}
