from pydantic import BaseModel
from typing import Optional


class Item(BaseModel):
    name : str
    number : int
    content : str

class DiscountItem(BaseModel):
    name : str
    number : int
    content : str
    carnumber : Optional[str] = None

class IsDiscount(BaseModel):
    discount : Optional[bool] = False