/*
    Ameer Arsala
    aarsala@calpoly.edu
*/

CREATE TABLE Customers(
    customer_id INT,
    first_name VARCHAR(32),
    last_name VARCHAR(32),

    PRIMARY KEY(customer_id),
    CONSTRAINT uk_fullname UNIQUE KEY(first_name, last_name)
);

CREATE TABLE Goods(
    good_id VARCHAR(32),
    flavor VARCHAR(16),
    food VARCHAR(16),
    price FLOAT,

    PRIMARY KEY(good_id),
    CONSTRAINT uk_flavor_food UNIQUE KEY(flavor, food)
);

CREATE TABLE PurchasedItems(
    receipt_id INT,
    ordinal INT,
    good_id VARCHAR(32),

    CONSTRAINT pk_receipt_ordinal PRIMARY KEY(receipt_id, ordinal),
    CONSTRAINT fk_receipt_id FOREIGN KEY(receipt_id) REFERENCES Receipts(receipt_id),
    CONSTRAINT fk_good_id FOREIGN KEY(good_id) REFERENCES Goods(good_id)
);

CREATE TABLE Receipts(
    receipt_id INT,
    purchase_date DATE,
    customer_id INT,

    PRIMARY KEY(receipt_id),
    CONSTRAINT fk_customer_id FOREIGN KEY(customer_id) REFERENCES Customers(customer_id)    
);