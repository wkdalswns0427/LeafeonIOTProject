import uvicorn
import paho.mqtt.client as mqtt
from fastapi_mqtt.fastmqtt import FastMQTT
from fastapi_mqtt.config import MQTTConfig
from pydantic import BaseModel
from typing import Optional,List
from fastapi import FastAPI, status,HTTPException
from starlette.middleware.cors import CORSMiddleware
from apiutils.models import Item, SensorData, FullSensorData
from dbutils.database import SessionLocal
import dbutils.models as dbmodels

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

db=SessionLocal()
@app.get("/")
def init():
    hello = "Hello Charmander"
    return hello

@app.post('/postSensordata',response_model=Item,
        status_code=status.HTTP_201_CREATED)
def create_an_item(item:SensorData):
    db_item=db.query(dbmodels.SensorData).filter(dbmodels.SensorData.id==item.id).first()

    if db_item is not None:
        raise HTTPException(status_code=400,detail="Item already exists")

    new_item=dbmodels.SensorData(
        # id=item.id
        time=item.time,
        sensortype=item.sensortype,
        sensordata=item.sensordata
    )
    db.add(new_item)
    db.commit()

    return new_item

@app.post('/postFullSensordata',response_model=Item,
        status_code=status.HTTP_201_CREATED)
def create_an_item(item:FullSensorData):
    db_item=db.query(dbmodels.FullSensorData).filter(dbmodels.FullSensorData.id==item.id).first()

    if db_item is not None:
        raise HTTPException(status_code=400,detail="Item already exists")

    new_item=dbmodels.FullSensorData(
        id=item.id,
        time=item.time,
        tempdata=item.tempdata,
        humidata=item.humidata,
        pressdata=item.pressdata,
        altdata=item.altdata,
        eco2data=item.eco2data,
        tvocdata=item.tvocdata,
        pm01data=item.pm01data,
        pm25data=item.pm25data,
        pm10data=item.pm10data
    )
    db.add(new_item)
    db.commit()

    return new_item

###################################### SQLALCHEMY ####################################
@app.get('/items',response_model=List[Item],status_code=200)
def get_all_items():
    items=db.query(dbmodels.Item).all()

    return items

@app.get('/item/{item_id}',response_model=Item,status_code=status.HTTP_200_OK)
def get_an_item(item_id:int):
    item=db.query(dbmodels.Item).filter(dbmodels.Item.id==item_id).first()
    return item

@app.post('/items',response_model=Item,
        status_code=status.HTTP_201_CREATED)
def create_an_item(item:Item):
    db_item=db.query(dbmodels.Item).filter(dbmodels.Item.name==item.name).first()

    if db_item is not None:
        raise HTTPException(status_code=400,detail="Item already exists")

    new_item=dbmodels.Item(
        name=item.name,
        price=item.price,
        description=item.description,
        on_offer=item.on_offer
    )
    db.add(new_item)
    db.commit()

    return new_item

@app.put('/item/{item_id}',response_model=Item,status_code=status.HTTP_200_OK)
def update_an_item(item_id:int,item:Item):
    item_to_update=db.query(dbmodels.Item).filter(dbmodels.Item.id==item_id).first()
    item_to_update.name=item.name
    item_to_update.price=item.price
    item_to_update.description=item.description
    item_to_update.on_offer=item.on_offer

    db.commit()

    return item_to_update

@app.delete('/item/{item_id}')
def delete_item(item_id:int):
    item_to_delete=db.query(dbmodels.Item).filter(dbmodels.Item.id==item_id).first()

    if item_to_delete is None:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND,detail="Resource Not Found")
    
    db.delete(item_to_delete)
    db.commit()

    return item_to_delete

###################################################################################################