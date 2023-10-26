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
        
        # AIRLINES-1
        sql = "DELETE FROM Flights WHERE source_airport != 'AKI' AND dest_airport != 'AKI';"
        cursor.execute(sql)
        
        # AIRLINES-2
        remaining_airline_ids = "SELECT DISTINCT airline_id FROM Airlines WHERE abbreviation IN('Continental', 'AirTran', 'Virgin')"
        survivor_airline_ids = "SELECT DISTINCT airline_id FROM Airlines WHERE abbreviation IN('AirTran', 'Virgin')"
        
        # AIRLINES-3
        # For all flights NOT operated by Continental, AirTran or Virgin, increase the flight number by 2000. (this will ensure that after the corporate takeover, flight numbers are still unique)
        settings = "flight_no = flight_no + 2000"
        sql = f"UPDATE Flights SET {settings} WHERE airline NOT IN({remaining_airline_ids});"
        cursor.execute(sql)
        
        # AIRLINES-4
        # Switch up the flight numbers from even to odd for each pair of flights
        
        ## AIRLINES-4: Even
        sql = "UPDATE Flights SET flight_no = flight_no + 1 WHERE MOD(flight_no, 2) = 0;"
        cursor.execute(sql)
        
        ## AIRLINES-4: Odd
        sql = "UPDATE Flights SET flight_no = flight_no - 1 WHERE MOD(flight_no, 2) = 1;"
        cursor.execute(sql)
        
        # AIRLINES-5
        # Replace the airline for all flights to and from AKI except for [AirTran and Virgin] with Continental.
        settings = "airline = (SELECT DISTINCT airline_id FROM Airlines WHERE abbreviation = 'Continental')"
        sql = f"UPDATE Flights SET {settings} WHERE ((source_airport = 'AKI' OR dest_airport = 'AKI') AND (airline NOT IN({survivor_airline_ids})));"
        cursor.execute(sql)
        
        # AIRLINES-6
        sql = "SELECT * FROM Flights ORDER BY airline, flight_no;"
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