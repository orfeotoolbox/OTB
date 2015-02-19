
# ------- ImageRegionIteratorWithIndexTest----------

otb_add_test(NAME itTeImageRegionIteratorWithIndexTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/ImageRegionIteratorWithIndexOutput.png
    ${TEMP}/ImageRegionIteratorWithIndexOutput.png
  $<TARGET_FILE:ImageRegionIteratorWithIndex>
    ${INPUTDATA}/ROI_QB_MUL_2.tif
    ${TEMP}/ImageRegionIteratorWithIndexOutput.png
)

# ------- NeighborhoodIterators1Test----------

otb_add_test(NAME itTeNeighborhoodIterators1Test COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/NeighborhoodIterators1a.png
    ${TEMP}/NeighborhoodIterators1a.png
  $<TARGET_FILE:NeighborhoodIterators1>
    ${INPUTDATA}/ROI_QB_PAN_1.tif
    ${TEMP}/NeighborhoodIterators1a.png
)

# ------- NeighborhoodIterators4Test----------

otb_add_test(NAME itTeNeighborhoodIterators4aTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/NeighborhoodIterators4a.png
    ${TEMP}/NeighborhoodIterators4a.png
  $<TARGET_FILE:NeighborhoodIterators4>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/NeighborhoodIterators4a.png
    0
)

# ------- NeighborhoodIterators4Test----------

otb_add_test(NAME itTeNeighborhoodIterators4bTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/NeighborhoodIterators4b.png
    ${TEMP}/NeighborhoodIterators4b.png
  $<TARGET_FILE:NeighborhoodIterators4>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/NeighborhoodIterators4b.png
    1
)

# ------- NeighborhoodIterators4Test----------

otb_add_test(NAME itTeNeighborhoodIterators4cTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/NeighborhoodIterators4c.png
    ${TEMP}/NeighborhoodIterators4c.png
  $<TARGET_FILE:NeighborhoodIterators4>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/NeighborhoodIterators4c.png
    2
)

# ------- NeighborhoodIterators4Test----------

otb_add_test(NAME itTeNeighborhoodIterators4dTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/NeighborhoodIterators4d.png
    ${TEMP}/NeighborhoodIterators4d.png
  $<TARGET_FILE:NeighborhoodIterators4>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/NeighborhoodIterators4d.png
    5
)

# ------- NeighborhoodIterators6Test----------

otb_add_test(NAME itTeNeighborhoodIterators6aTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/NeighborhoodIterators6a.png
    ${TEMP}/NeighborhoodIterators6a.png
  $<TARGET_FILE:NeighborhoodIterators6>
    ${TEMP}/NeighborhoodIterators6a.png
    100 100
)

# ------- NeighborhoodIterators6Test----------

otb_add_test(NAME itTeNeighborhoodIterators6bTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/NeighborhoodIterators6b.png
    ${TEMP}/NeighborhoodIterators6b.png
  $<TARGET_FILE:NeighborhoodIterators6>
    ${TEMP}/NeighborhoodIterators6b.png
    50 150
)

# ------- NeighborhoodIterators6Test----------

otb_add_test(NAME itTeNeighborhoodIterators6cTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/NeighborhoodIterators6c.png
    ${TEMP}/NeighborhoodIterators6c.png
  $<TARGET_FILE:NeighborhoodIterators6>
    ${TEMP}/NeighborhoodIterators6c.png
    150 50
)

# ------- ImageRegionIteratorTest----------

otb_add_test(NAME itTeImageRegionIteratorTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/ImageRegionIteratorOutput.png
    ${TEMP}/ImageRegionIteratorOutput.png
  $<TARGET_FILE:ImageRegionIterator>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/ImageRegionIteratorOutput.png
    10 10 110 140
)

# ------- ShapedNeighborhoodIterators2Test----------

otb_add_test(NAME itTeShapedNeighborhoodIterators2Test COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/ShapedNeighborhoodIterators1b.png
    ${TEMP}/ShapedNeighborhoodIterators1b.png
  $<TARGET_FILE:ShapedNeighborhoodIterators2>
    ${INPUTDATA}/BinaryImage.png
    ${TEMP}/ShapedNeighborhoodIterators1b.png
    4
)
