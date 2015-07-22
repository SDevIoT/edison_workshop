/*
  ProxToServo.ino
  2015 Copyright (c) Intel Corporation.  All right reserved.

  Author: Jose Nunez
*/

#include <Wire.h>
#include "rgb_lcd.h"
#include <Servo.h> 

#define echo    13
#define trigger 12
#define vcc     11
rgb_lcd lcd;

Servo myservo;

void setup() 
{
    lcd.begin(16, 2);
    lcd.print("HI!. I'm PROXIMITY");
    myservo.attach(6); 

    pinMode(echo,INPUT);
    pinMode(trigger,OUTPUT);
    pinMode(vcc,OUTPUT);

    digitalWrite(vcc,HIGH);
    digitalWrite(trigger,LOW);
}

void loop() 
{
  setServoAngle();
  readPotAndUseIt();
}

void readPotAndUseIt(){
    int red, green, blue;
    red = 0;
    green = 0;
    int potValue = analogRead(0);
    blue = map(potValue, 0, 1024, 0, 255);
    red = 255 - map(potValue, 0, 1024, 0, 255);
    lcd.setRGB(red, green, blue);
}

void setServoAngle(){
  digitalWrite(trigger, HIGH);
  delay(10);
  digitalWrite(trigger,LOW);
  int duration = pulseIn(echo, HIGH);
  int distance = (duration/2) / 29.1;
  lcd.setCursor(0,2);
  lcd.print("DISTANCE:");
  lcd.print(distance);
  lcd.print("cm     ");  

  int distanceForAngle = distance;

  if(distanceForAngle > 20) distanceForAngle = 20;
  int val = map(distanceForAngle, 0, 20, 0, 140);     // scale it to use it with the servo (value between 0 and 150) 
  myservo.write(val);                  // sets the servo position according to the scaled value 
  delay(15);  
}

