// sd card header files declaration
#include <SPI.h>
#include <SD.h>

//imu bmx sensor header file declaration
#include <DFRobot_BMX160.h>

//gps header files
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#define rxGPS 2
#define txGPS 3

//sd card variable declaration
File myFile;

//imu bmx sensor variable declaration
DFRobot_BMX160 bmx160;

//Gps variables declation
long lat, lon;
SoftwareSerial gpsSerial(rxGPS, txGPS);
TinyGPSPlus gps;

//global variables
float mag_x, mag_y, mag_z;
float acc_x, acc_y, acc_z;
float gyr_x, gyr_y, gyr_z;
/*int gps_date, gps_month, gps_year;
  int gps_hour, gps_mins, gps_sec;
  float gps_lat, gps_long, gps_sats, gps_alt, gps_speed;*/

void setup()
{
  Serial.begin(9600);
  sd_initialization();
  bmx_initialize();
  gps_intialization();


}

void loop()
{
  bmx_process();
  gps_data();
  sd_write();


}

void sd_initialization()
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

void sd_write()
{
  gps_data();
  myFile = SD.open("drifter.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    /*
        //acc value write to sd card
        Serial.print("acc x,y,z:"); Serial.print(acc_x); Serial.print(','); Serial.print(acc_y); Serial.print(','); Serial.print(acc_z); Serial.print("::");
        myFile.print("acc x,y,z:"); myFile.print(acc_x); myFile.print(','); myFile.print(acc_y); myFile.print(','); myFile.print(acc_z); myFile.print("::");

        //gyr value write to sd card
        Serial.print("gyr x,y,z:"); Serial.print(gyr_x); Serial.print(','); Serial.print(gyr_y); Serial.print(','); Serial.print(gyr_z); Serial.print("::");
        myFile.print("gyr x,y,z:"); myFile.print(gyr_x); myFile.print(','); myFile.print(gyr_y); myFile.print(','); myFile.print(gyr_z); myFile.print("::");

        //mag value write to sd card
        Serial.print("mag x,y,z:"); Serial.print(mag_x); Serial.print(','); Serial.print(mag_y); Serial.print(','); Serial.print(mag_z); Serial.print("::");
        myFile.print("mag x,y,z:"); myFile.print(mag_x); myFile.print(','); myFile.print(mag_y); myFile.print(','); myFile.print(mag_z); myFile.print("::");
    */

    delay(5000);
    // GPS Data
    Serial.print("Sats"); Serial.print(gps.satellites.value()); Serial.print(',');
    Serial.print("Lat"); Serial.print(' '); Serial.print(gps.location.lat()); Serial.print(',');
    Serial.print("Long"); Serial.print(' '); Serial.print(gps.location.lng()); Serial.print(',');
    Serial.print("Alt"); Serial.print(' '); Serial.print(gps.altitude.meters()); Serial.print(',');
    Serial.print("Speed"); Serial.print(' '); Serial.print(gps.speed.mps()); Serial.print(',');
    Serial.print("date"); Serial.print(' '); Serial.print(gps.date.day()); Serial.print(',');
    Serial.print("month"); Serial.print(' '); Serial.print(gps.date.month()); Serial.print(',');
    Serial.print("year"); Serial.print(' '); Serial.print(gps.date.year()); Serial.print(',');
    Serial.print("Hour"); Serial.print(' '); Serial.print(gps.time.hour()); Serial.print(',');
    Serial.print("Mins"); Serial.print(' '); Serial.print(gps.time.minute()); Serial.print(',');
    Serial.print("Sec"); Serial.print(' '); Serial.print(gps.time.second()); Serial.print(';');Serial.println(' ');

    myFile.print("Sats"); myFile.print(gps.satellites.value()); myFile.print(',');
    myFile.print("Lat"); myFile.print(' '); myFile.print(gps.location.lat()); myFile.print(',');
    myFile.print("Long"); myFile.print(' '); myFile.print(gps.location.lng()); myFile.print(',');
    myFile.print("Alt"); myFile.print(' '); myFile.print(gps.altitude.meters()); myFile.print(',');
    myFile.print("Speed"); myFile.print(' '); myFile.print(gps.speed.mps()); myFile.print(',');
    myFile.print("date"); myFile.print(' '); myFile.print(gps.date.day()); myFile.print(',');
    myFile.print("month"); myFile.print(' '); myFile.print(gps.date.month()); myFile.print(',');
    myFile.print("year"); myFile.print(' '); myFile.print(gps.date.year()); myFile.print(',');
    myFile.print("Hour"); myFile.print(' '); myFile.print(gps.time.hour()); myFile.print(',');
    myFile.print("Mins"); myFile.print(' '); myFile.print(gps.time.minute()); myFile.print(',');
    myFile.print("Sec"); myFile.print(' '); myFile.print(gps.time.second()); myFile.print(';');myFile.println(' ');

    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening the file");
  }
}

void sd_read()
{
  
  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}
void bmx_process()
{
  sBmx160SensorData_t Omagn, Ogyro, Oaccel;

  /* Get a new sensor event */
  bmx160.getAllData(&Omagn, &Ogyro, &Oaccel);

  /* Display the magnetometer results (magn is magnetometer in uTesla) */
  //Serial.print("M ");
  //Serial.print("X: "); Serial.print(Omagn.x); Serial.print("  ");
  mag_x = float(Omagn.x);
  //Serial.print("Y: "); Serial.print(Omagn.y); Serial.print("  ");
  mag_y = float(Omagn.y);
  //Serial.print("Z: "); Serial.print(Omagn.z); Serial.print("  ");
  mag_z = float(Omagn.z);
  // Serial.println("uT");

  /* Display the gyroscope results (gyroscope data is in g) */
  //Serial.print("G ");
  //Serial.print("X: "); Serial.print(Ogyro.x); Serial.print("  ");
  gyr_x = float(Ogyro.x);
  //Serial.print("Y: "); Serial.print(Ogyro.y); Serial.print("  ");
  gyr_y = float(Ogyro.y);
  // Serial.print("Z: "); Serial.print(Ogyro.z); Serial.print("  ");
  gyr_z = float(Ogyro.z);
  //Serial.println("g");

  /* Display the accelerometer results (accelerometer data is in m/s^2) */
  //Serial.print("A ");
  //Serial.print("X: "); Serial.print(Oaccel.x); Serial.print("  ");
  acc_x = float(Oaccel.x);
  //Serial.print("Y: "); Serial.print(Oaccel.y); Serial.print("  ");
  acc_y = float(Oaccel.y);
  // Serial.print("Z: "); Serial.print(Oaccel.z); Serial.print("  ");
  acc_z = float(Oaccel.z);
  //Serial.println("m/s^2");

  // Serial.println("");

  // delay(500);
}
void bmx_initialize()
{
  //init the hardware bmx160
  if (bmx160.begin() != true) {
    Serial.println("init false");
    while (1);
  }
  //bmx160.setLowPower();   //disable the gyroscope and accelerometer sensor
  //bmx160.wakeUp();        //enable the gyroscope and accelerometer sensor
  //bmx160.softReset();     //reset the sensor

  /**
     enum{eGyroRange_2000DPS,
           eGyroRange_1000DPS,
           eGyroRange_500DPS,
           eGyroRange_250DPS,
           eGyroRange_125DPS
           }eGyroRange_t;
   **/
  //bmx160.setGyroRange(eGyroRange_125DPS);

  /**
      enum{eAccelRange_2G,
           eAccelRange_4G,
           eAccelRange_8G,
           eAccelRange_16G
           }eAccelRange_t;
  */
  //bmx160.setAccelRange(eAccelRange_2G);
  delay(100);
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
      // Serial.print("LAT: ");
      // Serial.println(gps.location.lat(), 6);
      //gps_lat = float(gps.location.lat());
      // Serial.print("LONG: ");
      // Serial.println(gps.location.lng(), 6);
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
