PROJECT(OBIAExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(ImageToLabelToImage ImageToLabelToImage.cxx )
TARGET_LINK_LIBRARIES(ImageToLabelToImage OTBCommon OTBIO)

IF(ITK_USE_REVIEW)
  ADD_EXECUTABLE(ShapeAttributeComputation ShapeAttributeComputation.cxx )
  TARGET_LINK_LIBRARIES(ShapeAttributeComputation OTBCommon OTBIO)
  
  ADD_EXECUTABLE(KeepNObjects KeepNObjects.cxx )
  TARGET_LINK_LIBRARIES(KeepNObjects OTBCommon OTBIO)
ENDIF(ITK_USE_REVIEW)

ADD_EXECUTABLE(RadiometricAttributesLabelMapFilterExample RadiometricAttributesLabelMapFilterExample.cxx )
TARGET_LINK_LIBRARIES(RadiometricAttributesLabelMapFilterExample OTBCommon OTBIO OTBBasicFilters OTBSpatialReasoning  OTBFeatureExtraction OTBLearning)

ADD_EXECUTABLE(LabelMapToVectorData LabelMapToVectorData.cxx )
TARGET_LINK_LIBRARIES(LabelMapToVectorData OTBCommon OTBIO OTBBasicFilters)

ADD_EXECUTABLE(HooverMetricsEstimation HooverMetricsEstimation.cxx )
TARGET_LINK_LIBRARIES(HooverMetricsEstimation OTBCommon OTBIO OTBBasicFilters)

IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/OBIA)
SET(INPUTDATA ${OTB_DATA_ROOT}/Examples)

#Remote sensing images (large images )
IF(OTB_DATA_USE_LARGEINPUT)
  SET(INPUTLARGEDATA ${OTB_DATA_LARGEINPUT_ROOT} )
ENDIF(OTB_DATA_USE_LARGEINPUT)

SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbOBIAExamplesTests)
SET(EXE_TESTS1 ${CXX_TEST_PATH}/otbOBIAExamplesTests1)

SET(TOL 0.0)
SET(EPS 0.001)

ADD_TEST( trTeOBIAImageToLabelToImageTest ${EXE_TESTS}
        --compare-image ${TOL}          ${BASELINE}/OBIAI2L2IOutput.tif
                                        ${TEMP}/OBIAI2L2IOutput.tif
        OBIAImageToLabelToImageTest
        ${INPUTDATA}/MSLabeledOutput.tif
        ${TEMP}/OBIAI2L2IOutput.tif
        ${TEMP}/OBIAI2L2IInPretty.png
        ${TEMP}/OBIAI2L2IOutPretty.png
        1 1108 0
        )
IF(ITK_USE_REVIEW)

ADD_TEST(trTeOBIAShapeAttributeComputationTest ${EXE_TESTS1}
        --compare-ascii ${EPS}        ${BASELINE}/OBIAShapeAttribute.txt
                                ${TEMP}/OBIAShapeAttribute.txt
        OBIAShapeAttributeComputationTest
        ${INPUTDATA}/MSLabeledOutput.tif
        ${TEMP}/OBIAShapeAttribute.txt
)

ADD_TEST(trTeOBIARadiometricAttributesLabelMapFilterTest ${EXE_TESTS1}
        --compare-image ${TOL}        ${BASELINE}/OBIARadiometricAttributes.png
                                ${TEMP}/OBIARadiometricAttributes.png
        RadiometricAttributesLabelMapFilterExample
        ${INPUTDATA}/qb_RoadExtract.tif
        ${TEMP}/OBIARadiometricAttributes.png
        ${TEMP}/qb_RoadExtract_Radiometry_pretty.png
        STATS::Band1::Mean
        0
        0.5
        16
        16
        50
        1.0
)
ENDIF(ITK_USE_REVIEW)
    
ADD_TEST(trTeLabelMapToVectorData ${EXE_TESTS}
        LabelMapToVectorDataTest
        ${INPUTDATA}/rcc8_mire1.png
        ${TEMP}/rcc8_mire2_vectorizer.shp
)
#need to add the test of radiometric labelmap filter
  
INCLUDE_DIRECTORIES(${OTB_SOURCE_DIR}/Testing/Code)
ADD_EXECUTABLE(otbOBIAExamplesTests otbOBIAExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbOBIAExamplesTests OTBCommon OTBIO OTBTesting)

# Link with ITKReview if used in ITK
IF(ITK_USE_REVIEW)
  ADD_EXECUTABLE(otbOBIAExamplesTests1 otbOBIAExamplesTests1.cxx)
  TARGET_LINK_LIBRARIES(otbOBIAExamplesTests1 ${ITK_LIBRARIES} OTBCommon OTBIO OTBTesting)
ENDIF(ITK_USE_REVIEW)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
