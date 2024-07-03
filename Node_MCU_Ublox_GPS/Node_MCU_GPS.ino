//gps
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = D4, TXPin = D3;
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);
String lat_data, lon_data,speed_data;

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID           "TMPLAhjSDFhl"
#define BLYNK_TEMPLATE_NAME         "ESP32 wifi test"
#define BLYNK_AUTH_TOKEN            "hsgXthNqpxj4JHhAG3_9QJB--AC3MFv0"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "4GWIFI_51910";
char pass[] = "12345678";

BlynkTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more than 10 values per second.
String lat1 = lat_data;
String lng1 = lon_data;
String speed1 = speed_data;
  Blynk.virtualWrite(V8, lng1,lat1);
  Serial.println(lat1);
    Serial.println(lng1);

  Blynk.virtualWrite(V9, speed1);
  Serial.println(speed1);
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  ss.begin(GPSBaud);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer

  while (ss.available() > 0)
  {
    if (gps.encode(ss.read()))
    {
      if (gps.location.isValid())
      {
        lat_data = String(gps.location.lat(), 8);
        lon_data = String(gps.location.lng(), 8);
        speed_data = String(gps.speed.kmph());
      }
      else
      {
        Serial.println("Invalid GPS data");
      }
    }
  }
}
