// Dallas Temperature sensor 
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 4

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
  float tempC1;
  float tempC2;
  float tempC3;
  float tempC4;

//pressure sensor 
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
char ssid[] = "4GWIFI_51910";
char pass[] = "12345678";

///pressure sensor
const float  OffSet = 2.00 ;
int V, P;

void setup()
{
  // Debug console
  Serial.begin(115200);
  temp_ini();
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
  temp_run();


  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}
void myTimerEvent()
{
  V = analogRead(34);
  Serial.print("Voltage:"); Serial.print(V, 3); Serial.println("V");
  Blynk.virtualWrite(V0, V);
  Blynk.virtualWrite(V1, tempC1);
  Blynk.virtualWrite(V2, tempC2);
  Blynk.virtualWrite(V3, tempC3);
  Blynk.virtualWrite(V4, tempC4);

}

void temp_ini()
{
    Serial.println("Dallas Temperature IC Control Library Demo");

  // Start up the library
  sensors.begin();
}
void temp_run()
{
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
   tempC1 = sensors.getTempCByIndex(0);
   tempC2 = sensors.getTempCByIndex(1);
   tempC3 = sensors.getTempCByIndex(2);
   tempC4 = sensors.getTempCByIndex(3);

  // Check if reading was successful
  if (tempC1 != DEVICE_DISCONNECTED_C)
  {
    Serial.print("Temperature for the device 1 (index 0) is: ");
    Serial.println(tempC1);
    Serial.print("Temperature for the device 1 (index 1) is: ");
    Serial.println(tempC2);
    Serial.print("Temperature for the device 1 (index 2) is: ");
    Serial.println(tempC3);
    Serial.print("Temperature for the device 1 (index 3) is: ");
    Serial.println(tempC4);
  }
  else
  {
    Serial.println("Error: Could not read temperature data");
  }
}
