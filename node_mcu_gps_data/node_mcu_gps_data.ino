#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = D4, TXPin = D3;
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);
}

void loop()
{
  while (ss.available() > 0)
  {
    if (gps.encode(ss.read()))
    {
      if (gps.location.isValid())
      {
        Serial.print("LAT: ");
        Serial.println(gps.location.lat(), 6);
        Serial.print("LONG: ");
        Serial.println(gps.location.lng(), 6);
        Serial.print("SPEED: ");
        Serial.print(gps.speed.kmph());
        Serial.println(" km/h");
      }
      else
      {
        Serial.println("Invalid GPS data");
      }
    }
  }
}
