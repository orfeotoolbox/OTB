namespace
{

/****************************************************************************/
//
// 'SQL_DB_CREATE' Generated from file 'sql_db_create.sql'
const char* SQL_DB_CREATE[] = {
"-----------------------------------------------------------------------------\n"
"CREATE TABLE dataset\n"
"(\n"
"        id    INTEGER PRIMARY KEY AUTOINCREMENT,\n"
"        hash  TEXT NOT NULL,\n"
"        alias TEXT\n"
")\n"
";",
"CREATE UNIQUE INDEX idx_dataset_hash ON dataset( hash )\n"
";",
"-----------------------------------------------------------------------------\n"
"CREATE TABLE tag\n"
"(\n"
"        id    INTEGER PRIMARY KEY AUTOINCREMENT,\n"
"        label TEXT NOT NULL\n"
")\n"
";",
"CREATE UNIQUE INDEX idx_tag_label ON tag( label )\n"
";",
"-----------------------------------------------------------------------------\n"
"CREATE TABLE node\n"
"(\n"
"        id        INTEGER PRIMARY KEY AUTOINCREMENT,\n"
"        parent_id INTEGER,\n"
"        tag_id    INTEGER,\n"
"        label     TEXT,\n"
"        level     INTEGER,\n"
"        path      TEXT,\n"
"        FOREIGN KEY( parent_id ) REFERENCES node( id ),\n"
"        FOREIGN KEY( tag_id )  REFERENCES tag( id )\n"
")\n"
";",
"CREATE INDEX idx_node_tid ON node( tag_id )\n"
";",
"CREATE INDEX idx_node_label ON node( label )\n"
";",
"CREATE INDEX idx_node_pid ON node( parent_id )\n"
";",
"-----------------------------------------------------------------------------\n"
"CREATE TABLE dataset_tag_membership(\n"
"       dataset_id       INTEGER,\n"
"       tag_id           INTEGER,\n"
"       PRIMARY KEY( dataset_id, tag_id ),\n"
"       FOREIGN KEY( dataset_id ) REFERENCES dataset( id ) ON DELETE CASCADE,\n"
"       FOREIGN KEY( tag_id ) REFERENCES tag( id ) ON DELETE CASCADE\n"
")\n"
";",
"CREATE INDEX idx_ds_tag_membership_dataset_id\n"
"ON dataset_tag_membership( dataset_id )\n"
";",
"CREATE INDEX idx_ds_tag_membership_tag_id\n"
"ON dataset_tag_membership( tag_id )\n"
";",
"-----------------------------------------------------------------------------\n"
"CREATE TABLE dataset_node_membership(\n"
"       dataset_id       INTEGER,\n"
"       node_id      INTEGER,\n"
"       PRIMARY KEY( dataset_id, node_id ),\n"
"       FOREIGN KEY( dataset_id ) REFERENCES dataset( id ) ON DELETE CASCADE,\n"
"       FOREIGN KEY( node_id ) REFERENCES node( id ) ON DELETE CASCADE\n"
")\n"
";",
"CREATE INDEX idx_ds_node_membership_dataset_id\n"
"ON dataset_node_membership( dataset_id )\n"
";",
"CREATE INDEX idx_ds_node_membership_node_id\n"
"ON dataset_node_membership( node_id )\n"
";",
"-----------------------------------------------------------------------------\n"
"CREATE TABLE dataset_attribute\n"
"(\n"
"        id         INTEGER PRIMARY KEY AUTOINCREMENT,\n"
"        dataset_id INTEGER NOT NULL REFERENCES dataset( id ),\n"
"        rank       INTEGER NOT NULL,\n"
"        name       TEXT NOT NULL,\n"
"        value      TEXT\n"
")\n"
";",
"CREATE INDEX idx_dataset_attribute_dsid ON dataset_attribute( dataset_id )\n"
";",
"CREATE INDEX idx_dataset_attribute_name ON dataset_attribute( name )\n"
";",
"-----------------------------------------------------------------------------\n"
"CREATE TABLE database_attribute\n"
"(\n"
"        id    INTEGER PRIMARY KEY AUTOINCREMENT,\n"
"        name  TEXT NOT NULL,\n"
"        value TEXT\n"
")\n"
";",
"CREATE UNIQUE INDEX idx_database_attribute_name ON database_attribute( name )\n"
";",
};

const int SQL_DB_CREATE_COUNT = 19;

/****************************************************************************/
//
// 'SQL_DB_SETUP' Generated from file 'sql_db_setup.sql'
const char* SQL_DB_SETUP[] = {
"-----------------------------------------------------------------------------\n"
"INSERT INTO tag( id, label ) VALUES( 1, 'Root' )\n"
";",
"INSERT INTO tag( id, label ) VALUES( 2, 'Datasets' )\n"
";",
"INSERT INTO tag( id, label ) VALUES( 3, 'Temporary' )\n"
";",
"-----------------------------------------------------------------------------\n"
"-- Root\n"
"INSERT INTO node( id, parent_id, tag_id, label, level, path )\n"
"VALUES( 1, NULL, 1, 'Root',      0, '/')\n"
";",
"-- Root/Datasets\n"
"INSERT INTO node( id, parent_id, tag_id, label, level, path )\n"
"VALUES( 2, 1,    2, 'Datasets',  1, '/1' )\n"
";",
"-- Root/Datasets/Cached\n"
"INSERT INTO node( id, parent_id, tag_id, label, level, path )\n"
"VALUES( 3, 2,    3, 'Temporary', 2, '/1/2' )\n"
";",
};

const int SQL_DB_SETUP_COUNT = 6;

/****************************************************************************/
//
// 'SQL_QUERIES_INSERT' Generated from file 'sql_queries_insert.sql'
const char* SQL_QUERIES_INSERT[] = {
"-----------------------------------------------------------------------------\n"
"-- SQLQ_INSERT_NODE\n"
"-- Insert tag-node for tag :child_label under parent-node of tag-node\n"
"--  :parent_label.\n"
"INSERT INTO node( parent_id, tag_id, label, level, path )\n"
"        SELECT -- ID (automatic)\n"
"               node.id AS parent_id,\n"
"               (SELECT tag.id FROM tag WHERE tag.label=:child_label) AS tag_id,\n"
"               :child_label AS label,\n"
"               node.level+1 AS level,\n"
"               rtrim( node.path, '/' ) || '/' || node.id AS path\n"
"        FROM   node\n"
"        -- JOIN   tag ON node.tag_id=tag.id\n"
"        -- WHERE  tag.id=(SELECT tag.id FROM tag WHERE tag.label=:parent_label)\n"
"        WHERE  node.label=:parent_label\n"
";",
"-----------------------------------------------------------------------------\n"
"-- SQLQ_INSERT_DATASET_TAG_MEMBERSHIP\n"
"-- Add dataset-membership of dataset %1 to each tag related to\n"
"-- %2 tag-node path list of the form (<id_0>, ...).\n"
"INSERT INTO dataset_tag_membership( dataset_id, tag_id )\n"
"       SELECT %1 AS 'dataset_id',\n"
"              node.tag_id\n"
"       FROM   node\n"
"       JOIN   tag ON node.tag_id=tag.id\n"
"       WHERE  node.id IN (%2)\n"
";",
};

const int SQL_QUERIES_INSERT_COUNT = 2;

/****************************************************************************/
//
// 'SQL_QUERIES_SELECT' Generated from file 'sql_queries_select.sql'
const char* SQL_QUERIES_SELECT[] = {
"-----------------------------------------------------------------------------\n"
"-- SQLQ_SELECT_NODE_BY_TAG_LABEL\n"
"-- Find tag-node given tag-label.\n"
"SELECT node.id,\n"
"       node.parent_id,\n"
"       node.tag_id,\n"
"       node.level,\n"
"       node.path,\n"
"       tag.label\n"
"FROM node\n"
"JOIN tag ON node.tag_id=tag.id\n"
"WHERE node.tag_id=(SELECT tag.id FROM tag WHERE tag.label=:label)\n"
";",
"-----------------------------------------------------------------------------\n"
"-- SQLQ_SELECT_NODE_ROOT\n"
"-- Select root tag-node.\n"
"SELECT node.id,\n"
"       node.parent_id,\n"
"       node.tag_id,\n"
"       node.level,\n"
"       node.path,\n"
"       tag.label\n"
"FROM node\n"
"JOIN tag ON node.tag_id=tag.id\n"
"WHERE (node.parent_id IS NULL) AND (node.level=0)\n"
";",
"-----------------------------------------------------------------------------\n"
"-- SQLQ_SELECT_NODE_CHILDREN\n"
"-- List direct children of tag-node identified by :node_id\n"
"SELECT\n"
"  -- node_i.*, tag_i.label,\n"
"  node_ip1.id,\n"
"  node_ip1.parent_id,\n"
"  node_ip1.tag_id,\n"
"  node_ip1.level,\n"
"  node_ip1.path,\n"
"  tag_ip1.label\n"
"FROM node AS node_i\n"
"JOIN node AS node_ip1 ON node_i.id=node_ip1.parent_id\n"
"JOIN tag AS tag_i ON node_i.tag_id=tag_i.id\n"
"JOIN tag AS tag_ip1 ON node_ip1.tag_id=tag_ip1.id\n"
"WHERE node_i.tag_id=:node_id\n"
";",
};

const int SQL_QUERIES_SELECT_COUNT = 3;
} // End of anonymous namespace.
