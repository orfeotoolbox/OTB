PROJECT(BasicFilters)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(LeeImageFilter LeeImageFilter.cxx )
#TARGET_LINK_LIBRARIES(LeeImageFilter OTBCommon OTBIO ITKCommon ITKIO ITKNumerics ITKBasicFilters ITKCommon ITKStatistics itkvnl_inst itkvnl_algo itkvnl itkvcl itknetlib itksys  ITKNrrdIO itkpng itktiff itkjpeg8 itkjpeg12 itkjpeg16 ITKMetaIO itkzlib ITKDICOMParser ITKEXPAT )
TARGET_LINK_LIBRARIES(LeeImageFilter OTBCommon OTBIO ITKCommon ITKIO ITKNumerics ITKBasicFilters ITKCommon ITKStatistics itkvnl_inst itkvnl_algo itkvnl itkvcl itksys  ITKNrrdIO itkpng itktiff itkjpeg8 itkjpeg12 itkjpeg16 ITKMetaIO itkzlib ITKDICOMParser ITKEXPAT )



IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/BasicFilters)

SET(INPUTDATA ${OTB_SOURCE_DIR}/Examples/Data)
SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbBasicFiltersExamplesTests)

SET(TOL 0.0)


# ------- LeeImageFilterTest----------

ADD_TEST(LeeImageFilterTest ${EXE_TESTS}
	--compare-n-images ${TOL} 1
	${BASELINE}/GomaSmallLeeFiltered.png
	${TEMP}/GomaSmallLeeFiltered.png
	LeeImageFilterTest
	${INPUTDATA}/GomaSmall.png
	${TEMP}/GomaSmallLeeFiltered.png
    3 1
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbBasicFiltersExamplesTests otbBasicFiltersExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbBasicFiltersExamplesTests gdal ITKIO ITKAlgorithms ITKStatistics ITKCommon OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale)

ENDIF( NOT OTB_DISABLE_CXX_TESTING )
