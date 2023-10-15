# Ameer Arsala
# aarsala@calpoly.edu

import mysql.connector
from mysql.connector import Error

HOST = "mysql.labthreesixfive.com"
PORT = "3306"

INDEX_USERNAME = 0
INDEX_PASSWORD = 1

# Read credentials
(username, password) = (None, None)
with open("../../account.info") as f:
    account_info = f.read().splitlines()
    
    username = account_info[INDEX_USERNAME]
    password = account_info[INDEX_PASSWORD]

# connect to database
try:
    connection = mysql.connector.connect(host=HOST, port=PORT, user=username, password=password);
    
    if connection.is_connected():
        cursor = connection.cursor()
        
        # WINE-1
        sql = "ALTER Wines DROP COLUMN appellation_name, DROP COLUMNN wine_name;"
        cursor.execute(sql)
        
        # WINE-2
        sql = "DELETE FROM Wines WHERE grape_varietal_name != 'Zinfandel';"
        cursor.execute(sql)
        
        # WINE-3
        sql = "ALTER Wines ADD COLUMN Revenue INT;"
        cursor.execute(sql)
        
        # WINE-4
        sql = "UPDATE Wines SET Revenue = num_cases_produced * 12 * bottle_price_usd;"
        cursor.execute(sql)
        
        # WINE-5
        sql = "SELECT * FROM Wines ORDER BY Revenue;"
        cursor.execute(sql)
        
        # Commit the transaction (flush the buffer)
        connection.commit()
        
except Error as e:
    print("MySQL Connection Error: ", e)
finally:
    if connection.is_connected():
        # wrap up work
        cursor.close()
    
    connection.close()