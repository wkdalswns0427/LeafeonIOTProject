from dataclasses import dataclass
import commons.secret as secret

@dataclass
class db_config:
    host: str = secret.host
    port : int = 3306
    username : str = secret.username
    password : str = secret.password
    database : str = secret.database

