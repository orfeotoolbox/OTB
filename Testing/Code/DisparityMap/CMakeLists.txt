 
IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Images de teledetection (grosses images )
SET(IMAGEDATA ${OTB_DATA_ROOT}/LargeInput )
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

#Tolerance sur diff pixel image
SET(TOL 0.0)


SET(DISPARITYMAP_TESTS ${CXX_TEST_PATH}/otbDisparityMapTests)


# -------            otb::DisparityMapEstimationMethod   ----------

ADD_TEST(dmTuDisparityMapEstimationMethodNew ${DISPARITYMAP_TESTS} 
	 otbDisparityMapEstimationMethodNew)

ADD_TEST(dmTvDisparityMapEstimationMethod ${DISPARITYMAP_TESTS} 
   --compare-ascii ${TOL}
 		   ${BASELINE_FILES}/dmDisparityMapEstimationOutput1.txt
 		   ${TEMP}/dmDisparityMapEstimationOutput1.txt
	 otbDisparityMapEstimationMethod
	           ${INPUTDATA}/fixed.png
	           ${INPUTDATA}/moving.png
	           ${INPUTDATA}/pointSet.png
	           ${TEMP}/dmDisparityMapEstimationOutput1.txt
		   20 20
)

# -------            otb::PointSetToDeformationFieldGenerator   ----------

ADD_TEST(dmTuPointSetToDeformationFieldGeneratorNew ${DISPARITYMAP_TESTS} 
	 otbPointSetToDeformationFieldGeneratorNew)

# -------            otb::NearestPointDeformationFieldGenerator   ----------

ADD_TEST(dmTuNearestPointDeformationFieldGeneratorNew ${DISPARITYMAP_TESTS} 
	 otbNearestPointDeformationFieldGeneratorNew)

ADD_TEST(dmTvNearestPointDeformationFieldGenerator ${DISPARITYMAP_TESTS} 
              --compare-image ${TOL}
		${BASELINE}/dmTvNearestPointDeformationField.hdr
		${TEMP}/dmTvNearestPointDeformationField.hdr	
	 otbNearestPointDeformationFieldGenerator
	 ${TEMP}/dmTvNearestPointDeformationField.hdr
)

# -------  otb::NNearestPointsLinearInterpolateDeformationFieldGenerator   ----------

ADD_TEST(dmTuNNearestPointsLinearInterpolateDeformationFieldGeneratorNew ${DISPARITYMAP_TESTS} 
	 otbNNearestPointsLinearInterpolateDeformationFieldGeneratorNew)

ADD_TEST(dmTvNNearestPointsLinearInterpolateDeformationFieldGenerator ${DISPARITYMAP_TESTS} 
              --compare-image ${TOL}
		${BASELINE}/dmTvNNearestPointsLinearInterpolateDeformationField.hdr
		${TEMP}/dmTvNNearestPointsLinearInterpolateDeformationField.hdr	
	 otbNNearestPointsLinearInterpolateDeformationFieldGenerator
	 ${TEMP}/dmTvNNearestPointsLinearInterpolateDeformationField.hdr
)


# -------  otb::BSplinesInterpolateDeformationFieldGenerator   ----------

ADD_TEST(dmTuBSplinesInterpolateDeformationFieldGeneratorNew ${DISPARITYMAP_TESTS} 
	 otbNNearestPointsLinearInterpolateDeformationFieldGeneratorNew)

ADD_TEST(dmTvBSplinesInterpolateDeformationFieldGenerator ${DISPARITYMAP_TESTS} 
              --compare-image ${TOL}
		${BASELINE}/dmTvBSplinesInterpolateDeformationField.hdr
		${TEMP}/dmTvBSplinesInterpolateDeformationField.hdr	
	 otbBSplinesInterpolateDeformationFieldGenerator
	 ${TEMP}/dmTvBSplinesInterpolateDeformationField.hdr
)

# -------       Fichiers sources CXX -----------------------------------
SET(BasicDisparityMap_SRCS
otbDisparityMapEstimationMethodNew.cxx
otbDisparityMapEstimationMethod.cxx
otbPointSetToDeformationFieldGeneratorNew.cxx
otbNearestPointDeformationFieldGeneratorNew.cxx
otbNearestPointDeformationFieldGenerator.cxx
otbNNearestPointsLinearInterpolateDeformationFieldGeneratorNew.cxx
otbNNearestPointsLinearInterpolateDeformationFieldGenerator.cxx
otbBSplinesInterpolateDeformationFieldGeneratorNew.cxx
otbBSplinesInterpolateDeformationFieldGenerator.cxx

)

INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbDisparityMapTests otbDisparityMapTests.cxx ${BasicDisparityMap_SRCS})
TARGET_LINK_LIBRARIES(otbDisparityMapTests OTBIO OTBDisparityMap gdal ITKIO ITKAlgorithms ITKStatistics ITKCommon)

ENDIF( NOT OTB_DISABLE_CXX_TESTING )
