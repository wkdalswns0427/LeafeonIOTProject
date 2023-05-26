from fastapi import FastAPI
from starlette.middleware.cors import CORSMiddleware
from fastapi import Security, APIRouter
from fastapi.security import HTTPAuthorizationCredentials, HTTPBearer
from fastapi.responses import ORJSONResponse

from auth.auth import Auth
from database import mongodb
from odmantic.engine import SyncEngine
from pymongo.collection import Collection

from database.user.user_model import UserModel

from command.login_command import LoginCommand, SignUpCommand

app = FastAPI()
auth = Auth()
mongo = mongodb
database: SyncEngine
router = APIRouter()
security = HTTPBearer()

user_collection: Collection

app.add_middleware(
    CORSMiddleware,
    allow_origins=['*'],
    allow_credentials=True,
    allow_methods=['*'],
    allow_headers=["*"]
)


@app.on_event("startup")
def on_app_start():
    global database
    global user_collection

    mongo.connect()
    database = mongo.engine


@app.on_event("shutdown")
async def on_app_shutdown():
    mongo.close()


@app.post(path='/signup')
async def signup(command: SignUpCommand):
    try:
        result = database.find_one(UserModel, {+UserModel.user_name: command.user_name})
    except Exception as e:
        print("SIGN UP ERROR ${}".format(e))
        return ORJSONResponse(status_code=500, content={'Server DB ERROR'})
    else:
        if result:
            return ORJSONResponse(status_code=400, content={'description': "user already exists"})

    hashed_password = auth.encode_password(command.password)
    user_data = UserModel(user_name=command.user_name, password=hashed_password)
    try:
        database.save(user_data)
    except Exception as e:
        print("SIGN UP ERROR ${}".format(e))
        return ORJSONResponse(status_code=500, content={'Server DB ERROR'})
    else:
        return ORJSONResponse(status_code=200, content={'description': "Sign up success"})


@app.post(path='/login')
async def login(command: LoginCommand):
    result = database.find_one(UserModel, {+UserModel.user_name: command.user_name})
    if result is None:
        return ORJSONResponse(status_code=401, content={'description': "Invalid User"})

    if not auth.verify_password(command.password, result.password):
        return ORJSONResponse(status_code=401, content={'description': "Invalid Password"})

    access_token_data = auth.encode_token(command.user_name)
    return ORJSONResponse(status_code=200,
                          content={'access_token': access_token_data})


@app.get(path='/verify-user')
async def verify_user(credentials: HTTPAuthorizationCredentials = Security(security)):
    auth_token_data = credentials.credentials
    try:
        user_name = auth.decode_token(auth_token_data)
    except Exception as e:
        print("REFRESH ERROR {}".format(e))
        return ORJSONResponse(status_code=401, content={'description': "Invalid refresh token"})
    else:
        return ORJSONResponse(status_code=200, content={"user_name": user_name})
