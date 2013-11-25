-- Enable foreign keys support
PRAGMA foreign_keys = ON;

-------------------------------------------------------------------------------
INSERT INTO tag( id, label ) VALUES( 0, 'Root' );
INSERT INTO tag( id, label ) VALUES( 1, 'Datasets' );
INSERT INTO tag( id, label ) VALUES( 2, 'Cached' );

-------------------------------------------------------------------------------
INSERT INTO tag_node VALUES( 0, NULL, 0, 0, '/');       -- Root
INSERT INTO tag_node VALUES( 1, 0,    1, 1, '/0' );     -- Root/Datasets
INSERT INTO tag_node VALUES( 2, 1,    2, 2, '/0/1' );   -- Root/Datasets/Cached
