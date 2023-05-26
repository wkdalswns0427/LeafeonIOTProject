from dataclasses import dataclass
from pydantic import BaseModel


@dataclass(frozen=True)  # frozen은 각 속성의 값이 변하지 않도록 설정하는 기능 ex val/const
class SignUpCommand(BaseModel):
    user_name: str
    password: str


@dataclass(frozen=True)
class LoginCommand(BaseModel):
    user_name: str
    password: str
