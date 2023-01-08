#include "DFRobot_BME280.h"
#include "DFRobot_CCS811.h"
#include "PMS.h"
#define SEA_LEVEL_PRESSURE    1015.0f

// elements of SEN0335
typedef DFRobot_BME280_IIC    BME;   
typedef DFRobot_CCS811        CCS;
BME    bme(&Wire, 0x76);   
CCS CCS811(&Wire, 0x5A);

PMS pms(Serial1);
PMS::DATA pmsdata;

typedef struct{
    float temperature;
    float pressure;
    float altitude;
    float humidity;
    uint16_t eCO2;
    uint16_t eTVOC;
    uint16_t PM_AE_UG_1_0;
    uint16_t PM_AE_UG_2_5;
    uint16_t PM_AE_UG_10_0;
}SENDATA;

uint baseline = 0;

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

void setupBME(){
    Serial.println("bme read data test");
    while(bme.begin() != BME::eStatusOK) {
        Serial.println("bme begin faild");
        printLastOperateStatus(bme.lastOperateStatus);
        delay(2000);
    }
    Serial.println("bme begin success");
}

void setupCCS(){
    while(CCS811.begin() != 0){
            Serial.println("failed to init chip, please check if the chip connection is fine");
            delay(1000);
        }
    Serial.println("ccs begin success");
    delay(1000);
    Serial.println("ccs set baseline");
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

// int *a = (int *)malloc(sizeof(int) * 갯수);
// int *a = new int[갯수];

float* readBME(){
    // float data[4];
    float *data = (float*)malloc(sizeof(float)*4);
    data[0] = bme.getTemperature();
    data[1] = bme.getPressure();
    data[2] = bme.calAltitude(SEA_LEVEL_PRESSURE, data[1]);
    data[3] = bme.getHumidity();

    return data;
}

uint16_t* readCCS(){
    uint16_t *data = (uint16_t*)malloc(sizeof(uint16_t)*2);
    if(CCS811.checkDataReady() == true){
        data[0] = CCS811.getCO2PPM();
        data[1] = CCS811.getTVOCPPB();
    }

    return data;
}

uint16_t *readPMS(){
  uint16_t *data = (uint16_t*)malloc(sizeof(uint16_t)*3);
  while (Serial1.available()) { Serial1.read(); }
  pms.requestRead();
  if (pms.readUntil(pmsdata))
  {

      data[0] = pmsdata.PM_AE_UG_1_0;
      data[1] = pmsdata.PM_AE_UG_2_5;
      data[2] = pmsdata.PM_AE_UG_1_0;
  }
  return data;
}

int main(){
  float* BMEdata = readBME();
  uint16_t* CCSdata = readCCS();
  uint16_t* PMSdata = readPMS();
  SENDATA SENRESULT = 
          {BMEdata[0], BMEdata[1], BMEdata[2], BMEdata[3]
          ,CCSdata[0], CCSdata[1]
          ,PMSdata[0], PMSdata[1], PMSdata[2]};

  Serial.println();
  Serial.println("======== start print ========");
  Serial.print("temperature (unit Celsius): "); Serial.println(SENRESULT.temperature);
  Serial.print("pressure (unit pa):         "); Serial.println(SENRESULT.pressure);
  Serial.print("altitude (unit meter):      "); Serial.println(SENRESULT.altitude);
  Serial.print("humidity (unit percent):    "); Serial.println(SENRESULT.humidity);
  Serial.print("CO2(ppm): "); Serial.println(SENRESULT.eCO2);
  Serial.print("TVOC(ppb): "); Serial.println(SENRESULT.eTVOC);
  Serial.print("PM 1.0 (ug/m3): "); Serial.println(SENRESULT.PM_AE_UG_1_0);
  Serial.print("PM 2.5 (ug/m3): "); Serial.println(SENRESULT.PM_AE_UG_2_5);
  Serial.print("PM 10.0 (ug/m3): "); Serial.println(SENRESULT.PM_AE_UG_10_0);
  Serial.println("========  end print  ========");

  CCS811.writeBaseLine(baseline);
  delay(900);
  return 0;
}

void setup()
{
  Serial.println("======== Begin Leafeon ========");
  Serial1.begin(PMS::BAUD_RATE, SERIAL_8N1, 3, 1);
  Serial.println("======== Setup UART Done ========");
  Serial.begin(9600);
  setupBME();
  setupCCS();
  Serial.println("======== Setup I2C Done ========");
  delay(100);
  Serial.println("======== Setup ALL Done ========");
}

void loop()
{
    main();
    delay(1000);
    Serial.println("======== loop ========");
}