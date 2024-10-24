/**
 * @file server_lab3.ino
 * @brief A simple Arduino server for a Tic-Tac-Toe game.
 *
 * This program enables an Arduino to act as a server for a Tic-Tac-Toe game,
 * supporting various game modes and AI strategies. The game state is managed
 * through a board represented by a 3x3 array. The server communicates with
 * clients via serial communication, parsing XML-like messages to update the game
 * state and respond to moves.
 *
 * The program supports two-player modes (human vs. human and human vs. AI)
 * as well as different AI strategies for making moves. It checks for winning
 * conditions and manages the game flow accordingly.
 *
 * @note This file uses serial communication at a baud rate of 9600 bps.
 */


String connect_arduino;  ///< Connection status flag (0: not connected, 1: connected)
String game_started;      ///< Game status flag (0: not started, 1: started)
String game_mode;        ///< Current game mode (e.g., "mva", "ava", "mvm")
String ai_strategy;       ///< AI strategy being used (e.g., "rand", "win")
String message;           ///< Message to be sent back to the client
String next_turn;         ///< Indicates whose turn is next (e.g., "x", "o")
String board[3][3];       ///< 3x3 Tic-Tac-Toe game board

/**
 * @brief Initializes the Arduino server.
 *
 * This function sets up the serial communication at 9600 bps and initializes
 * the random number generator.
 */
void setup() {
  Serial.begin(9600);  // Налаштування серійного зв'язку зі швидкістю 9600 біт/с
  randomSeed(analogRead(0));  // Ініціалізація генератора випадкових чисел

}

/**
 * @brief Main loop for handling game logic and serial communication.
 *
 * This function continually checks for incoming serial messages, processes
 * game actions based on the messages, and sends back the game state as an
 * XML string.
 */
void loop() {
  if (Serial.available() > 0) {

    String receivedMessage = Serial.readString();  // Читаємо дані з серійного порту
    parseXML(receivedMessage);

    if (connect_arduino == "0") { 
      connect_arduino = "1"; 
      message = "Successful connection";
    }
    else if (game_started == "1" && game_mode == "mva" && ai_strategy == "rand") { 
      mva_rand_move(); 
    }
    else if (game_started == "1" && game_mode == "mva" && ai_strategy == "win") { 
      win_move("o", true); 
    } 
    else if (game_started == "1" && game_mode == "ava" && ai_strategy == "rand") { 
      ava_rand_move(); 
    }
    else if (game_started == "1" && game_mode == "ava" && ai_strategy == "win") {
      if (next_turn == "o") { win_move("o", false); }
      else { win_move("x", false); }
    }
    else if (game_started == "1" && game_mode == "mvm") { 
      mvm_move_check(); 
    }
    else { 
      message = "Invalid input"; 
      game_started = "0";
    }

    String output = buildXML();
    output.replace("\n", ""); // Заміна /n на пустий рядок
    Serial.print(output);
  }
}

/**
 * @brief Checks if a given player has won the game.
 *
 * @param board  The current state of the game board.
 * @param player The player symbol to check for (either "x" or "o").
 * @return True if the player has won, false otherwise.
 */
bool checkBoardWin(String board[3][3], String player) {
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

/**
 * @brief Checks if all fields on the game board are occupied.
 *
 * @param board The current state of the game board.
 * @return True if all fields are occupied, false otherwise.
 */
bool allFieldsOccupied(String board[3][3]) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board[i][j] != "x" && board[i][j] != "o") {
        return false; // Знайшли пусте поле, повертаємо false
      }
    }
  }
  return true; // Якщо всі поля заповнені
}

/**
 * @brief Checks for a winner or a draw in the game.
 *
 * This function evaluates the game state and sets the message and game
 * status accordingly.
 *
 * @return True if there is a winner or a draw, false otherwise.
 */
bool checkForWinner() {
  if (checkBoardWin(board, "x")) {
    message = "Winner - X";
    game_started = "0";
    return true;
  }
  else if (checkBoardWin(board, "o")) {
    message = "Winner - O";
    game_started = "0";
    return true;
  }
  else if (allFieldsOccupied(board)) {
    message = "Draw";
    game_started = "0";
    return true;
  }
  else { 
    return false;
  }
}

/**
 * @brief Makes a random move for the AI in "ava" mode.
 *
 * The function searches for empty cells on the board and makes a random
 * move. If a player has won, it stops making moves.
 */
void ava_rand_move() {
  int emptyCells[9];
  int emptyCount = 0;

  if (checkForWinner()) { return; }

  // Проходимо через поле та знаходимо всі порожні клітинки
  for (int row = 0; row < 3; row++) {
    for (int col = 0; col < 3; col++) {

      if (board[row][col] != "x" && board[row][col] != "o") {   
        emptyCells[emptyCount] = row * 3 + col;  // Зберігаємо індекс клітинки у вигляді одного числа
        emptyCount++;
      }
    }
  }

  // Якщо є порожні клітинки, вибираємо випадкову
  if (emptyCount > 0) {
    int randomIndex = random(0, emptyCount);  // Випадковий індекс від 0 до emptyCount-1
    int rand_cell = emptyCells[randomIndex];  // Індекс випадкової клітинки

    // Перетворюємо індекс назад у координати (row, col)
    int row = rand_cell / 3;
    int col = rand_cell % 3;

    if(next_turn == "o"){
      board[row][col] = "o"; 
      next_turn = "x";
      message = "Player X turn";
    }
    else {
      board[row][col] = "x"; 
      next_turn = "o";
      message = "Player O turn";
    } 

    if (checkForWinner()) { return; }

  }
  else {
    message = "Draw";
    game_started = "0";
  }
}

/**
 * @brief Checks the state of the game for "mvm" mode.
 *
 * This function checks for available moves and determines the turn for the
 * players.
 */
void mvm_move_check() {
  int emptyCount = 0;

  if (checkForWinner()) { return; }

  // Проходимо через поле та знаходимо всі порожні клітинки
  for (int row = 0; row < 3; row++) {
    for (int col = 0; col < 3; col++) {

      if (board[row][col] != "x" && board[row][col] != "o") {
        emptyCount++;
      }
    }
  }

  // Вибір гравця для першого ходу
  if (emptyCount == 9) {
    int randomChoice = random(0, 2);  // Випадковий вибір: 0 або 1
    if (randomChoice == 0) {
      message = "Player X turn";
      next_turn = "x";
      return;
    }
    else {
      message = "Player O turn";
      next_turn = "o";
      return;
    }
  }

  if(next_turn == "x") {
    message = "Player O turn";
    next_turn = "o";
  }
  else {
    message = "Player X turn";
    next_turn = "x";
  }
}

/**
 * @brief Makes a random move for the AI in "mva" mode.
 *
 * The function searches for empty cells on the board and makes a random
 * move, if available.
 */
void mva_rand_move() {
  int emptyCells[9];
  int emptyCount = 0;

  if (checkForWinner()) { return; }

  // Проходимо через поле та знаходимо всі порожні клітинки
  for (int row = 0; row < 3; row++) {
    for (int col = 0; col < 3; col++) {

      if (board[row][col] != "x" && board[row][col] != "o") {   

        emptyCells[emptyCount] = row * 3 + col;  // Зберігаємо індекс клітинки у вигляді одного числа
        emptyCount++;
      }
    }
  }

  next_turn = "x";
  message = "Player X turn";

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

  checkForWinner();
}


/**
 * @brief Checks if a player can win in the next move.
 *
 * @param player The symbol of the player to check.
 * @param currentPlayer The current player making the move.
 * @return True if a winning move is made, false otherwise.
 */
bool checkWin(String player, String currentPlayer) {
  for (int i = 0; i < 3; i++) {
    // Перевірка рядків і стовпців
    if ((board[i][0] == player && board[i][1] == player && board[i][2] == "-") ||
        (board[i][0] == player && board[i][2] == player && board[i][1] == "-") ||
        (board[i][1] == player && board[i][2] == player && board[i][0] == "-")) {
      board[i][0] == "-" ? board[i][0] = currentPlayer : 
      board[i][1] == "-" ? board[i][1] = currentPlayer : board[i][2] = currentPlayer;
      return true;
    }
    
    if ((board[0][i] == player && board[1][i] == player && board[2][i] == "-") ||
        (board[0][i] == player && board[2][i] == player && board[1][i] == "-") ||
        (board[1][i] == player && board[2][i] == player && board[0][i] == "-")) {
      board[0][i] == "-" ? board[0][i] = currentPlayer : 
      board[1][i] == "-" ? board[1][i] = currentPlayer : board[2][i] = currentPlayer;
      return true;
    }
  }

  // Перевірка діагоналей
  if ((board[0][0] == player && board[1][1] == player && board[2][2] == "-") ||
      (board[0][0] == player && board[2][2] == player && board[1][1] == "-") ||
      (board[1][1] == player && board[2][2] == player && board[0][0] == "-")) {
    board[0][0] == "-" ? board[0][0] = currentPlayer : 
    board[1][1] == "-" ? board[1][1] = currentPlayer : board[2][2] = currentPlayer;
    return true;
  }

  if ((board[0][2] == player && board[1][1] == player && board[2][0] == "-") ||
      (board[0][2] == player && board[2][0] == player && board[1][1] == "-") ||
      (board[1][1] == player && board[2][0] == player && board[0][2] == "-")) {
    board[0][2] == "-" ? board[0][2] = currentPlayer : 
    board[1][1] == "-" ? board[1][1] = currentPlayer : board[2][0] = currentPlayer;
    return true;
  }

  return false;
}

/**
 * @brief Attempts to make a winning move for the current player.
 *
 * The function will prioritize winning moves, blocking opponent's winning moves,
 * and then filling available spaces strategically.
 *
 * @param currentPlayer The current player symbol making the move.
 * @param randomPlayer Indicates if the player can make a random move.
 */
void win_move(String currentPlayer, bool randomPlayer) {

  if (checkForWinner()) { return; }

  bool made_move = false;

  String opponent = (currentPlayer == "x") ? "o" : "x";

  next_turn = opponent;

  String upperOpponent = opponent;
  upperOpponent.toUpperCase();
  message = "Player "+ upperOpponent +" turn";

  if (randomPlayer){
    int emptyCount = 0;
    for (int row = 0; row < 3; row++) {
      for (int col = 0; col < 3; col++) {
        if (board[row][col] != "x" && board[row][col] != "0") {
          emptyCount++;
        }
      }
    }

    if (emptyCount == 9) {
      int randomChoice = random(0, 2);  // Випадковий вибір: 0 або 1
      if (randomChoice == 0) { return; }
    }
  }
  
  // 1. Спробуємо виграти
  if (!made_move && checkWin(currentPlayer, currentPlayer)) {
    made_move = true;
  } 

  // 2. Заблокуємо "x", якщо він може виграти
  if (!made_move && checkWin(opponent, currentPlayer)) {
    made_move = true;
  }

  // 3. Займаємо центр, якщо він вільний
  if (!made_move && board[1][1] == "-") {
    board[1][1] = currentPlayer;
    made_move = true;
  }

  // 4. Займаємо кути, якщо вони вільні
  if (!made_move && board[0][0] == "-" && board[2][2] != opponent) {
    board[0][0] = currentPlayer;
    made_move = true;
  }
  if (!made_move && board[0][2] == "-" && board[2][0] != opponent) {
    board[0][2] = currentPlayer;
    made_move = true;
  }
  if (!made_move && board[2][0] == "-" && board[0][2] != opponent) {
    board[2][0] = currentPlayer;
    made_move = true;
  }
  if (!made_move && board[2][2] == "-" && board[0][0] != opponent) {
    board[2][2] = currentPlayer;
    made_move = true;
  }
  if (!made_move) {
    int emptyCells[9];
    int emptyCount = 0;

    // Проходимо через поле та знаходимо всі порожні клітинки
    for (int row = 0; row < 3; row++) {
      for (int col = 0; col < 3; col++) {

        if (board[row][col] != "x" && board[row][col] != "o") {   
          emptyCells[emptyCount] = row * 3 + col;  // Зберігаємо індекс клітинки у вигляді одного числа
          emptyCount++;
        }
      }
    }

    // Якщо є порожні клітинки, вибираємо випадкову
    if (emptyCount > 0) {
      int randomIndex = random(0, emptyCount);  // Випадковий індекс від 0 до emptyCount-1
      int rand_cell = emptyCells[randomIndex];  // Індекс випадкової клітинки

      // Перетворюємо індекс назад у координати (row, col)
      int row = rand_cell / 3;
      int col = rand_cell % 3;

      board[row][col] = currentPlayer; 
    }
  }

  // 5. Займаємо будь-яке інше місце
  // for (int i = 0; i < 3; i++) {
  //   for (int j = 0; j < 3; j++) {

  //     if (!made_move && board[i][j] == "-") {
  //       board[i][j] = currentPlayer;
  //       made_move = true;
  //     }
  //   }
  // }

  if (checkForWinner()) { return; }
}


/**
 * @brief Parses incoming XML-like messages to update game state variables.
 *
 * @param input The incoming message containing game state data.
 */
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

/**
 * @brief Retrieves the value of a specified XML tag from a string.
 *
 * @param input The input string containing XML data.
 * @param tag The tag to retrieve the value for.
 * @return The value of the tag or "-" if the tag is not found.
 */
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


/**
 * @brief Constructs an XML string representing the current game state.
 *
 * @return A string formatted as XML containing game state information.
 */
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

