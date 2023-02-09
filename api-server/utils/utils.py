import datetime as dt
import typing
import csv
import pandas as pd

class dataHandler():
    def __init__(self):
        self.df = pd.read_csv("db/dummy.csv",header=0, index_col=0)

    def getDate(self):
        self.x = dt.datetime.now()
        date = self.x.strftime("%Y%m%d")
        return date

    def matchDate(self, idx):
        date = self.getDate()
        # fixed date
        # date = "20231212"
        parkdate = self.df.park_date[idx].astype(str)
        
        if date == parkdate:
            return True

        return False
    
    def matchCar(self, carno : str):
        if carno == None:
            print("can't find your car")
            return False

        if (self.df.car_number==carno).any():
            index = self.df.index[self.df.car_number==carno].tolist()[0]

            if self.matchDate(index):
                return True
            else:
                return False
        else:
            return False