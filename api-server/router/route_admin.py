import os
from fastapi import APIRouter, Request, Form, status
from fastapi.responses import HTMLResponse
from fastapi.templating import Jinja2Templates
from starlette.responses import RedirectResponse
from typing import Optional,List

from utils.apiutils.models import User, SensorData, FullSensorData
from utils.dbutils.database import SessionLocal
import utils.dbutils.models as dbmodels 

db=SessionLocal()
router = APIRouter()
templates = Jinja2Templates(directory=os.path.abspath(os.path.expanduser('templates')))

@router.get('/admin/userinfo',response_model=List[User],status_code=200)
def get_all_items():
    users=db.query(dbmodels.User).all()    
    return users


@router.get('/admin/userinfo/{id}',response_model=User,status_code=status.HTTP_200_OK)
def get_an_item(id:str):
    context = {}
    user=db.query(dbmodels.User).filter(dbmodels.User.id==id).first()
    context["request"] = Request
    context["id"] = user.id
    context["pw"] = user.pw
    context["email"] = user.email
    return context
    # return templates.TemplateResponse("user_lists.html", context)