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

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef char s8;
typedef short s16;
typedef int s32;

const char* host = "esp32";
const char* ssid = "**********";        //U+Net850C
const char* password = "**********";    //C87568BJ$F//12345678///csdowu38
const char* ntpServer = "pool.ntp.org";
const char* mqtt_server = "192.168.11.196";
const char* topic = "home/master";
const long  gmtOffset_sec = 32400;
const int   daylightOffset_sec = 0;