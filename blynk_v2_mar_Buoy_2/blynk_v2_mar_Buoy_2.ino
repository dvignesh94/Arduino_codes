/*************************************************************

  You’ll need:
   - Blynk IoT app (download from App Store or Google Play)
   - ESP32 board
   - Decide how to connect to Blynk
     (USB, Ethernet, Wi-Fi, Bluetooth, ...)

  There is a bunch of great example sketches included to show you how to get
  started. Think of them as LEGO bricks  and combine them as you wish.
  For example, take the Ethernet Shield sketch and combine it with the
  Servo example, or choose a USB sketch and add a code from SendData
  example.
 *************************************************************/

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPLAhjSDFhl"
#define BLYNK_TEMPLATE_NAME "ESP32 wifi test"
#define BLYNK_AUTH_TOKEN "hsgXthNqpxj4JHhAG3_9QJB--AC3MFv0"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
BlynkTimer timer;


// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Airtel-MyWiFi-AMF-311WW-32D2";
char pass[] = "08ec0d8e";

///pressure sensor
const float  OffSet = 2.00 ;
int V, P;

void setup()
{
  // Debug console
  Serial.begin(115200);
  Serial.println("/** Water pressure sensor initialized **/");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);
  timer.setInterval(100, myTimerEvent);

}

void loop()
{
  Blynk.run();
  //pressure_sensor();
  timer.run();


  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}
void myTimerEvent()
{
  V = analogRead(34);
  Serial.print("Voltage:"); Serial.print(V, 3); Serial.println("V");
  Blynk.virtualWrite(V0, V);
}
