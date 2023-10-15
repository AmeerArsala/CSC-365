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
        
        # KATZENJAMMER-1
        sql = "UPDA"
        cursor.execute(sql)
        
        # KATZENJAMMER-2
        
        # KATZENJAMMER-3
        
        # KATZENJAMMER-4
        
        # KATZENJAMMER-5
        
        # KATZENJAMMER-6
        
        # Commit the transaction (flush the buffer)
        connection.commit()
        
except Error as e:
    print("MySQL Connection Error: ", e)
finally:
    if connection.is_connected():
        # wrap up work
        cursor.close()
    
    connection.close()