import sys
import os
import requests 
import base64
import json
import logging
import time 
import pymysql
import pandas as pd
from dbutils.awsconfig import awsconfig
from dbutils.query import Query
import traceback
from psycopg2.extras import RealDictCursor, execute_values

#RDS info declared this way in awsconfig
# class awsconfig:
#     host: str 
#     port : int
#     username : str
#     password : str
#     database : str

class DBManager():
    def __init__(self):
        self.conn = self.__connect()

    def teardown(self):
        if self.conn:
            self.conn.close()
            self.conn = None

    def __connect(self):
        return pymysql.connect(
            host = awsconfig.host, 
            user = awsconfig.username, 
            passwd = awsconfig.password, 
            db = awsconfig.database, 
            port = awsconfig.port)


    def __read_query(self, query_filename):
        query = ''
        with open(query_filename, 'r') as query_file:
            query = query_file.read()

        return query

    def __insert_sensorclass(self,item):
        if not self.conn:
            self.conn = self.__connect()
        cursor = self.conn.cursor(cursor_factory=RealDictCursor)
        query = Query.insert_data()
        try:
            cursor.execute(query, (item.id, item.sensor, item.parameter))
            result = cursor.fetchall()
        except Exception as e:
            msg = traceback.format_exc()
            msg += '\n\n Query: \n' + query
            print(msg)
            cursor.close()
            return False
        return True
    
    def __insert_sensordata(self,item):

        return True
        
    # ------------------------------------------------------

    def __execute(self, query):
        if not self.conn:
            self.conn = self.__connect()
        cur = self.conn.cursor(cursor_factory=RealDictCursor)

        try:
            cur.execute(query)
            result = cur.fetchall()
        except Exception as e:
            msg = traceback.format_exc()
            msg += '\n\n Query: \n' + query
            print(msg)
            cur.close()
            return None

        cur.close()
        return result