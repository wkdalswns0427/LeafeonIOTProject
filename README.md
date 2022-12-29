# **LeafeonRTOS**

an ESP32 FreeRTOS based IoT indoor environment sensor cluster

measures indoor **Temperature**, **humidity**, **eCO2**, **eTVOC**, **Particle Dust**, and little bit inaccurate **altitude**

## **Components**

### **Electronics**

- ESP32 dev module : Lolin D32 Pro (any ESP32 module would be fine)
- DFRobot SEN0335(CCS811 + BME280) : CO2, TVOC sensor + Temperature, Humidity sensor
- Plantower PMS7003 : Particle Dust sensor
- bunch of resistors, caps etc

### **Software**

- either Arduino IDE or Espressif IDF
- intermediate level of C/C++