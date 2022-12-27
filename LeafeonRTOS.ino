#include "src/config.h"
#include "src/mywifi.h"
#include "src/CCS811.h"
#include "src/BME280.h"
#include "src/PMS7003.h"
#include "src/task.h"

#define ArduinoCore0 0
#define ArduinoCore1 1

#define STACK_SIZE 1024

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

void setup() {
  
  Serial.begin(115200);
  init_BME280();
  
  xTaskCreatePinnedToCore(
    TaskCCS811
    ,  "TaskCCS811"   
    ,  STACK_SIZE  
    ,  NULL
    ,  2  
    ,  NULL 
    ,  ArduinoCore1);

  xTaskCreatePinnedToCore(
    TaskPMS7003
    ,  "TaskPMS7003"
    ,  STACK_SIZE  
    ,  NULL
    ,  1  // Priority
    ,  NULL 
    ,  ArduinoCore1);

  xTaskCreatePinnedToCore(
    TaskBME280
    ,  "TaskBME280"
    ,  STACK_SIZE  
    ,  NULL
    ,  3  // Priority
    ,  NULL 
    ,  ArduinoCore1);

    xTaskCreatePinnedToCore(
    TaskBlink
    ,  "TaskBlink"
    ,  STACK_SIZE  
    ,  NULL
    ,  0  // Priority
    ,  NULL 
    ,  ArduinoCore1);
  
}

void loop()
{
  // Empty
}

