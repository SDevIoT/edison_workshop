/*
  POTToLCD.ino
  2013 Copyright (c) Intel Corporation.  All right reserved.

  Author: Jose Nunez
*/

#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;


void setup() 
{
    lcd.begin(16, 2); //SPECIFY THE COLUMNS AND ROWS OF THE DISPLAY
    lcd.print("HI!. I'm POT"); //PRINT AN INITIAL MESSAGE
    setBackgroundColor();
}

void loop() 
{
  readPotAndPrint();
  //readStabilizePot();
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

int previousPotValue = -1;
int potStableLimit = 20;
void readStabilizePot(){
  int potValue = analogRead(0);
  if(potValue-previousPotValue >potStableLimit || previousPotValue-potValue>potStableLimit){
    previousPotValue = potValue;  
    lcd.setCursor(0,2);
    lcd.print("POT VALUE:");
    lcd.print(potValue);
    lcd.print("     ");      
  }
}

