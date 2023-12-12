// here will be functions for the menu 
#include<EEPROM.h>
#include "LiquidCrystal.h"
const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int LCDBrightness = 6;

int currentRoom = 0;

//MAX7219 connection
const byte dinPin = 12; // pin 12 is connected to the MAX7219 pin 1
const byte clockPin = 11; // pin 11 is connected to the CLK pin 13
const byte loadPin = 10; // pin 10 is connected to LOAD pin 12
const byte matrixSize = 8; // 1 as we are only using 1 MAX7219
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); //DIN, CLK, LOAD, No. DRIVER

// start and reset button
const int startButton = 3;

bool startButtonPressed = false;
bool gameState = false; // true - game start, false - game reset 
bool pauseGame = false;

int startValue;
int resetValue;

const int buttonDebounce = 400;
long long lastPress = 0;


// LED pins
const int blueLedPin = A2;
const int greenLedPin = A3;
const int redLedPin = A4;

int blueLedValue;
int greenLedValue;
int redLedValue;

// buzzer pin
const int buzzPin = 13;

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

long long endGameTimer = 0;

const int setBombTimer = 5;
int bombTimer = 5; 
const int second = 1000;
long long bombSet = 0;

long long startTimer = 0;
long long timer = 0;

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
const int numWallPoints = 4;
const int totalWalls = 16;
int wallsRemain = 16;
int wallsDestroyed = 0;

// matrix 
const int numCols = 8;
const int numRows = 8;
const int numRooms = 4;
const int firstRoom = 0;
const int lastRoom = 3;

// joystick pins
const int joystickX = A0;
const int joystickY = A1;
const int joystickSW = 2;

const int minThreshold = 300;
const int maxThreshold = 700;
const int joystickDebounce = 500; 
long long lastMove = 0;
bool joyMoved = false;

long long lastJoystickPressed = 0;


//sign for scrolling trough menu
const byte scroll[] = {
  B00100,
  B01110,
  B11111,
  B00000,
  B00000,
  B11111,
  B01110,
  B00100
};

//left parenthesis
const byte leftParenthesis[] = {
  B00001,
  B00010,
  B00100,
  B01000,
  B00100,
  B00010,
  B00001,
  B00000
};

//right parenthesis
const byte rightParenthesis[] = {
  B01000,
  B00100,
  B00010,
  B00001,
  B00010,
  B00100,
  B01000,
  B00000
};

//hourglass for time
const byte time[] = {
  B11111,
  B11111,
  B01110,
  B00100,
  B00100,
  B01110,
  B11111,
  B11111
};

//walls 
const byte walls[] = {
  B00000,
  B00000,
  B00000,
  B10101,
  B11111,
  B11111,
  B00000,
  B00000
};

//bright icon
const byte bright[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

int firstChoice = -1;
int secondChoice = -1;

const String principalMenuItems[3] = {
  "Start Game",
  "Settings",
  "About"
};

const int startIndex = 0;
const int settingsIndex = 1;
const int aboutIndex = 2;

long long lastText = 0;

const byte principalMenuIcon[3][8] = {
  {
    B10000,
    B11000,
    B11100,
    B11110,
    B11110,
    B11100,
    B11000,
    B10000
  },
  {
    B10001,
    B01110,
    B10001,
    B01110,
    B01010,
    B01110,
    B10001,
    B01110
  },
  {
    B01110,
    B10001,
    B10101,
    B10101,
    B10001,
    B10101,
    B10001,
    B01110
  }
};

const String settings[] = {
  "LCD bright",
  "Matrix bright",
};
int brightMatrix;
int brightLCD;

int brightness[2] = {
  brightLCD,brightMatrix
};

int currentItem = 0;

int currentSetting = 0;

const String aboutText = "Game name: BomberMan | Made by Antonia Oancea | GitHub : github.com/AntoniaOancea";
void moveMenu()
{
  xValue = analogRead(joystickX); // Read the X-axis value
  yValue = analogRead(joystickY); // Read the Y-axis value

  lastX = playerX;
  lastY = playerY;

  if (xValue < minThreshold && !joyMoved) {
    lastMove = millis();
    joyMoved = !joyMoved;
    currentItem = (currentItem + 1) % 3;
    lcd.clear();
  }
  else if (xValue > maxThreshold && !joyMoved) {
    lastMove = millis();
    joyMoved = !joyMoved;
    currentItem = currentItem ? (currentItem - 1) : 2;
    lcd.clear();
  }

  if(millis()-lastMove > joystickDebounce)
    joyMoved = !joyMoved;
}

void moveSetting()
{
  xValue = analogRead(joystickX); // Read the X-axis value
  yValue = analogRead(joystickY); // Read the Y-axis value

  lastX = playerX;
  lastY = playerY;

  if (xValue < minThreshold && !joyMoved) {
    lastMove = millis();
    joyMoved = !joyMoved;
    currentSetting = (currentSetting + 1) % 2;
    lcd.clear();
  }
  else if (xValue > maxThreshold && !joyMoved) {
    lastMove = millis();
    joyMoved = !joyMoved;
    currentSetting = currentSetting ? (currentSetting - 1) : 1;
    lcd.clear();
  }

  if (yValue > maxThreshold && !joyMoved) {
    lastMove = millis();
    joyMoved = !joyMoved;
    brightness[currentSetting] = brightness[currentSetting] < 2 ? (brightness[currentSetting] + 1) :  2;
    
    //update brightness with new values and save it in EEPROM
    EEPROM.update(currentSetting, brightness[currentSetting]);
    lc.shutdown(0, false); // turn off power saving, enables display
    lc.setIntensity(0, (brightness[1]+1)*5); // sets brightness (0~15 possible values)
    lc.clearDisplay(0);// clear screen
    analogWrite(LCDBrightness, (brightness[0]+1)*50); // sets brightness (0~15 possible values)
  }
  else if (yValue < minThreshold && !joyMoved) {
    lastMove = millis();
    joyMoved = !joyMoved;
    brightness[currentSetting] = brightness[currentSetting] ? (brightness[currentSetting] - 1) : brightness[currentSetting];
    //update brightness with new values and save it in EEPROM
    EEPROM.update(currentSetting, brightness[currentSetting]);
    lc.shutdown(0, false); // turn off power saving, enables display
    lc.setIntensity(0, (brightness[1]+1)*5); // sets brightness (0~15 possible values)
    lc.clearDisplay(0);// clear screen
    analogWrite(LCDBrightness, (brightness[0]+1)*50); // sets brightness (0~15 possible values)
  }

  if(millis()-lastMove > joystickDebounce)
    joyMoved = !joyMoved;
}

void selectItem(){
  int joystickPressed = digitalRead(joystickSW); // if is pressed => 0
  if(!joystickPressed && millis() - lastJoystickPressed > joystickDebounce){
      if(currentItem == startIndex) { // the game starts
        startButtonPressed = true;
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Room ");
        lcd.print(currentRoom + 1);
      }
      else
        firstChoice = currentItem; // enter submenu
      lastJoystickPressed = millis();
      lcd.clear();
  }
}

void settingsMenu(){ // move trough settings submenu and update values on joystick move
  int joystickPressed = digitalRead(joystickSW); // if is pressed => 0
  if(!joystickPressed && millis() - lastJoystickPressed > joystickDebounce){
    firstChoice = -1;
    lastJoystickPressed = millis();
    lcd.clear();
    return;
  }
  lcd.setCursor(0, 0);
  lcd.write(1);
  lcd.print(settings[currentSetting]);
  lcd.setCursor(5,1);
  lcd.print("-");
  for (int i = 0; i < 3; i++)
  {
    if(brightness[currentSetting] >= i)
      lcd.write(1);
    else
      lcd.print(" ");
  }
  lcd.print("+");

  moveSetting();
}

void welcome()
{
  lcd.setCursor(0, 0);
  lcd.print("Welcome to");

  
  lcd.setCursor(0, 1);
  lcd.print("BomberMan!");
}

//create custom character 
void createCustomCharacter(){
  for(int i = 0; i < 3; i++)
    lcd.createChar(i, principalMenuIcon[i]);
  //lcd.createChar(3, leftParenthesis);
  //lcd.createChar(4, rightParenthesis);
  lcd.createChar(5, scroll);
  lcd.createChar(6, time);
  lcd.createChar(7, walls);
}

void secondaryMenu(){ 
  if(firstChoice == aboutIndex){
    for (int i = 0; i < aboutText.length(); i++) {
      int joystickPressed = digitalRead(joystickSW); // if is pressed => 0 => go back to principal menu
      if(!joystickPressed && millis() - lastJoystickPressed > joystickDebounce){//scrolling the text
        firstChoice = -1;
        lastJoystickPressed = millis();
        lcd.clear();
        return;
      }

      if(millis() - lastText > second/2 && lastText){ 
        lastText = millis();
        lcd.clear();
        continue;
      }

      lcd.setCursor(0,0);
      lcd.print("<ABOUT>");
      lcd.setCursor(0, 1);
      lcd.print(aboutText.substring(i, i + 16));
      
      if(!lastText) 
        lastText = millis();

      i--;
    }
  }
  else
    if(firstChoice == settingsIndex){
      settingsMenu();
    }
}

void principalMenu()
{
  
  if(firstChoice != -1){ // pass principal menu if you are in a submenu
    secondaryMenu();
    return;
  }

  brightness[0] = brightLCD;
  brightness[1]=brightMatrix;

  lcd.setCursor(0,0);
  lcd.print("<MENU>");
  lcd.setCursor(15,0);
  lcd.write(5);

  //show current item on menu
  lcd.setCursor(0,1);
  lcd.write(currentItem);
  lcd.print(principalMenuItems[currentItem]);

  if(!startButtonPressed)
    moveMenu();
  
  selectItem();
}


