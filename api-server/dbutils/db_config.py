from dataclasses import dataclass

@dataclass
class db_config:
    host: str = "db-leafeon.cp1bodcsjnqi.us-west-1.rds.amazonaws.com"
    port : int = 3306
    username : str = "*********"
    password : str = "******"
    database : str = "leafeon_db"

