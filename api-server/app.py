import uvicorn
import paho.mqtt.client as mqtt
from fastapi_mqtt.fastmqtt import FastMQTT
from fastapi_mqtt.config import MQTTConfig
from fastapi import FastAPI
from src.model import SensorClassItem
# from src.utils import dataHandler
from starlette.middleware.cors import CORSMiddleware
from database import insertClass as ic

app = FastAPI()
mqtt_config = MQTTConfig()
fast_mqtt = FastMQTT(config=mqtt_config)
fast_mqtt.init_app(app)

app.add_middleware(
    CORSMiddleware,
    allow_origins=['*'],
    allow_credentials=True,
    allow_methods=['*'],
    allow_headers=["*"]
)

@app.get("/")
async def first():
    hi = "Hello, Leafeon"
    return hi

# display sensor data
@app.post("/sensordata")
async def postsensordata():
    
    return 0

@app.post("/addSensorClass")
async def addSensorClass(item: SensorClassItem):
    ic.add_sensor_class(item.id, item.sensor, item.parameter)
    return True
