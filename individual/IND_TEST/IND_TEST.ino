#include "DFRobot_BME280.h"
#include "DFRobot_CCS811.h"
#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SEA_LEVEL_PRESSURE    1015.0f

#define SleftEEN_WIDTH 128 // OLED display width, in pixels
#define SleftEEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     -1 

#define SleftEEN_ADDRESS 0x3C 
Adafruit_SSD1306 display(SleftEEN_WIDTH, SleftEEN_HEIGHT, &Wire, OLED_RESET);

#define OLED_DATA "fw_w ver. 0.9"

#define I2C_SDA 12
#define I2C_SCL 11
TwoWire I2Ccustom = TwoWire(0);

const char* host = "esp32";
const char* ssid = "Drimaes_AP";
const char* password = "drimaes1303!";

// elements of SEN0335
typedef DFRobot_BME280_IIC    BME;   
typedef DFRobot_CCS811        CCS;
BME    bme(&I2Ccustom, 0x76);   
CCS CCS811(&I2Ccustom, 0x5A);

typedef struct SENDATA{
    float temperature;
    float pressure;
    float altitude;
    float humidity;
    uint16_t eCO2;
    uint16_t eTVOC;
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
    display.clearDisplay();
    display.print("bme begin");
    display.display();
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

void setupOLED(){
  if(!display.begin(SSD1306_SWITCHCAPVCC, SleftEEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }

  delay(100); 
  display.clearDisplay();

  //display.setRotation(2); // Uncomment to rotate display 180 degrees
  display.setTextSize(1);  
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.cp437(true); 

  display.setCursor(0, 0);  // 0,0 / 8 / 16 / 24
  display.print(OLED_DATA);
  display.setCursor(0, 8); 
  display.print("OLED INIT");
  display.display();
}

void setupWIFI(){
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);

  display.clearDisplay();
  display.setCursor( 0,  0);
  display.print("Connected: ");
  display.println(ssid);
  display.setCursor( 0,  8);
  display.print("IP: ");
  display.print(WiFi.localIP());
  display.display();
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

int main(){
  float* BMEdata = readBME();
  delay(100);
  uint16_t* CCSdata = readCCS();
  SENDATA SENRESULT = {BMEdata[0],BMEdata[1],BMEdata[2],BMEdata[3],CCSdata[0],CCSdata[1]};

  Serial.println();
  Serial.println("======== start print ========");
  Serial.print("temperature (unit Celsius): "); Serial.println(SENRESULT.temperature);
  Serial.print("pressure (unit pa):         "); Serial.println(SENRESULT.pressure);
  Serial.print("altitude (unit meter):      "); Serial.println(SENRESULT.altitude);
  Serial.print("humidity (unit percent):    "); Serial.println(SENRESULT.humidity);
  Serial.print("CO2(ppm): "); Serial.println(SENRESULT.eCO2);
  Serial.print("TVOC(ppb): "); Serial.println(SENRESULT.eTVOC);
  Serial.println("========  end print  ========");

  CCS811.writeBaseLine(baseline);
  delay(900);
  return 0;
}

void setup()
{
  Serial.begin(115200);
  Serial.println("======== start serial ========");
  setupOLED();
  setupWIFI();
  // I2Ccustom.begin(I2C_SDA,I2C_SCL,100000);
  bool status = I2Ccustom.begin(I2C_SDA, I2C_SCL, 100000);
  display.setCursor(0,16);
  display.print(status);
  display.display();
  Serial.println("======== start IIC ========");
  setupBME();
  delay(100);
  setupCCS();
  Serial.println("======== Setup Done ========");
}

void loop()
{
    main();
    delay(10000);
}


//--------------------- bewlow 22.12.21 legacy -------------------------------
// #include "DFRobot_BME280.h"
// #include "DFRobot_CCS811.h"
// #define SEA_LEVEL_PRESSURE    1015.0f

// // elements of SEN0335
// typedef DFRobot_BME280_IIC    BME;   
// typedef DFRobot_CCS811        CCS;
// BME    bme(&Wire, 0x76);   
// CCS CCS811(&Wire, 0x5A);

// typedef struct{
//     float temperature;
//     float pressure;
//     float altitude;
//     float humidity;
//     uint16_t eCO2;
//     uint16_t eTVOC;
// }SENDATA;

// uint baseline = 0;

// // show last sensor operate status
// void printLastOperateStatus(BME::eStatus_t eStatus)
// {
//   switch(eStatus) {
//   case BME::eStatusOK:    Serial.println("everything ok"); break;
//   case BME::eStatusErr:   Serial.println("unknow error"); break;
//   case BME::eStatusErrDeviceNotDetected:    Serial.println("device not detected"); break;
//   case BME::eStatusErrParameter:    Serial.println("parameter error"); break;
//   default: Serial.println("unknow status"); break;
//   }
// }

// void setupBME(){
//     Serial.println("bme read data test");
//     while(bme.begin() != BME::eStatusOK) {
//         Serial.println("bme begin faild");
//         printLastOperateStatus(bme.lastOperateStatus);
//         delay(2000);
//     }
//     Serial.println("bme begin success");
// }

// void setupCCS(){
//     while(CCS811.begin() != 0){
//             Serial.println("failed to init chip, please check if the chip connection is fine");
//             delay(1000);
//         }
//     Serial.println("ccs begin success");
//     delay(1000);
//     Serial.println("ccs set baseline");
//     if(CCS811.checkDataReady() == true){
//         /*!
//          * @brief Set baseline
//          * @return baseline in clear air
//          */
//         baseline = CCS811.readBaseLine();
//         Serial.println(baseline, HEX);
        
//     } else {
//         Serial.println("Data is not ready!");
//     }
// }

// // int *a = (int *)malloc(sizeof(int) * 갯수);
// // int *a = new int[갯수];

// float* readBME(){
//     // float data[4];
//     float *data = (float*)malloc(sizeof(float)*4);
//     data[0] = bme.getTemperature();
//     data[1] = bme.getPressure();
//     data[2] = bme.calAltitude(SEA_LEVEL_PRESSURE, data[1]);
//     data[3] = bme.getHumidity();

//     return data;
// }

// uint16_t* readCCS(){
//     uint16_t *data = (uint16_t*)malloc(sizeof(uint16_t)*2);
//     if(CCS811.checkDataReady() == true){
//         data[0] = CCS811.getCO2PPM();
//         data[1] = CCS811.getTVOCPPB();
//     }

//     return data;
// }

// int main(){
//   float* BMEdata = readBME();
//   delay(100);
//   uint16_t* CCSdata = readCCS();
//   SENDATA SENRESULT = {BMEdata[0],BMEdata[1],BMEdata[2],BMEdata[3],CCSdata[0],CCSdata[1]};

//   Serial.println();
//   Serial.println("======== start print ========");
//   Serial.print("temperature (unit Celsius): "); Serial.println(SENRESULT.temperature);
//   Serial.print("pressure (unit pa):         "); Serial.println(SENRESULT.pressure);
//   Serial.print("altitude (unit meter):      "); Serial.println(SENRESULT.altitude);
//   Serial.print("humidity (unit percent):    "); Serial.println(SENRESULT.humidity);
//   Serial.print("CO2(ppm): "); Serial.println(SENRESULT.eCO2);
//   Serial.print("TVOC(ppb): "); Serial.println(SENRESULT.eTVOC);
//   Serial.println("========  end print  ========");

//   CCS811.writeBaseLine(baseline);
//   delay(900);
//   return 0;
// }

// void setup()
// {
//   Serial.begin(115200);
//   setupBME();
//   delay(100);
//   setupCCS();
//   Serial.println("======== Setup Done ========");
// }

// void loop()
// {
//     main();
//     delay(10000);
// }