const int redPotPin = A2;
const int greenPotPin = A1;
const int bluePotPin = A0;

const int redLedPin = 11;
const int greenLedPin = 10;
const int blueLedPin = 9;

int redValue = 0;
int greenValue = 0;
int blueValue = 0;

const int maxPotValue = 1023;
const int maxValue = 255;
const int minPotValue = 0;
const int minValue = 0;

void setup() {
  pinMode(redPotPin, INPUT);
  pinMode(greenPotPin, INPUT);
  pinMode(bluePotPin, INPUT);

  pinMode(redLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  redValue = analogRead(redPotPin);
  greenValue = analogRead(greenPotPin);
  blueValue = analogRead(bluePotPin);

  redValue = map(redValue, minPotValue, maxPotValue, minValue, maxValue);
  greenValue = map(greenValue, minPotValue, maxPotValue, minValue, maxValue);
  blueValue = map(blueValue, minPotValue, maxPotValue, minValue, maxValue);

  Serial.print("red = ");
  Serial.println(redValue);
  analogWrite(redLedPin, redValue);

  Serial.print("green = ");
  Serial.println(greenValue);
  analogWrite(greenLedPin, greenValue);

  Serial.print("blue = ");
  Serial.println(blueValue);
  analogWrite(blueLedPin, blueValue);
}
