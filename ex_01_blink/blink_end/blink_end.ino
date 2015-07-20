/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://www.arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald

  modified 20 July 2015
  by Jose Nunez
  
 */


// the setup function runs once when you press reset or power the board
void setup() {
  
  Serial.begin(9600); //initialize Serial Port
  Serial.println("BLINK SETUP"); //Sends "BLINK SETUP" to serial port
  pinMode(13, OUTPUT); // initialize digital pin 13 as an output.
}

// the loop function runs over and over again forever
void loop() {
  Serial.println("TURNING BLINK LED ON");
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  
  Serial.println("TURNING BLINK LED OFF");
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
 
}
