import jwt
from fastapi import HTTPException
from passlib.context import CryptContext
from datetime import datetime, timedelta


class Auth:
    hasher = CryptContext(schemes=['bcrypt'])
    # secret = os.getenv("APP_SECRET_STRING")
    secret = "key"

    def encode_password(self, password):
        return self.hasher.hash(password)

    def verify_password(self, password, encoded_password):
        return self.hasher.verify(password, encoded_password)

    def encode_token(self, username):
        payload = {
            # 'exp': datetime.utcnow() + timedelta(days=0, minutes=30), #유효시간 없는 형태로 처리
            'iat': datetime.utcnow(),
            'scope': 'access_token',
            'sub': username
        }
        return jwt.encode(
            payload,
            self.secret,
            algorithm='HS256'
        )

    def decode_token(self, token):
        payload = jwt.decode(token, self.secret, algorithms=['HS256'])
        if payload['scope'] == 'access_token':
            return payload['sub']
