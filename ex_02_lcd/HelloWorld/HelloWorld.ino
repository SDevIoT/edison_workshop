/*
  Hello World.ino
  2013 Copyright (c) Seeed Technology Inc.  All right reserved.

  Author:Loovee
  2013-9-18

  Grove - Serial LCD RGB Backlight demo.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified on 2015-JULY-20
  By Jose Nunez
  Adding RGB Effect
  
*/

#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

int colorR = 0;
int colorG = 0;
int colorB = 0;


void setup() 
{
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    // Print a message to the LCD.
    lcd.print("Hello Makers!!!");
}

void loop() 
{

    colorTransition();
    lcd.setRGB(colorR, colorG, colorB);
    lcd.setCursor(0,1);
    
    lcd.print(colorR);lcd.print("-");
    lcd.print(colorG);;lcd.print("-");
    lcd.print(colorB);
    delay(100);

}

void colorTransition(){
    colorR +=5;
    if(colorR>=255){
      colorR=255;
      colorG+=5;
      if(colorG>=255){
        colorG=255;
        colorB+=5;
        if(colorB==255){
          colorB=0;
          colorR=0;
          colorG=0;
        }
      }
    }
}
