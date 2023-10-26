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
        
        # BAKERY-1
        ## BAKERY-1: Delete Associated PurchasedItems whose price < $5
        sql = "DELETE FROM PurchasedItems WHERE good_id IN (SELECT good_id FROM Goods WHERE prcie < 5.0);"
        cursor.execute(sql)
        
        ## BAKERY-1: Delete Goods whose price is < $5
        sql = "DELETE FROM Goods WHERE price < 5.0;"
        cursor.execute(sql)
        
        # BAKERY-2
        # Increase the prices of the chocolate-flavored items by 35%
        sql = "UPDATE Goods SET price = (price * 1.35) WHERE flavor = 'Chocolate';"
        cursor.execute(sql)
        
        # BAKERY-3
        # Reduce lemon flavored goods by $2
        sql = "UPDATE Goods SET price = (price - 2) WHERE flavor = 'Lemon';"
        cursor.execute(sql)
        
        # BAKERY-4
        # Reduce price of all other cakes by 10%
        sql = "UPDATE Goods SET price = (price * 0.9) WHERE (food = 'Cake' AND flavor NOT IN('Chocolate', 'Lemon'));"
        cursor.execute(sql)
        
        # BAKERY-5
        # Make the price of pie items equal to the price of the least expensive cake
        least_expensive_cake_price = "8.95"
        sql = f"UPDATE Goods SET price = {least_expensive_cake_price} WHERE food = 'Pie';"
        cursor.execute(sql)
        
        # BAKERY-6
        sql = "SELECT * FROM Goods ORDER BY GID;"
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