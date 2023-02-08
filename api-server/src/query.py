import pymysql

def make_table():
    query = '''CREATE TABLE sensor_types(
        id int(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,
        sensor varchar(256),
        parameter varchar(256)
    )
    '''

    return query