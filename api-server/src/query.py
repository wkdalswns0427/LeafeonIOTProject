import pymysql

class Query:
    def make_table():
        query = '''CREATE TABLE sensor_types(
            id int(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,
            sensor varchar(256),
            parameter varchar(256)
        )
        '''

        return query

    def insert_data():
        query = '''insert into sensor_types (id, sensor, parameter)
            values (%s, %s, %s)
        '''
        return query