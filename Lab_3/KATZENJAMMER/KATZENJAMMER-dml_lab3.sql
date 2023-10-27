/* KATZENJAMMER-1 */
--Change the name of 2 instruments [InstrumentUsages]
--'bass balalaika' -> 'awesome bass balalaika'
--'guitar' -> 'acoustic guitar'

-- Statement to alter the table to increase the character limit to avoid truncation
ALTER InstrumentUsages
CHANGE COLUMN instrument_name 
instrument_name VARCHAR(32);

-- Change the instruments
UPDATE InstrumentUsages
SET instrument_name = 'awesome bass balalaika'
WHERE instrument_name = 'bass balalaika';

UPDATE InstrumentUsages
SET instrument_name = 'acoustic guitar'
WHERE instrument_name = 'guitar';

/* KATZENJAMMER-2 */
--Keep in VocalPerformances table only the performances where id = 1 did not sing lead 
DELETE * FROM VocalPerformances
WHERE song_id = (SELECT song_id FROM VocalPerformances WHERE (bandmember_id = 1 AND vocal_performance_type != 'lead'));