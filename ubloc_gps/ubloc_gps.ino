//gps header files
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#define rxGPS D5//goes to tx of gps
#define txGPS D6//goes to rx of gps


 //Gps variables declation
long lat, lon;
SoftwareSerial gpsSerial(rxGPS, txGPS);
TinyGPSPlus gps;
 
void setup()
{
  Serial.begin(9600); // connect serial
  gps_intialization();
}
 
void loop()
{
  gps_data();
}
void gps_intialization()
{
    gpsSerial.begin(115200); // connect gps sensor
}

void gps_data()
{
  while (gpsSerial.available())     // check for gps data
  {
    if (gps.encode(gpsSerial.read()))   // encode gps data
    {
      Serial.print("SATS: ");
      Serial.println(gps.satellites.value());
      Serial.print("LAT: ");
      Serial.println(gps.location.lat(), 8);
      Serial.print("LONG: ");
      Serial.println(gps.location.lng(), 8);
      Serial.print("ALT: ");
      Serial.println(gps.altitude.meters());
      Serial.print("SPEED: ");
      Serial.println(gps.speed.mps());
 
      Serial.print("Date: ");
      Serial.print(gps.date.day()); Serial.print("/");
      Serial.print(gps.date.month()); Serial.print("/");
      Serial.println(gps.date.year());
 
      Serial.print("Hour: ");
      Serial.print(gps.time.hour()); Serial.print(":");
      Serial.print(gps.time.minute()); Serial.print(":");
      Serial.println(gps.time.second());
      Serial.println("---------------------------");
      delay(4000);
    }
  }
}
