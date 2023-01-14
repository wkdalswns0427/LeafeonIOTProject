// #include <Arduino.h>
#include "WiFi.h"
#include "time.h"
#include "DFRobot_BME280.h"
#include "DFRobot_CCS811.h"
#include "PMS.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define RUNNING_CORE 1
#define SDA_PIN 21
#define SCL_PIN 22
#define TX_PIN 1
#define RX_PIN 3
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* ssid = "Roboin";        //U+Net850C
const char* password = "roboin1234";    //C87568BJ$F//12345678///csdowu38
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 32400;
const int   daylightOffset_sec = 0;

// elements of SEN0335 : I2C
typedef DFRobot_BME280_IIC    BME;   
typedef DFRobot_CCS811        CCS;
BME    bme(&Wire, 0x76);   
CCS CCS811(&Wire, 0x5A);
#define SEA_LEVEL_PRESSURE    1015.0f

// PMS7003 sensor : UART - using PMSSerial
PMS pms(Serial1);
PMS::DATA pmsdata;

// data cluster
typedef struct SENDATA{
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

void printLocalTime() {
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        return;
    }
    // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    display.println(&timeinfo, "%H:%M:%S");
}

void displayInitialTime(){
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        return;
    }

    display.clearDisplay();
    display.setCursor(0, 24);
    display.setTextSize(1);
    display.println(&timeinfo, "%A, %B %d");
    display.println(&timeinfo, "%H:%M:%S");
    display.display();
    delay(3000);
}

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
        baseline = CCS811.readBaseLine();
        Serial.println(baseline, HEX);
        
    } else {
        Serial.println("Data is not ready!");
    }
}

void setupOLED(){
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;); // Don't proceed, loop forever
    }
    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(4, 28);
    display.println("** Begin Leafeon **");
    display.display();
    delay(2000);
}

void setupWiFi(){
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    resetDisplay();
    display.println("WIFI CONNECTED");
    display.print("IP :"); display.println(WiFi.localIP());
    delay(2000);
}

void resetDisplay(){
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
}

float *readBME(){
    float *data = (float*)malloc(sizeof(float)*4);
    
    data[0] = bme.getTemperature();
    data[1] = bme.getPressure();
    data[2] = bme.calAltitude(SEA_LEVEL_PRESSURE, data[1]);
    data[3] = bme.getHumidity();

    return data;
}

uint16_t *readCCS(){
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
  if (pms.readUntil(pmsdata)){
      data[0] = pmsdata.PM_AE_UG_1_0;
      data[1] = pmsdata.PM_AE_UG_2_5;
      data[2] = pmsdata.PM_AE_UG_1_0;
  }
  return data;
}

// main function
void sensortask( void *pvParameters){

  (void) pvParameters;

  float* BMEdata = readBME();
  uint16_t* CCSdata = readCCS();
  uint16_t* PMSdata = readPMS();
  SENDATA SENRESULT = 
          {BMEdata[0], BMEdata[1], BMEdata[2], BMEdata[3]
          ,CCSdata[0], CCSdata[1]
          ,PMSdata[0], PMSdata[1], PMSdata[2]};

  //================= this part shall be either posting or OLED =================
  resetDisplay();
  printLocalTime();
  display.print("temp(C): "); display.println(SENRESULT.temperature);
  display.print("P(Pa): "); display.println(SENRESULT.pressure);
//   display.print("alt(m):  "); display.println(SENRESULT.altitude);
  display.print("hum(%): "); display.println(SENRESULT.humidity);
  display.print("CO2(ppm): "); display.println(SENRESULT.eCO2);
  display.print("TVOC(ppb): "); display.println(SENRESULT.eTVOC);
  display.display();
  delay(5000);
  
  resetDisplay();
  display.println("micro particle(ug/m3)");
  display.print("PM 1.0: "); display.println(SENRESULT.PM_AE_UG_1_0);
  display.print("PM 2.5: "); display.println(SENRESULT.PM_AE_UG_2_5);
  display.print("PM 10.0: "); display.println(SENRESULT.PM_AE_UG_10_0);
  display.display();
  delay(5000);
  //==============================================================================

  CCS811.writeBaseLine(baseline);
}

void setup()
{
  Serial1.begin(PMS::BAUD_RATE, SERIAL_8N1, RX_PIN, TX_PIN);
  Serial.begin(9600);
  setupBME();
  setupCCS();
  setupOLED();
  setupWiFi();
  
  // init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  displayInitialTime();

  xTaskCreatePinnedToCore(
        sensortask,
        "sensortask",
        4096,
        NULL, // task function input
        1,
        NULL,
        RUNNING_CORE
    );
}

void loop(){}