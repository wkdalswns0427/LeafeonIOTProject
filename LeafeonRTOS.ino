#include "config.h"
#include "mywifi.h"
#include "CCS811.h"
#include "BME280.h"
#include "PMS7003.h"

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

#define STACK_SIZE 1024


void TaskCCS811( void *pvParameters );
void TaskPMS7003( void *pvParameters );
void TaskBME280( void *pvParameters );


void setup() {
  
  Serial.begin(115200);
  
  xTaskCreatePinnedToCore(
    TaskCCS811
    ,  "TaskCCS811"   
    ,  STACK_SIZE  
    ,  NULL
    ,  2  
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
    TaskPMS7003
    ,  "TaskPMS7003"
    ,  STACK_SIZE  
    ,  NULL
    ,  1  // Priority
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
    TaskBME280
    ,  "TaskBME280"
    ,  STACK_SIZE  
    ,  NULL
    ,  0  // Priority
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);

    xTaskCreatePinnedToCore(
    ServerTask
    ,  "ServerTask"
    ,  STACK_SIZE  
    ,  NULL
    ,  0  // Priority
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);
  
}

void loop()
{
  // Empty
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskCCS811(void *pvParameters)  // This is a task.
{
  (void) pvParameters;


  pinMode(LED_BUILTIN, OUTPUT);

  for (;;) // A Task shall never return or exit.
  {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    vTaskDelay(100);  // one tick delay (15ms) in between reads for stability
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    vTaskDelay(100);  // one tick delay (15ms) in between reads for stability
  }
}

void TaskPMS7003(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  for (;;)
  {
    // read the input on analog pin A3:
    int sensorValueA3 = analogRead(A3);
    // print out the value you read:
    Serial.println(sensorValueA3);
    vTaskDelay(10);  // one tick delay (15ms) in between reads for stability
  }
}

void TaskBME280(void *pvParameters)
{
    (void) pvParameters;

    for(;;)
    {

    }
}


void serverTask(void *pvParameters)
{
    (void) pvParameters;

    for(;;)
    {

    }
}