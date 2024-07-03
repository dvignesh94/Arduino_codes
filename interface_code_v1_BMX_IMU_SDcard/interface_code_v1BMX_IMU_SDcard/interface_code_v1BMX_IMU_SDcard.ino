// sd card header files declaration
#include <SPI.h>
#include <SD.h>

//imu bmx sensor header file declaration
#include <DFRobot_BMX160.h>


//sd card variable declaration
File myFile;

//imu bmx sensor variable declaration
DFRobot_BMX160 bmx160;

//global variables
float mag_x, mag_y, mag_z;
float acc_x, acc_y, acc_z;
float gyr_x, gyr_y, gyr_z;
void setup()
{
  Serial.begin(9600);
  sd_initialization();
  bmx_initialize();

}

void loop()
{
  bmx_process();
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
  myFile = SD.open("drifter.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {

    //acc value write to sd card
    Serial.print("acc x,y,z:"); Serial.print(acc_x); Serial.print(','); Serial.print(acc_y); Serial.print(','); Serial.print(acc_z);Serial.print("::");
    myFile.print("acc x,y,z:"); myFile.print(acc_x); myFile.print(','); myFile.print(acc_y); myFile.print(','); myFile.print(acc_z);myFile.print("::");

        //gyr value write to sd card
    Serial.print("gyr x,y,z:"); Serial.print(gyr_x); Serial.print(','); Serial.print(gyr_y); Serial.print(','); Serial.print(gyr_z);Serial.print("::");
    myFile.print("gyr x,y,z:"); myFile.print(gyr_x); myFile.print(','); myFile.print(gyr_y); myFile.print(','); myFile.print(gyr_z);myFile.print("::");
    
    //mag value write to sd card
    Serial.print("mag x,y,z:"); Serial.print(mag_x); Serial.print(','); Serial.print(mag_y); Serial.print(','); Serial.println(mag_z);
    myFile.print("mag x,y,z:"); myFile.print(mag_x); myFile.print(','); myFile.print(mag_y); myFile.print(','); myFile.println(mag_z);
   
    
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
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
