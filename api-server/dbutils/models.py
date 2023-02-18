from sqlalchemy.sql.expression import null
from dbutils.database import Base
from sqlalchemy import String,Boolean,Float,Integer,Column,Text, Sequence, DateTime


class Item(Base):
    __tablename__='items'
    id=Column(Integer,primary_key=True)
    name=Column(String(255),nullable=False,unique=True)
    description=Column(Text)
    price=Column(Integer,nullable=False)
    on_offer=Column(Boolean,default=False)


    def __repr__(self):
        return f"<Item name={self.name} price={self.price}>"


class SensorData(Base):
    __tablename__='sensor_data_individual'
    id=Column(Integer,primary_key=True)
    time=Column(DateTime)
    sensortype=Column(Integer, nullable=False, default=-1)
    sensordata=Column(Float, nullable=False, default=32768)


class FullSensorData(Base):
    __tablename__='sensor_data'
    id=Column(String,primary_key=True,nullable=False)
    time=Column(String)
    tempdata=Column(Float, nullable=False, default=32768)
    humidata=Column(Float, nullable=False, default=32768)
    presdata=Column(Float, nullable=False, default=32768)
    altdata=Column(Float, nullable=False, default=32768)
    eco2data=Column(Integer, nullable=False, default=32768)
    tvocdata=Column(Integer, nullable=False, default=32768)
    pm01data=Column(Integer, nullable=False, default=32768)
    pm25data=Column(Integer, nullable=False, default=32768)
    pm10data=Column(Integer, nullable=False, default=32768)
    
    
class RegisterID(Base):
    __tablename__='users'
    id=Column(String,primary_key=True,nullable=False)
    pw=Column(String,primary_key=False,nullable=False)
    