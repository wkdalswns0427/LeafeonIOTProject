from sqlalchemy.orm import declarative_base
from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker
from dbutils.db_config import db_config
from sqlalchemy.engine import URL

url = URL.create(drivername="mysql",
            username=db_config.username,
            password = db_config.password,
            host=db_config.host, 
            database=db_config.database,
            port = db_config.port
        )

engine=create_engine(url,
    echo=True
)
# "postgresql://{YOUR_DATABASE_USER}:{YOUR_DATABASE_PASSWORD}@localhost/{YOUR_DATABASE_NAME}"

Base=declarative_base()

SessionLocal=sessionmaker(bind=engine)