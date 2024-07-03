#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#define RX_PIN D5  // GPS module's TX pin connected to NodeMCU's D5 pin (GPIO 14)
#define TX_PIN D6  // GPS module's RX pin connected to NodeMCU's D6 pin (GPIO 12)

SoftwareSerial gpsSerial(RX_PIN, TX_PIN);
TinyGPSPlus gps;

void setup()
{
  Serial.begin(9600);  // Initialize serial communication for the serial monitor
  gpsSerial.begin(9600);  // Initialize software serial communication for the GPS module
}

void loop()
{
  while (gpsSerial.available() > 0)
  {
    if (gps.encode(gpsSerial.read()))
    {
      if (gps.location.isValid())
      {
        Serial.print("LAT: ");
        Serial.println(gps.location.lat(), 6);
        Serial.print("LONG: ");
        Serial.println(gps.location.lng(), 6);
      }
      else
      {
        Serial.println("Invalid GPS data");
      }
    }
  }
}
