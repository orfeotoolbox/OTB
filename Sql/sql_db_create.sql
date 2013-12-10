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
CREATE TABLE node
(
        id        INTEGER PRIMARY KEY AUTOINCREMENT,
        parent_id INTEGER,
        tag_id    INTEGER,
        label     TEXT,
        level     INTEGER,
        path      TEXT,
        FOREIGN KEY( parent_id ) REFERENCES node( id ),
        FOREIGN KEY( tag_id )  REFERENCES tag( id )
);

CREATE INDEX idx_node_tid ON node( tag_id );
CREATE INDEX idx_node_label ON node( label );
CREATE INDEX idx_node_pid ON node( parent_id );

-----------------------------------------------------------------------------
CREATE TABLE dataset_tag_membership(
       dataset_id       INTEGER,
       tag_id           INTEGER,
       PRIMARY KEY( dataset_id, tag_id ),
       FOREIGN KEY( dataset_id ) REFERENCES dataset( id ) ON DELETE CASCADE,
       FOREIGN KEY( tag_id ) REFERENCES tag( id ) ON DELETE CASCADE
);

CREATE INDEX idx_ds_tag_membership_dataset_id
ON dataset_tag_membership( dataset_id );

CREATE INDEX idx_ds_tag_membership_tag_id
ON dataset_tag_membership( tag_id );

-----------------------------------------------------------------------------
CREATE TABLE dataset_node_membership(
       dataset_id       INTEGER,
       node_id      INTEGER,
       PRIMARY KEY( dataset_id, node_id ),
       FOREIGN KEY( dataset_id ) REFERENCES dataset( id ) ON DELETE CASCADE,
       FOREIGN KEY( node_id ) REFERENCES node( id ) ON DELETE CASCADE
);

CREATE INDEX idx_ds_node_membership_dataset_id
ON dataset_node_membership( dataset_id );

CREATE INDEX idx_ds_node_membership_node_id
ON dataset_node_membership( node_id );

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
