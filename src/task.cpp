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

    pinMode(LED_BUILTIN, OUTPUT);
    for(;;)
    { 
      digitalWrite(LED_BUILTIN, HIGH);   
      vTaskDelay(100);  
      digitalWrite(LED_BUILTIN, LOW);   
      vTaskDelay(100);  
      
    }
}