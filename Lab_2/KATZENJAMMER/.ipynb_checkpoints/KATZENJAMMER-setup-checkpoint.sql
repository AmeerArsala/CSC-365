/*
    Ameer Arsala
    aarsala@calpoly.edu
*/

CREATE TABLE BandMembers(
    bandmember_id INT,
    first_name VARCHAR(16),
    last_name VARCHAR(16),

    PRIMARY KEY(bandmember_id),
    CONSTRAINT uk_fullname UNIQUE KEY(first_name, last_name)
);

CREATE TABLE Songs(
    song_id INT,
    title VARCHAR(64),

    PRIMARY KEY(song_id),
    UNIQUE KEY(title)
);

CREATE TABLE Albums(
    album_id INT,
    title VARCHAR(64),
    year_released INT,
    label VARCHAR(32),
    type VARCHAR(32),

    PRIMARY KEY(album_id),
    UNIQUE KEY(title)
);

CREATE TABLE InstrumentUsages(
    song_id INT,
    bandmember_id INT,
    instrument_name VARCHAR(16),

    CONSTRAINT pk_instrument_usage PRIMARY KEY(song_id, bandmember_id, instrument_name),
    CONSTRAINT fk_song_id FOREIGN KEY(song_id) REFERENCES Songs(song_id),
    CONSTRAINT fk_bandmember_id FOREIGN KEY(bandmember_id) REFERENCES BandMembers(bandmember_id)
);

CREATE TABLE BandMemberPerformances(
    song_id INT,
    bandmember_id INT,
    stage_position VARCHAR(8),

    CONSTRAINT pk_song_bandmember PRIMARY KEY(song_id, bandmember_id),
    CONSTRAINT fk_song_id FOREIGN KEY(song_id) REFERENCES Songs(song_id),
    CONSTRAINT fk_bandmember_id FOREIGN KEY(bandmember_id) REFERENCES BandMembers(bandmember_id)
);

CREATE TABLE Tracks(
    album_id INT,
    position INT,
    song_id INT,
    
    CONSTRAINT pk_album_position PRIMARY KEY(album_id, position),
    CONSTRAINT fk_album_id FOREIGN KEY(album_id) REFERENCES Albums(album_id),
    CONSTRAINT fk_song_id FOREIGN KEY(song_id) REFERENCES Songs(song_id)
);

CREATE TABLE VocalPerformances(
    song_id INT,
    bandmember_id INT,
    vocal_performance_type VARCHAR(8),

    CONSTRAINT pk_vocalperformance PRIMARY KEY(song_id, bandmember_id, vocal_performance_type),
    CONSTRAINT fk_song_id FOREIGN KEY(song_id) REFERENCES Songs(song_id),
    CONSTRAINT fk_bandmember_id FOREIGN KEY(bandmember_id) REFERENCES BandMembers(bandmember_id)
);