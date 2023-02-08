import sys
import os
import requests 
import base64
import json
import logging
import time 
import pymysql
import pandas as pd
import csv
from awsconfig import awsconfig

#RDS info declared this way
# class awsconfig:
#     host: str 
#     port : int
#     username : str
#     password : str
#     database : str
RDSconfig = awsconfig()

def connect_RDS(host, port, username, password, database):

    try:
        conn = pymysql.connect(host = host, user = username, passwd = password, db = database, 
            port = port, use_unicode = True, charset = 'utf8')
        cursor = conn.cursor()
    except:
        logging.error("DB Not connecting...")

    return conn, cursor
        


def main():
    #call RDS
    conn, cursor = connect_RDS(awsconfig.host, awsconfig.port, awsconfig.username, awsconfig.password, awsconfig.database)
    print("DB connected")

if __name__ == "__main__":
    main()