#include <LiquidCrystal.h>
#include "LedControl.h" 
#include "pitches.h"
#include "menu.h"

bool matrix[numRooms][numRows][numCols] = {
  //Room 0
  {
  // 0 1 2 3 4 5 6 7
    {0,0,0,0,0,0,0,0}, //0
    {0,0,0,0,0,0,0,0}, //1
    {0,0,0,0,0,0,0,0}, //2
    {0,0,0,0,0,0,0,0}, //3
    {0,0,0,0,0,0,0,0}, //4
    {0,0,0,0,0,0,0,0}, //5
    {0,0,0,0,0,0,0,0}, //6
    {0,0,0,0,0,0,0,0}  //7
  },
  //Room 1
  {
  // 0 1 2 3 4 5 6 7
    {0,0,0,0,0,0,0,0}, //0
    {0,0,0,0,0,0,0,0}, //1
    {0,0,0,0,0,0,0,0}, //2
    {0,0,0,0,0,0,0,0}, //3
    {0,0,0,0,0,0,0,0}, //4
    {0,0,0,0,0,0,0,0}, //5
    {0,0,0,0,0,0,0,0}, //6
    {0,0,0,0,0,0,0,0}  //7
  },
  //Room 3
  {
  // 0 1 2 3 4 5 6 7
    {0,0,0,0,0,0,0,0}, //0
    {0,0,0,0,0,0,0,0}, //1
    {0,0,0,0,0,0,0,0}, //2
    {0,0,0,0,0,0,0,0}, //3
    {0,0,0,0,0,0,0,0}, //4
    {0,0,0,0,0,0,0,0}, //5
    {0,0,0,0,0,0,0,0}, //6
    {0,0,0,0,0,0,0,0}  //7
  },
  //Room 3
  {
  // 0 1 2 3 4 5 6 7
    {0,0,0,0,0,0,0,0}, //0
    {0,0,0,0,0,0,0,0}, //1
    {0,0,0,0,0,0,0,0}, //2
    {0,0,0,0,0,0,0,0}, //3
    {0,0,0,0,0,0,0,0}, //4
    {0,0,0,0,0,0,0,0}, //5
    {0,0,0,0,0,0,0,0}, //6
    {0,0,0,0,0,0,0,0}  //7
  }
};

bool roomGenerated[] = {0,0,0,0}; // new room will be generated when the player arrive for the first time there

//animations matrix
const byte matrixHappy[8] = {
  0b00111100,
  0b01000010,
  0b10100101,
  0b10000001,
  0b10100101,
  0b10011001,
  0b01000010,
  0b00111100
};

const byte matrixSad[8] = {
  0b00111100,
  0b01000010,
  0b10100101,
  0b10000001,
  0b10011001,
  0b10100101,
  0b01000010,
  0b00111100
};

const byte matrixWinner[8] = {
  0b00000000,
  0b00011000,
  0b10111101,
  0b11111111,
  0b11111111,
  0b11111111,
  0b00000000,
  0b00000000
};

const byte matrixPause[8] = {
  0b00100000,
  0b00110000,
  0b00111000,
  0b00111100,
  0b00111000,
  0b00110000,
  0b00100000,
  0b00000000
};

void setup() {
  Serial.begin(9600);
  
  brightMatrix = EEPROM.read(0);
  brightLCD = EEPROM.read(1);
  Serial.println(brightMatrix);
  Serial.println(brightLCD);

  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false); // turn off power saving, enables display
  //lc.setIntensity(0, (brightMatrix+1)*5); // sets brightness (0~15 possible values)
  lc.setIntensity(0, 15);
  lc.clearDisplay(0);// clear screen

  // initialize random seed
  randomSeed(analogRead(0));
  generateRandomPlayerPosition(); // random player
  generateRandomWallPoints(); // random walls
  roomGenerated[currentRoom] = true;

  //buttons
  pinMode(startButton,INPUT_PULLUP);

  //joystick
  pinMode(joystickSW, INPUT_PULLUP);//buton
  pinMode(joystickX,  INPUT_PULLUP);//Ox
  pinMode(joystickY,  INPUT_PULLUP);//Oy
  

  //rgb led pins
  pinMode(blueLedPin, OUTPUT);
  pinMode(greenLedPin,OUTPUT);
  pinMode(redLedPin,  OUTPUT);

  //buzzer pin
  pinMode(buzzPin,  OUTPUT);

  //LCD brightness pin
  pinMode(LCDBrightness, OUTPUT);

  lcd.begin(16, 2);
  welcome();
  createCustomCharacter();
}

void loop() {
  showMatrix(); 
}

void showMatrix(){

  //debounce for starting game
  if(!startButtonPressed){
    if(millis() > 5000)
    {
      principalMenu(); 
    }
    else
      if(millis() > 4000)
        lcd.clear();
  }

  startValue = digitalRead(startButton);

  if(!roomGenerated[currentRoom]){
    generateRoom(currentRoom);
    return 0;
  }

  if(!startValue && !gameState)//if is pressed => game start
  {
    startButtonPressed = true;
    gameState = true;
    lastPress = millis();
    return;
  }

  if(!startButtonPressed){//before game start
    for (int row = 0; row < 8; row++) {
        lc.setRow(0, row, matrixHappy[row]);
      }
    return;
  }

  if(!playerState) //if the player died
    {
      //Serial.println("Game Over!");
      for (int row = 0; row < 8; row++) {
        lc.setRow(0, row, matrixSad[row]);
      }
      //startButtonPressed = false;
      if(millis() - endGameTimer > 5000 && endGameTimer){
        resetAll();
      }
      else
        if(!endGameTimer)
          endGameTimer = millis();
        else {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Sorry!");
          lcd.setCursor(0,1);
          lcd.print("You lose!");
          //Serial.println("Sorry!");
        }
      return;
    }
  
  if(!startValue && gameState && (millis() - lastPress > buttonDebounce || !lastPress)){// pause game
    pauseGame = !pauseGame;
    lastPress = millis();
  }

  if(pauseGame){
    //Serial.println("Pause!");
    for (int row = 0; row < 8; row++) {
      lc.setRow(0, row, matrixPause[row]);
    }
    return;
  }

  //update timer during the game and showing which room you are in 
  if(millis() - startTimer > second && wallsDestroyed != totalWalls){
    timer ++;
    startTimer = millis();
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Room ");
    lcd.print(currentRoom + 1);
    lcd.setCursor(0, 1);

    lcd.write(6);
    lcd.print(int(timer));
    lcd.setCursor(10, 1);
    lcd.write(7);
    lcd.print(totalWalls - wallsDestroyed);
  }
  
  if(wallsDestroyed == totalWalls && playerState) // if no wall remain and the player is alive 
    {  
      //Serial.println("Winner!");
      for (int row = 0; row < 8; row++) {
        lc.setRow(0, row, matrixWinner[row]);
      }
      if(millis() - endGameTimer > 5000 && endGameTimer){ // after 5 second reset the game
        resetAll();
      }
      else
        if(!endGameTimer)
          endGameTimer = millis();
        else {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Congratulation!");
         // Serial.println("Congratulation!");
        }
      return;
    }

  //show walls
  wallsRemain = 0; 
  for(int i = 0; i < numRows; i++)
    for(int j = 0; j < numCols; j++){
      if(matrix[currentRoom][i][j] == 1) wallsRemain ++;
        lc.setLed(0, i, j, matrix[currentRoom][i][j]);
    }
  showPlayer();
  

  movePlayer();

  if(timer < 2)
    return;

  if(bombX != -1 && bombY != -1) // a bomb was set
    showBomb();
  else
    setBomb();//if button is pressed 

}

void showPlayer(){ //player should blink 
  if(millis() - lastPlayerBlink > playerBlink || !lastPlayerBlink){
    lastPlayerBlink = millis();
    playerLed = !playerLed;
    lc.setLed(0, playerX, playerY, playerLed);
  }
}

void showBomb(){ //the bomb should blink faster than the player
  if(millis() - lastBombBlink > bombBlink || !lastBombBlink){
    lastBombBlink = millis();
    bombLed = !bombLed;
    lc.setLed(0, bombX, bombY, bombLed);
  }

  if(millis() - bombSet > second && !pauseGame){
    bombTimer --;
    bombSet = millis();
    //Serial.println(bombTimer);
    if(bombTimer == 0){
      destroyWalls();
      bombTimer = setBombTimer;
      bombSet = 0;
      lc.setLed(0, bombX, bombY, false);
      bombX = -1;
      bombY = -1;
    }
  }
}

void setBomb(){ // add condition to set the bomb when button is pressed 
  int joystickPressed = digitalRead(joystickSW); // if is pressed => 0
  if(bombX == -1 && bombY == -1 && !joystickPressed){
      bombX = playerX;
      bombY = playerY;
      bombSet = millis();
  }
}

void destroyWalls()// should destroy maximum 4 walls on each bomb explosion
{
  int countWalls = 0;
  //stop on first wall on each side
  //left
  for (int i = bombY; i >= 0; i--){
    if(matrix[currentRoom][bombX][i]){
      matrix[currentRoom][bombX][i] = 0;
      countWalls++;
      break;
    }
    if(bombX == playerX && i == playerY){
      playerState = false;
      break;
    }
  }

  //right
  for (int i = bombY; i < numCols; i++){
    if(matrix[currentRoom][bombX][i]){
      matrix[currentRoom][bombX][i] = 0;
      countWalls++;
      break;
    }
    if(bombX == playerX && i == playerY){
      playerState = false;
      break;
    }
  }

  //up
  for (int i = bombX; i >= 0; i--){
    if(matrix[currentRoom][i][bombY]){
      matrix[currentRoom][i][bombY] = 0;
      countWalls++;
      break;
    }
    if(bombY == playerY && i == playerX){
      playerState = false;
      break;
    }
  }

  //down
  for (int i = bombX; i < numRows; i++){
    if(matrix[currentRoom][i][bombY]){
      matrix[currentRoom][i][bombY] = 0;
      countWalls++;
      break;
    }
    if(bombY == playerY && i == playerX){
      playerState = false;
      break;
    }
  }

  wallsDestroyed += countWalls;
}
void movePlayer()//on joystickMove
{
  xValue = analogRead(joystickX); // Read the X-axis value
  yValue = analogRead(joystickY); // Read the Y-axis value

  lastX = playerX;
  lastY = playerY;

  if (xValue < minThreshold && !joyMoved) {
    lastMove = millis();
    joyMoved = !joyMoved;
    if(playerX < numRows - 1 && !matrix[currentRoom][playerX+1][playerY])
      playerX = playerX + 1;
    else{
      if(currentRoom < lastRoom && bombX == -1 && bombY == -1){
        currentRoom++;
        
        lcd.setCursor(1, 0);
        lcd.print("Room ");
        lcd.print(currentRoom + 1);
        
        playerX = 0;
      }
      else
        playerX = playerX;
    }
    lc.setLed(0, lastX, lastY, false);
  } else if (xValue > maxThreshold && !joyMoved) {
    lastMove = millis();
    joyMoved = !joyMoved;
    if(playerX > 0 && !matrix[currentRoom][playerX-1][playerY])
      playerX = playerX - 1;
    else{
      if(currentRoom > firstRoom && bombX == -1 && bombY == -1){
        currentRoom --;

        lcd.setCursor(1, 0);
        lcd.print("Room ");
        lcd.print(currentRoom + 1);
        
        playerX = numCols - 1;
      }
      else
        playerX = playerX;
    }
    lc.setLed(0, lastX, lastY, false);
  }

  if (yValue < minThreshold && !joyMoved) {
    lastMove = millis();
    joyMoved = !joyMoved;
    playerY = (playerY > 0 && !matrix[currentRoom][playerX][playerY - 1]) ? playerY - 1 : playerY;
    lc.setLed(0, lastX, lastY, false);
  } else if (yValue > maxThreshold && !joyMoved) {
    lastMove = millis();
    joyMoved = !joyMoved;
    playerY = (playerY < numCols - 1 && !matrix[currentRoom][playerX][playerY + 1]) ? (playerY + 1)  : playerY;
    //Serial.println(playerY);
    lc.setLed(0, lastX, lastY, false);
  }


  if(millis()-lastMove > joystickDebounce)
    joyMoved = !joyMoved;
}
void generateRoom(int currentRoom){
  generateRandomWallPoints();
  roomGenerated[currentRoom] = true;
}

void generateRandomWallPoints() {
  for (int i = 0; i < numWallPoints; ++i) {
    // Generate random coordinates
    int x, y;
    x = random(numRows);
    y = random(numCols);
    if(matrix[currentRoom][x][y] == 1 || ( x == playerX && y == playerY )) // if the wall already exists or the coordinates corespond to player coordonates search another one
      i--;
    else
      matrix[currentRoom][x][y] = 1; // if wasn't set the wall
  }
}

void generateRandomPlayerPosition() {
  playerX = random(numRows);
  playerY = random(numCols);
}

void resetAll(){ //reset all non-constant variables
  startButtonPressed = false;
  gameState = false;  
  pauseGame = false;

  lastPress = 0;
  lastMove = 0;
  joyMoved = false;

  playerState = true; 
  openGame = true;
  startGame = false;
  endGame = false; 
  winner = false;

  bombX = -1;
  bombY = -1;

  bombTimer = 5; 
  bombSet = 0;
  lastBombBlink = 0; 
  bombLed = true;

  playerX = 0;
  playerY = 0;

  lastPlayerBlink = 0; 
  playerLed = true;

  currentRoom = 0;

  wallsRemain = 35;
  for(int k=0; k < numRooms; k++){
    for(int i = 0; i < numRows; i++)
      for(int j = 0; j < numCols; j++)
      matrix[k][i][j] = 0;
    roomGenerated[k] = 0;
  }

  firstChoice = -1;
  secondChoice = -1;

  wallsRemain = 32;
  wallsDestroyed = 0;

  endGameTimer = 0;
  startTimer = 0;
  timer = 0;
  lastText = 0;

  lcd.clear();
}