/*
    Ameer Arsala
    aarsala@calpoly.edu
*/

CREATE TABLE Grapes(
    grape_id INT,
    grape_varietal_name VARCHAR(24),
    wine_color VARCHAR(8),

    PRIMARY KEY(grape_id),
    UNIQUE KEY(grape_varietal_name)
);

CREATE TABLE Appellations(
    appellation_id INT,
    appellation_name VARCHAR(64),
    county VARCHAR(32),
    appellation_state VARCHAR(32),
    geographic_area VARCHAR(32),
    is_ava VARCHAR(8),

    PRIMARY KEY(appellation_id),
    UNIQUE KEY(appellation_name)
);

CREATE TABLE Wines(
    wine_id INT,
    grape_varietal_name VARCHAR(24),
    wine_maker VARCHAR(32),
    appellation_name VARCHAR(64),
    wine_name VARCHAR(32),
    vintage_year INT,
    bottle_price_usd INT,
    wine_rating INT,
    num_cases_produced INT,

    PRIMARY KEY(wine_id),
    CONSTRAINT uk_winedetails UNIQUE KEY(grape_varietal_name, wine_maker, appellation_name, vintage_year),
    CONSTRAINT fk_grape_varietal FOREIGN KEY(grape_varietal_name) REFERENCES Grapes(grape_varietal_name),
    CONSTRAINT fk_appellation FOREIGN KEY(appellation_name) REFERENCES Appellations(appellation_name)
);