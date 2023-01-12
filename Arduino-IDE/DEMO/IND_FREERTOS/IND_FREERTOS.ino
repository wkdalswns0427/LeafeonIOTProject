#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "TASK.h"

// #define OLED_RESET     -1 
// #define SleftEEN_WIDTH 128 // OLED display width, in pixels
// #define SleftEEN_HEIGHT 32 // OLED display height, in pixels
// #define SleftEEN_ADDRESS 0x3C 
// Adafruit_SSD1306 display(SleftEEN_WIDTH, SleftEEN_HEIGHT, &Wire, OLED_RESET);

#define OLED_DATA "fw_RTOS v.1.0"

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

void setup(){
    Serial.begin(115200);
    setupOLED();
    xTaskCreate(
        task1,
        "task1",
        1024,
        NULL, // task function input
        1,
        NULL
    );
    xTaskCreate(
        task2,
        "task2",
        1024,
        NULL, // task function input
        1,
        NULL
    );
    xTaskCreate(
        task3,
        "task3",
        1024,
        NULL, // task function input
        1,
        NULL
    );
}

void loop(){
    
}

// void task1(void *parameters){
//     for(;;){
//         Serial.print("task1_counter :");
//         Serial.println(cnt1++);
//         vTaskDelay(1000 / portTICK_PERIOD_MS);
//     }
// }

// void task2(void *parameters){
//     for(;;){
//         Serial.print("task2_counter :");
//         Serial.println(cnt2++);
//         vTaskDelay(1500 / portTICK_PERIOD_MS);
//     }
// }

// void task3(void *parameters){
//     for(;;){
//         Serial.print("task3_counter :");
//         display.clearDisplay();
//         display.setCursor( 0,  0);
//         display.print("t3_cnt : ");
//         display.println(cnt1+cnt2);
//         display.display();
//         vTaskDelay(3000 / portTICK_PERIOD_MS);
//     }
// }