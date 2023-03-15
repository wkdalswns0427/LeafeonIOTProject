# **LeafeonRTOS**

An ESP32 FreeRTOS based IoT indoor environment monitoring system

measures indoor **Temperature**, **humidity**, **eCO2**, **eTVOC**, **Particle Dust**, and little bit inaccurate **altitude**

## **Components**

### **Electronics**

- ESP32 dev module : Lolin D32 Pro (any ESP32 module would be fine)
- DFRobot SEN0335(CCS811 + BME280) : CO2, TVOC sensor + Temperature, Humidity sensor
- Plantower PMS7003 : Particle Dust sensor
- YW Robotics 0.96 inch OLED <- this thing is way off spec sheet (works fine with 3.3v, addr : 0x3C, ssd1306 not ssh...)
- indicator LED and a switch
- bunch of resistors, caps etc

### **Software**

- either Arduino IDE or Espressif IDF
- intermediate level of C/C++
- Fast API based REST API server running at localhost:8000
- super simple javascript frontend ... or a NodeJS dashboard...
- AWS RDS database
- Docker capabilities

**5 types of firmwares**
```
1. Sensors Only -> only display with oled
2. Sensors OTA -> a local OTA server
3. Sensors DB -> a light api server running along with AWS RDS DB + OTA
4. Sensors MQTT -> a light api server usage of MQTT control + OTA (dev)
```

## **System Usage**

![service-diagram](https://user-images.githubusercontent.com/68832065/219280953-859f745e-9cce-441e-b211-0c6bc0a822d6.png)

### **Firmware**
open up `config.h` and change&add network settings and db api uri

### **OTA Update**
If LCD display is ready, check display for update server IP

if you access the ip you get

![otalogin](https://user-images.githubusercontent.com/68832065/224346160-5647183e-5392-47a3-8dd2-db6064e5d675.PNG)

log in with default admin:admin and update with your binary file

![otaupdate](https://user-images.githubusercontent.com/68832065/224346167-eeb9d4da-8304-4b5e-9428-5214e132aeed.PNG)

### **API Server**
RestAPI by FastAPI framework for python

download requirements before running ( I recommend using Anaconda )

```shell
cd api-server
sudo pip3 install -r requirements.txt
sudo pip3 install -r requirements_windows.txt
```

- running local server

```shell
uvicorn app:app --host 0.0.0.0 --reload
```

- running local server via Docker

```shell
docker build -t <image_name> .
docker run -p 8000:8000 <image_name>
```

### **Database**

My MySQL database is running on AWS RDS service.
![캡처](https://user-images.githubusercontent.com/68832065/224329307-613c248f-3214-41a7-a1c0-c7b162cc01ff.PNG)

### **Frontend**

I do not have a domain at this point. Using `vscode` provided liveserver for development.

![화면 캡처 2023-03-15 152823](https://user-images.githubusercontent.com/68832065/225224989-8a82bafa-7a7b-432c-8bf8-fd83154bbd58.png)

---

### Reference

- https://microcontrollerslab.com/esp32-rest-api-web-server-get-post-postman/
- https://github.com/jigneshk5/Blynk-IoT-Dashboards
- https://github.com/subhashissuara/IoT_Dashboard 
