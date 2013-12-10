-----------------------------------------------------------------------------
-- SQLQ_SELECT_NODE_BY_TAG_LABEL
-- Find node given tag label.
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
-- Find root node.
SELECT node.id,
       node.parent_id,
       node.tag_id,
       node.label,
       node.level,
       node.path
FROM node
WHERE (node.parent_id IS NULL) AND (node.level=0);

-----------------------------------------------------------------------------
-- SQLQ_SELECT_NODE
-- Find root node.
SELECT node.id,
       node.parent_id,
       node.tag_id,
       node.label,
       node.level,
       node.path
FROM node
WHERE node.id=:id;

-----------------------------------------------------------------------------
-- SQLQ_SELECT_NODE_CHILDREN
-- List direct children of node identified by :node_id
SELECT
  node_ip1.id,
  node_ip1.parent_id,
  node_ip1.tag_id,
  node_ip1.label,
  node_ip1.level,
  node_ip1.path
FROM node AS node_i
JOIN node AS node_ip1 ON node_i.id=node_ip1.parent_id
WHERE node_i.id=:node_id;

-----------------------------------------------------------------------------
-- SQLQ_SELECT_NODE_CHILD
-- Find child labelled :child_label of node identified by :node_id
SELECT
  node_ip1.id,
  node_ip1.parent_id,
  node_ip1.tag_id,
  node_ip1.label,
  node_ip1.level,
  node_ip1.path
FROM node AS node_i
JOIN node AS node_ip1 ON node_i.id=node_ip1.parent_id
WHERE node_i.id=:node_id AND node_ip1.label=:child_label;
