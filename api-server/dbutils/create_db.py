# run this only once

from dbutils.database import Base,engine
from utils.models import Item

print("Creating database ....")

Base.metadata.create_all(engine)