-----------------------------------------------------------------------------
INSERT INTO tag( id, label ) VALUES( 1, 'Root' );
INSERT INTO tag( id, label ) VALUES( 2, 'Datasets' );
INSERT INTO tag( id, label ) VALUES( 3, 'Temporary' );

-----------------------------------------------------------------------------
-- Root
INSERT INTO node( id, parent_id, tag_id, label, level, path )
VALUES( 1, NULL, 1, 'Root',      0, '/');
-- Root/Datasets
INSERT INTO node( id, parent_id, tag_id, label, level, path )
VALUES( 2, 1,    2, 'Datasets',  1, '/1' );
-- Root/Datasets/Cached
INSERT INTO node( id, parent_id, tag_id, label, level, path )
VALUES( 3, 2,    3, 'Temporary', 2, '/1/2' );
