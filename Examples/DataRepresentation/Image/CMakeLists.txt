project(ImageExamples)
include_regular_expression("^.*$")

add_executable(Image1 Image1.cxx )
target_link_libraries(Image1 OTBCommon OTBIO )

add_executable(Image2 Image2.cxx )
target_link_libraries(Image2 OTBCommon OTBIO )

add_executable(Image3 Image3.cxx )
target_link_libraries(Image3 OTBCommon OTBIO )

add_executable(Image4 Image4.cxx )
target_link_libraries(Image4 OTBCommon OTBIO )

add_executable(Image5 Image5.cxx )
target_link_libraries(Image5 OTBCommon OTBIO )

add_executable(Image6 Image6.cxx )
target_link_libraries(Image6 OTBCommon OTBIO )

add_executable(RGBImage RGBImage.cxx )
target_link_libraries(RGBImage OTBCommon OTBIO )

add_executable(VectorImage VectorImage.cxx )
target_link_libraries(VectorImage OTBCommon OTBIO )

add_executable(ImageAdaptor1 ImageAdaptor1.cxx )
target_link_libraries(ImageAdaptor1 OTBCommon OTBIO )

add_executable(ImageAdaptor2 ImageAdaptor2.cxx )
target_link_libraries(ImageAdaptor2 OTBCommon OTBIO )

add_executable(ImageAdaptor3 ImageAdaptor3.cxx )
target_link_libraries(ImageAdaptor3 OTBCommon OTBIO )

add_executable(ImageAdaptor4 ImageAdaptor4.cxx )
target_link_libraries(ImageAdaptor4 OTBCommon OTBIO )

add_executable(ImageListExample ImageListExample.cxx )
target_link_libraries(ImageListExample OTBCommon OTBIO )


if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/DataRepresentation/Image)

set(INPUTDATA ${OTB_DATA_ROOT}/Examples)
set(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

set(EXE_TESTS ${CXX_TEST_PATH}/otbImageExamplesTests)

set(TOL 0.0)


# ------- ImageAdaptor4Test----------

add_test(diTeImageAdaptor4Test1 ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/ImageAdaptorThresholdingA.png
        ${TEMP}/ImageAdaptorThresholdingA.png
        ImageAdaptor4Test
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/ImageAdaptorThresholdingA.png
    100
)

# ------- ImageAdaptor4Test----------

add_test(diTeImageAdaptor4Test2 ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/ImageAdaptorThresholdingB.png
        ${TEMP}/ImageAdaptorThresholdingB.png
        ImageAdaptor4Test
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/ImageAdaptorThresholdingB.png
    200
)

# ------- Image5Test----------

add_test(diTeImage5Test ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/Image5.png
        ${TEMP}/Image5.png
        Image5Test
        ${TEMP}/Image5.png
)

# ------- Image6Test----------

add_test(diTeImage6Test ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/Image6.tif
        ${TEMP}/Image6.tif
        Image6Test
        ${TEMP}/Image6.tif
)

include_directories(${OTB_SOURCE_DIR}/Testing/Code)
add_executable(otbImageExamplesTests otbImageExamplesTests.cxx)
target_link_libraries(otbImageExamplesTests   ${ITK_LIBRARIES} OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale  OTBTesting)

endif()
