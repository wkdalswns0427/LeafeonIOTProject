#include "BME280.h"

Adafruit_BME280 bme;
float BME280_data[4];

void init_BME280(){

    bool status;

    status = bme.begin(BME280_I2C_ADDR);  
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
}

void read_BME280(float data[]){
    data[0] = bme.readPressure() / 100.0F; // pressure in hPa
    data[1] = bme.readAltitude(SEALEVELPRESSURE_HPA); // altitude in m
    data[2] = bme.readTemperature(); // temperature in C
    data[3] = bme.readHumidity(); // humidity in %
}

void main_BME280(){
    read_BME280(BME280_data);
    Serial.print(BME280_data[0]);Serial.print("   "); Serial.print(BME280_data[1]);Serial.print("   ");
    Serial.println(BME280_data[2]);Serial.print("   "); Serial.println(BME280_data[3]);Serial.print("\n");
    
}