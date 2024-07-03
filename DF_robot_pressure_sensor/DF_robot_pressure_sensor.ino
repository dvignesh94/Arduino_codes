const float  OffSet = 0.479 ;
float V, P;
void setup()
{
  Serial.begin(9600);        // open serial port, set the baud rate to 9600 bps
  Serial.println("/** Water pressure sensor demo **/");
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

  Serial.print("Voltage:");
  Serial.print(V, 3);
  Serial.println("V");

  Serial.print(" Pressure:");
  Serial.print(P, 1);
  Serial.println(" KPa");
  Serial.println();

  delay(500);
}
