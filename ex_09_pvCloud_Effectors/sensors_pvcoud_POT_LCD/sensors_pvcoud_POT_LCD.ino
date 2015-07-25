#include <Wire.h>
#include "rgb_lcd.h"
#include "pvcloud_lib.h"

rgb_lcd lcd;
PVCloud pvCloud;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Setting up...");
  pvCloud.SendString("SKETCH MONITOR","SETUP BEGINS");
  pvCloud.LogEntry("setup() begin");

  
  pinMode(13,OUTPUT);
  blinkSignal();
  
  // put your setup code here, to run once:

  previousValueTest();



  pvCloud.SendString("SKETCH MONITOR","SETUP COMPLETE");
  pvCloud.LogEntry("setup() end");
}

void loop() {
  int potValue = analogRead(0);
  
  if(valueHasChanged(potValue, 20)){
    Serial.print("New Value: ");
    Serial.println(potValue);
    lcd.setCursor(0,0);lcd.print("Value changed! Sending..");
    
    pvCloud.SendFloat("POT_NEW_VALUE",potValue);

    lcd.setCursor(0,0);lcd.print("Retrieving from pvCloud...");
    String lastValue = pvCloud.RetrieveStringValue("POT_NEW_VALUE");
    Serial.print("LAST VALUE READ FROM PVCLOUD: -");
    Serial.print(lastValue);    
    Serial.println("-");

    lcd.setCursor(0,0);lcd.print(lastValue + "                   ");
    if(lastValue > "500") {
      lcd.setCursor(0,1);
      lcd.print("Beyond 500");
    } else {
      lcd.setCursor(0,1);
      lcd.print("Below 500  ");      
    }
  }  
}

int previousValue = -1;
bool valueHasChanged(int value, int trigger){
    if(abs(value-previousValue) >= trigger || previousValue == -1){
      previousValue = value;
      return true;
    }
    else {
      return false;
    }
}


void previousValueTest(){
  int fakeTrigger = 5;
  int fakeValue = 100;
  if(valueHasChanged(fakeValue,fakeTrigger)){
    pvCloud.SendString("SKETCH MONITOR","previousValueTest() Successfully detected first value change (100)");
  }

  fakeValue = 102;
  if( ! valueHasChanged(fakeValue,fakeTrigger)){
    pvCloud.SendString("SKETCH MONITOR","previousValueTest() Successfully detected small value as no big deal (102)");
  }

  fakeValue = 110;
  if(valueHasChanged(fakeValue,fakeTrigger)){
    pvCloud.SendString("SKETCH MONITOR","previousValueTest() Successfully detected important value change (110)");
  }  
}

void blinkSignal(){
  for(int i=0; i<5; i++){
    digitalWrite(13,HIGH);
    delay(200);
    digitalWrite(13,LOW);
    delay(200);
  }
}

