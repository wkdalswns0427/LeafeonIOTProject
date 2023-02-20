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
    
    class Config:
        orm_mode=True

class FullSensorData(BaseModel):
    id:str
    time:str
    tempdata:float
    humidata:float
    presdata:float
    altdata:float
    eco2data:int
    tvocdata:int
    pm01data:int
    pm25data:int
    pm10data:int
    
    class Config:
        orm_mode=True

class User(BaseModel):
    id:str
    pw:str
    email:str
    
    class Config:
        orm_mode=True
        
# class AuthModel(BaseModel):
#     id: str
#     pw: str
    
#     class Config:
#         orm_mode=True
    