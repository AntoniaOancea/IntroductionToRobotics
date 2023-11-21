#include<EEPROM.h>


int firstChoice = 0;
int secondChoice = 0;

int subMenu1Choice = 0;
int subMenu2Choice = 0;
int subMenu3Choice = 0;
int subMenu4Choice = 0;

bool showPrincipalMenu = false;
bool showSecundaryMenu = false;

//RGB-led pins
const int redLedPin = 13;
const int greenLedPin = 12;
const int blueLedPin = 11;

bool redLedState = false;
bool greenLedState = false;
bool blueLedState = false;


//photocell
const int photocellPin = A0; 
int photocellValue;
int ldrThreshold;

//ultrasonic sensor pins
const int trigPin = 9;
const int echoPin = 10;
int distance;
int duration;
int ultrasonicThreshold;

//debounce for sampling
int samplingInterval = -1;
long long lastDebounce = 0;

//delay for print menu
const int delayMenu = 5000;
long long lastMenu = 0; 

bool automaticMode = false;

//delay for option
const int waitValue = 8000;
long long lastWaitValue = 0;

void setup(void) {
  pinMode(redLedPin,OUTPUT);
  pinMode(greenLedPin,OUTPUT);
  pinMode(blueLedPin,OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  samplingInterval = EEPROM.read(0);
  ultrasonicThreshold = EEPROM.read(1);
  ldrThreshold = EEPROM.read(2);

  Serial.begin(9600); 
}
 
void loop(void) {
  ledColor();
  principalMenu();
}

void ledColor(){
  if(automaticMode)
  {
    digitalWrite(redLedPin,false);
    digitalWrite(greenLedPin,true);
    digitalWrite(blueLedPin,false);
  }
  else
  {
    digitalWrite(redLedPin,redLedState);
    digitalWrite(greenLedPin,greenLedState);
    digitalWrite(blueLedPin,blueLedState);
  }
}

void principalMenu()
{
  if(Serial.available() > 0 && firstChoice == 0)
    firstChoice = Serial.parseInt(); 
  
  switch (firstChoice) {
    case 0:
      break;
    case 1:
      subMenu1();
      break;
    case 2:
      subMenu2();
      break;
    case 3:
      subMenu3();
      break;
    case 4:
      subMenu4();
      break;
    default:
      Serial.println("Invalid choice. Please try again.");
      break;
  }
  if(firstChoice == 0 && (millis()-lastMenu > delayMenu || !lastMenu) ){ 
    lastMenu = millis();
    Serial.println("Main Menu:");
    Serial.println("1. Sensor Settings");
    Serial.println("2. Reset Logger Data"); 
    Serial.println("3. System Status");
    Serial.println("4. RGB LED Control");
    Serial.println("What is your choice?");
  }
}

void subMenu1()
{
  if((millis() - lastMenu > delayMenu || !lastMenu) && !secondChoice)
  {
    lastMenu = millis();
    Serial.println("1. Sensor Settings");
    Serial.println("  1. Sensors Sampling Interval");
    Serial.println("  2. Ultrasonic Alert Treshold");
    Serial.println("  3. LDR Alert Treshold");
    Serial.println("  4. Back");
  }

  if (Serial.available() > 0 || secondChoice) {
    lastMenu = 0;
    if(!secondChoice)
      secondChoice = Serial.parseInt();
    switch (secondChoice) {
      case 1:
        if(millis() - lastWaitValue > waitValue || !lastWaitValue)
        {
          lastWaitValue = millis();
          Serial.println("Enter new sampling interval (1-10 seconds): ");
        }
        if(Serial.available() > 0)
        {
          samplingInterval = Serial.parseInt();
          Serial.println("Sampling interval set to: " + String(samplingInterval) + " seconds");
          EEPROM.update(0, samplingInterval);
          secondChoice = 0;    
          lastMenu = 0;    
        }
        break;
      case 2:
        if(millis() - lastWaitValue > waitValue || !lastWaitValue)
        {
          lastWaitValue = millis();
          Serial.println("Enter new ultrasonic threshold: ");
        }
        if(Serial.available() > 0)
        {
          ultrasonicThreshold = Serial.parseInt();
          Serial.println("Ultrasonic threshold set to: " + String(ultrasonicThreshold));
          EEPROM.update(1, ultrasonicThreshold);
          secondChoice = 0;
          lastMenu = 0;
        }
        break;
      case 3:
        if(millis() - lastWaitValue > waitValue || !lastWaitValue)
        {
          lastWaitValue = millis();
          Serial.println("Enter new LDR threshold: ");
        }
        if(Serial.available() > 0)
        {
          ldrThreshold = Serial.parseInt();
          Serial.println("LDR threshold set to: " + String(ldrThreshold));
          EEPROM.update(2, ldrThreshold);          
          firstChoice = 0; // go back to principal menu
          secondChoice = 0;
          lastMenu = 0;
        }
        break;
      case 4:
        firstChoice = 0; // go back to principal menu
        secondChoice = 0;
        lastMenu = 0;
        break;
      default:
        Serial.println("Invalid choice. Please try again.");
        break;
    }
  }
}


void subMenu2()
{
  if(millis() - lastMenu > delayMenu || !lastMenu)
  {
    lastMenu = millis();
    Serial.println("2. Reset Logger Data");
    Serial.println("  Are you sure?");
    Serial.println("    1. Yes.");
    Serial.println("    2. No.");
  }

  if(Serial.available() > 0 && secondChoice == 0)
  {
    secondChoice = Serial.parseInt();
    switch(secondChoice){
      case 1:
        eraseAllData();
        Serial.println("All data was erased!");
        secondChoice = 0;
        firstChoice = 0;
        break;
      case 2:
        Serial.println("Data wasn't erased!");
        secondChoice = 0; // go back to principal menu
        firstChoice = 0;
        break;
      default:
        Serial.println("Invalid choice. Please try again.");
        break;
    }
  }
}

void subMenu3()
{
  if(millis() - lastMenu > delayMenu || !lastMenu)
  {
    lastMenu = millis();
    Serial.println("3. System Status");
    Serial.println("  1. Current Sensor Readings");
    Serial.println("  2. Current Sensor Settings");
    Serial.println("  3. Display Logged Data");
    Serial.println("  4. Back");
  }

  if (Serial.available() > 0) {
    secondChoice = Serial.parseInt();
    switch (secondChoice) {
      case 1:
        photocellValue = analogRead(photocellPin);
        Serial.print("Photocell Value: ");
        Serial.println(photocellValue);

        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
       
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
        
        duration = pulseIn(echoPin, HIGH);
        distance = duration*0.034/2;
        
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.println(" cm");
        break;
      case 2:
        Serial.println("Current Sensor Settings:");
        Serial.println("  Sampling Interval: " + String(samplingInterval) + " seconds");
        Serial.println("  Ultrasonic Threshold: " + String(ultrasonicThreshold));
        Serial.println("  LDR Threshold: " + String(ldrThreshold));
        break;
      case 3:
        Serial.println("Current Sensor Settings:");
        Serial.println("  Sampling Interval: " + String(samplingInterval) + " seconds");
        Serial.println("  Ultrasonic Threshold: " + String(ultrasonicThreshold));
        Serial.println("  LDR Threshold: " + String(ldrThreshold));
        break;
      case 4:
        firstChoice = 0;
        secondChoice = 0;
        break;
      default:
        Serial.println("Invalid choice. Please try again.");
        break;
    }
  }
}

void subMenu4() {
  if (millis() - lastMenu > delayMenu || !lastMenu) {
    lastMenu = millis();
    Serial.println("4. RGB LED Control");
    Serial.println("  1. Manual Color Control");
    Serial.println("  2. LED: Toggle Automatic ON/OFF");
    Serial.println("  3. Back");
  }

  if (Serial.available() > 0) {
    if(secondChoice == 0)
      secondChoice = Serial.parseInt();
    switch (secondChoice) {
      case 1:
        setColor();
        break;
      case 2:
        automaticMode = !automaticMode; // Toggle automatic mode
        Serial.println("Automatic mode is now " + String(automaticMode ? "ON" : "OFF"));
        firstChoice = 0; // go back to principal menu
        secondChoice = 0;
        break;
      case 3:
        firstChoice = 0; // go back to principal menu
        secondChoice = 0;
        break;
      default:
        Serial.println("Invalid choice. Please try again.");
        break;
    }
  }
}

void setColor(){
  Serial.println("1. Red");
  Serial.println("2. Green");
  Serial.println("3. Blue");
  Serial.println("4. Back");
  if(Serial.available() > 0)
  {
    int colorChoice = Serial.parseInt();

    switch (colorChoice) {
      case 1:
        redLedState = true;
        greenLedState = false;
        blueLedState = false;
        break;
      case 2:
        redLedState = false;
        greenLedState = true;
        blueLedState = false; 
        break;
      case 3:
        redLedState = false;
        greenLedState = false;
        blueLedState = true;
        break;
      case 4:
        redLedState = false;
        greenLedState = false;
        blueLedState = false;
        firstChoice = 0;
        secondChoice = 0;
        break;
      default:
        Serial.println("Invalid choice. Please try again.");
        break;
    }  
  }
}

void eraseAllData(){
  for (int i = 0; i < EEPROM.length(); i++) 
    EEPROM.write(i, 0xFF); // write 0xFF value on each byte
  samplingInterval = 0;
  ultrasonicThreshold = 0;
  ldrThreshold = 0;
}

