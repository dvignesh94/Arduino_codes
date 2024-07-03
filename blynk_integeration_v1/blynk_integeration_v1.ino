//ping sonar declaration

#include "ping1d.h"

#include "SoftwareSerial.h"

// This serial port is used to communicate with the Ping device
// If you are using and Arduino UNO or Nano, this must be software serial, and you must use
// 9600 baud communication
// Here, we use pin 9 as arduino rx (Ping tx, white), 10 as arduino tx (Ping rx, green)
static const uint8_t arduinoRxPin = 32;
static const uint8_t arduinoTxPin = 33;
SoftwareSerial pingSerial(arduinoRxPin, arduinoTxPin);
static Ping1D ping { pingSerial };

static const uint8_t ledPin = 13;



/* Fill-in your Template ID (only if using Blynk.Cloud) */
#define BLYNK_TEMPLATE_ID "TMPL2PjphxOJ"
#define BLYNK_TEMPLATE_NAME "TTGO GPS"
#define BLYNK_AUTH_TOKEN "zc9GWgaF9TkTXhZU4LqNGOiHU686B5mf"

// Select your modem:
#define TINY_GSM_MODEM_SIM7000
#define TINY_GSM_RX_BUFFER 1024
#define SerialAT Serial1
#include <TinyGsmClient.h>
#include <BlynkSimpleTinyGSM.h>
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Ticker.h>
#include <Wire.h>

/////GPS initialization
#define SerialMon Serial
#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

#define uS_TO_S_FACTOR      1000000ULL  // Conversion factor for micro seconds to seconds
#define TIME_TO_SLEEP       60

BlynkTimer timer;
File myFile;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = BLYNK_AUTH_TOKEN;

// Your GPRS credentials
// Leave empty, if missing user or pass
char apn[]  = "airtelgprs.com";
char user[] = "";
char pass[] = "";

#define SerialAT Serial1
#define UART_BAUD   9600
#define PIN_DTR     25
#define PIN_TX      27
#define PIN_RX      26
#define PWR_PIN     4
#define LED_PIN     12
#define BAT_ADC     35
#define SD_MISO             2
#define SD_MOSI             15
#define SD_SCLK             14
#define SD_CS               13
#define LED_PIN             12
bool reply = false;

//TinyGsm modem(SerialAT);

//gps lat long global variable
String lat_data, lon_data,raw;

BLYNK_WRITE(V0) {
  if (param.asInt() == 1)
  {

    digitalWrite(LED_PIN, LOW);
    Blynk.logEvent("led_off");//Sending Events
  }
  else {
    digitalWrite(LED_PIN, HIGH);
    Blynk.logEvent("led_on");//Sending Events
  }
}

//Syncing the output state with the app at startup
BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V0);  // will cause BLYNK_WRITE(V3) to be executed
}


float readBattery(uint8_t pin)
{
  int vref = 1100;
  uint16_t volt = analogRead(pin);
  float battery_voltage = ((float)volt / 4095.0) * 2.0 * 3.3 * (vref);
  return battery_voltage;
}

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  //float h = 32.2;
  //float t = 55.3;
  //float mv = readBattery(BAT_ADC);
 // Serial.print("mv :");  Serial.println(mv);
 // Serial.print("Pressure :");  Serial.println(h);
  //Serial.print("Temperature :");  Serial.println(t);

  // You can send any value at any time.
  // Please don't send more that 10 values per second.

  // Blynk.virtualWrite(V0, t);
  //Blynk.virtualWrite(V1, h);
  //Blynk.virtualWrite(V2, ((mv / 4200) * 100));
  Blynk.virtualWrite(V0, lon_data, lat_data);
  Blynk.virtualWrite(V1, raw);
  ping_run();

  }

void setup() {
  Serial.begin(115200); // Set console baud rate
  ping_int();
  delay(100);

  // Set LED OFF
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  pinMode(PWR_PIN, OUTPUT);
  digitalWrite(PWR_PIN, HIGH);
  delay(300);
  digitalWrite(PWR_PIN, LOW);

  Serial.println("\nWait...");

  delay(1000);

  SerialAT.begin(UART_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);


  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  Serial.println("Initializing modem...");
  if (!modem.restart()) {
    Serial.println("Failed to restart modem, attempting to continue without restarting");
  }

  String name = modem.getModemName();
  modem.setNetworkMode(13); ////................Updated #new(25/03/2023)
  // 2 Automatic
  // 13 GSM only
  // 38 LTE only
  // 51 GSM and LTE only
  // Set network mode to auto
  delay(500);
  Serial.println("Modem Name: " + name);

  Blynk.begin(auth, modem, apn, user, pass);
  // Setup a function to be called every second
  timer.setInterval(2000L, sendSensor);
  gps_initialization7000();

}

void loop() {

  Blynk.run();
  timer.run();
  gps_loop();
  //ping_run();


}
void enableGPS(void)
{
  // Set SIM7000G GPIO4 LOW ,turn on GPS power
  // CMD:AT+SGPIO=0,4,1,1
  // Only in version 20200415 is there a function to control GPS power
  modem.sendAT("+SGPIO=0,4,1,1");
  if (modem.waitResponse(10000L) != 1) {
    DBG(" SGPIO=0,4,1,1 false ");
  }
  modem.enableGPS();


}

void disableGPS(void)
{
  // Set SIM7000G GPIO4 LOW ,turn off GPS power
  // CMD:AT+SGPIO=0,4,1,0
  // Only in version 20200415 is there a function to control GPS power
  modem.sendAT("+SGPIO=0,4,1,0");
  if (modem.waitResponse(10000L) != 1) {
    DBG(" SGPIO=0,4,1,0 false ");
  }
  modem.disableGPS();
}

void modemPowerOn()
{
  pinMode(PWR_PIN, OUTPUT);
  digitalWrite(PWR_PIN, LOW);
  delay(1000);    //Datasheet Ton mintues = 1S
  digitalWrite(PWR_PIN, HIGH);
}

void modemPowerOff()
{
  pinMode(PWR_PIN, OUTPUT);
  digitalWrite(PWR_PIN, LOW);
  delay(1500);    //Datasheet Ton mintues = 1.2S
  digitalWrite(PWR_PIN, HIGH);
}


void modemRestart()
{
  modemPowerOff();
  delay(1000);
  modemPowerOn();
}
void gps_loop()
{
  if (!modem.testAT()) {
    Serial.println("Failed to restart modem, attempting to continue without restarting");
    modemRestart();
    return;
  }

  //Serial.println("Start positioning . Make sure to locate outdoors.");
  //Serial.println("The blue indicator light flashes to indicate positioning.");

  enableGPS();

  float lat,  lon;
  while (1) {
    if (modem.getGPS(&lat, &lon)) {
      Serial.println("The location has been locked, the latitude and longitude are:");
      Serial.print("latitude:"); Serial.println(lat, 8);
      lat_data = String(lat, 8);
      lon_data = String(lon, 8);
      Serial.print("longitude:"); Serial.println(lon, 8);
      raw = String(modem.getGPSraw());
      break;
      //Blynk.virtualWrite(V3, String(lon,8));


    }
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(2000);
  }

  disableGPS();
}

void gps_initialization7000()
{

  delay(10);

  // Set LED OFF
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  modemPowerOn();

  SerialAT.begin(UART_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);

  Serial.println("/**********************************************************/");
  Serial.println("To initialize the network test, please make sure your GPS");
  Serial.println("antenna has been connected to the GPS port on the board.");
  Serial.println("/**********************************************************/\n\n");

  delay(10000);
}
void ping_int()
{
  pingSerial.begin(9600);
    pinMode(ledPin, OUTPUT);
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
void ping_run()
{
  if (ping.update()) {
        Serial.print("Distance: ");
        Serial.print(ping.distance());
        Serial.print("\tConfidence: ");
        Serial.println(ping.confidence());
    } else {
        Serial.println("No update received!");
    }
}
