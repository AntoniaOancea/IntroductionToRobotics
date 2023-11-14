// Define connections to the shift register
const int latchPin = 11; // Connects to STCP (latch pin) on the shift register
const int clockPin = 10; // Connects to SHCP (clock pin) on the shift register
const int dataPin = 12; // Connects to DS (data pin) on the shift register

// Define connections to the digit control pins for a 4-digit display
const int segD1 = 4;
const int segD2 = 5;
const int segD3 = 6;
const int segD4 = 7;

// Define buttons pin
const int startStopButton = 2;
const int lapButton = 3;
const int resetButton = 8;


// Store the digits in an array for easy access
int displayDigits[] = {segD1, segD2, segD3, segD4};
const int displayCount = 4; // Number of digits in the display

// Define the number of unique encodings (0-9, A-F for hexadecimal)
const int encodingsNumber = 16;
// Define byte encodings for the hexadecimal characters 0-F
byte byteEncodings[encodingsNumber] = {
//A B C D E F G DP 
  B11111100, // 0 
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110, // 9
};
byte encoding = B00000000; 
const int indexDP = 7;
const int thirdDigitIndex = 2;
// Variables for controlling the display update timing
unsigned long lastIncrement = 0;
unsigned long delayCount = 500; // Delay between updates (milliseconds)
unsigned long number = 0; // The number being displayed


const int numberOfLaps = 4;
int laps[numberOfLaps] = {0,0,0,0}; 
int currentNumberOfLaps = 0;
int currentLapShow = -1;

bool lapShow = false;
int startOrStop = 0; 
int startState = 0;
int resetState = 0;
int lapState = 0;
long long lastResetButtonDebounce = 0;
long long lastStartButtonDebounce = 0;
long long lastLapButtonDebounce = 0;
int delayButton = 1000;
long long lapDebounce = 0;


void setup() {
  // Initialize the pins connected to the shift register as outputs
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(startStopButton,INPUT_PULLUP);
  pinMode(resetButton,INPUT_PULLUP);
  pinMode(lapButton,INPUT_PULLUP);
  // Initialize digit control pins and set them to LOW (off)
  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }  
  // Begin serial communication for debugging purposes
  Serial.begin(9600);
}

void loop() {
  //If the start button was pressed
  if (millis() - lastIncrement > 100 && startOrStop) {
  	number++;
    number = number % 10000;
    lastIncrement = millis();
  }
  
  //start button
  startState = digitalRead(startStopButton);
  if(!startState && (millis() - lastStartButtonDebounce > delayButton || !lastStartButtonDebounce)){
    lastStartButtonDebounce = millis();
    startOrStop = !startOrStop;//switch at each button press
  }

  
  //reset button
  resetState = digitalRead(resetButton);
  if(!resetState && !startOrStop && (millis() - lastResetButtonDebounce > delayButton || !lastResetButtonDebounce)){
     lastResetButtonDebounce = millis();
     number = 0;
  }

  //lap button
  lapState = digitalRead(lapButton);
  
  //If lap button was pressed during pause, show saving laps, else show current number
  if(lapShow){
    writeNumber(laps[currentLapShow]);
    if(millis() - lapDebounce > delayButton || !lapDebounce){
      currentLapShow ++;
      //Serial.println(laps[currentLapShow]);
      lapDebounce = millis();
    }
    if(currentLapShow == numberOfLaps-1){
      lapShow = false;
      currentLapShow = -1;
    }
  }
  else
    writeNumber(number);

  //If the lap button was pressed(not in pause state) save the current number 
  if(!lapState && (millis() - lastLapButtonDebounce > delayButton || !lastLapButtonDebounce) && !lapShow){
    if(startOrStop){
      lastLapButtonDebounce = millis();
      saveLap(number);
    }
    else
    {
      lastLapButtonDebounce = millis();
      lapShow = true;
    }
  }
  
} 

// Add current number to array of saving laps
void saveLap(int number){
  laps[currentNumberOfLaps] = number;
  currentNumberOfLaps ++;
  currentNumberOfLaps = currentNumberOfLaps % numberOfLaps; // if(currentNumberOfLaps > 4) REPLACE OLD VALUES
  Serial.println(number);
}


void writeReg(int digit) {
    // Prepare to shift data by setting the latch pin low
    digitalWrite(latchPin, LOW);
    // Shift out the byte representing the current digit to the shift register
    shiftOut(dataPin, clockPin, MSBFIRST, digit);
    // Latch the data onto the output pins by setting the latch pin high
    digitalWrite(latchPin, HIGH);
}

void activateDisplay(int displayNumber) {
  // Turn off all digit control pins to avoid ghosting
  for (int i = 0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }
  // Turn on the current digit control pin
  digitalWrite(displayDigits[displayNumber], LOW);
}

void writeNumber(int number) {
  int currentNumber = number;
  int displayDigit = displayCount - 1;
  
  for (int i=0; i < displayCount; i++) {
  	activateDisplay(displayDigit); //CURRENT DIGIT
    encoding = byteEncodings[currentNumber%10];
    if (displayDigit == thirdDigitIndex) {
      encoding = encoding ^ B00000001;  // Set the DP bit to 1 (XOR)
    }
    writeReg(encoding); //SET LEDS OFF
    displayDigit--;
    currentNumber = currentNumber/10;
    writeReg(B00000000);
  }
}