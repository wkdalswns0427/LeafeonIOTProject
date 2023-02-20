import os
# from apis.version1.route_login import login_for_access_token

from fastapi import APIRouter, HTTPException, Request, status
from fastapi import Depends, Security
from fastapi.security import HTTPAuthorizationCredentials, HTTPBearer
from fastapi.templating import Jinja2Templates
from sqlalchemy.orm import Session
from utils.dbutils.database import SessionLocal
import utils.dbutils.models as dbmodels 
from utils.apiutils.models import User
from commons.Forms import LoginForm
from commons.auth import Auth

auth_handler = Auth()
security = HTTPBearer()

db = SessionLocal()
router = APIRouter() #include_in_schema=False
templates = Jinja2Templates(directory=os.path.abspath(os.path.expanduser('templates')))


@router.post('/signup',response_model=User,
        status_code=status.HTTP_201_CREATED)
def create_user_data(item: User):
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


@router.get("/login/")
def login(request: Request):
    return templates.TemplateResponse("login.html", {"request": request})

# this one not working yet
@router.post("/login/")
async def login(request: Request): # , db: Session = Depends(db)
    form = LoginForm(request, db = db)
    await form.load_data()
    if await form.is_valid():
        try:
            form.__dict__.update(msg="Login Successful :)")
            response = templates.TemplateResponse("login.html", form.__dict__)
            user_data=db.query(dbmodels.User).filter(dbmodels.User.id==id).first()
            if user_data.email == response["email"] and user_data.pw == response["password"]:
                return response
            else:
                form.__dict__.update(msg="")
                form.__dict__.get("errors").append("Incorrect Email or Password")
                return templates.TemplateResponse("auth/login.html", form.__dict__)
        except HTTPException:
            form.__dict__.update(msg="")
            form.__dict__.get("errors").append("Incorrect Email or Password")
            return templates.TemplateResponse("auth/login.html", form.__dict__)
    return templates.TemplateResponse("auth/login.html", form.__dict__)

##################################################################################################
######################################### AUTH EXAMPLES ##########################################
##################################################################################################
@router.post('/login')
def login(user_details: User):
    user=db.query(dbmodels.User).filter(dbmodels.User.id==user_details.id).first()
    if user is None:
        return HTTPException(status_code=401, detail='Invalid username')
    if not auth_handler.verify_password(user_details.pw, user.pw):
        return HTTPException(status_code=401, detail='Invalid password')

    access_token = auth_handler.encode_token(user['key'])
    refresh_token = auth_handler.encode_refresh_token(user['key'])
    return {'access_token': access_token, 'refresh_token': refresh_token}
# Todo : this login need to be fixed error
# ValueError: malformed bcrypt hash (checksum must be exactly 31 chars)


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


@router.post('/get-user-data')
def secret_data(credentials: HTTPAuthorizationCredentials = Security(security)):
    token = credentials.credentials
    decode_result = auth_handler.decode_token(token)
    if decode_result:
        return dummyUserData[decode_result]


@router.get('/notsecret')
def not_secret_data():
    return 'Not secret data'