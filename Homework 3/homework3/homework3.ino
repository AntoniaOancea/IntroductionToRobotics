#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include "pitches.h"

QueueHandle_t calls;


const int buzzPin = 12;
const int ledPinMove = 13;

const int buttonPinLevels[] = {6, 5, 3};
const int ledPinLevels[] = {11, 10, 9};

int buttonStateLevels[] = {0, 0, 0}; 
int ledStateLevels[] = {1, 0, 0}; 

byte buttonStateMove = 0;

int requestedLevel = -1;
int requestedLevel2 = -1;
byte requestedLevelState = 0;
int currentLevel = 0;

const byte ledON = 1;
const byte ledOFF = 0;

long long lastDebounceTime = 0;
long long lastDebounceTime2 = 0;
int debounceDelay = 100;

int toneDurationBetweenLevels = 0;
int toneDurationClose = 0;
int toneDurationOpen = 0;

long long lastTimeMove = 0;
int moveDelay = 3000;
int currentLevelDelay = 1000;
int closeDoor = 2000;
long long call = 0;

void setup() {

  pinMode(buttonPinLevels[0], INPUT_PULLUP);
  pinMode(buttonPinLevels[1], INPUT_PULLUP);
  pinMode(buttonPinLevels[2], INPUT_PULLUP);

  pinMode(ledPinLevels[0], OUTPUT);
  pinMode(ledPinLevels[1], OUTPUT);
  pinMode(ledPinLevels[2], OUTPUT);
  pinMode(ledPinMove, OUTPUT);
  
  Serial.begin(9600);

  calls = xQueueCreate(20, sizeof(int));
}
void loop() {
  buttonStateLevels[0] = digitalRead(buttonPinLevels[0]);
  buttonStateLevels[1] = digitalRead(buttonPinLevels[1]);
  buttonStateLevels[2] = digitalRead(buttonPinLevels[2]);
  Serial.println(buttonStateMove);
  
  digitalWrite(ledPinLevels[0],ledStateLevels[0]);
  digitalWrite(ledPinLevels[1],ledStateLevels[1]);
  digitalWrite(ledPinLevels[2],ledStateLevels[2]);

  if(millis() - lastDebounceTime2 > debounceDelay || !lastDebounceTime2){
    lastDebounceTime2 = millis();
    for(int i=0;i<3;i++)
    {
      if(!buttonStateLevels[i] && i != requestedLevel)
      {
        if (xQueueSend(calls, &i, portMAX_DELAY) == pdPASS) {
            Serial.println("Pushed data to the queue");
        }
      }
    }
  }

  if(!ledStateLevels[0] && !ledStateLevels[1] && !ledStateLevels[2] && requestedLevel != -1) 
  {
      if(millis() - lastDebounceTime > debounceDelay || !lastDebounceTime)
      {
        lastDebounceTime = millis();
        buttonStateMove = !buttonStateMove;
        digitalWrite(ledPinMove, buttonStateMove);
        if(!toneDurationBetweenLevels)
        {
            toneDurationBetweenLevels = millis();
            tone(buzzPin, NOTE_E6, moveDelay);
        }
      }
      else
      {
        buttonStateMove = ledON;
        digitalWrite(ledPinMove, buttonStateMove);
      }
  }
  if(requestedLevel == -1)// verificam daca e chemat liftul
  {
    buttonStateMove = ledOFF;
    digitalWrite(ledPinMove, buttonStateMove);
    for(int i = 0; i < 3; i++)
      if(!buttonStateLevels[i])
      {
        requestedLevel = i;
        call = millis();
        tone(buzzPin, NOTE_DS2, closeDoor);
        break;
      }
  }
  
  if(millis() - call > closeDoor){ // delay pentru inchidere usa
    if(millis() - lastTimeMove > moveDelay || !lastTimeMove) // delay pentru timpul de trecere de la un etaj la altul
    {
      ledStateLevels[currentLevel] = ledON;
    }
    if(requestedLevel!=-1 && (millis() - lastTimeMove > moveDelay +  currentLevelDelay || !lastTimeMove)) // daca e chemat si a trecut timpul de asteptare dintre 2 nivele
    {
        if(currentLevel < requestedLevel)//urca
        {
          ledStateLevels[currentLevel] = ledOFF;
          digitalWrite(ledPinMove, ledOFF);
          currentLevel ++;
          toneDurationBetweenLevels = 0;
          lastTimeMove = millis();
        }
        else//coboara
          if(currentLevel > requestedLevel)
          {
            ledStateLevels[currentLevel] = ledOFF;
            digitalWrite(ledPinMove, ledOFF);
            currentLevel --;
            toneDurationBetweenLevels = 0;
            lastTimeMove = millis();
          }
        else//a ajuns
        {
          if(!toneDurationOpen && millis() - toneDurationClose > closeDoor)
          {
            toneDurationClose = 0;
            toneDurationOpen = millis();
            tone(buzzPin, NOTE_DS8, closeDoor);
          }
          
          lastTimeMove = 0;
          if(millis() - toneDurationOpen > closeDoor) //
          {
            toneDurationClose = millis();
            toneDurationOpen = 0;
            
            if (uxQueueMessagesWaiting(calls) != 0 && xQueueReceive(calls, &requestedLevel2, portMAX_DELAY) == pdPASS) // luam din coada urmatorul nivel cerut(daca exista)
                {
                  Serial.print("Popped data from the queue: ");
                  if(requestedLevel != requestedLevel2) //daca din coada preluam etajul curent trecem peste
                  {
                    requestedLevel = requestedLevel2;
                    tone(buzzPin, NOTE_DS2, closeDoor);
                  }
                }
            else
              requestedLevel = -1;
          }
          
          call = millis();
          Serial.println(requestedLevel);
          buttonStateMove = ledOFF;
          digitalWrite(ledPinMove, buttonStateMove);
        }
    }
  }
}
