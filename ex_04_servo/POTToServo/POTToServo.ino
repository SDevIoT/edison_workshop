/*
  POTToServo.ino
  2015 Copyright (c) Intel Corporation.  All right reserved.

  Author: Jose Nunez
*/

#include <Wire.h>
#include "rgb_lcd.h"
#include <Servo.h> 

rgb_lcd lcd;

Servo myservo;  // create servo object to control a servo

void setup() 
{
    lcd.begin(16, 2); //SPECIFY THE COLUMNS AND ROWS OF THE DISPLAY
    lcd.print("HI!. I'm SERVO"); //PRINT AN INITIAL MESSAGE
    setBackgroundColor();
    myservo.attach(6); 
}

void loop() 
{
  readPotAndPrint();
  setServoAngle();
}

void setBackgroundColor(){
  int red, green, blue;
  red = 64;
  green = 64;
  blue = 255;

  lcd.setRGB(red, green, blue);
}

void readPotAndPrint(){
    int potValue = analogRead(0);
    lcd.setCursor(0,2);
    lcd.print("POT VALUE:");
    lcd.print(potValue);
    lcd.print("     ");      
}

void setServoAngle(){
  int val = analogRead(0);            // reads the value of the potentiometer (value between 0 and 1023) 
  val = map(val, 0, 1023, 0, 140);     // scale it to use it with the servo (value between 0 and 150) 
  myservo.write(val);                  // sets the servo position according to the scaled value 
  delay(15);  
}

