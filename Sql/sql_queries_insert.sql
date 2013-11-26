-----------------------------------------------------------------------------
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
