from pydantic import BaseModel
from typing import Optional


class Item(BaseModel):
    name : str
    number : int
    content : str

class DiscountItem(BaseModel):
    name : str
    number : int
    content : str
    carnumber : Optional[str] = None

class IsDiscount(BaseModel):
    discount : Optional[bool] = False

class SensorClassItem(BaseModel):
    id : str
    sensor : str
    parameter : str

class SensorData(BaseModel):
    Temp : float
    Humi : float
    Press : float
    eCO2 : float
    eTVOC : float
    pm01 : float
    pm25 : float
    pm10 : float