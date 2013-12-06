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

-----------------------------------------------------------------------------
-- SQLQ_SELECT_TAG_NODE_ROOT
-- Select root tag-node.
SELECT tag_node.id,
       tag_node.parent_id,
       tag_node.tag_id,
       tag_node.level,
       tag_node.path,
       tag.label
FROM tag_node
JOIN tag ON tag_node.tag_id=tag.id
WHERE (tag_node.parent_id IS NULL) AND (tag_node.level=0);

-----------------------------------------------------------------------------
-- SQLQ_SELECT_TAG_NODE_CHILDREN
-- List direct children of tag-node identified by :tag_node_id
SELECT
  -- tag_node_i.*, tag_i.label,
  tag_node_ip1.id,
  tag_node_ip1.parent_id,
  tag_node_ip1.tag_id,
  tag_node_ip1.level,
  tag_node_ip1.path,
  tag_ip1.label
FROM tag_node AS tag_node_i
JOIN tag_node AS tag_node_ip1 ON tag_node_i.id=tag_node_ip1.parent_id
JOIN tag AS tag_i ON tag_node_i.tag_id=tag_i.id
JOIN tag AS tag_ip1 ON tag_node_ip1.tag_id=tag_ip1.id
WHERE tag_node_i.tag_id=:tag_node_id;
