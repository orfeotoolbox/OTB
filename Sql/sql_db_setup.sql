-----------------------------------------------------------------------------
INSERT INTO tag( id, label ) VALUES( 1, 'Root' );
INSERT INTO tag( id, label ) VALUES( 2, 'Datasets' );
INSERT INTO tag( id, label ) VALUES( 3, 'Cached' );

-----------------------------------------------------------------------------
-- Root
INSERT INTO tag_node( id, parent_id, tag_id, level, path )
VALUES( 1, NULL, 1, 0, '/');
-- Root/Datasets
INSERT INTO tag_node( id, parent_id, tag_id, level, path )
VALUES( 2, 1,    2, 1, '/1' );
-- Root/Datasets/Cached
INSERT INTO tag_node( id, parent_id, tag_id, level, path )
VALUES( 3, 2,    3, 2, '/1/2' );
