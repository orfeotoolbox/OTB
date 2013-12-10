-----------------------------------------------------------------------------
-- SQLQ_INSERT_NODE
-- Insert tag-node for tag :child_label under parent-node of tag-node
--  :parent_label.
INSERT INTO node( parent_id, tag_id, label, level, path )
        SELECT -- ID (automatic)
               node.id AS parent_id,
               (SELECT tag.id FROM tag WHERE tag.label=:child_label) AS tag_id,
               :child_label AS label,
               node.level+1 AS level,
               rtrim( node.path, '/' ) || '/' || node.id AS path
        FROM   node
        -- JOIN   tag ON node.tag_id=tag.id
        -- WHERE  tag.id=(SELECT tag.id FROM tag WHERE tag.label=:parent_label)
        WHERE  node.label=:parent_label;

-----------------------------------------------------------------------------
-- SQLQ_INSERT_NODE_CHILD
-- Insert node labelled as :child_label under parent node :parent_id
INSERT INTO node( parent_id, tag_id, label, level, path )
        SELECT -- ID (automatic)
               node.id AS parent_id,
               (SELECT tag.id FROM tag WHERE tag.label=:child_label) AS tag_id,
               :child_label AS label,
               node.level+1 AS level,
               rtrim( node.path, '/' ) || '/' || node.id AS path
        FROM   node
        WHERE  node.id=:parent_id;

-----------------------------------------------------------------------------
-- SQLQ_INSERT_DATASET_TAG_MEMBERSHIP
-- Add dataset-membership of dataset %1 to each tag related to
-- %2 tag-node path list of the form (<id_0>, ...).
INSERT INTO dataset_tag_membership( dataset_id, tag_id )
       SELECT %1 AS 'dataset_id',
              node.tag_id
       FROM   node
       JOIN   tag ON node.tag_id=tag.id
       WHERE  node.id IN (%2);

-----------------------------------------------------------------------------
-- SQLQ_INSERT_DATASET_NODE_MEMBERSHIP
-- Add dataset-membership of dataset identified by :dataset_id to node
-- identified bby :node_id.
INSERT INTO dataset_node_membership( dataset_id, node_id )
VALUES( :dataset_id, :node_id );
