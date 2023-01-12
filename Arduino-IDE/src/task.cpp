/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/
#include "task.h"
#include "CCS811.h"
#include "BME280.h"
#include "PMS7003.h"
#include <Arduino.h>

void TaskCCS811(void *pvParameters)  // This is a task.
{
  (void) pvParameters;


  pinMode(LED_BUILTIN, OUTPUT);

  for (;;) // A Task shall never return or exit.
  {
    vTaskDelay(10);
  }
}

void TaskPMS7003(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  for (;;)
  {
    main_PMS7003();
    vTaskDelay(10);  // one tick delay (15ms) in between reads for stability
  }
}

void TaskBME280(void *pvParameters)
{
    (void) pvParameters;

    for(;;)
    {
      main_BME280();
      vTaskDelay(10);
    }
}


void ServerTask(void *pvParameters)
{
    (void) pvParameters;

    pinMode(LED_BUILT, OUTPUT);
    for(;;)
    { 
      digitalWrite(LED_BUILT, HIGH);   
      vTaskDelay(100);  
      digitalWrite(LED_BUILT, LOW);   
      vTaskDelay(100);  
      
    }
}

void TaskBlink(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
    
  If you want to know what pin the on-board LED is connected to on your ESP32 model, check
  the Technical Specs of your board.
*/

  // initialize digital LED_BUILTIN on pin 13 as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  for (;;) // A Task shall never return or exit.
  {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    vTaskDelay(100);  // one tick delay (15ms) in between reads for stability
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    vTaskDelay(100);  // one tick delay (15ms) in between reads for stability
  }
}
