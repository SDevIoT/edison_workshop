#define ss Serial1
#include "rgb_lcd.h"
#include "pvcloud_edison_lib.h"
#include "TinyGPS.h"

TinyGPS gps;
PVCloud pvcloud;


int unoRxPin = 0; // connected to Tx pin of the GPS
int unoTxPin = 1; // connected to Rx pin of the GPS
//SoftwareSerial ss(unoRxPin, unoTxPin);

rgb_lcd lcd;

long startMillis;
long secondsToFirstLocation = 0;

void setup()
{
  ss.begin(9600);
  Serial.begin(9600);
  lcd.begin(16,2); // columns, rows.  use 16,2 for a 16x2 LCD, etc.
  startMillis = millis();
}


float pflat, pflon;
void loop()
{
  bool newData = false;
  unsigned long chars = 0;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      int c = ss.read();
      ++chars;
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData                                                               )
  {
    // we have a location fix so output the lat / long and time to acquire
    if(secondsToFirstLocation == 0){
      secondsToFirstLocation = (millis() - startMillis) / 1000;
    }
    
    lcd.clear();  // start with a blank screen
    
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    lcd.setCursor(0,0);           // set cursor to column 0, row 0 (the first row)
    lcd.print("LAT:");

    lcd.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);

    lcd.print("|");
    lcd.print(secondsToFirstLocation);
    lcd.print("s");
    
    lcd.setCursor(0,1);
    lcd.print("LON:");
    lcd.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);

   if(pflat!=flat || pflon!=flon){
      Serial.print("NEW DATA:  ");
      pflat = flat;
      pflon = flon;
      Serial.print(flat,6);
      Serial.print(",");
      Serial.println(flon,6 );

      char strValue[200];
      String strLat  = "";

      sprintf(strValue, "%.7f", flat);
      strLat = strValue;

      String strLong = "";
      sprintf(strValue, "%.7f", flon);
      strLong = strValue;      

      String pvcloudData = "{\"lat\":\"" +strLat + "\",\"long\":\""+strLong+"\"}";
      Serial.println(pvcloudData);
      pvcloud.Write("GPS", pvcloudData);
    }

  }
  
  if (chars == 0){
    // if you haven't got any chars then likely a wiring issue
    lcd.setCursor(0,0);           // set cursor to column 0, row 0 (the first row)
    lcd.print("No GPS: check wiring");
  }
  else if(secondsToFirstLocation == 0){
    // if you have received some chars but not yet got a fix then indicate still searching and elapsed time
    lcd.clear();  // start with a blank screen

    long seconds = (millis() - startMillis) / 1000;
    
    lcd.setCursor(0,0);           // set cursor to column 0, row 0 (the first row)
    lcd.print("Searching ");
    for(int i = 0; i < seconds % 4; ++i){
      lcd.print(".");
    }
    
    lcd.setCursor(0,1);
    lcd.print("Elapsed time:");
    lcd.print(seconds);
    lcd.print("s");

    
  }
}
