// ping sonar 

#include "ping1d.h"

#include "SoftwareSerial.h"

// This serial port is used to communicate with the Ping device
// If you are using and Arduino UNO or Nano, this must be software serial, and you must use
// 9600 baud communication
// Here, we use pin 9 as arduino rx (Ping tx, white), 10 as arduino tx (Ping rx, green)
static const uint8_t arduinoRxPin = 9;
static const uint8_t arduinoTxPin = 10;
SoftwareSerial pingSerial(arduinoRxPin, arduinoTxPin);
static Ping1D ping { pingSerial };

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
  ping_sonar_init();
}

void loop()
{
  sd_write();
 // pingloop();
  //sd_read();
  delay(500);
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

      /*  Serial.print("Sats"); Serial.print(gps.satellites.value()); Serial.print(',');
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
*/
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
        myFile.print(' '); myFile.print(gps.time.second()); myFile.print(';'); myFile.print(' ');
        myFile.print(ping.distance()); myFile.println(' ');
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
  gpsSerial.begin(115200); // connect gps sensor
}

void ping_sonar_init()
{
  pingSerial.begin(115200);
    Serial.println("Blue Robotics ping1d-simple.ino");
    while (!ping.initialize()) {
        Serial.println("\nPing device failed to initialize!");
        Serial.println("Are the Ping rx/tx wired correctly?");
        Serial.print("Ping rx is the green wire, and should be connected to Arduino pin ");
        Serial.print(arduinoTxPin);
        Serial.println(" (Arduino tx)");
        Serial.print("Ping tx is the white wire, and should be connected to Arduino pin ");
        Serial.print(arduinoRxPin);
        Serial.println(" (Arduino rx)");
        delay(2000);
    }
}
/*void pingloop()
{
  if (ping.update()) {
        Serial.print("Distance: ");
        //Serial.print(ping.distance());
        
        Serial.print("\tConfidence: ");
        Serial.println(ping.confidence());
    } else {
        Serial.println("No update received!");
    }
}*/
