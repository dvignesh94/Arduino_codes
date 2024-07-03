#include <MPU9250_WE.h>
#include <Wire.h>
#define MPU9250_ADDR 0x68
#include <SPI.h>
#include <SD.h>
File myFile;
MPU9250_WE myMPU9250 = MPU9250_WE(MPU9250_ADDR);
void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");

  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
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
  myMPU9250.setSampleRateDivider(5);
  myMPU9250.setAccRange(MPU9250_ACC_RANGE_2G);
  myMPU9250.enableAccDLPF(true);
}

void loop() {
  xyzFloat accRaw = myMPU9250.getAccRawValues();
  xyzFloat accCorrRaw = myMPU9250.getCorrectedAccRawValues();
  xyzFloat gValue = myMPU9250.getGValues();
  xyzFloat angle = myMPU9250.getAngles();

  float resultantG = myMPU9250.getResultantG(gValue);
  /*
    Serial.println("Raw acceleration values (x,y,z):");
    Serial.print(accRaw.x); Serial.print("   "); Serial.print(accRaw.y); Serial.print("   "); Serial.println(accRaw.z);
    Serial.println("Corrected ('calibrated') acceleration values (x,y,z):");
    Serial.print(accCorrRaw.x); Serial.print("   "); Serial.print(accCorrRaw.y); Serial.print("   "); Serial.println(accCorrRaw.z);
    Serial.println("g values (x,y,z):");
    Serial.print(gValue.x); Serial.print("   "); Serial.print(gValue.y); Serial.print("   "); Serial.println(gValue.z);
    Serial.print("Resultant g: "); Serial.println(resultantG); // should always be 1 g if only gravity acts on the sensor.
    Serial.println();
    /* //Angles are also based on the corrected raws. Angles are simply calculated by
      // angle = arcsin(g Value)
    Serial.print("Angle x  = "); Serial.print(angle.x); Serial.print("  |  Angle y  = "); Serial.print(angle.y); Serial.print("  |  Angle z  = "); Serial.println(angle.z);
    Serial.print("Orientation of the module: ");
    Serial.println(myMPU9250.getOrientationAsString());
    Serial.println();*/

  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    //raw acceleration values
    myFile.print(accRaw.x); myFile.print(","); myFile.print(accRaw.y); myFile.print(","); myFile.print(accRaw.z); myFile.print(",");
    //caliberated acceleration
    myFile.print(accCorrRaw.x); myFile.print(","); myFile.print(accCorrRaw.y); myFile.print(","); myFile.print(accCorrRaw.z); myFile.print(",");
    //G values
    myFile.print(gValue.x); myFile.print(","); myFile.print(gValue.y); myFile.print(","); myFile.print(gValue.z); myFile.print(",");
    // resultant g value
    myFile.print(resultantG); myFile.print(",");
    //angles
    myFile.print(angle.x); myFile.print(","); myFile.print(angle.y); myFile.print(","); myFile.print(angle.z); myFile.print(",");
    //orientation
    myFile.println(myMPU9250.getOrientationAsString());
    // close the file:
    myFile.close();
    ////////////////////////////////////////////////////////////////////////////////////
    Serial.println("writing....");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    //raw acceleration values
    //Serial.print(accRaw.x); Serial.print(","); Serial.print(accRaw.y); Serial.print(","); Serial.print(accRaw.z); Serial.print(",");
    //caliberated acceleration
    //Serial.print(accCorrRaw.x); Serial.print(","); Serial.print(accCorrRaw.y); Serial.print(","); Serial.print(accCorrRaw.z); Serial.print(",");
    //G values
    //Serial.print(gValue.x); Serial.print(","); Serial.print(gValue.y); Serial.print(","); Serial.print(gValue.z); Serial.print(",");
    // resultant g value
    //Serial.print(resultantG); Serial.print(",");
    //angles
    //Serial.print(angle.x); Serial.print(","); Serial.print(angle.y); Serial.print(","); Serial.print(angle.z); Serial.print(",");
    //orientation
    //Serial.println(myMPU9250.getOrientationAsString());
    // close the file:
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  delay(1000);
}
