#include "Plantower_PMS7003.h"
#include <stdio.h>
#include <stdlib.h>

Plantower_PMS7003 sensor = Plantower_PMS7003();

void setup()
{
  // Initialize the sensor with the serial device
  // that it's connected to. Hardware Serial1 is the
  // default, if no parameter is provided to init()

  Serial1.begin(9600);    // Note: 9600 baud
  sensor.init(&Serial1);
  Serial.begin(9600);
  Serial.println("Serial init");

}

void loop()
{
  // Use updateFrame() to read in sensor data in your
  // loop until hasNewData() returns true.
  sensor.updateFrame();

  // Note: once updateFrame() is called, all data is
  // invalid until hasNewData() returns true.
  if (sensor.hasNewData())
  {
    // PM1.0: getPM_1_0() 
    Serial.print("PM1.0: ");
    Serial.print(sensor.getPM_1_0());
    
    // PM2.5: getPM_2_5()
    Serial.print(" PM2.5: ");
    Serial.print(sensor.getPM_2_5());
    
    // PM10: getPM_10_0()
    Serial.print(" PM10: ");
    Serial.print(sensor.getPM_10_0());
    Serial.println();
  }

}