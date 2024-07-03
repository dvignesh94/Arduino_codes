//gps header files
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#define rxGPS 3
#define txGPS 2
//Gps variables declaration
long lat, lon;
SoftwareSerial gpsSerial(rxGPS, txGPS);
TinyGPSPlus gps;

//sd card program
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#define SCK  18
#define MISO  19
#define MOSI  23
#define CS  5

SPIClass spi = SPIClass(VSPI);
File dataFile;
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
  sd_card_ini();
  gps_intialization();

  dataFile = SD.open("/sensor_data.txt", FILE_WRITE);

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
  gps_data();


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

  dataFile.print(V);
  dataFile.print(";");
  dataFile.print(tempC1);
  dataFile.print(";");
  dataFile.print(tempC2);
  dataFile.print(";");
  dataFile.print(tempC3);
  dataFile.print(";");
  dataFile.print(tempC4);
  dataFile.print(";");
  dataFile.print(gps.location.lat());
  dataFile.print(";");
  dataFile.print(gps.location.lng());
  dataFile.print(";");
  dataFile.print(gps.speed.mps());
  dataFile.print(";");
  dataFile.println();
  dataFile.flush();
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
void sd_card_ini()
{

  spi.begin(SCK, MISO, MOSI, CS);

  if (!SD.begin(CS, spi, 80000000)) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
}
void writeFile(fs::FS &fs, const char * path, char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}
void gps_intialization()
{
  gpsSerial.begin(9600); // connect gps sensor
}

void gps_data()
{
  while (gpsSerial.available())     // check for gps data
  {
    if (gps.encode(gpsSerial.read()))   // encode gps data
    {
      //Serial.print("SATS: ");
      // Serial.println(gps.satellites.value());
      //gps_sats = float(gps.satellites.value());
       //Serial.print("LAT: ");
      Serial.println(gps.location.lat(), 6);
      //gps_lat = float(gps.location.lat());
       //Serial.print("LONG: ");
      Serial.println(gps.location.lng(), 6);
      //gps_long = float(gps.location.lng());
      // Serial.print("ALT: ");
      // Serial.println(gps.altitude.meters());
      //gps_alt = float(gps.altitude.meters());
      //  Serial.print("SPEED: ");
      //  Serial.println(gps.speed.mps());
      //gps_speed = float(gps.speed.mps());

      //  Serial.print("Date: ");
      //  Serial.print(gps.date.day()); Serial.print("/");
      //gps_date = int(gps.date.day());
      //   Serial.print(gps.date.month()); Serial.print("/");
      //gps_month = int(gps.date.month());
      //  Serial.println(gps.date.year());
      //gps_year = int(gps.date.year());

      // Serial.print("Hour: ");
      // Serial.print(gps.time.hour()); Serial.print(":");
      //gps_hour = int(gps.time.hour());
      // Serial.print(gps.time.minute()); Serial.print(":");
      //gps_mins = int(gps.time.minute());
      // Serial.println(gps.time.second());
      //gps_sec = int(gps.time.second());
      //  Serial.println("---------------------------");
      // delay(4000);
    }
  }
}
