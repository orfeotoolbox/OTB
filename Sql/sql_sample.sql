-- Enable foreign keys support
PRAGMA foreign_keys = ON;

-------------------------------------------------------------------------------
INSERT INTO dataset( id, hash, alias )
VALUES( 0, '4ecf0c9d296cd27b550543980a1da496', 'lena512color' );

INSERT INTO dataset( id, hash, alias )
VALUES( 1, 'b622d2129232a1d6103a803077c4038d', 'QB_1_ortho' );

INSERT INTO dataset( id, hash, alias )
VALUES( 2, '9e0debeb00975dcc6cec36fbceee55e4', 'Rio de Janeiro (SPOT4)' );

-------------------------------------------------------------------------------
INSERT INTO tag( id, label ) VALUES( 3, 'Pleiades' );
INSERT INTO tag( id, label ) VALUES( 4, 'Quickbird' );
INSERT INTO tag( id, label ) VALUES( 5, 'SPOT' );

-------------------------------------------------------------------------------
INSERT INTO tag_node VALUES( 3, 1,    3, 2, '/0/1' );   -- Root/Datasets/Pleiades
INSERT INTO tag_node VALUES( 4, 1,    4, 2, '/0/1' );   -- Root/Datasets/Quickbird
INSERT INTO tag_node VALUES( 5, 1,    5, 2, '/0/1' );   -- Root/Datasets/SPOT

-------------------------------------------------------------------------------
 -- Datasets - Rio de Janeiro
INSERT INTO dataset_membership( dataset_id, tag_id ) VALUES( 2, 1 );
 -- Datasets/SPORT - Rio de Janeiro
INSERT INTO dataset_membership( dataset_id, tag_id ) VALUES( 2, 5 );
 -- Datasets - lena512color
INSERT INTO dataset_membership( dataset_id, tag_id ) VALUES( 0, 1 );
 -- Datasets/Cached - lena512color
INSERT INTO dataset_membership( dataset_id, tag_id ) VALUES( 1, 2 );
 -- Datasets - QB_1_ortho
INSERT INTO dataset_membership( dataset_id, tag_id ) VALUES( 1, 1 );
