import os
from fastapi import APIRouter, HTTPException, Request, status
from fastapi import Depends, Security
from fastapi.security import HTTPAuthorizationCredentials, HTTPBearer
from fastapi.templating import Jinja2Templates
from fastapi.responses import HTMLResponse, RedirectResponse
from utils.dbutils.database import SessionLocal
import utils.dbutils.models as dbmodels 
from utils.apiutils.models import User,LoginModel
from commons.auth import Auth
from commons.Forms import SignUpForm

auth_handler = Auth()
security = HTTPBearer()

db = SessionLocal()
router = APIRouter()
templates = Jinja2Templates(directory=os.path.abspath(os.path.expanduser('templates')))


@router.get('/signup',status_code=status.HTTP_200_OK)
def signup_page(request: Request):
    return templates.TemplateResponse("signup.html", {"request": request})

# done ( stable )
@router.post('/signup',response_model=User,status_code=status.HTTP_201_CREATED)
async def create_user_data(item: User):
    db_item=db.query(dbmodels.User).filter(dbmodels.User.id==item.id).first()
    db_email=db.query(dbmodels.User).filter(dbmodels.User.email==item.email).first()

    if db_item is not None:
        raise HTTPException(status_code=400,detail="Item already exists")
    if db_email is not None:
        raise HTTPException(status_code=400,detail="Email already in use")

    try:
        hashed_password = auth_handler.encode_password(item.pw)
        user_data = dbmodels.User(
            id = item.id,
            pw = hashed_password, # shall be hashed password
            email = item.email
        )
        db.add(user_data)
        db.commit()
        return user_data
    except:
        error_msg = 'Failed to signup user'
        return error_msg

# how the fuck do I use this template?
@router.get("/login")
def login(request: Request):
    return templates.TemplateResponse("login.html", {"request": request})

# done
@router.post('/login')
def login(user_details: LoginModel):
    user=db.query(dbmodels.User).filter(dbmodels.User.id==user_details.id).first()
    if user is None:
        return HTTPException(status_code=401, detail='Invalid username')
    if not auth_handler.verify_password(user_details.pw, user.pw):
        return HTTPException(status_code=401, detail='Invalid password')
    
    access_token = auth_handler.encode_token(user.id)
    refresh_token = auth_handler.encode_refresh_token(user.id)
    return {'access_token': access_token, 'refresh_token': refresh_token}


@router.get('/refresh_token')
def refresh_token(credentials: HTTPAuthorizationCredentials = Security(security)):
    refresh_token = credentials.credentials
    new_token = auth_handler.refresh_token(refresh_token)
    return {'access_token': new_token}


@router.post('/secret')
def secret_data(credentials: HTTPAuthorizationCredentials = Security(security)):
    token = credentials.credentials
    if auth_handler.decode_token(token):
        return 'Top Secret data only authorized users can access this info'


@router.get('/get-user-data') # post?
def secret_data(credentials: HTTPAuthorizationCredentials = Security(security)):
    token = credentials.credentials
    decode_result = auth_handler.decode_token(token) # returns id
    if decode_result:
        user=db.query(dbmodels.User).filter(dbmodels.User.id==decode_result).first()
        return user


@router.get('/notsecret')
def not_secret_data():
    return 'Not secret data'