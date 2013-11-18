-- Enable foreign keys support
PRAGMA foreign_keys = ON;

-------------------------------------------------------------------------------
CREATE TABLE dataset
(
        id      INTEGER PRIMARY KEY,
        hash    TEXT NOT NULL
);

CREATE UNIQUE INDEX idx_dataset_id ON dataset( id );
-- CREATE UNIQUE INDEX idx_dataset_hash ON dataset( hash );

-------------------------------------------------------------------------------
CREATE TABLE tag
(
        id              INTEGER PRIMARY KEY,
        parent_id       INTEGER NOT NULL DEFAULT 0 REFERENCES tag( id ),
        label           TEXT NOT NULL DEFAULT 'Label'
);

CREATE UNIQUE INDEX idx_tag_id ON tag( id );
CREATE UNIQUE INDEX idx_tag_label ON tag( label );

INSERT INTO tag( id, parent_id, label ) VALUES( 0, 0, 'Database');
INSERT INTO tag( id, parent_id, label ) VALUES( 1, 0, 'Incoming');


-------------------------------------------------------------------------------
CREATE TABLE dataset_memberships(
        id              INTEGER PRIMARY KEY,
        dataset_id      INTEGER NOT NULL REFERENCES dataset( id ),
        tag_id          INTEGER NOT NULL REFERENCES tag( id )
);

CREATE UNIQUE INDEX idx_ds_memberships_dataset_id
ON dataset_memberships( dataset_id );

CREATE UNIQUE INDEX idx_dsmemberships_dataset_tag_id
ON dataset_memberships( tag_id );

-------------------------------------------------------------------------------
CREATE TABLE dataset_attributes
(
        id              INTEGER PRIMARY KEY,
        dataset_id      INTEGER NOT NULL REFERENCES dataset( id ),
        rank            INTEGER NOT NULL,
        name            TEXT NOT NULL,
        value           TEXT
);

CREATE UNIQUE INDEX idx_dataset_attributes_id ON dataset_attributes( id );
CREATE INDEX idx_dataset_attributes_dsid ON dataset_attributes( dataset_id );

-------------------------------------------------------------------------------
