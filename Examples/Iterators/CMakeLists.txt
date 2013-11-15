#
#  Examples on the use of Iterators.
#
project(IteratorExamples)
include_regular_expression("^.*$")

add_executable(ImageRandomConstIteratorWithIndex ImageRandomConstIteratorWithIndex.cxx)
target_link_libraries(ImageRandomConstIteratorWithIndex OTBIO OTBCommon   )

add_executable(NeighborhoodIterators4 NeighborhoodIterators4.cxx)
target_link_libraries(NeighborhoodIterators4 OTBIO OTBCommon)

add_executable(NeighborhoodIterators5 NeighborhoodIterators5.cxx)
target_link_libraries(NeighborhoodIterators5 OTBIO OTBCommon)

add_executable(NeighborhoodIterators1 NeighborhoodIterators1.cxx)
target_link_libraries(NeighborhoodIterators1 OTBIO OTBCommon)

add_executable(NeighborhoodIterators2 NeighborhoodIterators2.cxx)
target_link_libraries(NeighborhoodIterators2 OTBIO OTBCommon)

add_executable(NeighborhoodIterators3 NeighborhoodIterators3.cxx)
target_link_libraries(NeighborhoodIterators3 OTBIO OTBCommon)

add_executable(NeighborhoodIterators6 NeighborhoodIterators6.cxx)
target_link_libraries(NeighborhoodIterators6 OTBIO OTBCommon)

add_executable(ImageRegionIteratorWithIndex ImageRegionIteratorWithIndex.cxx)
target_link_libraries(ImageRegionIteratorWithIndex OTBIO OTBCommon)

add_executable(ImageRegionIterator ImageRegionIterator.cxx)
target_link_libraries(ImageRegionIterator OTBIO OTBCommon)

add_executable(ImageLinearIteratorWithIndex2 ImageLinearIteratorWithIndex2.cxx)
target_link_libraries(ImageLinearIteratorWithIndex2 OTBIO OTBCommon)

add_executable(ImageLinearIteratorWithIndex ImageLinearIteratorWithIndex.cxx)
target_link_libraries(ImageLinearIteratorWithIndex OTBIO OTBCommon)

add_executable(ImageSliceIteratorWithIndex ImageSliceIteratorWithIndex.cxx)
target_link_libraries(ImageSliceIteratorWithIndex OTBIO OTBCommon)

add_executable(ShapedNeighborhoodIterators1 ShapedNeighborhoodIterators1.cxx)
target_link_libraries(ShapedNeighborhoodIterators1  OTBIO OTBCommon )

add_executable(ShapedNeighborhoodIterators2 ShapedNeighborhoodIterators2.cxx)
target_link_libraries(ShapedNeighborhoodIterators2  OTBIO OTBCommon )

add_executable(IteratorsExamples IteratorsExamples.cxx )
target_link_libraries(IteratorsExamples OTBIO OTBCommon OTBTesting)


if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Iterators)

set(INPUTDATA ${OTB_DATA_ROOT}/Examples)
set(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

set(EXE_TESTS ${CXX_TEST_PATH}/otbIteratorsExamplesTests)

set(TOL 0.0)


# ------- ImageRegionIteratorWithIndexTest----------

add_test(itTeImageRegionIteratorWithIndexTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/ImageRegionIteratorWithIndexOutput.png
        ${TEMP}/ImageRegionIteratorWithIndexOutput.png
        ImageRegionIteratorWithIndexTest
        ${INPUTDATA}/ROI_QB_MUL_2.tif
        ${TEMP}/ImageRegionIteratorWithIndexOutput.png
)

# ------- NeighborhoodIterators1Test----------

add_test(itTeNeighborhoodIterators1Test ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/NeighborhoodIterators1a.png
        ${TEMP}/NeighborhoodIterators1a.png
        NeighborhoodIterators1Test
        ${INPUTDATA}/ROI_QB_PAN_1.tif
        ${TEMP}/NeighborhoodIterators1a.png
)

# ------- NeighborhoodIterators4Test----------

add_test(itTeNeighborhoodIterators4Test ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/NeighborhoodIterators4a.png
        ${TEMP}/NeighborhoodIterators4a.png
        NeighborhoodIterators4Test
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/NeighborhoodIterators4a.png
    0
)

# ------- NeighborhoodIterators4Test----------

add_test(itTeNeighborhoodIterators4Test ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/NeighborhoodIterators4b.png
        ${TEMP}/NeighborhoodIterators4b.png
        NeighborhoodIterators4Test
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/NeighborhoodIterators4b.png
    1
)

# ------- NeighborhoodIterators4Test----------

add_test(itTeNeighborhoodIterators4Test ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/NeighborhoodIterators4c.png
        ${TEMP}/NeighborhoodIterators4c.png
        NeighborhoodIterators4Test
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/NeighborhoodIterators4c.png
    2
)

# ------- NeighborhoodIterators4Test----------

add_test(itTeNeighborhoodIterators4Test ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/NeighborhoodIterators4d.png
        ${TEMP}/NeighborhoodIterators4d.png
        NeighborhoodIterators4Test
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/NeighborhoodIterators4d.png
    5
)

# ------- NeighborhoodIterators6Test----------

add_test(itTeNeighborhoodIterators6Test ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/NeighborhoodIterators6a.png
        ${TEMP}/NeighborhoodIterators6a.png
        NeighborhoodIterators6Test
        ${TEMP}/NeighborhoodIterators6a.png
    100 100
)

# ------- NeighborhoodIterators6Test----------

add_test(itTeNeighborhoodIterators6Test ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/NeighborhoodIterators6b.png
        ${TEMP}/NeighborhoodIterators6b.png
        NeighborhoodIterators6Test
        ${TEMP}/NeighborhoodIterators6b.png
    50 150
)

# ------- NeighborhoodIterators6Test----------

add_test(itTeNeighborhoodIterators6Test ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/NeighborhoodIterators6c.png
        ${TEMP}/NeighborhoodIterators6c.png
        NeighborhoodIterators6Test
        ${TEMP}/NeighborhoodIterators6c.png
    150 50
)

# ------- ImageRegionIteratorTest----------

add_test(itTeImageRegionIteratorTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/ImageRegionIteratorOutput.png
        ${TEMP}/ImageRegionIteratorOutput.png
        ImageRegionIteratorTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/ImageRegionIteratorOutput.png
     10 10 110 140
)

# ------- ShapedNeighborhoodIterators2Test----------

add_test(itTeShapedNeighborhoodIterators2Test ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/ShapedNeighborhoodIterators1b.png
        ${TEMP}/ShapedNeighborhoodIterators1b.png
        ShapedNeighborhoodIterators2Test
        ${INPUTDATA}/BinaryImage.png
        ${TEMP}/ShapedNeighborhoodIterators1b.png
  4
)

include_directories(${OTB_SOURCE_DIR}/Testing/Code)
add_executable(otbIteratorsExamplesTests otbIteratorsExamplesTests.cxx)
target_link_libraries(otbIteratorsExamplesTests OTBBasicFilters OTBDisparityMap OTBTesting)

endif( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
