// sd card header files declaration
#include <SPI.h>
#include <SD.h>

//gps header files
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#define rxGPS 2
#define txGPS 3

//sd card variable declaration
File myFile;

//Gps variables declation
long lat, lon;
SoftwareSerial gpsSerial(rxGPS, txGPS);
TinyGPSPlus gps;

void setup()
{
  Serial.begin(9600);
  sd_initialization();
  gps_intialization();
}

void loop()
{
  sd_write();
  //sd_read();
  delay(1000);
}

void sd_initialization()
{
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
}

void sd_write()
{
  //ACTUAL SD CODE
  myFile = SD.open("drifter.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {

    //////////////////////////////// GPS Data////////////////////////////////
    while (gpsSerial.available())     // check for gps data
    {
      if (gps.encode(gpsSerial.read()))   // encode gps data
      {

        Serial.print("Sats"); Serial.print(gps.satellites.value()); Serial.print(',');
        Serial.print("Lat"); Serial.print(' '); Serial.print(gps.location.lat(), 6); Serial.print(',');
        Serial.print("Long"); Serial.print(' '); Serial.print(gps.location.lng(), 6); Serial.print(',');
        Serial.print("Alt"); Serial.print(' '); Serial.print(gps.altitude.meters()); Serial.print(',');
        Serial.print("Speed"); Serial.print(' '); Serial.print(gps.speed.mps()); Serial.print(',');
        Serial.print("date"); Serial.print(' '); Serial.print(gps.date.day()); Serial.print(',');
        Serial.print("month"); Serial.print(' '); Serial.print(gps.date.month()); Serial.print(',');
        Serial.print("year"); Serial.print(' '); Serial.print(gps.date.year()); Serial.print(',');
        Serial.print("Hour"); Serial.print(' '); Serial.print(gps.time.hour()); Serial.print(',');
        Serial.print("Mins"); Serial.print(' '); Serial.print(gps.time.minute()); Serial.print(',');
        Serial.print("Sec"); Serial.print(' '); Serial.print(gps.time.second()); Serial.print(';'); Serial.println(' ');

        myFile.print("Sats");
        myFile.print(gps.satellites.value()); myFile.print(',');
        // myFile.print("Lat");
        myFile.print(' '); myFile.print(gps.location.lat(), 6); myFile.print(',');
        //myFile.print("Long");
        myFile.print(' '); myFile.print(gps.location.lng(), 6); myFile.print(',');
        // myFile.print("Alt");
        myFile.print(' '); myFile.print(gps.altitude.meters()); myFile.print(',');
        // myFile.print("Speed");
        myFile.print(' '); myFile.print(gps.speed.mps()); myFile.print(',');
        // myFile.print("date");
        myFile.print(' '); myFile.print(gps.date.day()); myFile.print(',');
        //myFile.print("month");
        myFile.print(' '); myFile.print(gps.date.month()); myFile.print(',');
        // myFile.print("year");
        myFile.print(' '); myFile.print(gps.date.year()); myFile.print(',');
        // myFile.print("Hour");
        myFile.print(' '); myFile.print(gps.time.hour()); myFile.print(',');
        // myFile.print("Mins");
        myFile.print(' '); myFile.print(gps.time.minute()); myFile.print(',');
        //myFile.print("Sec");
        myFile.print(' '); myFile.print(gps.time.second()); myFile.print(';'); myFile.println(' ');
        delay(500);
      }
    }

    // close the file:
    myFile.close();
    //Serial.println("Writing in SD card");
    //delay(1000);
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening the file");
  }
  //delay(1000);
}
void gps_intialization()
{
  gpsSerial.begin(9600); // connect gps sensor
}
