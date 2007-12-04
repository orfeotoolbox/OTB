PROJECT(ImageExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

SET(TEMP ${ITK_BINARY_DIR}/Testing/Temporary)

ADD_EXECUTABLE(Image1 Image1.cxx )
TARGET_LINK_LIBRARIES(Image1 OTBCommon ITKCommon OTBIO ITKIO)

ADD_EXECUTABLE(Image2 Image2.cxx )
TARGET_LINK_LIBRARIES(Image2 OTBCommon ITKCommon OTBIO ITKIO)

ADD_EXECUTABLE(Image3 Image3.cxx )
TARGET_LINK_LIBRARIES(Image3 OTBCommon ITKCommon OTBIO ITKIO)

ADD_EXECUTABLE(Image4 Image4.cxx )
TARGET_LINK_LIBRARIES(Image4 OTBCommon ITKCommon OTBIO ITKIO)

ADD_EXECUTABLE(Image5 Image5.cxx )
TARGET_LINK_LIBRARIES(Image5 OTBCommon ITKCommon OTBIO ITKIO)

ADD_EXECUTABLE(RGBImage RGBImage.cxx )
TARGET_LINK_LIBRARIES(RGBImage OTBCommon OTBIO ITKCommon ITKIO) 

ADD_EXECUTABLE(VectorImage VectorImage.cxx )
TARGET_LINK_LIBRARIES(VectorImage OTBCommon ITKCommon OTBIO ITKIO)

ADD_EXECUTABLE(ImageAdaptor1 ImageAdaptor1.cxx )
TARGET_LINK_LIBRARIES(ImageAdaptor1 OTBCommon ITKCommon OTBIO ITKIO)

ADD_EXECUTABLE(ImageAdaptor2 ImageAdaptor2.cxx )
TARGET_LINK_LIBRARIES(ImageAdaptor2 OTBCommon ITKCommon OTBIO ITKIO)

ADD_EXECUTABLE(ImageAdaptor3 ImageAdaptor3.cxx )
TARGET_LINK_LIBRARIES(ImageAdaptor3 OTBCommon ITKCommon OTBIO ITKIO)

ADD_EXECUTABLE(ImageAdaptor4 ImageAdaptor4.cxx )
TARGET_LINK_LIBRARIES(ImageAdaptor4 OTBCommon ITKCommon OTBIO ITKIO)

ADD_EXECUTABLE(ImageListExample ImageListExample.cxx )
TARGET_LINK_LIBRARIES(ImageListExample OTBCommon ITKCommon OTBIO ITKIO)

IF( NOT OTB_DISABLE_CXX_TESTING AND NOT OTB_DISABLE_CXX_EXAMPLES_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/DataRepresentation/Image)

SET(INPUTDATA ${OTB_SOURCE_DIR}/Examples/Data)
SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbImageExamplesTests)

SET(TOL 0.0)


# ------- ImageAdaptor4Test----------

ADD_TEST(ImageAdaptor4Test1 ${EXE_TESTS}
	--compare-n-images ${TOL} 1
	${BASELINE}/ImageAdaptorThresholdingA.png
	${TEMP}/ImageAdaptorThresholdingA.png
	ImageAdaptor4Test
	${INPUTDATA}/QB_Suburb.png
	${TEMP}/ImageAdaptorThresholdingA.png
    100
)

# ------- ImageAdaptor4Test----------

ADD_TEST(ImageAdaptor4Test2 ${EXE_TESTS}
	--compare-n-images ${TOL} 1
	${BASELINE}/ImageAdaptorThresholdingB.png
	${TEMP}/ImageAdaptorThresholdingB.png
	ImageAdaptor4Test
	${INPUTDATA}/QB_Suburb.png
	${TEMP}/ImageAdaptorThresholdingB.png
    200
)

# ------- Image5Test----------

ADD_TEST(Image5Test ${EXE_TESTS}
	Image5Test
	${TEMP}/Image5.png
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbImageExamplesTests otbImageExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbImageExamplesTests gdal ITKIO ITKAlgorithms ITKStatistics ITKCommon OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND NOT OTB_DISABLE_CXX_EXAMPLES_TESTING )
