/*
 Omni_Robot1_6C_bluet from 4C pulsein timeout in 100000
 
Omni_Robot1_4C_bluet
implement blue tooth for driving control from real sense gestures  
notes: every letter received is one secound of move, 
the remote have to be on because is the way robot stop by reading stop signals in remote.
bluetooth board doing wird staff only shield board working fine, not sure why.

web server

Omni_Robot1_4B 
remove  ch2_duration == 0 from all axis to solve issue that jumps while traveling = means that the control lost signal or turn off while moving will stay on las movement 

Omni_Robot version beta1_3
avoid to repeat pwm setup every cycle flag

Omni_Robot version beta 1_2
deleted funtion if for not read control signals every cicle
encrease remote control stop range

Omni_Robot version beta 1_1
Using PWM as step output
so need to used new pins outputs

pwm read 
subrutines 

 */


#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>


// pin definitions 
#define  FRONT_LEFT_STEP_PIN  3
#define  FRONT_LEFT_DIR_PIN  2

#define  FRONT_RIGHT_STEP_PIN  5
#define  FRONT_RIGHT_DIR_PIN  4

#define  REAR_LEFT_STEP_PIN  6
#define  REAR_LEFT_DIR_PIN  7

#define  REAR_RIGHT_STEP_PIN  9
#define  REAR_RIGHT_DIR_PIN  8

#define CH1 10
#define CH2 11
#define CH3 12
#define CH4 13

#define TURNED_ON true
#define TURNED_OFF false

//variables to capture pulse width
int ch1_duration = 0;
int ch2_duration = 0;
int ch3_duration = 0;
int ch4_duration = 0;

//variables
int CH1_off = 1;
int CH2_off = 1;
int CH3_off = 1;
int CH4_off = 1;

bool motors_on = TURNED_ON; //DEFAULTS TO ON TO MAKE IT POSSIBLE TO TURN THEM OFF INITIALLY.
int incomingByte;
int bluetooth_flag = 0; //if bluetooth activated need to have timer because remote control signal will stop it instantly

String lastValueRead = ""; //FILE LAST VALUE READ for WEB CONTROL

bool serialLogEnabled = false; //DISABLE if NEEDED due to NOISE

void setup() {
    // initialize digital input and output pins.
    delay(3000);
    Serial.begin(9600);
    SerialPrintln("ROBOT SETUP! (0408-1839)");
    SerialPrintln("Initiating Web Server...");
    system("echo HELLO > /home/root/serverlog.txt");
    system("/usr/bin/node /home/root/node_modules/jn_jj_server/index.js >> /home/root/serverlog.txt & ");

    SerialPrintln("Serial1 @ 57600...");
    Serial1.begin(57600);

    SerialPrintln("Setting up 4 RC Channels (CHn)...");
    pinMode(CH1, INPUT);
    pinMode(CH2, INPUT);
    pinMode(CH3, INPUT);
    pinMode(CH4, INPUT); // will read Remote Control Reciver

    SerialPrintln("Setting up STEP and DIRECTION pins FL/FR/RL/RR...");
    pinMode(FRONT_LEFT_STEP_PIN, OUTPUT);
    pinMode(FRONT_LEFT_DIR_PIN, OUTPUT);

    pinMode(FRONT_RIGHT_STEP_PIN, OUTPUT);
    pinMode(FRONT_RIGHT_DIR_PIN, OUTPUT);

    pinMode(REAR_LEFT_STEP_PIN, OUTPUT);
    pinMode(REAR_LEFT_DIR_PIN, OUTPUT);

    pinMode(REAR_RIGHT_STEP_PIN, OUTPUT);
    pinMode(REAR_RIGHT_DIR_PIN, OUTPUT);

    SerialPrintln("Setup Complete!");
}


int ch1_consecutiveReadings_R = 0;
int ch1_consecutiveReadings_L = 0;

int ch2_consecutiveReadings_B = 0;
int ch2_consecutiveReadings_F = 0;

int ch4_consecutiveReadings_R = 0;
int ch4_consecutiveReadings_L = 0;



void loop() {

    String fileValue = readWebCommandFile();

    if (Serial1.available() > 0 || fileValue == "MOVE") {
        // read serial and store in incomingByte  to control by bluetooth
        incomingByte = Serial1.read();

        if (incomingByte == 'R') {
            CH1_off = 0;
            Turn_Right();
        }
        
        else if (incomingByte == 'L') {
            CH1_off = 0;
            Turn_Left();
        }
        
        else if (incomingByte == 'B') {
            CH2_off = 0;
            Backward();
        }//ch2_duration > 2 solve when remote is off = 0
        
        else if (incomingByte == 'F') {
            CH2_off = 0;
            Forward();
        }
        
        else if (incomingByte == 'Z') {
            CH4_off = 0;
            Left_Side();
        }//ch2_duration > 2 solve when remote is off = 0
        
        else if (incomingByte == 'z') {
            CH4_off = 0;
            Right_Side();
        }

        else if (fileValue == "MOVE") {
            SerialPrintln(" Moving ");
            CH2_off = 0;
            Forward();
        }


        Serial1.print("incomingbyte");
        Serial1.print(incomingByte);
        bluetooth_flag = 1;


    } else {
        ch1_duration = pulseIn(CH1, HIGH, 100000);
        ch2_duration = pulseIn(CH2, HIGH, 100000);
        //ch3_duration = pulseIn(CH3,HIGH);
        ch4_duration = pulseIn(CH4, HIGH, 100000);

        //CHX_off flag is how pulses for the steppers are activated.
        //incomingByte is signal from serial =~ bluetooth.

        if (ch1_duration > 1700 ) {
            Serial.print("TURN RIGHT: ");
            Serial.println(ch1_duration);
            ch1_consecutiveReadings_L=0;
            if(ch1_consecutiveReadings_R>2){            
            CH1_off = 0;
            Turn_Right();
            } else {
            ch1_consecutiveReadings_R++;  
            }

        }//ch1_duration > 2 solve when remote is off = 0
        else if (ch1_duration < 1200 && ch1_duration > 100) {
            Serial.print("TURN LEFT: ");
            Serial.println(ch1_duration);
            ch1_consecutiveReadings_R=0;
            if(ch1_consecutiveReadings_L>2){            
            CH1_off = 0;
            Turn_Left();
            } else {
            ch1_consecutiveReadings_L++;  
            }
        }// ch1_duration == 0 solves if remote turn off while moving handle.
        else if (ch1_duration != 0) { //IGNORING CH ZEROES HERE.... WHEN CH1 and CH2 are ZERO WE TURN ALL OFF LATER ON
            int ch1_consecutiveReadings_R = 0;
            int ch1_consecutiveReadings_L = 0;     
            Serial.print("CH1 OFF: ");
            Serial.println(ch1_duration);          
            CH1_off = 1;
        }

        if (ch2_duration > 1700) {
            Serial.print("BACKWARD: ");
            Serial.println(ch2_duration);          
            ch2_consecutiveReadings_F=0;
            if(ch2_consecutiveReadings_B>2){
              CH2_off = 0;
              Backward();
            } else {
              ch2_consecutiveReadings_B++;
            }
            
        }//ch2_duration > 2 solve when remote is off = 0
        else if (ch2_duration < 1200 && ch2_duration > 100) {
            Serial.print("FORWARD: ");
            Serial.println(ch2_duration);
            ch2_consecutiveReadings_B=0;
            
            if(ch2_consecutiveReadings_F>2){            
              CH2_off = 0;
              Forward();
            } else {
              ch2_consecutiveReadings_F++;
            }
        }// ch2_duration == 0 solves if remote turn off while moving handle.
        else if (ch2_duration != 0) { //IGNORING CH ZEROES HERE.... WHEN CH1 and CH2 are ZERO WE TURN ALL OFF LATER ON
            ch2_consecutiveReadings_B=0;
            ch2_consecutiveReadings_F=0;
            Serial.print("CH2 OFF: ");
            Serial.println(ch2_duration);            
            CH2_off = 1;
        }

        if (ch4_duration > 1700) {
            ch4_consecutiveReadings_R=0;
            if(ch4_consecutiveReadings_L>2){          
            Serial.print("LEFT SIDE: ");
            Serial.println(ch4_duration);  
                      
            CH4_off = 0;
            Left_Side();
            } else {
            ch4_consecutiveReadings_L++;  
            }
        }//ch4_duration > 2 solve when remote is off = 0
        else if (ch4_duration < 1200 && ch4_duration > 100) {
               ch4_consecutiveReadings_L=0;
            if(ch4_consecutiveReadings_R>2){       
            Serial.print("RIGHT SIDE: ");
            Serial.println(ch4_duration);                           
            CH4_off = 0;
            Right_Side();
            } else {
            ch4_consecutiveReadings_R++;   
            }
        }// ch3_duration == 0 solves if remote turn off while moving handle.
        else if (ch4_duration != 0) { //IGNORING CH4 ZEROES HERE.... CH4 ZEROES SUCK
            Serial.print("CH4 OFF: ");
            Serial.println(ch4_duration);                  
            CH4_off = 1;
            ch4_consecutiveReadings_L=0;
            ch4_consecutiveReadings_R=0;
            
        }


        if(ch1_duration == 0 && ch2_duration==0 && ch4_duration==0){
          Serial.println("REMOTE OFF DETECTED!!!!!");
          CH1_off=1;
          CH2_off=1;
          CH4_off=1;
        }

    } // else bracket from (Serial.available)

    if (CH1_off == 0 || CH2_off == 0 || CH4_off == 0) {
        turnMotorsOn();
    } else {
        turnMotorsOff();
    }

    if (bluetooth_flag == 1) {
        bluetooth_flag = 0;
        delay(2000);
        Serial.flush();
    }


} ///void_loop bracket

//***********************************subrutines********************************************************************

void turnMotorsOff(){
    if (motors_on == TURNED_ON) {//TURN MOTORS OFF ONLY IF FLAG IS ON
        Serial.println("TURNING Motors OFF...");
        motors_on = TURNED_OFF; //to avoid to set pwm pin when not changed 
        analogWrite(FRONT_LEFT_STEP_PIN, 0); // using PWM to complete low, no pulses
        analogWrite(FRONT_RIGHT_STEP_PIN, 0);
        analogWrite(REAR_LEFT_STEP_PIN, 0);
        analogWrite(REAR_RIGHT_STEP_PIN, 0);
    }
}

void turnMotorsOn(){
    if (motors_on == TURNED_OFF) {//TURN MOTORS ON ONLY IF FLAG IS OFF
        Serial.println("Activating Motors...");
        motors_on = TURNED_ON; //to avoid to set pwm pin when not changed, trying to solve not continuos movement
        analogWrite(FRONT_LEFT_STEP_PIN, 127); // using PWM as pulse with 50% duty cicle =127
        analogWrite(FRONT_RIGHT_STEP_PIN, 127);
        analogWrite(REAR_LEFT_STEP_PIN, 127);
        analogWrite(REAR_RIGHT_STEP_PIN, 127);
    }  
}

int Backward() {
    digitalWrite(FRONT_LEFT_DIR_PIN, LOW);
    digitalWrite(FRONT_RIGHT_DIR_PIN, HIGH);
    digitalWrite(REAR_LEFT_DIR_PIN, LOW);
    digitalWrite(REAR_RIGHT_DIR_PIN, HIGH);
}

int Forward() {
    digitalWrite(FRONT_LEFT_DIR_PIN, HIGH);
    digitalWrite(FRONT_RIGHT_DIR_PIN, LOW);
    digitalWrite(REAR_LEFT_DIR_PIN, HIGH);
    digitalWrite(REAR_RIGHT_DIR_PIN, LOW);
}

int Right_Side() {
    digitalWrite(FRONT_LEFT_DIR_PIN, HIGH);
    digitalWrite(FRONT_RIGHT_DIR_PIN, HIGH);
    digitalWrite(REAR_LEFT_DIR_PIN, LOW);
    digitalWrite(REAR_RIGHT_DIR_PIN, LOW);
}

int Left_Side() {
    digitalWrite(FRONT_LEFT_DIR_PIN, LOW);
    digitalWrite(FRONT_RIGHT_DIR_PIN, LOW);
    digitalWrite(REAR_LEFT_DIR_PIN, HIGH);
    digitalWrite(REAR_RIGHT_DIR_PIN, HIGH);
}

int Turn_Left() {
    digitalWrite(FRONT_LEFT_DIR_PIN, HIGH);
    digitalWrite(FRONT_RIGHT_DIR_PIN, HIGH);
    digitalWrite(REAR_LEFT_DIR_PIN, HIGH);
    digitalWrite(REAR_RIGHT_DIR_PIN, HIGH);
}

int Turn_Right() {
    digitalWrite(FRONT_LEFT_DIR_PIN, LOW);
    digitalWrite(FRONT_RIGHT_DIR_PIN, LOW);
    digitalWrite(REAR_LEFT_DIR_PIN, LOW);
    digitalWrite(REAR_RIGHT_DIR_PIN, LOW);
}

String readWebCommandFile() {

    String fileName = "/home/root/command.txt";
    FILE *filePointer;
    filePointer = fopen(fileName.buffer, "rb"); /*rb is important for the whole thing to work!!!*/
    char fileContent[200];

    fgets(fileContent, 200, filePointer);
    String fileValue = fileContent;
    fileValue.trim();

    fclose(filePointer);

    if (fileValue != lastValueRead) {
        String msg = " - WEB COMMAND:" + fileValue;
        SerialPrintln(msg);
        lastValueRead = fileValue;

        String deleteCommand = "echo > " + fileName;
        system(deleteCommand.buffer);
    }

    return fileValue;
}

void SerialPrint(String message){
    if(serialLogEnabled){
      Serial.print(message);
    }
}

void SerialPrintln(String message){
    if(serialLogEnabled){
      Serial.print(millis());
      Serial.print(" - ");
      Serial.println(message);
    }
}



