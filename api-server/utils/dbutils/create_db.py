# run this only once

from utils.dbutils.database import Base,engine
from utils.apiutils.models import Item

print("Creating database ....")

Base.metadata.create_all(engine)