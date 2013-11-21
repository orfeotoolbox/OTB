namespace
{

/****************************************************************************/
//
// 'SQL_CREATE_DB' Generated from file 'sql_create_db.sql'
const char* SQL_CREATE_DB[] = {
"-------------------------------------------------------------------------------\n"
"CREATE TABLE dataset\n"
"(\n"
"        id    INTEGER PRIMARY KEY,\n"
"        hash  TEXT NOT NULL,\n"
"        alias TEXT\n"
");",
"-------------------------------------------------------------------------------\n"
"CREATE TABLE tag\n"
"(\n"
"        id    INTEGER PRIMARY KEY,\n"
"        label TEXT NOT NULL DEFAULT 'Label'\n"
");",
"CREATE UNIQUE INDEX idx_tag_label ON tag( label );",
"-------------------------------------------------------------------------------\n"
"CREATE TABLE tag_node\n"
"(\n"
"        id        INTEGER PRIMARY KEY,\n"
"        parent_id INTEGER REFERENCES tag_node( id ),\n"
"        tag_id    INTEGER NOT NULL REFERENCES tag( id ),\n"
"        level     INTEGER,\n"
"        path      TEXT\n"
");",
"CREATE INDEX idx_tag_node_tid ON tag_node( tag_id );",
"-------------------------------------------------------------------------------\n"
"CREATE TABLE dataset_membership(\n"
"        -- id              INTEGER PRIMARY KEY,\n"
"        dataset_id INTEGER NOT NULL REFERENCES dataset( id ),\n"
"        tag_id     INTEGER NOT NULL REFERENCES tag( id ),\n"
"        PRIMARY KEY( dataset_id, tag_id )\n"
");",
"CREATE INDEX idx_ds_membership_dataset_id ON dataset_membership( dataset_id );",
"-------------------------------------------------------------------------------\n"
"CREATE TABLE dataset_attribute\n"
"(\n"
"        id         INTEGER PRIMARY KEY,\n"
"        dataset_id INTEGER NOT NULL REFERENCES dataset( id ),\n"
"        rank       INTEGER NOT NULL,\n"
"        name       TEXT NOT NULL,\n"
"        value      TEXT\n"
");",
"CREATE INDEX idx_dataset_attribute_dsid ON dataset_attribute( dataset_id );",
};

const int SQL_CREATE_DB_COUNT = 9;

/****************************************************************************/
//
// 'SQL_QUERIES' Generated from file 'sql_queries.sql'
const char* SQL_QUERIES[] = {
"-------------------------------------------------------------------------------\n"
"-- List datasets tagged by given label.\n"
"SELECT dataset.id, tag.label, dataset.alias, dataset.hash\n"
"FROM dataset\n"
"JOIN dataset_membership ON dataset.id=dataset_membership.dataset_id\n"
"JOIN tag ON dataset_membership.tag_id=tag.id\n"
"WHERE tag.label='Datasets';",
"-------------------------------------------------------------------------------\n"
"-- List tags marking a given dataset ordered by tree level.\n"
"SELECT tag_node.path, tag_node.level, tag.id AS id, tag.label\n"
"FROM tag_node\n"
"JOIN tag ON tag_node.tag_id=tag.id\n"
"JOIN dataset_membership ON tag.id=dataset_membership.tag_id\n"
"WHERE dataset_membership.dataset_id=2\n"
"ORDER BY tag_node.level;",
};

const int SQL_QUERIES_COUNT = 2;

/****************************************************************************/
//
// 'SQL_INSERT_ITEMS' Generated from file 'sql_insert_items.sql'
const char* SQL_INSERT_ITEMS[] = {
"-------------------------------------------------------------------------------\n"
"INSERT INTO dataset( id, hash, alias )\n"
"VALUES( 0, '4ecf0c9d296cd27b550543980a1da496', 'lena512color' );",
"INSERT INTO dataset( id, hash, alias )\n"
"VALUES( 1, 'b622d2129232a1d6103a803077c4038d', 'QB_1_ortho' );",
"INSERT INTO dataset( id, hash, alias )\n"
"VALUES( 2, '9e0debeb00975dcc6cec36fbceee55e4', 'Rio de Janeiro (SPOT4)' );",
"-------------------------------------------------------------------------------\n"
"INSERT INTO tag( id, label ) VALUES( 0, 'Root' );",
"-------------------------------------------------------------------------------\n"
"INSERT INTO tag_node VALUES( 0, NULL, 0, 0, '/');",
"   -- Root/Datasets/SPOT\n"
"-------------------------------------------------------------------------------\n"
" -- Datasets - Rio de Janeiro\n"
"INSERT INTO dataset_membership( dataset_id, tag_id ) VALUES( 2, 1 );",
" -- Datasets/SPORT - Rio de Janeiro\n"
"INSERT INTO dataset_membership( dataset_id, tag_id ) VALUES( 2, 5 );",
" -- Datasets - lena512color\n"
"INSERT INTO dataset_membership( dataset_id, tag_id ) VALUES( 0, 1 );",
" -- Datasets/Cached - lena512color\n"
"INSERT INTO dataset_membership( dataset_id, tag_id ) VALUES( 1, 2 );",
" -- Datasets - QB_1_ortho\n"
"INSERT INTO dataset_membership( dataset_id, tag_id ) VALUES( 1, 1 );",
};

const int SQL_INSERT_ITEMS_COUNT = 10;
} // End of anonymous namespace.
