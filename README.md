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
      • State Change & Timers: If the elevator is already at the desired floor, pressing the button for that floor should have no effect. 
      Otherwise, after a button press, the elevator should ”wait for the doors to close” and then ”move” to the corresponding floor. 
      If the elevator is in movement, it should either do nothing or it should stack its decision (get to the first programmed floor, 
      open the doors, wait, close them and then go to the next desired floor).
      • Debounce: for the buttons to avoid unintentional repeated button presses

  ### Bonus:
  ##### Buzzer
  ##### Queue for elevator calls

  ### Setup
  ![](https://github.com/AntoniaOancea/IntroductionToRobotics/blob/main/Homework%203/homework3.jpg)

  ### [Video](https://www.youtube.com/watch?v=2bYAEGiis1o)

  ### [Code](https://github.com/AntoniaOancea/IntroductionToRobotics/blob/main/Homework%203/homework3/homework3.ino)

  </details>
