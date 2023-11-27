# Introduction To Robotics

Hello! This repository is dedicated to my Introduction to Robotics course. Over the semester, I'll be uploading the assignments and projects I've been working on.

## Homework 1
<details>
  <summary>See details</summary>

  ### Create this repository.
</details>

## Homework 2
<details>
  <summary>See details</summary>

  ### This assignment focuses on controlling each channel (Red, Green, and Blue)of  an  RGB  LED  using  individual  potentiometers.
  ### Components: 
      RGB LED (at least 1)
      Potentiometers (at least 3)
      Resistors and wires as needed

  ### Task
      Use a separate potentiometer for controlling each color of the RGB LED:Red,Green, andBlue.  
      This control must leverage digital electronics.  
      Specifically,I  need  to  read  the  potentiometer’s  value  with  Arduino  and  then  write  a mapped value to the LED pins.

  ### Setup
  ![](https://github.com/AntoniaOancea/IntroductionToRobotics/blob/main/Homework%202/setup-2.jpg)

  ### [Video](https://youtu.be/AsdBe3tzsQs)

  ### [Code](https://github.com/AntoniaOancea/IntroductionToRobotics/blob/main/Homework%202/homework_2/homework_2.ino)

  </details>


## Homework 3
<details>
  <summary>See details</summary>

  ### This assignment involves simulating a 3-floor elevator control system using LEDs, buttons, and a buzzer with Arduino.
  ### Components: 
      • LEDs (At least 4: 3 for the floors and 1 for the elevator’s operational state)
      • Buttons (At least 3 for floor calls)
      • Buzzer (1)
      • Resistors and wires as needed

  ### Task
      Design a control system that simulates a 3-floor elevator using the Arduino platform. Here are the specific requirements:
      • LED Indicators: Each of the 3 LEDs should represent one of the 3 floors.
          The LED corresponding to the current floor should light up. Additionally,another LED should represent the elevator’s 
          operational state. It should blink when the elevator is moving and remain static when stationary.
      • Buttons: Implement 3 buttons that represent the call buttons from the 3 floors. When pressed, the elevator should simulate
      movement towards the floor after a short interval (2-3 seconds).
      • Buzzer : The buzzer should sound briefly during the following scenarios:
          – Elevator arriving at the desired floor (something resembling a ”cling”).
          – Elevator doors closing and movement (pro tip: split them into 2 different sounds)
      • State Change & Timers: If the elevator is already at the desired floor, pressing the button for that floor should have 
      no effect. Otherwise, after a button press, the elevator should ”wait for the doors to close” and then ”move” to the 
      corresponding floor. If the elevator is in movement, it should either do nothing or it should stack its decision 
      (get to the first programmed floor, open the doors, wait, close them and then go to the next desired floor).
      • Debounce: for the buttons to avoid unintentional repeated button presses

  ### Bonus:
  ##### Buzzer
  ##### Queue for elevator calls

  ### Setup
  ![](https://github.com/AntoniaOancea/IntroductionToRobotics/blob/main/Homework%203/homework3.jpg)

  ### [Video](https://www.youtube.com/watch?v=2bYAEGiis1o)

  ### [Code](https://github.com/AntoniaOancea/IntroductionToRobotics/blob/main/Homework%203/homework3/homework3.ino)

  </details>

  ## Homework 4
<details>
  <summary>See details</summary>

  ### This assignment involves using a joystick to control the position of the segment and set on/off the leds on the display.
  ### Components: 
      • 7-segment display
      • Joystick
      • Resistors and wires as needed

  ### Task
      • the initial position should be on the DP
      • the current position should blinck
      • the joystick control the movement from one segment to another
      • short press on the button should change the segment state from ON to OFF or from OFF to ON
      • long pressing the button resets the entire display by turning all the segments OFF and moving the current position to DP

  ### Table for corresponding movement
  ![](https://github.com/AntoniaOancea/IntroductionToRobotics/blob/main/Homework%204/image.png)

      
  ### Setup
  ![](https://github.com/AntoniaOancea/IntroductionToRobotics/blob/main/Homework%204/homework4.jpg)

  ### [Video](https://www.youtube.com/watch?v=t3oWBuS-0tk)

  ### [Code](https://github.com/AntoniaOancea/IntroductionToRobotics/blob/main/Homework%204/homework4/homework4.ino)

  </details>

  ## Homework 5
<details>
  <summary>See details</summary>

  ### This assignment involves using a 4 digits 7 segment display to implement a stopwatcher timer with a functionality for saving laps.
  ### Components: 
      • 4 digit 7-segment display
      • 3 buttons
      • Resistors and wires as needed

  ### Task
      • Implement 3 buttons: LAP, START/STOP, RESET 
      • Start with "000.0". Press START button  => the timer will start.
      • LAP button will save the current time if timer is not in pause mode, else it will display saved laps.
      • If the timer is in PAUSE mode, Reset button will restart timer showing "000.0".
 
  ### Setup
  ![](https://github.com/AntoniaOancea/IntroductionToRobotics/blob/main/Homework%205/homework5.jpg)

  ### [Video](https://www.youtube.com/watch?v=X44U0F4q8nE)

  ### [Code](https://github.com/AntoniaOancea/IntroductionToRobotics/blob/main/Homework%205/homework5/homework5.ino)

  </details>

   ## Homework 6
<details>
  <summary>See details</summary>

  ### This assignment involves using Serial Monitor as an Interctive Menu for controlling Sensor Readings and LED Colors.
  ### Components: 
      • Ultrasonic Sensor (HC-SR04)
      • LDR (Light-Dependent Resistor)
      • RGB LED
      • 3 buttons
      • Resistors and wires as needed

  ### Task
      Interactive Menu in Serial Monitor with the following options:
        1.Sensor Settings
          1.1. Sensor Sampling Interval => ask for a value between 1 and 10
          1.2. Ultrasonic Alert Treshold => ask for a value
          1.3. LDR Alert Treshold => ask for a value
        2.Reset Logger Data
          2.1. Yes => delete all data from EEPROM
          2.2. No => return to principal menu without any modification
        3.System Status
          3.1.Current Sensor Readings => show current sensor values
          3.2.Current Sensor Settings => show current sensor settings 
          3.3.Display Logged Data => show data from EEPROM
          3.4.Back => back to principal menu
        4.RGB LED Control
          4.1. Manual Color Control => set led color
            4.1.1.Red
            4.1.2.Green
            4.1.3.Blue
            4.1.4.Back
          4.2.LED: Toggle Automatic ON/OFF => if it's ON , LED will be green
          4.3.Back 
  ### Setup
  ![](https://github.com/AntoniaOancea/IntroductionToRobotics/blob/main/Homework%206/homework6.jpg)

  ### [Video](https://www.youtube.com/watch?v=eQnhL6g1S2o)

  ### [Code](https://github.com/AntoniaOancea/IntroductionToRobotics/blob/main/Homework%206/homework6/homework6.ino)

  </details>

  ## Homework 7
<details>
  <summary>See details</summary>

  ### This assignment involves using the 8x8 matrix to create a game. I made a game similar to bomberman.
  ### Components: 
      • 8x8 LED matrix
      • joystick
      • MAX7219
      • 2 buttons
      • Resistors and wires as needed

  ### Task
      The game should contain 3 types of object. I chose to use walls, player and bomb.
      The walls are the lights that are always on. The player and the bomb should blink, the bomb faster than the player.
      The walls and the player are randomly generated. The bomb is set by the player to break down the walls.
      The player should run away from the bomb's range after placing it on the map. 
      The bomb will explode 5 seconds after it is placed and will destroy the first wall on each direction (up, down, left, right).
      If the player is in range of the bomb when it explodes, he will lose the game.
      If the player manages to break down all the walls, he wins.
      The player sets the bomb and moves on the map with the joystick.
  
    Bonus:    
      I added 3 animations depending on the stage of the game. A smiley face before the game starts, a sad face if the player lost and a crown if he won.
      I used 2 buttons: start/pause and reset.
  ### Setup
  ![](https://github.com/AntoniaOancea/IntroductionToRobotics/blob/main/Homework%207/homework7.jpg)

  ### [Video](https://youtu.be/XaEDOVq2WG4)

  ### [Code](https://github.com/AntoniaOancea/IntroductionToRobotics/blob/main/Homework%207/homework7/homework7.ino)

  </details>

