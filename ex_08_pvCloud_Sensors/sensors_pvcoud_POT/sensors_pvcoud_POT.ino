#include "pvcloud_lib.h"

PVCloud pvCloud;

void setup() {
  pvCloud.SendString("SKETCH MONITOR","SETUP BEGINS");
  pvCloud.LogEntry("setup() begin");

  Serial.begin(9600);
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
    pvCloud.SendFloat("POT_NEW_VALUE",potValue);
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

