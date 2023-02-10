import uvicorn
import paho.mqtt.client as mqtt
from fastapi_mqtt.fastmqtt import FastMQTT
from fastapi_mqtt.config import MQTTConfig
from pydantic import BaseModel
from typing import Optional,List
from fastapi import FastAPI, status,HTTPException
from starlette.middleware.cors import CORSMiddleware

from utils.model import SensorClassItem
from dbutils.database import DBManager

app = FastAPI()
mqtt_config = MQTTConfig()
fast_mqtt = FastMQTT(config=mqtt_config)
fast_mqtt.init_app(app)

class UMCW_auth(BaseModel): #serializer
    table_id:int
    serial_no:str
    coreg_no:str
    device_id:int
    device_no:int
    device_pw:str
    kiosk_pw:str
    anydesk_id:str
    anydesk_pw:str

    class Config:
        orm_mode=True

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
@app.post("/addSebsorData")
async def postsensordata():
    dbmanager = DBManager()
    
    return 0

@app.post("/addSensorClass")
async def addSensorClass(item: SensorClassItem):
    dbmanager = DBManager()
    dbmanager.__insert_sensorclass(item)
    return True

@app.post("/uploadsensordata",response_model=UMCW_auth,
        status_code=status.HTTP_201_CREATED)
def uploadsensordata():

    return True


########### SQLALCHEMY #########
from dbutils.database import SessionLocal
import utils.models as models
# db=SessionLocal()
# @app.get("/")
# def init():
#     hello = "Hello Charmander"
#     return hello

# @app.get('/items',response_model=List[Item],status_code=200)
# def get_all_items():
#     items=db.query(models.Item).all()

#     return items

# @app.get('/item/{item_id}',response_model=Item,status_code=status.HTTP_200_OK)
# def get_an_item(item_id:int):
#     item=db.query(models.Item).filter(models.Item.id==item_id).first()
#     return item

# @app.post('/items',response_model=Item,
#         status_code=status.HTTP_201_CREATED)
# def create_an_item(item:Item):
#     db_item=db.query(models.Item).filter(models.Item.name==item.name).first()

#     if db_item is not None:
#         raise HTTPException(status_code=400,detail="Item already exists")

#     new_item=models.Item(
#         name=item.name,
#         price=item.price,
#         description=item.description,
#         on_offer=item.on_offer
#     )
#     db.add(new_item)
#     db.commit()

#     return new_item

# @app.put('/item/{item_id}',response_model=Item,status_code=status.HTTP_200_OK)
# def update_an_item(item_id:int,item:Item):
#     item_to_update=db.query(models.Item).filter(models.Item.id==item_id).first()
#     item_to_update.name=item.name
#     item_to_update.price=item.price
#     item_to_update.description=item.description
#     item_to_update.on_offer=item.on_offer

#     db.commit()

#     return item_to_update

# @app.delete('/item/{item_id}')
# def delete_item(item_id:int):
#     item_to_delete=db.query(models.Item).filter(models.Item.id==item_id).first()

#     if item_to_delete is None:
#         raise HTTPException(status_code=status.HTTP_404_NOT_FOUND,detail="Resource Not Found")
    
#     db.delete(item_to_delete)
#     db.commit()

#     return item_to_delete