from odmantic import Model, Field
from bson import ObjectId


class UserModel(Model):
    user_name: str
    password: str
