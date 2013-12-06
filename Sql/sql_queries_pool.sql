-----------------------------------------------------------------------------
-- List datasets tagged by given label.
SELECT dataset.id, tag.label, dataset.alias, dataset.hash
FROM dataset
JOIN dataset_membership ON dataset.id=dataset_membership.dataset_id
JOIN tag ON dataset_membership.tag_id=tag.id
WHERE tag.label='Datasets';

-----------------------------------------------------------------------------
-- List tags marking a given dataset ordered by tree level.
SELECT tag_node.path, tag_node.level, tag.id AS id, tag.label
FROM tag_node
JOIN tag ON tag_node.tag_id=tag.id
JOIN dataset_membership ON tag.id=dataset_membership.tag_id
WHERE dataset_membership.dataset_id=2
ORDER BY tag_node.level;

-----------------------------------------------------------------------------
-- List tag-node given tag-label.
SELECT tag.label,
       tag_node.id,
       tag_node.parent_id,
       tag_node.tag_id,
       tag_node.level,
       tag_node.path
FROM tag_node
JOIN tag ON tag_node.tag_id=tag.id
WHERE tag_node.tag_id=2
ORDER BY tag_node.level;

-----------------------------------------------------------------------------
-- List hierarchy tree as column indents.
SELECT tn_0.id AS level_0,
        tn_1.id AS level_1,
        tn_2.id AS level_2,
        tn_3.id AS level_3
FROM tag_node AS tn_0
LEFT JOIN tag_node AS tn_1 ON tn_0.id=tn_1.parent_id
LEFT JOIN tag_node AS tn_2 ON tn_1.id=tn_2.parent_id
LEFT JOIN tag_node AS tn_3 ON tn_2.id=tn_3.parent_id
LEFT JOIN tag_node AS tn_4 ON tn_3.id=tn_4.parent_id
WHERE tn_0.tag_id=(SELECT tag.id FROM tag WHERE tag.label='Root');

-----------------------------------------------------------------------------
-- List all leaf nodes.
SELECT tag_node.id, tag_node.level, tag_node.path, tag.label
FROM tag_node 
LEFT JOIN tag_node AS tn_2 ON tag_node.id = tn_2.parent_id
JOIN tag ON tag_node.tag_id=tag.id
WHERE tn_2.id IS NULL;

-----------------------------------------------------------------------------
-- List all non-leaf nodes.
SELECT tag_node.id, tag_node.level, tag_node.path, tag.label, tn_2.id AS child_id
FROM tag_node 
JOIN tag_node AS tn_2 ON tag_node.id = tn_2.parent_id
JOIN tag ON tag_node.tag_id=tag.id
ORDER BY tag_node.id;

-----------------------------------------------------------------------------
-- List all children of all/given (non-leaf) node(s).
SELECT tag_node.id, tag_node.level, tag_node.path, tag.label, tn_2.id AS child_id, tn_2.tag_id AS child_tag_id, t2.label
FROM tag_node 
JOIN tag_node AS tn_2 ON tag_node.id = tn_2.parent_id
JOIN tag ON tag_node.tag_id=tag.id
JOIN tag AS t2 ON tn_2.tag_id=t2.id
-- WHERE tag_node.tag_id=(SELECT tag.id FROM tag WHERE tag.label='Root')
ORDER BY tag_node.id;

-----------------------------------------------------------------------------
-- Select root tag-node.
SELECT tag_node.id, tag_node.parent_id, tag_node.tag_id, tag_node.level, tag_node.path, tag.label
FROM tag_node
JOIN tag ON tag_node.tag_id=tag.id
WHERE (tag_node.parent_id IS NULL) AND (tag_node.level=0);
