//imu

#include <MPU9250_WE.h>
#include <Wire.h>
#define MPU9250_ADDR 0x68
MPU9250_WE myMPU9250 = MPU9250_WE(MPU9250_ADDR);

String accx1, accy1, accz1, accx2, accy2, accz2, gx1, gy1, gz1;

//gps header files
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9800;

TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);
//Gps variables declaration
long lat_data, lon_data, speed_data;

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
#define ONE_WIRE_BUS 15

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
  imu_ini();

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
  imu_run();
  //imu variables accRaw.x,accCorrRaw.x,gValue.x
  V = analogRead(34);
  Serial.print("Voltage:"); Serial.print(V, 3); Serial.println("V");
  Blynk.virtualWrite(V0, V);
  Blynk.virtualWrite(V1, tempC1);
  Blynk.virtualWrite(V2, tempC2);
  Blynk.virtualWrite(V3, tempC3);
  Blynk.virtualWrite(V4, tempC4);
  Blynk.virtualWrite(V5, accx1);
  Blynk.virtualWrite(V6, accy1);
  Blynk.virtualWrite(V7, accz1);

  dataFile.print(V);                    dataFile.print(";");
  dataFile.print(tempC1);               dataFile.print(";");
  dataFile.print(tempC2);               dataFile.print(";");
  dataFile.print(tempC3);               dataFile.print(";");
  dataFile.print(tempC4);               dataFile.print(";");
  dataFile.print(gps.location.lat());   dataFile.print(";");
  dataFile.print(gps.location.lng());   dataFile.print(";");
  dataFile.print(gps.speed.mps());      dataFile.print(";");
  dataFile.print(accx1);                dataFile.print(";");
  dataFile.print(accy1);                dataFile.print(";");
  dataFile.print(accz1);                dataFile.print(";");
  dataFile.print(accx2);                dataFile.print(";");
  dataFile.print(accy2);                dataFile.print(";");
  dataFile.print(accz2);                dataFile.print(";");
  dataFile.print(gx1);                  dataFile.print(";");
  dataFile.print(gy1);                  dataFile.print(";");
  dataFile.print(gz1);                  dataFile.print(";");
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
  ss.begin(GPSBaud);
}

void gps_data()
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

void imu_ini()
{
  Wire.begin();
  if (!myMPU9250.init()) {
    Serial.println("MPU9250 does not respond");
  }
  else {
    Serial.println("MPU9250 is connected");
  }
  Serial.println("Position you MPU9250 flat and don't move it - calibrating...");
  delay(1000);
  myMPU9250.autoOffsets();
  Serial.println("Done!");

  /*  This is a more accurate method for calibration. You have to determine the minimum and maximum
      raw acceleration values of the axes determined in the range +/- 2 g.
      You call the function as follows: setAccOffsets(xMin,xMax,yMin,yMax,zMin,zMax);
      Use either autoOffset or setAccOffsets, not both.
  */
  //myMPU9250.setAccOffsets(-14240.0, 18220.0, -17280.0, 15590.0, -20930.0, 12080.0);

  /*  Sample rate divider divides the output rate of the gyroscope and accelerometer.
      Sample rate = Internal sample rate / (1 + divider)
      It can only be applied if the corresponding DLPF is enabled and 0<DLPF<7!
      Divider is a number 0...255
  */
  myMPU9250.setSampleRateDivider(5);

  /*  MPU9250_ACC_RANGE_2G      2 g
      MPU9250_ACC_RANGE_4G      4 g
      MPU9250_ACC_RANGE_8G      8 g
      MPU9250_ACC_RANGE_16G    16 g
  */
  myMPU9250.setAccRange(MPU9250_ACC_RANGE_2G);

  /*  Enable/disable the digital low pass filter for the accelerometer
      If disabled the bandwidth is 1.13 kHz, delay is 0.75 ms, output rate is 4 kHz
  */
  myMPU9250.enableAccDLPF(true);

  /*  Digital low pass filter (DLPF) for the accelerometer, if enabled
      MPU9250_DPLF_0, MPU9250_DPLF_2, ...... MPU9250_DPLF_7
       DLPF     Bandwidth [Hz]      Delay [ms]    Output rate [kHz]
         0           460               1.94           1
         1           184               5.80           1
         2            92               7.80           1
         3            41              11.80           1
         4            20              19.80           1
         5            10              35.70           1
         6             5              66.96           1
         7           460               1.94           1
  */
  myMPU9250.setAccDLPF(MPU9250_DLPF_6);

  /*  Set accelerometer output data rate in low power mode (cycle enabled)
       MPU9250_LP_ACC_ODR_0_24          0.24 Hz
       MPU9250_LP_ACC_ODR_0_49          0.49 Hz
       MPU9250_LP_ACC_ODR_0_98          0.98 Hz
       MPU9250_LP_ACC_ODR_1_95          1.95 Hz
       MPU9250_LP_ACC_ODR_3_91          3.91 Hz
       MPU9250_LP_ACC_ODR_7_81          7.81 Hz
       MPU9250_LP_ACC_ODR_15_63        15.63 Hz
       MPU9250_LP_ACC_ODR_31_25        31.25 Hz
       MPU9250_LP_ACC_ODR_62_5         62.5 Hz
       MPU9250_LP_ACC_ODR_125         125 Hz
       MPU9250_LP_ACC_ODR_250         250 Hz
       MPU9250_LP_ACC_ODR_500         500 Hz
  */
  //myMPU9250.setLowPowerAccDataRate(MPU9250_LP_ACC_ODR_500);

  /* sleep() sends the MPU9250 to sleep or wakes it up.
     Please note that the gyroscope needs 35 milliseconds to wake up.
  */
  //myMPU9250.sleep(true);

  /* If cycle is set, and standby or sleep are not set, the module will cycle between
       sleep and taking a sample at a rate determined by setLowPowerAccDataRate().
  */
  //myMPU9250.enableCycle(true);

  /* You can enable or disable the axes for gyroscope and/or accelerometer measurements.
     By default all axes are enabled. Parameters are:
     MPU9250_ENABLE_XYZ  //all axes are enabled (default)
     MPU9250_ENABLE_XY0  // X, Y enabled, Z disabled
     MPU9250_ENABLE_X0Z
     MPU9250_ENABLE_X00
     MPU9250_ENABLE_0YZ
     MPU9250_ENABLE_0Y0
     MPU9250_ENABLE_00Z
     MPU9250_ENABLE_000  // all axes disabled
  */
  //myMPU9250.enableAccAxes(MPU9250_ENABLE_XYZ);

}
void imu_run()
{

  xyzFloat accRaw = myMPU9250.getAccRawValues();
  xyzFloat accCorrRaw = myMPU9250.getCorrectedAccRawValues();
  xyzFloat gValue = myMPU9250.getGValues();
  float resultantG = myMPU9250.getResultantG(gValue);

  Serial.println("Raw acceleration values (x,y,z):");
  Serial.print(accRaw.x);
  accx1 = String(accRaw.x);
  Serial.print("   ");
  Serial.print(accRaw.y);
  accy1 = String(accRaw.y);

  Serial.print("   ");
  Serial.println(accRaw.z);
  accz1 = String(accRaw.z);


  Serial.println("Corrected ('calibrated') acceleration values (x,y,z):");
  Serial.print(accCorrRaw.x);
  accx2 = String(accCorrRaw.x);

  Serial.print("   ");
  Serial.print(accCorrRaw.y);
  accy2 = String(accCorrRaw.y);
  Serial.print("   ");
  Serial.println(accCorrRaw.z);
  accz2 = String(accCorrRaw.z);

  Serial.println("g values (x,y,z):");
  Serial.print(gValue.x);
  gx1 = String(gValue.x);
  Serial.print("   ");
  Serial.print(gValue.y);
  gy1 = String(gValue.y);

  Serial.print("   ");
  Serial.println(gValue.z);
  gz1 = String(gValue.z);


  Serial.print("Resultant g: ");
  Serial.println(resultantG); // should always be 1 g if only gravity acts on the sensor.
  Serial.println();

  delay(1000);
}
