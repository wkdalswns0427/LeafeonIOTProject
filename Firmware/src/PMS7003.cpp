#include <Arduino.h>
#include "PMS7003.h"

byte bytCount1 = 0;  
byte bytCount2 = 0;  
unsigned char chrRecv;  
unsigned char chrData[30];  
int PM01;  
int PM25;  
int PM10;  
unsigned int PMS7003_data[3];

#include <HardwareSerial.h>
HardwareSerial SenSerial(2);

unsigned int GetPM_Data(unsigned char chrSrc[], byte bytHigh, byte bytLow)  
{  
   return (chrSrc[bytHigh] << 8) + chrSrc[bytLow];  
}  

void read_PMS7003(unsigned int data[]){
    if (SenSerial.available())   {  
       for(int i = 0; i < 32; i++)     {  
           chrRecv = SenSerial.read();  
           if (chrRecv == START_2 ) {   
              bytCount1 = 2;  
              break;  
            }  
       }   
      if (bytCount1 == 2)  
      {  
         bytCount1 = 0;  
         for(int i = 0; i < 30; i++){  
            chrData[i] = SenSerial.read();  
         }   
  
         if ( (unsigned int) chrData[ERROR_CODE] == 0 ) {  
            data[0]  = GetPM_Data(chrData, PM1_0_ATMOSPHERE_H, PM1_0_ATMOSPHERE_L);  //PM01
            data[1]  = GetPM_Data(chrData, PM2_5_ATMOSPHERE_H, PM2_5_ATMOSPHERE_L);  //PM25
            data[2]  = GetPM_Data(chrData, PM10_ATMOSPHERE_H, PM10_ATMOSPHERE_L);  //PM10
            Serial.print("PM1.0=");Serial.print(data[0]);  
            Serial.print(",PM2.5=");Serial.print(data[1]);  
            Serial.print(",PM10=");Serial.println(data[2]);  
            }  
         else{  
            Serial.println("PMS7003  ERROR");  
         }  
      }   
   }  
   else{  
      Serial.println("PMS7003 NOT available");  
   }  
}

void main_PMS7003(){
    read_PMS7003(PMS7003_data);
}