/////pressure sensor
#include <SPI.h>
#include <SD.h>
/////rtc
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>


//////////////SD card
File myFile;

//////////////////PRessure sensor
const float  OffSet = 0.469;
float V, P;
void setup()
{
  Serial.begin(9600);        // open serial port, set the baud rate to 9600 bps
  Serial.println("/** Initializing Water pressure sensor **/");
  sd_initialize();
  rtc_initialize();
}
void loop()
{
  pressure_sensor();
}
void pressure_sensor()
{
  //Connect sensor to Analog 0
  V = analogRead(0) * 5.00 / 1024;     //Sensor output voltage
  P = (V - OffSet) * 250;             //Calculate water pressure

  //Serial.print("Voltage:");
  //Serial.print(V, 3);
  //Serial.println("V");

  //Serial.print(" Pressure:");
  //Serial.print(P, 1);
  //Serial.println(" KPa");
  //Serial.println();
  delay(500);
  write_sd();
}
void sd_initialize()
{
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
}
void write_sd()
{
  tmElements_t tm;

  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Voltage"); Serial.print(V, 3); Serial.print("V"); Serial.print(";");
    Serial.print("Pressure"); Serial.print(P, 3); Serial.print("KPa"); Serial.print(";");
    Serial.print("Ok, Time = "); print2digits(tm.Hour); Serial.write(':'); print2digits(tm.Minute); Serial.write(':'); print2digits(tm.Second);
    Serial.print(", Date (D/M/Y) = "); Serial.print(tm.Day); Serial.write('/'); Serial.print(tm.Month); Serial.write('/'); Serial.print(tmYearToCalendar(tm.Year)); Serial.println();
    myFile.print("Voltage"); myFile.print(V, 3); myFile.print("V"); myFile.print(";"); myFile.print("Pressure"); myFile.print(P, 3); myFile.print("KPa"); myFile.print(";");
    // close the file:
    myFile.close();
    Serial.print("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}
void rtc_initialize()
{
  while (!Serial) ; // wait for serial
  delay(200);
  Serial.println("DS1307RTC Read Test");
  Serial.println("-------------------");
}
void rtc()
{

  tmElements_t tm;

  if (RTC.read(tm)) {
    Serial.print("Ok, Time = ");
    print2digits(tm.Hour);
    Serial.write(':');
    print2digits(tm.Minute);
    Serial.write(':');
    print2digits(tm.Second);
    Serial.print(", Date (D/M/Y) = ");
    Serial.print(tm.Day);
    Serial.write('/');
    Serial.print(tm.Month);
    Serial.write('/');
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.println();
  } else {
    if (RTC.chipPresent()) {
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
      Serial.println("example to initialize the time and begin running.");
      Serial.println();
    } else {
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }
    delay(9000);
  }
  delay(1000);
  write_sd();
}
void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}
