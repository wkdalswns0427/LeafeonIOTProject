#ifndef TASKS_H
#define TASKS_H
#endif

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET     -1 
#define SleftEEN_WIDTH 128 // OLED display width, in pixels
#define SleftEEN_HEIGHT 32 // OLED display height, in pixels
#define SleftEEN_ADDRESS 0x3C 
Adafruit_SSD1306 display(SleftEEN_WIDTH, SleftEEN_HEIGHT, &Wire, OLED_RESET);

int cnt1 = 0;
int cnt2 = 100;

void task1(void *parameters){
    for(;;){
        Serial.print("task1_counter :");
        Serial.println(cnt1++);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void task2(void *parameters){
    for(;;){
        Serial.print("task2_counter :");
        Serial.println(cnt2++);
        vTaskDelay(1500 / portTICK_PERIOD_MS);
    }
}

void task3(void *parameters){
    for(;;){
        display.clearDisplay();
        display.setCursor( 0,  0);
        display.print("t3_cnt : ");
        display.println(cnt1+cnt2);
        display.display();
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}