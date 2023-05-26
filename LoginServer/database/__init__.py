# /app/database/__init__.py

# pymongo - MongoDB 용 동기 python 라이브러리
from pymongo.mongo_client import MongoClient
from odmantic import SyncEngine

# 소중한 Secrets.json 가져오기
from config import MONGO_DB_NAME, MONGO_DB_URL


class MongoDB:
    def __init__(self):
        self.client = None
        self.engine = None

    def connect(self):
        self.client = MongoClient(host=MONGO_DB_URL)
        self.engine = SyncEngine(client=self.client, database=MONGO_DB_NAME)
        print("DB CONNECTED")

    def close(self):
        self.client.close()
        print("DB CLOSED")


mongodb = MongoDB()
