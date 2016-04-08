#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>


void setup() {
  delay(3000);
  Serial.begin(9600);
  Serial.println("FILE READER READY!");
  system("echo HELLO > /home/root/serverlog.txt");
  system("/usr/bin/node /home/root/node_modules/jn_jj_server/index.js >> /home/root/serverlog.txt & ");
}

String lastValueRead = "";
void loop() { 
    String fileName = "/home/root/command.txt";

    FILE *filePointer;
    filePointer = fopen(fileName.buffer, "rb");/*rb is important for the whole thing to work!!!*/
    char fileContent[200];

    fgets(fileContent, 200, filePointer);
    String value = fileContent;
    value.trim();

    fclose(filePointer);
    
    if(value!=lastValueRead){
      Serial.print(millis());
      Serial.print(" - COMMAND: ");
      Serial.println(value);
      lastValueRead=value;

      String deleteCommand = "echo > " + fileName;
      system(deleteCommand.buffer);
    }

}
