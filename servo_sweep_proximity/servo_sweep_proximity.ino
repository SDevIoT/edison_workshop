// Sweep
// by BARRAGAN <http://barraganstudio.com> 
// This example code is in the public domain.

#define trigger    4
#define echo  5

#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 
int pos = 0;    // variable to store the servo position 
 
void setup() 
{ 

  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  myservo.attach(3);  // attaches the servo on pin 9 to the servo object 
  Serial.begin(9600);
} 
 
 
void loop() 
{ 
  int distance = readDistance();
  Serial.print(millis());
  Serial.print(":");
  Serial.print(distance);
  Serial.println("cm. ");
  return;

/*
  for(int i=0; i<5; i++){
    myservo.write(0);
    delay (1000);
    myservo.write(180);
    delay (1000);
  }
*/
  
  for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    Serial.print(readDistance());
    Serial.print("|");
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  

  Serial.println(">");
  for(pos = 180; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    Serial.print(readDistance());
    Serial.print("|");
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  Serial.println("<");
} 

int readDistance(){
  digitalWrite(trigger, HIGH);
  delay(10);
  digitalWrite(trigger,LOW);
  int duration = pulseIn(echo, HIGH, 100);
  int distance = (duration/2) / 29.1;  
  return distance;
}


