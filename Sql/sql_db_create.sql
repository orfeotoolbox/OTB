-----------------------------------------------------------------------------
CREATE TABLE dataset
(
        id    INTEGER PRIMARY KEY AUTOINCREMENT,
        hash  TEXT NOT NULL,
        alias TEXT
);

CREATE UNIQUE INDEX idx_dataset_hash ON dataset( hash );

-----------------------------------------------------------------------------
CREATE TABLE tag
(
        id    INTEGER PRIMARY KEY AUTOINCREMENT,
        label TEXT NOT NULL
);

CREATE UNIQUE INDEX idx_tag_label ON tag( label );

-----------------------------------------------------------------------------
CREATE TABLE tag_node
(
        id        INTEGER PRIMARY KEY AUTOINCREMENT,
        parent_id INTEGER,
        tag_id    INTEGER,
        level     INTEGER,
        path      TEXT,
        FOREIGN KEY( parent_id ) REFERENCES tag_node( id ),
        FOREIGN KEY( tag_id )  REFERENCES tag( id )
);

CREATE INDEX idx_tag_node_tid ON tag_node( tag_id );
CREATE INDEX idx_tag_node_pid ON tag_node( parent_id );

-----------------------------------------------------------------------------
CREATE TABLE dataset_membership(
       dataset_id       INTEGER,
       tag_id           INTEGER,
       PRIMARY KEY( dataset_id, tag_id ),
       FOREIGN KEY( dataset_id ) REFERENCES dataset( id ) ON DELETE CASCADE,
       FOREIGN KEY( tag_id ) REFERENCES tag( id ) ON DELETE CASCADE
);

CREATE INDEX idx_ds_membership_dataset_id ON dataset_membership( dataset_id );
CREATE INDEX idx_ds_membership_dataset_tag_id ON dataset_membership( tag_id );

-----------------------------------------------------------------------------
CREATE TABLE dataset_attribute
(
        id         INTEGER PRIMARY KEY AUTOINCREMENT,
        dataset_id INTEGER NOT NULL REFERENCES dataset( id ),
        rank       INTEGER NOT NULL,
        name       TEXT NOT NULL,
        value      TEXT
);

CREATE INDEX idx_dataset_attribute_dsid ON dataset_attribute( dataset_id );
CREATE INDEX idx_dataset_attribute_name ON dataset_attribute( name );

-----------------------------------------------------------------------------
CREATE TABLE database_attribute
(
        id    INTEGER PRIMARY KEY AUTOINCREMENT,
        name  TEXT NOT NULL,
        value TEXT
);

CREATE UNIQUE INDEX idx_database_attribute_name ON database_attribute( name );
