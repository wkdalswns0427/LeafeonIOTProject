from pydantic import BaseModel

class Item(BaseModel): #serializer
    id:int
    name:str
    description:str
    price:int
    on_offer:bool

    class Config:
        orm_mode=True

class SensorData(BaseModel):
    id:int
    time:str
    sensortype:int
    sensordata:float

class FullSensorData(BaseModel):
    id:int
    time:str
    tempdata:float
    pressdata:float
    altdata:float
    humidata:float
    eco2data:int
    tvocdata:int
    pm01data:int
    pm25data:int
    pm10data:int
    
    class Config:
        orm_mode=True