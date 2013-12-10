-----------------------------------------------------------------------------
-- SQLQ_SELECT_NODE_BY_TAG_LABEL
-- Find tag-node given tag-label.
SELECT node.id,
       node.parent_id,
       node.tag_id,
       node.level,
       node.path,
       tag.label
FROM node
JOIN tag ON node.tag_id=tag.id
WHERE node.tag_id=(SELECT tag.id FROM tag WHERE tag.label=:label);

-----------------------------------------------------------------------------
-- SQLQ_SELECT_NODE_ROOT
-- Select root tag-node.
SELECT node.id,
       node.parent_id,
       node.tag_id,
       node.level,
       node.path,
       tag.label
FROM node
JOIN tag ON node.tag_id=tag.id
WHERE (node.parent_id IS NULL) AND (node.level=0);

-----------------------------------------------------------------------------
-- SQLQ_SELECT_NODE_CHILDREN
-- List direct children of tag-node identified by :node_id
SELECT
  -- node_i.*, tag_i.label,
  node_ip1.id,
  node_ip1.parent_id,
  node_ip1.tag_id,
  node_ip1.level,
  node_ip1.path,
  tag_ip1.label
FROM node AS node_i
JOIN node AS node_ip1 ON node_i.id=node_ip1.parent_id
JOIN tag AS tag_i ON node_i.tag_id=tag_i.id
JOIN tag AS tag_ip1 ON node_ip1.tag_id=tag_ip1.id
WHERE node_i.tag_id=:node_id;
