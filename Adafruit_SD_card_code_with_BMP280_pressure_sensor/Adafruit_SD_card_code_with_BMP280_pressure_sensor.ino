/* Starter code for the Nano Data Logger. @file NanoDataLoggerBMPv4.ino
The Nano Logger is an Arduino Nano with data logging shield with realtime clock (RTC) and microSD socket.
For use with a BMP280 I2C sensor for barometric pressure and temperature.
The components are not slept between read/write events so a sustained power source is required for 
longterm operation (small batteries last just a couple of days).
Saves and displays data every x seconds (modify near line 32).
C. Fastie 12/2017
*/
#include <SdFat.h>                            // https://github.com/greiman/SdFat/
#include  <SPI.h>
#include <Wire.h>
#include <RTClib.h>                           // library from https://github.com/adafruit/RTClib    
#include <Adafruit_Sensor.h>                  // https://github.com/adafruit/Adafruit_Sensor
#include <Adafruit_BMP280.h>                  // https://github.com/adafruit/Adafruit_BMP280_Library
Adafruit_BMP280 bmp;                          // The BMP280 will be an I2C device, and "bmp" is the object.
       // The following two lines allow code in the BMP280 library to compute altitude from pressure.
       // Before use, change the hgInches variable to the actual sealevel barometric pressure.
       // Consult a weather website for the current value. Then save and load the sketch onto the Nano.
float hgInches = 30.00;                       // Enter the sealevel barometric pressure here (xx.xx inches Hg)
#define SEALEVELPRESSURE_HPA (hgInches/0.02952998751)      // hPa=(inches Hg)/0.02952998751
RTC_DS1307 RTC;                               // The real time clock object is "RTC"
#define DS1307_I2C_ADDRESS 0x68
SdFat SD;                                     // The SdFat object is "SD"
#define MOSIpin 11                            // For SD card
#define MISOpin 12                            // For SD card
const int chipSelect = 10;                    // CS pin for the SD card
char tmeStrng[ ] = "0000/00/00,00:00:00";     // a template for a data/time string
float BMPt; 
float BMPp; 
float BMPa; 
long utc;
unsigned long logSeconds = 5;                  // ****** Enter ****** the number of seconds between logging events
unsigned long logMillis = logSeconds * 1000;      
                         
void setup() {  
  Serial.begin(9600);              // Open serial communications and wait for port to open:
  Wire.begin();                    // initialize the I2C interface
  RTC.begin();                     // initialize the RTC 
      // Uncomment the line below and load the sketch to the Nano to set the RTC time. Then the line must 
      // be commented out and the sketch loaded again or the time will be wrong.
//  RTC.adjust(DateTime((__DATE__), (__TIME__)));    //sets the RTC to the time the sketch was compiled.

  while (!Serial) {
    ;                              // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("NanoDataLoggerBMPv4 first tests");
  Serial.print("Sealevel pressure used is ");
  Serial.println(hgInches);
  Serial.print("Find SD card: ");          // initialize and display the status of the SD card:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed");
    while(1);
  }
  Serial.println(" SD card OK");   
  Serial.print("Logging to microSD card every "); 
  Serial.print(logSeconds);   
  Serial.println(" seconds.");  
  Serial.print("Which equals "); 
  Serial.print(logMillis); 
  Serial.println(" milliseconds."); 
  Serial.println();     
                                           //print a header to the data file with column headings
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {                          // if the file is available, write to it:
    dataFile.println("NanoDataLogger_bmp first tests");
    dataFile.print("Sealevel pressure used is ");
    dataFile.println(hgInches);
    dataFile.println("Date,Time,UTCtime,Temp_C,Pressure_hPa,Altitude_M");
    dataFile.close();
  }
  else {
    Serial.println("file error");          // if the file is not open, display an error:
  }
    bool status;                           // initialize the BMP280 sensor and display the status
    status = bmp.begin(0x76);              // the I2C address of these BMP280 sensors is 0x76;
    if (!status) {
        Serial.println("No BMP280");
        while (1);
    } 
    delay(2000);                           // Wait so the sensor can initialize 
}                                          // end of setup
void loop() 
{
    BMPt = (bmp.readTemperature());                               // read the temperature
    BMPp = (bmp.readPressure() / 100.0F);                         // read the pressure
    BMPa = (bmp.readAltitude(SEALEVELPRESSURE_HPA));              // compute the altitude
    DateTime now = RTC.now();                                     // read the time from the RTC
    utc = (now.unixtime());  
    sprintf(tmeStrng, "%04d/%02d/%02d,%02d:%02d:%02d", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second()); // [added seconds]
                                    
    Serial.print("RTC utc Time: ");                               // Display results on the serial monitor:   
    Serial.print(now.unixtime());
    Serial.println();
    Serial.print("RTC time: ");
    Serial.println(tmeStrng);
    Serial.print("BMP280 temp: ");
    Serial.print(BMPt);
    Serial.println(" C");
    Serial.print("Pressure: ");
    Serial.print(BMPp);
    Serial.println(" hPa");
    Serial.print("Elevation: ");
    Serial.print(BMPa);
    Serial.println(" m");
    Serial.println();
                                                                // write the data to the SD card:                                                                 
    File dataFile = SD.open("datalog.txt", FILE_WRITE);         // if the file is available, write to it:
      dataFile.print(tmeStrng);dataFile.print(",");
      dataFile.print(utc);dataFile.print(",");
      dataFile.print(BMPt);dataFile.print(",");
      dataFile.print(BMPp);dataFile.print(",");
      dataFile.println(BMPa);
      dataFile.flush();                                        // wait for serial data to complete transmission
      dataFile.close();
    delay(logMillis);                                          // Write data every logMillis/1000 seconds
}                                                              // end of the MAIN LOOP
