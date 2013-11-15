#
#  Examples on the use of Iterators.
#
PROJECT(IteratorExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(ImageRandomConstIteratorWithIndex ImageRandomConstIteratorWithIndex.cxx)
TARGET_LINK_LIBRARIES(ImageRandomConstIteratorWithIndex OTBIO OTBCommon   )

ADD_EXECUTABLE(NeighborhoodIterators4 NeighborhoodIterators4.cxx)
TARGET_LINK_LIBRARIES(NeighborhoodIterators4 OTBIO OTBCommon)

ADD_EXECUTABLE(NeighborhoodIterators5 NeighborhoodIterators5.cxx)
TARGET_LINK_LIBRARIES(NeighborhoodIterators5 OTBIO OTBCommon)

ADD_EXECUTABLE(NeighborhoodIterators1 NeighborhoodIterators1.cxx)
TARGET_LINK_LIBRARIES(NeighborhoodIterators1 OTBIO OTBCommon)

ADD_EXECUTABLE(NeighborhoodIterators2 NeighborhoodIterators2.cxx)
TARGET_LINK_LIBRARIES(NeighborhoodIterators2 OTBIO OTBCommon)

ADD_EXECUTABLE(NeighborhoodIterators3 NeighborhoodIterators3.cxx)
TARGET_LINK_LIBRARIES(NeighborhoodIterators3 OTBIO OTBCommon)

ADD_EXECUTABLE(NeighborhoodIterators6 NeighborhoodIterators6.cxx)
TARGET_LINK_LIBRARIES(NeighborhoodIterators6 OTBIO OTBCommon)

ADD_EXECUTABLE(ImageRegionIteratorWithIndex ImageRegionIteratorWithIndex.cxx)
TARGET_LINK_LIBRARIES(ImageRegionIteratorWithIndex OTBIO OTBCommon)

ADD_EXECUTABLE(ImageRegionIterator ImageRegionIterator.cxx)
TARGET_LINK_LIBRARIES(ImageRegionIterator OTBIO OTBCommon)

ADD_EXECUTABLE(ImageLinearIteratorWithIndex2 ImageLinearIteratorWithIndex2.cxx)
TARGET_LINK_LIBRARIES(ImageLinearIteratorWithIndex2 OTBIO OTBCommon)

ADD_EXECUTABLE(ImageLinearIteratorWithIndex ImageLinearIteratorWithIndex.cxx)
TARGET_LINK_LIBRARIES(ImageLinearIteratorWithIndex OTBIO OTBCommon)

ADD_EXECUTABLE(ImageSliceIteratorWithIndex ImageSliceIteratorWithIndex.cxx)
TARGET_LINK_LIBRARIES(ImageSliceIteratorWithIndex OTBIO OTBCommon)

ADD_EXECUTABLE(ShapedNeighborhoodIterators1 ShapedNeighborhoodIterators1.cxx)
TARGET_LINK_LIBRARIES(ShapedNeighborhoodIterators1  OTBIO OTBCommon )

ADD_EXECUTABLE(ShapedNeighborhoodIterators2 ShapedNeighborhoodIterators2.cxx)
TARGET_LINK_LIBRARIES(ShapedNeighborhoodIterators2  OTBIO OTBCommon )

ADD_EXECUTABLE(IteratorsExamples IteratorsExamples.cxx )
TARGET_LINK_LIBRARIES(IteratorsExamples OTBIO OTBCommon OTBTesting)


IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Iterators)

SET(INPUTDATA ${OTB_DATA_ROOT}/Examples)
SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbIteratorsExamplesTests)

SET(TOL 0.0)


# ------- ImageRegionIteratorWithIndexTest----------

ADD_TEST(itTeImageRegionIteratorWithIndexTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/ImageRegionIteratorWithIndexOutput.png
        ${TEMP}/ImageRegionIteratorWithIndexOutput.png
        ImageRegionIteratorWithIndexTest
        ${INPUTDATA}/ROI_QB_MUL_2.tif
        ${TEMP}/ImageRegionIteratorWithIndexOutput.png
)

# ------- NeighborhoodIterators1Test----------

ADD_TEST(itTeNeighborhoodIterators1Test ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/NeighborhoodIterators1a.png
        ${TEMP}/NeighborhoodIterators1a.png
        NeighborhoodIterators1Test
        ${INPUTDATA}/ROI_QB_PAN_1.tif
        ${TEMP}/NeighborhoodIterators1a.png
)

# ------- NeighborhoodIterators4Test----------

ADD_TEST(itTeNeighborhoodIterators4Test ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/NeighborhoodIterators4a.png
        ${TEMP}/NeighborhoodIterators4a.png
        NeighborhoodIterators4Test
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/NeighborhoodIterators4a.png
    0
)

# ------- NeighborhoodIterators4Test----------

ADD_TEST(itTeNeighborhoodIterators4Test ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/NeighborhoodIterators4b.png
        ${TEMP}/NeighborhoodIterators4b.png
        NeighborhoodIterators4Test
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/NeighborhoodIterators4b.png
    1
)

# ------- NeighborhoodIterators4Test----------

ADD_TEST(itTeNeighborhoodIterators4Test ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/NeighborhoodIterators4c.png
        ${TEMP}/NeighborhoodIterators4c.png
        NeighborhoodIterators4Test
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/NeighborhoodIterators4c.png
    2
)

# ------- NeighborhoodIterators4Test----------

ADD_TEST(itTeNeighborhoodIterators4Test ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/NeighborhoodIterators4d.png
        ${TEMP}/NeighborhoodIterators4d.png
        NeighborhoodIterators4Test
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/NeighborhoodIterators4d.png
    5
)

# ------- NeighborhoodIterators6Test----------

ADD_TEST(itTeNeighborhoodIterators6Test ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/NeighborhoodIterators6a.png
        ${TEMP}/NeighborhoodIterators6a.png
        NeighborhoodIterators6Test
        ${TEMP}/NeighborhoodIterators6a.png
    100 100
)

# ------- NeighborhoodIterators6Test----------

ADD_TEST(itTeNeighborhoodIterators6Test ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/NeighborhoodIterators6b.png
        ${TEMP}/NeighborhoodIterators6b.png
        NeighborhoodIterators6Test
        ${TEMP}/NeighborhoodIterators6b.png
    50 150
)

# ------- NeighborhoodIterators6Test----------

ADD_TEST(itTeNeighborhoodIterators6Test ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/NeighborhoodIterators6c.png
        ${TEMP}/NeighborhoodIterators6c.png
        NeighborhoodIterators6Test
        ${TEMP}/NeighborhoodIterators6c.png
    150 50
)

# ------- ImageRegionIteratorTest----------

ADD_TEST(itTeImageRegionIteratorTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/ImageRegionIteratorOutput.png
        ${TEMP}/ImageRegionIteratorOutput.png
        ImageRegionIteratorTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/ImageRegionIteratorOutput.png
     10 10 110 140
)

# ------- ShapedNeighborhoodIterators2Test----------

ADD_TEST(itTeShapedNeighborhoodIterators2Test ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/ShapedNeighborhoodIterators1b.png
        ${TEMP}/ShapedNeighborhoodIterators1b.png
        ShapedNeighborhoodIterators2Test
        ${INPUTDATA}/BinaryImage.png
        ${TEMP}/ShapedNeighborhoodIterators1b.png
  4
)

INCLUDE_DIRECTORIES(${OTB_SOURCE_DIR}/Testing/Code)
ADD_EXECUTABLE(otbIteratorsExamplesTests otbIteratorsExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbIteratorsExamplesTests OTBBasicFilters OTBDisparityMap OTBTesting)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
