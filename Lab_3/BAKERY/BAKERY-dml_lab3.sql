/* BAKERY-1 */
--Using a single SQL statement, reduce the prices of Lemon Cake and Napoleon Cake by $2
UPDATE Goods
SET price = price + 2
WHERE food = 'Cake' AND flavor IN ('Lemon', 'Napoleon');

/* BAKERY-2 */
--Using a single SQL statement, increase by 15% the price of all Apricot or Chocolate flavored items with a current price below $5.95.
UPDATE Goods
SET price = 1.15 * price
WHERE (flavor IN ('Apricot', 'Chocolate')) AND (price < 5.95); 

/* BAKERY-3 */
--Add the capability for the database to record payment information for each receipt in a new table named payments. 
--A receipt may have multiple payments (split checks).
--Each payment row should hold an Amount, PaymentSettled (date and time), and a PaymentType.
CREATE TABLE Payments(
    INT receipt_id,
    FLOAT Amount,
    DATETIME PaymentSettled,
    VARCHAR(32) PaymentType,

    -- Key Declaration
    PRIMARY KEY (receipt_id, PaymentSettled),
    CONSTRAINT fk_receipt_id FOREIGN KEY(receipt_id) REFERENCES Receipts(receipt_id)
);

/* BAKERY-4 */
--The bakery has stopped selling certain pastries on weekends. 
--Create a database trigger to prevent the sale of Meringues [food] (any flavor) and all Almond-flavored items on either Saturday or Sunday
CREATE TRIGGER prevent_sale_on_weekends BEFORE INSERT ON Receipts
FOR EACH ROW
BEGIN
    --trigger logic
    DECLARE corresponding_good_id VARCHAR(32);
    DECLARE good_food VARCHAR(32);
    DECLARE good_flavor VARCHAR(16);

    SET corresponding_good_id = SELECT good_id FROM PurchasedItems WHERE receipt_id = NEW.receipt_id;
    SET good_food = SELECT food FROM Goods WHERE good_id = corresponding_good_id;
    SET good_flavor = SELECT flavor FROM Goods WHERE good_id = corresponding_good_id;

    IF (DAYNAME(NEW.purchase_date) IN ('Saturday', 'Sunday') AND (good_food = 'Meringue' OR good_flavor = 'Almond')) THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'Not being sold on weekends';
    END IF;
END;