-----------------------------------------------------------------------------
-- SQLQ_INSERT_TAG_NODE
-- Insert tag-node for tag :child_label under parent-node of tag-node
--  :parent_label.
INSERT INTO tag_node( parent_id, tag_id, level, path )
        SELECT -- ID (automatic)
               tag_node.id AS parent_id,
               (SELECT tag.id FROM tag WHERE tag.label=:child_label) AS tag_id,
               tag_node.level+1 AS level,
               rtrim( tag_node.path, '/' ) || '/' || tag_node.id AS path
        FROM   tag_node
        JOIN   tag ON tag_node.tag_id=tag.id
        WHERE  tag.id=(SELECT tag.id FROM tag WHERE tag.label=:parent_label);

-----------------------------------------------------------------------------
-- SQLQ_INSERT_DATASET_MEMBERSHIP
-- Add dataset-membership of dataset %1 to each tag related to
-- %2 tag-node path list of the form (<id_0>, ...).
INSERT INTO dataset_membership( dataset_id, tag_id )
       SELECT %1 AS 'dataset_id',
              tag_node.tag_id
       FROM   tag_node
       JOIN   tag ON tag_node.tag_id=tag.id
       WHERE  tag_node.id IN (%2);
