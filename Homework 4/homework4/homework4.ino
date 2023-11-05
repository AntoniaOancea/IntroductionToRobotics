
// Declare all the joystick pins
const int pinSW = 2; // Digital pin connected to switch output
const int pinX = A0; // A0 - Analog pin connected to X output
const int pinY = A1; // A1 - Analog pin connected to Y output

// Declare all the segments pins
const int pinA = 12;
const int pinB = 10;
const int pinC = 9;
const int pinD = 8;
const int pinE = 7;
const int pinF = 6;
const int pinG = 5;
const int pinDP = 4;

const int segSize = 8;
int index = 7;

byte swState = LOW;
int xValue = 0;
int yValue = 0;

bool joyMoved = false;
int minThreshold = 300;
int maxThreshold = 800;

int segments[segSize] = { 
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

int pinLeds[] = { 0, 0, 0, 0, 0, 0, 0, 0};

const int directions[] = {
  0, //up
  1, //down
  2, //left
  3  //right
};

const int noOfDirections = 4;
int indexDirection = -1;

byte currentSegm = LOW;

long long lastDebounce = 0;
const int delayDebounce = 50;

long long buttonDebounce = 0;
const int resetTime = 3000;

byte buttonPressed = false;

const int on = 1;
const int off = 0;

const int up = 0;
const int down = 1;
const int left = 2;
const int right = 3;
const int middle = -1;

//ledurile care ar trebui sa fie selectate in functie de pozitia curenta si directie
const int nextStep[noOfDirections][segSize] = {
  //      a     b     c     d     e     f     g     dp
//up      
          0,    0,    6,    6,    6,    0,    0,    7,    
//down
          6,    6,    3,    3,    3,    6,    3,    7,
//left
          5,    5,    4,    4,    4,    5,    6,    2,    
//right
          1,    1,    7,    2,    2,    1,    6,    7
};

//daca apesi 5 sec pe buton si ledul curent este dp sa se rezeteze 
void resetDisplay(){
  for (int i = 0; i < segSize; i++) {
    pinLeds[i] = off;
  }
  index = 7;
}

void setLeds(){
  for(int i = 0; i < segSize; i++)
    if(i != index){
    digitalWrite(segments[i], pinLeds[i]);
  }
}

void setup() {
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }

  pinMode(pinSW, INPUT_PULLUP);//buton
  pinMode(pinX, INPUT_PULLUP);//Ox
  pinMode(pinY, INPUT_PULLUP);//Oy

  Serial.begin(9600);
}

void loop() {
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  swState = digitalRead(pinSW);
  
  setLeds();

  if(xValue >= minThreshold && yValue >= minThreshold && xValue <= maxThreshold && yValue <= maxThreshold)
  {
    joyMoved = false;
    indexDirection = middle;
  }
  else
  {
    if(xValue < minThreshold) //left
      indexDirection = left;
    else{

      if(xValue > maxThreshold) //right
        indexDirection = right;
      else{

        if(yValue < minThreshold) //down
          indexDirection = down;
        else{

          if(yValue > maxThreshold) //up
            indexDirection = up;
        }
      }
    }
  }

  if(!swState && indexDirection == middle)
  {
      if(!buttonDebounce)
        buttonDebounce = millis();
      else
        //daca butonul este apasat 5 sec si ledul curent este dp => reset
        if(millis() - buttonDebounce > resetTime)
          {
            resetDisplay();
            buttonDebounce = millis();
          }
        //daca trebuie aprins/stins ledul curent
        else
          {
            if(millis() - buttonDebounce > delayDebounce && !buttonPressed)
            {
              buttonPressed = on;
              pinLeds[index] = !pinLeds[index]; //schimbam starea ledului curent
              buttonDebounce = millis();
            }
          }
  }
  else
    {
      buttonDebounce = off;
      buttonPressed = off;
    }



  digitalWrite(segments[index],LOW);
  if(!joyMoved && indexDirection != middle)
  {
    joyMoved = on; 
    index = nextStep[indexDirection][index];
  }
  if(millis() - lastDebounce > delayDebounce || !lastDebounce )
  {
    lastDebounce = millis();
    currentSegm = !currentSegm;
    digitalWrite(segments[index],currentSegm);
  }


}
