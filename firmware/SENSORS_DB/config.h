#define LED_BUILTIN 5
#define RUNNING_CORE 1
#define BASE_CORE 0
#define SDA_PIN 21
#define SCL_PIN 22
#define TX_PIN 1
#define RX_PIN 3
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define uS_TO_S_FACTOR 1000000ULL
#define TIME_TO_SLEEP  3600   
#define SEA_LEVEL_PRESSURE    1015.0f
uint baseline = 0;

#define SENSOR_TEMP 1
#define SENSOR_HUMI 2
#define SENSOR_PRES 3
#define SENSOR_ALTI 4
#define SENSOR_eCO2 8
#define SENSOR_TVOC 9
#define SENSOR_PM01 16
#define SENSOR_PM25 17
#define SENSOR_PM10 18

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef char s8;
typedef short s16;
typedef int s32;

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

const char* host = "esp32";
const char* ssid = "U+Net850C";        //U+Net850C
const char* password = "C87568BJ$F";    //C87568BJ$F//12345678///csdowu38
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 32400;
const int   daylightOffset_sec = 0;
unsigned long timerDelay = 5000;
unsigned long lastTime = 0;
StaticJsonDocument<4096> sensorData;

String Device_id = "LMJ-001";
// shall become a domain someday
String serverName_full = "http://192.168.219.105:8000/postFullSensordata";
// String serverName_ind = "http://{local IP}:8000/postSensordata"

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