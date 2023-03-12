import os
from fastapi import APIRouter, Request, status, HTTPException
from fastapi.responses import HTMLResponse
from fastapi.templating import Jinja2Templates
from starlette.responses import RedirectResponse
from typing import Optional,List

from utils.apiutils.models import User, SensorData, FullSensorData
from utils.dbutils.database import SessionLocal
import utils.dbutils.models as dbmodels 


db=SessionLocal()
router = APIRouter()

@router.post('/postFullSensordata',response_model=FullSensorData, status_code=status.HTTP_201_CREATED)
def create_sensor_data(item:FullSensorData):
    db_item=db.query(dbmodels.FullSensorData).filter(dbmodels.FullSensorData.time==item.time).first()
    if db_item is not None:
        raise HTTPException(status_code=400,detail="Item already exists")

    sensor_data=dbmodels.FullSensorData(
        id=item.id,
        time=item.time,
        tempdata=item.tempdata, 
        humidata=item.humidata,
        presdata=item.presdata,
        altdata=item.altdata,
        eco2data=item.eco2data,
        tvocdata=item.tvocdata,
        pm01data=item.pm01data,
        pm25data=item.pm25data,
        pm10data=item.pm10data
    )
    db.add(sensor_data)
    db.commit()

    return sensor_data

@router.get('/getLatestData',response_model=FullSensorData, status_code=status.HTTP_201_CREATED)
def getLatestData(item: FullSensorData):
    db_item=db.query(dbmodels.FullSensorData).order_by(dbmodels.FullSensorData.time.desc()).first()

    latest_data=dbmodels.FullSensorData(
        time=db_item.time,
        tempdata=db_item.tempdata,
        humidata=db_item.humidata,
        presdata=db_item.presdata,
        altdata=db_item.altdata,
        eco2data=db_item.eco2data,
        tvocdata=db_item.tvocdata,
        pm01data=db_item.pm01data,
        pm25data=db_item.pm25data,
        pm10data=db_item.pm10data
    )

    return latest_data


@router.delete('/item/{item_id}')
def delete_item(id:str):
    item_to_delete=db.query(dbmodels.User).filter(dbmodels.User.id==id).first()

    if item_to_delete is None:
        raise HTTPException(status_code=status.HTTP_404_NOT_FOUND,detail="Resource Not Found")
    
    db.delete(item_to_delete)
    db.commit()

    return item_to_delete
