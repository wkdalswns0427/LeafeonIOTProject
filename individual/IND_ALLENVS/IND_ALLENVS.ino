#include "DFRobot_BME280.h"
#include "DFRobot_CCS811.h"
#include "PMS.h"
#include "Wire.h"

typedef DFRobot_BME280_IIC    BME; 
typedef DFRobot_CCS811        CCS;

PMS pms(Serial1);
PMS::DATA data;
BME bme(&Wire, 0x76);   
CCS CCS811(&Wire, 0x5A);

uint8_t baseline = 0;

#define SEA_LEVEL_PRESSURE    1015.0f

struct BMEDATA{
    float temperature;
    uint32_t pressure;
    float altitude;
    float humidity;
};

struct BMEDATA *bmedata = malloc(sizeof(struct BMEDATA));

// show last sensor operate status
void printLastOperateStatus(BME::eStatus_t eStatus)
{
  switch(eStatus) {
  case BME::eStatusOK:    Serial.println("everything ok"); break;
  case BME::eStatusErr:   Serial.println("unknow error"); break;
  case BME::eStatusErrDeviceNotDetected:    Serial.println("device not detected"); break;
  case BME::eStatusErrParameter:    Serial.println("parameter error"); break;
  default: Serial.println("unknow status"); break;
  }
}

void setupSEN0335()
{
    Serial.println("bme read data test");
    while(bme.begin() != BME::eStatusOK) {
        Serial.println("bme begin faild");
        printLastOperateStatus(bme.lastOperateStatus);
        delay(2000);
    }
    Serial.println("bme begin success");
    delay(100);
    while(CCS811.begin() != 0){
            Serial.println("failed to init chip, please check if the chip connection is fine");
            delay(1000);
        }
    Serial.println("ccs begin success");
    delay(100);
    setBaselineCCS811();
}

void setBaselineCCS811(){
    if(CCS811.checkDataReady() == true){
        /*!
         * @brief Set baseline
         * @return baseline in clear air
         */
        baseline = CCS811.readBaseLine();
        Serial.println(baseline, HEX);
        
    } else {
        Serial.println("Data is not ready!");
    }
}

void setupPMS7003()
{
    Serial1.begin(PMS::BAUD_RATE, SERIAL_8N1, 3, 1);
}

void get

void setup()
{
  Serial.begin(115200);
  Serial.println("Serial Init");
  setupSEN0335();
  delay(100);
  setupPMS7003();
}

void loop()
{   
    bmedata->temperature = bme.getTemperature();
    bmedata->pressure = bme.getPressure();
    bmedata->altitude = bme.calAltitude(SEA_LEVEL_PRESSURE, press);
    bmedata->humidity = bme.getHumidity();

    Serial.println();
    Serial.println("======== start print ========");
    Serial.print("temperature (unit Celsius): "); Serial.println(BMEdata.temperature);
    Serial.print("pressure (unit pa):         "); Serial.println(BMEdata.pressure);
    Serial.print("altitude (unit meter):      "); Serial.println(BMEdata.altitude);
    Serial.print("humidity (unit percent):    "); Serial.println(BMEdata.humidity);
    Serial.println("========  end print  ========");

    delay(100);

    if(CCS811.checkDataReady() == true){
        Serial.print("CO2: ");
        Serial.print(CCS811.getCO2PPM());
        Serial.print("ppm, TVOC: ");
        Serial.print(CCS811.getTVOCPPB());
        Serial.println("ppb");
            
    } else {
        Serial.println("Data is not ready!");
    }

    CCS811.writeBaseLine(baseline);
    delay(900);

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
}