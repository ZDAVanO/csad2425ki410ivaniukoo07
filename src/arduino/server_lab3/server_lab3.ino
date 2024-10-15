



String connect, gstarted, gmode, aistrat, msg;
String board[3][3];



void setup() {
  Serial.begin(9600);  // Налаштування серійного зв'язку зі швидкістю 9600 біт/с


  // Приклад отриманого XML рядка
  // String input = "<g><con>0</con><gs>0</gs><gm>ManVsAI</gm><ais>Random</ais><msg>hello</msg><brd><c11>-</c11><c12>-</c12><c13>-</c13><c21>-</c21><c22>-</c22><c23>-</c23><c31>-</c31><c32>-</c32><c33>x</c33></brd></g>";

  // Парсинг XML
  // parseXML(input);

  // Друк значень для перевірки
  // Serial.println("connect: " + connect);
  // Serial.println("gstarted: " + gstarted);
  // Serial.println("gmode: " + gmode);
  // Serial.println("aistrat: " + aistrat);
  // Serial.println("msg: " + msg);

  // for (int i = 0; i < 3; i++) {
  //   for (int j = 0; j < 3; j++) {
  //     Serial.print(board[i][j] + " ");
  //   }
  //   Serial.println();
  // }

  // Збір нового XML рядка
  // String output = buildXML();
  // Serial.println(output);
  
}

void loop() {
  if (Serial.available() > 0) {




    String receivedMessage = Serial.readString();  // Читаємо дані з серійного порту

    
    // String parsedMessage = parseXML(receivedMessage);
    
    // // Перетворюємо всі літери в верхній регістр
    // for (int i = 0; i < parsedMessage.length(); i++) {
    //   parsedMessage[i] = toupper(parsedMessage[i]);
    // }

    // String xmlToSend = createXML(parsedMessage);


    parseXML(receivedMessage);

    // connectionStatus = "1";
    connect = "1";
    msg = "AAAA";

    String output = buildXML();
    output.replace("\n", ""); // Заміна /n на пустий рядок
    Serial.println(output);



    // String incomingMessage = Serial.readString();  // Читання повідомлення
    // if (incomingMessage == "0") {
    //   Serial.println("1");  // Відповідь, якщо отримали правильне повідомлення
    // } else {
    //   Serial.println("ERROR");  // Інша відповідь
    // }


  }
}



void parseXML(String input) {

  connect = getTagValue(input, "con");
  gstarted = getTagValue(input, "gs");
  gmode = getTagValue(input, "gm");
  aistrat = getTagValue(input, "ais");
  msg = getTagValue(input, "msg");

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


String getTagValue(String input, String tag) {

  String openTag = "<" + tag + ">";
  String closeTag = "</" + tag + ">";
  int start = input.indexOf(openTag) + openTag.length();
  int end = input.indexOf(closeTag);

  // Serial.println("tag: " + tag);

  // Serial.print("openTag: ");
  // Serial.println(openTag);

  // Serial.print("closeTag: ");
  // Serial.println(closeTag);

  // Serial.print("start: ");
  // Serial.println(start);

  // Serial.print("end: ");
  // Serial.println(end);

  // Serial.println();



  if (start == -1 || end == -1 || end < start) {
    return "-";
  }

  return input.substring(start, end);
}




// void printBoard() {
//     for (int row = 0; row < 3; row++) {
//         for (int col = 0; col < 3; col++) {
//             Serial.print(board[row][col] + " ");
//         }
//         Serial.println();
//     }
// }




// Функція для збирання нового XML рядка
String buildXML() {
  String output = "<g>\n";
  output += "<con>" + connect + "</con>\n";
  output += "<gs>" + gstarted + "</gs>\n";
  output += "<gm>" + gmode + "</gm>\n";
  output += "<ais>" + aistrat + "</ais>\n";
  output += "<msg>" + msg + "</msg>\n";
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

