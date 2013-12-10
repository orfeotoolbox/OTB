-----------------------------------------------------------------------------
-- List datasets tagged by given label.
SELECT dataset.id, tag.label, dataset.alias, dataset.hash
FROM dataset
JOIN dataset_membership ON dataset.id=dataset_membership.dataset_id
JOIN tag ON dataset_membership.tag_id=tag.id
WHERE tag.label='Datasets';

-----------------------------------------------------------------------------
-- List tags marking a given dataset ordered by tree level.
SELECT node.path, node.level, tag.id AS id, tag.label
FROM node
JOIN tag ON node.tag_id=tag.id
JOIN dataset_membership ON tag.id=dataset_membership.tag_id
WHERE dataset_membership.dataset_id=2
ORDER BY node.level;

-----------------------------------------------------------------------------
-- List tag-node given tag-label.
SELECT tag.label,
       node.id,
       node.parent_id,
       node.tag_id,
       node.level,
       node.path
FROM node
JOIN tag ON node.tag_id=tag.id
WHERE node.tag_id=2
ORDER BY node.level;

-----------------------------------------------------------------------------
-- List hierarchy tree as column indents.
SELECT tn_0.id AS level_0,
        tn_1.id AS level_1,
        tn_2.id AS level_2,
        tn_3.id AS level_3
FROM node AS tn_0
LEFT JOIN node AS tn_1 ON tn_0.id=tn_1.parent_id
LEFT JOIN node AS tn_2 ON tn_1.id=tn_2.parent_id
LEFT JOIN node AS tn_3 ON tn_2.id=tn_3.parent_id
LEFT JOIN node AS tn_4 ON tn_3.id=tn_4.parent_id
WHERE tn_0.tag_id=(SELECT tag.id FROM tag WHERE tag.label='Root');

-----------------------------------------------------------------------------
-- List all leaf nodes.
SELECT node.id, node.level, node.path, tag.label
FROM node 
LEFT JOIN node AS tn_2 ON node.id = tn_2.parent_id
JOIN tag ON node.tag_id=tag.id
WHERE tn_2.id IS NULL;

-----------------------------------------------------------------------------
-- List all non-leaf nodes.
SELECT node.id, node.level, node.path, tag.label, tn_2.id AS child_id
FROM node 
JOIN node AS tn_2 ON node.id = tn_2.parent_id
JOIN tag ON node.tag_id=tag.id
ORDER BY node.id;

-----------------------------------------------------------------------------
-- List all children of all/given (non-leaf) node(s).
SELECT node.id, node.level, node.path, tag.label, tn_2.id AS child_id, tn_2.tag_id AS child_tag_id, t2.label
FROM node 
JOIN node AS tn_2 ON node.id = tn_2.parent_id
JOIN tag ON node.tag_id=tag.id
JOIN tag AS t2 ON tn_2.tag_id=t2.id
-- WHERE node.tag_id=(SELECT tag.id FROM tag WHERE tag.label='Root')
ORDER BY node.id;

-----------------------------------------------------------------------------
-- Select root tag-node.
SELECT node.id, node.parent_id, node.tag_id, node.level, node.path, tag.label
FROM node
JOIN tag ON node.tag_id=tag.id
WHERE (node.parent_id IS NULL) AND (node.level=0);

-----------------------------------------------------------------------------
-- Select all children of a node which are leaves (have no child).
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
LEFT JOIN node AS node_ip2 ON node_ip1.id=node_ip2.parent_id
JOIN tag AS tag_i ON node_i.tag_id=tag_i.id
JOIN tag AS tag_ip1 ON node_ip1.tag_id=tag_ip1.id
WHERE node_i.tag_id=2 AND node_ip2.id IS NULL;
