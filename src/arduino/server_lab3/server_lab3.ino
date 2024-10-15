


String connect_arduino, game_started, game_mode, ai_strategy, message ;
String next_turn;
String board[3][3];


// int emptyCells[9];
// int emptyCount = 0;

// int x_cells = 0;
// int o_cells = 0;

void setup() {

  Serial.begin(9600);  // Налаштування серійного зв'язку зі швидкістю 9600 біт/с
  randomSeed(analogRead(0));  // Ініціалізація генератора випадкових чисел
}

void loop() {
  if (Serial.available() > 0) {

    String receivedMessage = Serial.readString();  // Читаємо дані з серійного порту
    parseXML(receivedMessage);

    if (connect_arduino == "0"){
      connect_arduino = "1";
    }

    if (game_started == "1" && game_mode == "mva" && ai_strategy == "rand"){
      makeRandomMove();  // Функція для випадкового ходу AI
    }

    if (game_started == "1" && game_mode == "ava" && ai_strategy == "rand"){
      avaMove();
    }

    if (game_started == "1" && game_mode == "mvm"){
      checkBoard();
    }





    if (game_started == "1" && game_mode == "mva" && ai_strategy == "win"){
      stratMove("o");
    }




      
    String output = buildXML();
    output.replace("\n", ""); // Заміна /n на пустий рядок
    Serial.print(output);


  }
}


bool checkWinner(String board[3][3], String player) {
  // Перевірка рядків (горизонталі)
  for (int i = 0; i < 3; i++) {
    if (board[i][0] == player && board[i][1] == player && board[i][2] == player) {
      return true;
    }
  }

  // Перевірка колонок (вертикалі)
  for (int i = 0; i < 3; i++) {
    if (board[0][i] == player && board[1][i] == player && board[2][i] == player) {
      return true;
    }
  }

  // Перевірка діагоналей
  if (board[0][0] == player && board[1][1] == player && board[2][2] == player) {
    return true;
  }
  if (board[0][2] == player && board[1][1] == player && board[2][0] == player) {
    return true;
  }

  return false;
}

void avaMove() {
  // Створюємо масив, що зберігає індекси порожніх клітинок
  int emptyCells[9];
  int emptyCount = 0;

  // emptyCount = 0;

  int x_cells = 0;
  int o_cells = 0;

  // x_cells = 0;
  // o_cells = 0;


  if (checkWinner(board, "x")) {
    message = "Winner_X";
    game_started = "0";
    return;
  }
  else if (checkWinner(board, "o")) {
    message = "Winner_O";
    game_started = "0";
    return;
  }



  // Проходимо через поле та знаходимо всі порожні клітинки
  for (int row = 0; row < 3; row++) {
    for (int col = 0; col < 3; col++) {

      if (board[row][col] == "x") {
        x_cells++;
      }
      else if (board[row][col] == "o") {
        o_cells++;
      }
      else {
        emptyCells[emptyCount] = row * 3 + col;  // Зберігаємо індекс клітинки у вигляді одного числа
        emptyCount++;
      }

    }
  }

  // next_turn = "o";
  // message = "ava";



  // Якщо є порожні клітинки, вибираємо випадкову
  if (emptyCount > 0) {
    int randomIndex = random(0, emptyCount);  // Випадковий індекс від 0 до emptyCount-1
    int rand_cell = emptyCells[randomIndex];  // Індекс випадкової клітинки

    // Перетворюємо індекс назад у координати (row, col)
    int row = rand_cell / 3;
    int col = rand_cell % 3;

    // Записуємо "o" в обрану клітинку
    

    if(next_turn == "o"){
      board[row][col] = "o"; 
      next_turn = "x";
    }
    else {
      board[row][col] = "x"; 
      next_turn = "o";
    }
    

  }
  else {
    message = "Draw";
    game_started = "0";
  }



}

void checkBoard() {

  int emptyCells[9];
  int emptyCount = 0;

  // emptyCount = 0;

  int x_cells = 0;
  int o_cells = 0;

  // x_cells = 0;
  // o_cells = 0;

  if (checkWinner(board, "x")) {
    message = "Winner_X!";
    return;
  }
  else if (checkWinner(board, "o")) {
    message = "Winner_O!";
    return;
  }


  // Проходимо через поле та знаходимо всі порожні клітинки
  for (int row = 0; row < 3; row++) {
    for (int col = 0; col < 3; col++) {

      if (board[row][col] == "x") {
        x_cells++;
      }
      else if (board[row][col] == "o") {
        o_cells++;
      }
      else {
        emptyCells[emptyCount] = row * 3 + col;  // Зберігаємо індекс клітинки у вигляді одного числа
        emptyCount++;
      }

    }
  }

  // Вибір гравця для першого ходу
  if (emptyCount == 9) {
    int randomChoice = random(0, 2);  // Випадковий вибір: 0 або 1
    if (randomChoice == 0) {
      message = "Player_X_turn";
      next_turn = "x";
      return;
    }
    else {
      message = "Player_O_turn";
      next_turn = "o";
      return;
    }
  }

  if(next_turn == "x"){
    message = "Player_O_turn";
    // message = String(x_cells);
    next_turn = "o";
  }
  else {
    message = "Player_X_turn";
    // message = String(x_cells);
    next_turn = "x";
  }


}

void makeRandomMove() {
  // Створюємо масив, що зберігає індекси порожніх клітинок
  int emptyCells[9];
  int emptyCount = 0;

  // emptyCount = 0;

  int x_cells = 0;
  int o_cells = 0;

  // x_cells = 0;
  // o_cells = 0;

  if (checkWinner(board, "x")) {
    message = "Winner_X!";
    return;
  }
  else if (checkWinner(board, "o")) {
    message = "Winner_O!";
    return;
  }


  // Проходимо через поле та знаходимо всі порожні клітинки
  for (int row = 0; row < 3; row++) {
    for (int col = 0; col < 3; col++) {

      if (board[row][col] == "x") {
        x_cells++;
      }
      else if (board[row][col] == "o") {
        o_cells++;
      }
      else {
        emptyCells[emptyCount] = row * 3 + col;  // Зберігаємо індекс клітинки у вигляді одного числа
        emptyCount++;
      }

    }
  }

  next_turn = "x";
  message = "Player turn";


  if (emptyCount == 9) {
    int randomChoice = random(0, 2);  // Випадковий вибір: 0 або 1
    if (randomChoice == 0) {
      return;
    }
  }

  // Якщо є порожні клітинки, вибираємо випадкову
  if (emptyCount > 0) {
    int randomIndex = random(0, emptyCount);  // Випадковий індекс від 0 до emptyCount-1
    int rand_cell = emptyCells[randomIndex];  // Індекс випадкової клітинки

    // Перетворюємо індекс назад у координати (row, col)
    int row = rand_cell / 3;
    int col = rand_cell % 3;

    // Записуємо "o" в обрану клітинку
    board[row][col] = "o"; 


    

  }

  if (checkWinner(board, "x")) {
    message = "Winner_X!";
    return;
  }
  else if (checkWinner(board, "o")) {
    message = "Winner_O!";
    return;
  }

}





bool checkWin(String player) {
  for (int i = 0; i < 3; i++) {
    // Перевірка рядків і стовпців
    if ((board[i][0] == player && board[i][1] == player && board[i][2] == "-") ||
        (board[i][0] == player && board[i][2] == player && board[i][1] == "-") ||
        (board[i][1] == player && board[i][2] == player && board[i][0] == "-")) {
      board[i][0] == "-" ? board[i][0] = "o" : 
      board[i][1] == "-" ? board[i][1] = "o" : board[i][2] = "o";
      return true;
    }
    
    if ((board[0][i] == player && board[1][i] == player && board[2][i] == "-") ||
        (board[0][i] == player && board[2][i] == player && board[1][i] == "-") ||
        (board[1][i] == player && board[2][i] == player && board[0][i] == "-")) {
      board[0][i] == "-" ? board[0][i] = "o" : 
      board[1][i] == "-" ? board[1][i] = "o" : board[2][i] = "o";
      return true;
    }
  }

  // Перевірка діагоналей
  if ((board[0][0] == player && board[1][1] == player && board[2][2] == "-") ||
      (board[0][0] == player && board[2][2] == player && board[1][1] == "-") ||
      (board[1][1] == player && board[2][2] == player && board[0][0] == "-")) {
    board[0][0] == "-" ? board[0][0] = "o" : 
    board[1][1] == "-" ? board[1][1] = "o" : board[2][2] = "o";
    return true;
  }

  if ((board[0][2] == player && board[1][1] == player && board[2][0] == "-") ||
      (board[0][2] == player && board[2][0] == player && board[1][1] == "-") ||
      (board[1][1] == player && board[2][0] == player && board[0][2] == "-")) {
    board[0][2] == "-" ? board[0][2] = "o" : 
    board[1][1] == "-" ? board[1][1] = "o" : board[2][0] = "o";
    return true;
  }

  return false;
}





void stratMove(String currentPlayer) {
  
  String opponent = (currentPlayer == "x") ? "o" : "x";


  int emptyCells[9];
  int emptyCount = 0;
  int x_cells = 0;
  int o_cells = 0;

  if (checkWinner(board, "x")) {
    message = "Winner_X";
    game_started = "0";
    return;
  }
  else if (checkWinner(board, "o")) {
    message = "Winner_O";
    game_started = "0";
    return;
  }


  next_turn = "x";
  message = "Player turn";

  // Проходимо через поле та знаходимо всі порожні клітинки
  for (int row = 0; row < 3; row++) {
    for (int col = 0; col < 3; col++) {

      if (board[row][col] == "x") {
        x_cells++;
      }
      else if (board[row][col] == "o") {
        o_cells++;
      }
      else {
        emptyCells[emptyCount] = row * 3 + col;  // Зберігаємо індекс клітинки у вигляді одного числа
        emptyCount++;
      }

    }
  }


  if (emptyCount == 9) {
    int randomChoice = random(0, 2);  // Випадковий вибір: 0 або 1
    if (randomChoice == 0) {
      return;
    }
  }

  

  // 1. Спробуємо виграти
  if (checkWin("o")) return;

  // 2. Заблокуємо "x", якщо він може виграти
  if (checkWin("x")) return;

  // 3. Займаємо центр, якщо він вільний
  if (board[1][1] == "-") {
    board[1][1] = "o";
    return;
  }

  // 4. Займаємо кути, якщо вони вільні
  if (board[0][0] == "-") {
    board[0][0] = "o";
    return;
  }
  if (board[0][2] == "-") {
    board[0][2] = "o";
    return;
  }
  if (board[2][0] == "-") {
    board[2][0] = "o";
    return;
  }
  if (board[2][2] == "-") {
    board[2][2] = "o";
    return;
  }

  // 5. Займаємо будь-яке інше місце
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board[i][j] == "-") {
        board[i][j] = "o";
        return;
      }
    }
  }



}






void parseXML(String input) {
  connect_arduino = getTagValue(input, "con");
  game_started = getTagValue(input, "gs");
  game_mode = getTagValue(input, "gm");
  ai_strategy = getTagValue(input, "ais");
  message = getTagValue(input, "msg");

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

  if (start == -1 || end == -1 || end < start) {
    return "-";
  }

  return input.substring(start, end);
}

// Функція для збирання нового XML рядка
String buildXML() {
  String output = "<g>\n";
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

