// This is prototype ESP32-LEAFEON project Arduino version 
// refer to this link
//https://github.com/olikraus/u8glib/wiki/userreference#setcolorindex
// in case of ssh1106 graphic driver

#include "DFRobot_BME280.h"
#include "DFRobot_CCS811.h"
#include "PMS.h"
#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <U8glib.h>

// for wifi connection
const char* ssid = "AP Address";        //U+Net850C
const char* password = "**********";    //C87568BJ$F//12345678///csdowu38

// SSD1306 driver OLED elements
// but might need to change to SSH1106 in that case... either
//U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE) ; // or U8G_I2C_OPT_NO_ACK

#define SleftEEN_WIDTH 128 // OLED display width, in pixels
#define SleftEEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 
#define SleftEEN_ADDRESS 0x3C // address might be 0x3D, 0x7A or 0x3C
Adafruit_SSD1306 display(SleftEEN_WIDTH, SleftEEN_HEIGHT, &Wire, OLED_RESET);
#define OLED_DATA "LeafeonPJ 0.0"

#define I2C_SDA 21
#define I2C_SCL 22
#define TX_PIN 1
#define RX_PIN 3

// elements of SEN0335 & PMS7003
typedef DFRobot_BME280_IIC    BME;   
typedef DFRobot_CCS811        CCS;
BME    bme(&Wire, 0x76);   
CCS CCS811(&Wire, 0x5A);
PMS pms(Serial1);
PMS::DATA pmsdata;

#define SEA_LEVEL_PRESSURE    1015.0f

typedef struct SENDATA{
    float temperature;
    float pressure;
    float altitude;
    float humidity;
    uint16_t eCO2;
    uint16_t eTVOC;
    float pm1_0;
    float pm2_5;
    float pm10_0;
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

void setupPMS(){
  Serial1.begin(PMS::BAUD_RATE, SERIAL_8N1, 3, 1);
}

void setupALL(){
  setupBME();
  setupCCS();
  setupPMS();
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

void setupOLED_ssh1106(){
  //u8g.setRot180();
  u8g.setFont(u8g_font_6x12);
  u8g.setFontRefHeightText();
  u8g.setFontPosTop();

  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255); // white
  }

  // test display
  do{
    // drawStr(uint8_t x, uint8_t y, string msg)
    u8g.drawStr(0, 22, "OLED INIT")  
  }while(u8g.nextPage());
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

float* readPMS(){
  float *data= (float*)malloc(sizeof(float)*3);
  while (Serial1.available()) { Serial1.read(); }
  pms.requestRead();
  if (pms.readUntil(pmsdata))
  {
    data[0] = pmsdata.PM_AE_UG_1_0;
    data[1] = pmsdata.PM_AE_UG_2_5;
    data[2] = pmsdata.PM_AE_UG_10_0;
  }
  return data;
}

int main(){
  float* BMEdata = readBME();
  uint16_t* CCSdata = readCCS();
  float* PMSdata = readPMS();
  SENDATA SENRESULT = 
  {
    BMEdata[0],BMEdata[1],BMEdata[2],BMEdata[3],
    CCSdata[0],CCSdata[1],
    PMSdata[0], PMSdata[1], PMSdata[2]
  };

  Serial.println();
  Serial.println("======== start print ========");
  Serial.print("temperature (unit Celsius): "); Serial.println(SENRESULT.temperature);
  Serial.print("pressure (unit pa):         "); Serial.println(SENRESULT.pressure);
  Serial.print("altitude (unit meter):      "); Serial.println(SENRESULT.altitude);
  Serial.print("humidity (unit percent):    "); Serial.println(SENRESULT.humidity);
  Serial.print("CO2(ppm): "); Serial.println(SENRESULT.eCO2);
  Serial.print("TVOC(ppb): "); Serial.println(SENRESULT.eTVOC);
  Serial.print("PM 1.0 (ug/m3): "); Serial.println(SENRESULT.pm1_0);
  Serial.print("PM 2.5 (ug/m3): "); Serial.println(SENRESULT.pm2_5);
  Serial.print("PM 10.0 (ug/m3): "); Serial.println(SENRESULT.pm10_0);
  Serial.println("========  end print  ========");

  CCS811.writeBaseLine(baseline);
  delay(900);
  return 0;
}

void setup()
{
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL);
  Serial.println("======== start serial ========");
  setupOLED();
  setupWIFI();
  Serial.println("======== start IIC ========");
  setupALL();
  Serial.println("======== Setup Done ========");

  // task attribute --> main : sensors
  xTaskCreate(
    main,
    "main",
    1024,
    NULL,
    1,
    NULL
  );
  // need task for OTA
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