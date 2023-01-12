#include "PMS.h"
#include "Wire.h"

PMS pms(Serial1);
PMS::DATA data;

void setup()
{
    Serial1.begin(PMS::BAUD_RATE, SERIAL_8N1, 3, 1);
    Serial.begin(9600);

}

void loop()
{

  while (Serial1.available()) { Serial1.read(); }
  pms.requestRead();
  if (pms.readUntil(data))
  {
      Serial.print("PM 1.0 (ug/m3): "); 
      Serial.println(data.PM_AE_UG_1_0);
      Serial.print("PM 2.5 (ug/m3): "); 
      Serial.println(data.PM_AE_UG_2_5);
      Serial.print("PM 10.0 (ug/m3): "); 
      Serial.println(data.PM_AE_UG_10_0);

  }
  Serial.print("sequence\n");
}