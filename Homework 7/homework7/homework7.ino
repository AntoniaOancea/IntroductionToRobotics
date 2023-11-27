#include "LedControl.h" 
//MAX7219 connection
const byte dinPin = 12; // pin 12 is connected to the MAX7219 pin 1
const byte clockPin = 11; // pin 11 is connected to the CLK pin 13
const byte loadPin = 10; // pin 10 is connected to LOAD pin 12
const byte matrixSize = 8; // 1 as we are only using 1 MAX7219
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); //DIN, CLK, LOAD, No. DRIVER
byte matrixBrightness = 2;

// start and reset button
const int startButton = 3;
const int resetButton = 4;

bool startButtonPressed = false;
bool gameState = false; // true - game start, false - game reset 
bool pauseGame = false;

int startValue;
int resetValue;

const int buttonDebounce = 50;
long long lastPress = 0;

// joystick pins
const int joystickX = A0;
const int joystickY = A1;
const int joystickSW = 2;

const int minThreshold = 300;
const int maxThreshold = 700;
const int joystickDebounce = 500; 
long long lastMove = 0;
bool joyMoved = false;

//auxialiars for joystick movement
//last position for player
int lastX;
int lastY;
//read values from joystick
int xValue;
int yValue;

// game state
bool playerState = true; // it will change if the bomb will kill the player , if it's false => game over
bool openGame = true; // show first animation (smiley face)
bool startGame = false; // first 3 seconds show arrow animation, after start the game
bool endGame = false; // set end game when player will won/lose
bool winner = false;// set when the game end
                    // no walls on the matrix and playerState is true => player won
                    // playerState false => player lost the game
//bomb coordonates
int bombX = -1;
int bombY = -1;

const int setBombTimer = 5;
int bombTimer = 5; 
const int second = 1000;
long long bombSet = 0;

const int bombBlink = 100;
long long lastBombBlink = 0; 
bool bombLed = true;

// player coordonates
int playerX = 0;
int playerY = 0;

const int playerBlink = 100;
long long lastPlayerBlink = 0; 
bool playerLed = true;

// Wall points
const int numWallPoints = 35;
int wallsRemain = 35;

// matrix 
const int numCols = 8;
const int numRows = 8;
bool matrix[numCols][numRows] = {
  // 0 1 2 3 4 5 6 7
    {0,0,0,0,0,0,0,0}, //0
    {0,0,0,0,0,0,0,0}, //1
    {0,0,0,0,0,0,0,0}, //2
    {0,0,0,0,0,0,0,0}, //3
    {0,0,0,0,0,0,0,0}, //4
    {0,0,0,0,0,0,0,0}, //5
    {0,0,0,0,0,0,0,0}, //6
    {0,0,0,0,0,0,0,0}  //7
};

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

void setup() {
  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, matrixBrightness); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen

  // initialize random seed
  randomSeed(analogRead(0));
  generateRandomWallPoints(); // random walls
  generateRandomPlayerPosition(); // random player

  //buttons
  pinMode(startButton,INPUT_PULLUP);
  pinMode(resetButton,INPUT_PULLUP);

  //joystick
  pinMode(joystickSW, INPUT_PULLUP);//buton
  pinMode(joystickX, INPUT_PULLUP);//Ox
  pinMode(joystickY, INPUT_PULLUP);//Oy
  

  Serial.begin(9600);
}

void loop() {
  showMatrix(); 
}

void showMatrix(){
  startValue = digitalRead(startButton);
  resetValue = digitalRead(resetButton);

  if(!resetValue){
    resetAll();
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
      Serial.println("Game Over!");
      for (int row = 0; row < 8; row++) {
        lc.setRow(0, row, matrixSad[row]);
      }
      return;
    }
  
  if(wallsRemain == 0 && playerState) // if no wall remain and the player is alive 
    {  
      Serial.println("Winner!");
      for (int row = 0; row < 8; row++) {
        lc.setRow(0, row, matrixWinner[row]);
      }
      return;
    }

  //show walls
  wallsRemain = 0; 
  for(int i = 0; i < numRows; i++)
    for(int j = 0; j < numCols; j++){
      if(matrix[i][j] == 1) wallsRemain ++;
      lc.setLed(0, i, j, matrix[i][j]);
    }
  showPlayer();

  if(!startValue && gameState && (millis() - lastPress > buttonDebounce || !lastPress)){// pause game
    pauseGame = !pauseGame;
    lastPress = millis();
  }
  
  if(pauseGame){
    showBomb();
    return;
  }

  movePlayer();

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
    Serial.println(bombTimer);
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
  //stop on first wall on each side
  //left
  for (int i = bombY; i >= 0; i--){
    if(matrix[bombX][i]){
      matrix[bombX][i] = 0;
      break;
    }
    if(bombX == playerX && i == playerY){
      playerState = false;
      break;
    }
  }

  //right
  for (int i = bombY; i < numCols; i++){
    if(matrix[bombX][i]){
      matrix[bombX][i] = 0;
      break;
    }
    if(bombX == playerX && i == playerY){
      playerState = false;
      break;
    }
  }

  //up
  for (int i = bombX; i >= 0; i--){
    if(matrix[i][bombY]){
      matrix[i][bombY] = 0;
      break;
    }
    if(bombY == playerY && i == playerX){
      playerState = false;
      break;
    }
  }

  //down
  for (int i = bombX; i < numRows; i++){
    if(matrix[i][bombY]){
      matrix[i][bombY] = 0;
      break;
    }
    if(bombY == playerY && i == playerX){
      playerState = false;
      break;
    }
  }
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
    playerX = (playerX < numRows - 1 && !matrix[playerX+1][playerY]) ? playerX + 1 : playerX;
    lc.setLed(0, lastX, lastY, false);
  } else if (xValue > maxThreshold && !joyMoved) {
    lastMove = millis();
    joyMoved = !joyMoved;
    playerX = (playerX > 0 && !matrix[playerX-1][playerY]) ? playerX - 1 : playerX;
    lc.setLed(0, lastX, lastY, false);
  }

  if (yValue < minThreshold && !joyMoved) {
    lastMove = millis();
    joyMoved = !joyMoved;
    playerY = (playerY > 0 && !matrix[playerX][playerY - 1]) ? playerY - 1 : playerY;
    lc.setLed(0, lastX, lastY, false);
  } else if (yValue > maxThreshold && !joyMoved) {
    lastMove = millis();
    joyMoved = !joyMoved;
    playerY = (playerY < numCols - 1 && !matrix[playerX][playerY + 1]) ? (playerY + 1)  : playerY;
    Serial.println(playerY);
    lc.setLed(0, lastX, lastY, false);
  }


  if(millis()-lastMove > joystickDebounce)
    joyMoved = !joyMoved;
}


void generateRandomWallPoints() {
  for (int i = 0; i < numWallPoints; ++i) {
    // Generate random coordinates
    int x, y;
    x = random(numRows);
    y = random(numCols);
    if(matrix[x][y] == 1) // if the wall already exists search another one
      i--;
    else
      matrix[x][y] = 1; // if wasn't set the wall
  }
}

void generateRandomPlayerPosition() {
  bool overlap = false; 
  do{
    overlap = false;
    playerX = random(numRows);
    playerY = random(numCols);
    if(matrix[playerX][playerY] == 1)
      overlap = true;
  } while (overlap);
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

  wallsRemain = 35;

  for(int i = 0; i < numRows; i++)
    for(int j = 0; j < numCols; j++)
      matrix[i][j] = 0;

  generateRandomWallPoints(); 
  generateRandomPlayerPosition(); 
}