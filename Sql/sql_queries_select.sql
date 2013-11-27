-----------------------------------------------------------------------------
-- SQLQ_SELECT_TAG_NODE_BY_TAG_LABEL
-- Find tag-node given tag-label.
SELECT tag_node.id,
       tag_node.parent_id,
       tag_node.tag_id,
       tag_node.level,
       tag_node.path,
       tag.label
FROM tag_node
JOIN tag ON tag_node.tag_id=tag.id
WHERE tag_node.tag_id=(SELECT tag.id FROM tag WHERE tag.label=:label);
