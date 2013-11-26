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
