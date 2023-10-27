/* AIRLINES-1 */
--Source airport and destination airport for a Flight can't be the same
--Add a trigger that enforces this
CREATE TRIGGER validate_source_dest BEFORE INSERT ON Flights
FOR EACH ROW
BEGIN
    IF NEW.source_airport = NEW.dest_airport THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'Source Airport and Destination Airport cannot be the same';
    END IF;
END;

/* AIRLINES-2 */
--Add Partner column to Airlines; symmetric partnership, but doesn't need to be enforced. 1 partnership at most
--Optional 
--If a new partnership is added that conflicts with an already-formed partnership, the database should refuse the change
ALTER Airlines
ADD COLUMN Partner VARCHAR(16); -- same as abbreviation column

CREATE TRIGGER validate_partnership BEFORE INSERT ON Airlines
FOR EACH ROW
BEGIN
    -- if partner already exists, decline
    IF (SELECT COUNT(Partner) FROM Airlines WHERE Partner = NEW.Partner) > 0 THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'Airlines can only have one partnership';
    END IF;
END;

CREATE TRIGGER validate_partnership_change BEFORE UPDATE ON Airlines
FOR EACH ROW
BEGIN
    -- if partner already exists, decline
    IF NEW.Partner != OLD.Partner AND (SELECT COUNT(Partner) FROM Airlines WHERE Partner = NEW.Partner) > 0 THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'Airlines can only have one partnership';
    END IF;
END;
