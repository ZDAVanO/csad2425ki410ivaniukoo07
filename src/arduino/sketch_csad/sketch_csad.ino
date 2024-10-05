void setup() {
  pinMode(13, OUTPUT); // Ініціалізація піну для лампочки
  digitalWrite(13, LOW); // Вимкнути лампочку

  Serial.begin(9600);  // Налаштування серійного зв'язку зі швидкістю 9600 біт/с
  while (!Serial);     // Очікуємо поки порт стане доступним (тільки для Leonardo і схожих плат)
}

void loop() {
  if (Serial.available() > 0) {
    String receivedMessage = Serial.readString();  // Читаємо дані з серійного порту
    
    // Перетворюємо всі літери в верхній регістр
    for (int i = 0; i < receivedMessage.length(); i++) {
      receivedMessage[i] = toupper(receivedMessage[i]);
    }

    // Відправляємо змінене повідомлення назад
    Serial.println(receivedMessage);
  }
}
