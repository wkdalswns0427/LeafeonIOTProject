#include <Arduino.h>
#include "WiFi.h"
#include "time.h"
#include "DFRobot_BME280.h"
#include "DFRobot_CCS811.h"
#include "PMS.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <PubSubClient.h>
#include "config.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// elements of SEN0335 : I2C
typedef DFRobot_BME280_IIC    BME;   
typedef DFRobot_CCS811        CCS;
BME    bme(&Wire, 0x76);   
CCS CCS811(&Wire, 0x5A);
#define SEA_LEVEL_PRESSURE    1015.0f

// PMS7003 sensor : UART - using PMSSerial
PMS pms(Serial1);
PMS::DATA pmsdata;

// web update server
WebServer server(80);
WiFiClient espClient;
PubSubClient client(espClient);

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
TaskHandle_t Task1;
TaskHandle_t Task2;

// ------------------------------------------------------------------------------------------
// these are not needed when using external web
/* Style */
String style =
"<style>#file-input,input{width:100%;height:44px;border-radius:4px;margin:10px auto;font-size:15px}"
"input{background:#f1f1f1;border:0;padding:0 15px}body{background:#3498db;font-family:sans-serif;font-size:14px;color:#777}"
"#file-input{padding:0;border:1px solid #ddd;line-height:44px;text-align:left;display:block;cursor:pointer}"
"#bar,#prgbar{background-color:#f1f1f1;border-radius:10px}#bar{background-color:#3498db;width:0%;height:10px}"
"form{background:#fff;max-width:258px;margin:75px auto;padding:30px;border-radius:5px;text-align:center}"
".btn{background:#3498db;color:#fff;cursor:pointer}</style>";

const char* loginIndex =
 "<form name='loginForm'>"
    "<table width='20%' bgcolor='A09F9F' align='center'>"
        "<tr>"
            "<td colspan=2>"
                "<center><font size=4><b>ESP32 Login Page</b></font></center>"
                "<br>"
            "</td>"
            "<br>"
            "<br>"
        "</tr>"
        "<tr>"
             "<td>Username:</td>"
             "<td><input type='text' size=25 name='userid'><br></td>"
        "</tr>"
        "<br>"
        "<br>"
        "<tr>"
            "<td>Password:</td>"
            "<td><input type='Password' size=25 name='pwd'><br></td>"
            "<br>"
            "<br>"
        "</tr>"
        "<tr>"
            "<td><input type='submit' onclick='check(this.form)' value='Login'></td>"
        "</tr>"
    "</table>"
"</form>"
"<script>"
    "function check(form)"
    "{"
    "if(form.userid.value=='admin' && form.pwd.value=='admin')"
    "{"
    "window.open('/serverIndex')"
    "}"
    "else"
    "{"
    " alert('Error Password or Username')/*displays error message*/"
    "}"
    "}"
"</script>";

/*
 * Server Index Page
 */

const char* serverIndex =
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
   "<input type='file' name='update'>"
        "<input type='submit' value='Update'>"
    "</form>"
 "<div id='prg'>progress: 0%</div>"
 "<script>"
  "$('form').submit(function(e){"
  "e.preventDefault();"
  "var form = $('#upload_form')[0];"
  "var data = new FormData(form);"
  " $.ajax({"
  "url: '/update',"
  "type: 'POST',"
  "data: data,"
  "contentType: false,"
  "processData:false,"
  "xhr: function() {"
  "var xhr = new window.XMLHttpRequest();"
  "xhr.upload.addEventListener('progress', function(evt) {"
  "if (evt.lengthComputable) {"
  "var per = evt.loaded / evt.total;"
  "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
  "}"
  "}, false);"
  "return xhr;"
  "},"
  "success:function(d, s) {"
  "console.log('success!')"
 "},"
 "error: function (a, b, c) {"
 "}"
 "});"
 "});"
 "</script>";
// ------------------------------------------------------------------------------------------

void printLocalTime() {
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        return;
    }
    display.println(&timeinfo, "%H:%M:%S");
}

void resetDisplay(){
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
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
  case BME::eStatusErrDeviceNotDetected:    /*Serial.println("device not detected");*/ break;
  case BME::eStatusErrParameter:    Serial.println("parameter error"); break;
  default: Serial.println("unknow status"); break;
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

// callback function --> sleep, reboot control from main broker server
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageServer;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageServer += (char)message[i];
  }
  Serial.println();

  if (String(topic) == topic) {
    Serial.print("Changing output to ");
    if(messageServer == "sleep"){
      Serial.println("sleep");
      digitalWrite(LED_BUILTIN, HIGH);
      //************* add deepsleep here ***************
    }
    else if(messageServer == "reboot"){
      Serial.println("reboot");
      digitalWrite(LED_BUILTIN, LOW);
      //************* add reboot here **************
    }
  }
}

void setupMQTT(){
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("LeafeonCLient")) {
      Serial.println("connected");  
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }

  client.subscribe(topic);
}

void setupBME(){
    Serial.println("bme read data test");
    while(bme.begin() != BME::eStatusOK) {
        // Serial.println("bme begin faild");
        printLastOperateStatus(bme.lastOperateStatus);
        delay(2000);
    }
    Serial.println("bme begin success");
}

void setupCCS(){
    while(CCS811.begin() != 0){
            // Serial.println("failed to init chip, please check if the chip connection is fine");
            delay(1000);
        }
    Serial.println("ccs begin success");
    delay(1000);
    // Serial.println("ccs set baseline");
    if(CCS811.checkDataReady() == true){
        baseline = CCS811.readBaseLine();
    } else {
        Serial.println("Data is not ready!");
    }
}

void setupOLED(){
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
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
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void setupWebServer(){
      /*use mdns for host name resolution*/
    if (!MDNS.begin(host)) { //http://esp32.local
        Serial.println("Error setting up MDNS responder!");
        while (1) {
        delay(1000);
        }
    }
    Serial.println("mDNS responder started");
    /*return index page which is stored in serverIndex */
    server.on("/", HTTP_GET, []() {
        server.sendHeader("Connection", "close");
        server.send(200, "text/html", loginIndex);
    });
    server.on("/serverIndex", HTTP_GET, []() {
        server.sendHeader("Connection", "close");
        server.send(200, "text/html", serverIndex);
    });
    /*handling uploading firmware file */
    server.on("/update", HTTP_POST, []() {
        server.sendHeader("Connection", "close");
        server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
        ESP.restart();
    }, []() {
        HTTPUpload& upload = server.upload();
        if (upload.status == UPLOAD_FILE_START) {
        Serial.printf("Update: %s\n", upload.filename.c_str());
        if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
            Update.printError(Serial);
        }
        } else if (upload.status == UPLOAD_FILE_WRITE) {
        /* flashing firmware to ESP*/
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
            Update.printError(Serial);
        }
        } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) { //true to set the size to the current progress
            Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
            Update.printError(Serial);
        }
        }
    });
    server.begin();
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

void publishData(char *serialData){
  if (!client.connected()) {
    reconnect();
  }
  client.publish("Leafeon/sensdata", serialData);
}

// main function
void sensorTask( void *pvParameters){

  (void) pvParameters;
    while(true){
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
        display.print("hum(%): "); display.println(SENRESULT.humidity);
        display.print("CO2(ppm): "); display.println(SENRESULT.eCO2);
        display.print("TVOC(ppb): "); display.println(SENRESULT.eTVOC);
        display.display();
        delay(3500);
        
        resetDisplay();
        display.println("micro particle(ug/m3)");
        display.print("PM 1.0: "); display.println(SENRESULT.PM_AE_UG_1_0);
        display.print("PM 2.5: "); display.println(SENRESULT.PM_AE_UG_2_5);
        display.print("PM 10.0: "); display.println(SENRESULT.PM_AE_UG_10_0);
        display.display();
        delay(3500);

        resetDisplay();
        display.println("OTA FW update");
        display.print("IP: "); display.println(WiFi.localIP());
        display.display();
        delay(3000);
        //==============================================================================

        CCS811.writeBaseLine(baseline);
        //publishData(//char_data);
    }
}

void serverTask(void *pvParameters){
    (void) pvParameters;
    while(true){
        server.handleClient();
    }
}

void setup()
{
  //==============================================================================
  Serial1.begin(PMS::BAUD_RATE, SERIAL_8N1, RX_PIN, TX_PIN);
  Serial.begin(115200);
  Serial.print("Leafeon V.1.3");
  setupWiFi();
  setupWebServer();
  setupMQTT();
  setupBME();
  setupCCS();
  setupOLED();
  //==============================================================================
  
  // init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  displayInitialTime();

    xTaskCreatePinnedToCore(
        sensorTask,
        "sensorTask",
        4096,
        NULL, // task function input
        2,
        &Task1,
        RUNNING_CORE
    );

    xTaskCreatePinnedToCore(
        serverTask,
        "serverTask",
        4096,
        NULL, // task function input
        1,
        &Task2,
        RUNNING_CORE
    );
}

void loop(){}