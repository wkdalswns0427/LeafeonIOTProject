import paho.mqtt.client as mqtt
from fastapi_mqtt.fastmqtt import FastMQTT
from fastapi_mqtt.config import MQTTConfig
from typing import Optional,List
from fastapi import FastAPI, status,HTTPException,Request

from utils.apiutils.models import FullSensorData, User
from utils.dbutils.database import SessionLocal
import utils.dbutils.models as dbmodels

# mqtt stuff needs to be hidden wen using lenovo
app = FastAPI()
mqtt_config = MQTTConfig()
fast_mqtt = FastMQTT(config=mqtt_config)
fast_mqtt.init_app(app)