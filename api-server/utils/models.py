from pydantic import BaseModel
from typing import Optional, List


class BaseItem(BaseModel):
    name : str
    number : int
    content : str

########## DB related Models ##########
class makeTable(BaseModel):
    name : str
    contents : List[str] = []

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

########### SQLALCHEMY #########
# from sqlalchemy.sql.expression import null
# from dbutils.database import Base
# from sqlalchemy import String,Boolean,Integer,Column,Text, Sequence, DateTime


# class Item(Base):
#     __tablename__='items'
#     id=Column(Integer,primary_key=True)
#     name=Column(String(255),nullable=False,unique=True)
#     description=Column(Text)
#     price=Column(Integer,nullable=False)
#     on_offer=Column(Boolean,default=False)


#     def __repr__(self):
#         return f"<Item name={self.name} price={self.price}>"
