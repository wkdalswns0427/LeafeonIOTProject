#ifndef BME280_H
#define BME280_H

#include "Adafruit_BME280.h"
#define BME280_I2C_ADDR 0x76
#define SEALEVELPRESSURE_HPA 1013.25


void init_BME280();
short read_BME280();
void main_BME280();

#endif