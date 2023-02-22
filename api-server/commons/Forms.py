from typing import List
from typing import Optional

from fastapi import Request

class SignUpForm:
    def __init__(self, request: Request):
        self.request: Request = request
        self.errors: List = []
        self.id : Optional[str] = None
        self.pw : Optional[str] = None
        self.email : Optional[str] = None
    
    async def load_data(self):
        form = await self.request.form()
        self.id = form.get("username")
        self.pw = form.get("password")
        self.email = form.get("email")
    
    async def is_valid(self):
        if not self.id or not len(self.id) > 3:
            self.errors.append("id should be more than 3 chars")
        if not self.pw or not len(self.id) > 3:
            self.errors.append("pw should be more than 3 chars")
        if not self.emial or not (self.email.__contains__("@")):
            self.errors.append("email should be in format")
        if not self.errors:
            return True
        return False

class LoginForm:
    def __init__(self, request: Request):
        self.request: Request = request
        self.errors: List = []
        self.id: Optional[str] = None
        self.email: Optional[str] = None
        self.pw: Optional[str] = None

    async def load_data(self):
        form = await self.request.form()
        self.id = form.get("id")
        self.email = form.get("email")
        self.pw = form.get("pw")

    async def is_valid(self):
        if not self.id or not (self.id.__contains__("@")):
            self.errors.append("username is required")
        if not self.email or not (self.email.__contains__("@")):
            self.errors.append("Email is required")    
        if not self.pw or not len(self.pw) >= 4:
            self.errors.append("A valid password is required")
        if not self.errors:
            return True
        return False

class LoginForm2:
    def __init__(self, request: Request):
        self.request: Request = request
        self.id: Optional[str] = None
        self.email: Optional[str] = None
        self.pw: Optional[str] = None

    async def create_oauth_form(self):
        form = await self.request.form()
        self.id = form.get("id")
        self.email = form.get("email")
        self.pw = form.get("pw")