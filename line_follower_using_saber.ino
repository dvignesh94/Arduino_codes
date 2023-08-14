#include <QTRSensors.h>
#include <Servo.h>
Servo myservo, myservo1;
QTRSensors qtr;
#define Kp  0.4
// experiment to determine this, start by something small that just makes your bot follow the line at a slow speed
#define Kd  1.2
// experiment to determine this, slowly increase the speeds and adjust this value. ( Note: Kp < Kd)
//#define rightenable 6
#define rightMotor1 6
#define rightMotor2 11
#define leftMotor1 3
#define leftMotor2 5
//#define leftenable 5
#define rightMaxSpeed 250 // max speed of the robot
#define leftMaxSpeed 250// max speed of the robot
#define rightBaseSpeed 240 // this is the speed at which the motors should spin when the robot is perfectly on the line
#define leftBaseSpeed 240
const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];
int lastError = 0;
int temprm, templm;
void setup()
{
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]) {
    A8, A9, A10, A11, A12, A13, A14, A15
  }, SensorCount);
  //qtr.setEmitterPin(2);
  myservo.attach(13); // Use PWM pin 14 to control Sabertooth.
  myservo1.attach(12); // Use PWM pin 14 to control Sabertooth.

  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
  //  pinMode(rightenable, OUTPUT);
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  // pinMode(leftenable, OUTPUT);
  Serial.begin(9600);
  for (uint8_t i = 0; i < 250; i++)
  {
    qtr.calibrate();
    delay(20);
  }
}

void loop()
{
  uint16_t position = qtr.readLineBlack(sensorValues);
  Serial.println(position);
  int error = position - 3600;
  int motorSpeed = Kp * error + Kd * (error - lastError);
  lastError = error;

  int rightMotorSpeed = rightBaseSpeed + motorSpeed;
  int leftMotorSpeed = leftBaseSpeed - motorSpeed;

  if (rightMotorSpeed > rightMaxSpeed ) rightMotorSpeed = rightMaxSpeed; // prevent the motor from going beyond max speed
  if (leftMotorSpeed > leftMaxSpeed ) leftMotorSpeed = leftMaxSpeed; // prevent the motor from going beyond max speed
  if (rightMotorSpeed < 0) rightMotorSpeed = 0; // keep the motor speed positive
  if (leftMotorSpeed < 0) leftMotorSpeed = 0; // keep the motor speed positive

  {
    temprm = map(rightMotorSpeed, 0, 250, 92, 160);
    templm = map(leftMotorSpeed, 0, 250, 92, 160);
    myservo.write(temprm);
    myservo1.write(templm);
  Serial.println(temprm);
  Serial.println(templm);

    //   digitalWrite(rightenable, HIGH);
    // digitalWrite(leftenable, HIGH);// move forward with appropriate speeds
    // analogWrite(rightMotor1, 0);
    // analogWrite(rightMotor2, rightMotorSpeed);
    // analogWrite(leftMotor1, leftMotorSpeed);
    // analogWrite(leftMotor2, 0);
  }
}

