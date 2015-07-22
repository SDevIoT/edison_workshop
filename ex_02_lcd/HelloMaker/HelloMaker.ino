/*
  HelloMaker.ino
  2013 Copyright (c) Intel Corporation.  All right reserved.

  Author:Jose Nunez
  2015-07-21

*/

#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

int red, green, blue;

void setup() 
{
    lcd.begin(16, 2); //SPECIFY THE COLUMNS AND ROWS OF THE DISPLAY
    lcd.print("Hello Makers!!!"); //PRINT AN INITIAL MESSAGE
}

void loop() 
{
    setBackgroundColor();
    printColors();
    delay(50);

}
void printColors(){
    lcd.setCursor(0,1);//Move cursor to col 0, row 1.
    lcd.print("R");lcd.print(red);lcd.print("|");
    lcd.print("G");lcd.print(green);lcd.print("|");
    lcd.print("B");lcd.print(blue);lcd.print("      ");  
}
void setBackgroundColor(){
    red +=5;
    if(red>=255){
      red=255;
      green+=5;
      if(green>=255){
        green=255;
        blue+=5;
        if(blue==255){
          blue=0;
          red=0;
          green=0;
        }
      }
    }
    lcd.setRGB(red, green, blue);
}
