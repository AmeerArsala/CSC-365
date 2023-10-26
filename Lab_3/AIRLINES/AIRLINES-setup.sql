/*
    Ameer Arsala
    aarsala@calpoly.edu
*/

CREATE TABLE Airlines(
    airline_id INT,
    airline_name VARCHAR(32),
    abbreviation VARCHAR(16),
    country VARCHAR(8),

    PRIMARY KEY(airline_id)
);

CREATE TABLE Airports(
    airport_code VARCHAR(3),
    airport_name VARCHAR(24),
    city VARCHAR(24),
    country VARCHAR(32),
    country_abbreviation VARCHAR(3),

    PRIMARY KEY(airport_code)
);

CREATE TABLE Flights(
    airline INT,
    flight_no INT,
    source_airport VARCHAR(3),
    dest_airport VARCHAR(3),

    CONSTRAINT pk_airline_flight PRIMARY KEY(airline, flight_no),
    CONSTRAINT fk_airline FOREIGN KEY(airline) REFERENCES Airlines(airline_id),
    CONSTRAINT fk_source_airport FOREIGN KEY(source_airport) REFERENCES Airports(airport_code),
    CONSTRAINT fk_dest_airport FOREIGN KEY(dest_airport) REFERENCES Airports(airport_code)
);