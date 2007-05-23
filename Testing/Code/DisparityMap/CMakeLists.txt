 
IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Images de teledetection (grosses images )
SET(IMAGEDATA ${OTB_DATA_ROOT}/LargeInput )
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

# Tolerance for pixel difference
SET(TOL 0.0)

#EPSILON loose tolerance for multiplatform support.
SET(EPSILON 0.0000000001)

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
              --compare-image ${EPSILON}
		${BASELINE}/dmTvNearestPointDeformationField.hdr
		${TEMP}/dmTvNearestPointDeformationField.hdr	
	 otbNearestPointDeformationFieldGenerator
	 ${TEMP}/dmTvNearestPointDeformationField.hdr
)

# -------  otb::NNearestPointsLinearInterpolateDeformationFieldGenerator   ----------

ADD_TEST(dmTuNNearestPointsLinearInterpolateDeformationFieldGeneratorNew ${DISPARITYMAP_TESTS} 
	 otbNNearestPointsLinearInterpolateDeformationFieldGeneratorNew)

ADD_TEST(dmTvNNearestPointsLinearInterpolateDeformationFieldGenerator ${DISPARITYMAP_TESTS} 
              --compare-image ${EPSILON}
		${BASELINE}/dmTvNNearestPointsLinearInterpolateDeformationField.hdr
		${TEMP}/dmTvNNearestPointsLinearInterpolateDeformationField.hdr	
	 otbNNearestPointsLinearInterpolateDeformationFieldGenerator
	 ${TEMP}/dmTvNNearestPointsLinearInterpolateDeformationField.hdr
)


# -------  otb::BSplinesInterpolateDeformationFieldGenerator   ----------

ADD_TEST(dmTuBSplinesInterpolateDeformationFieldGeneratorNew ${DISPARITYMAP_TESTS} 
	 otbNNearestPointsLinearInterpolateDeformationFieldGeneratorNew)

ADD_TEST(dmTvBSplinesInterpolateDeformationFieldGenerator ${DISPARITYMAP_TESTS} 
              --compare-image ${EPSILON}
		${BASELINE}/dmTvBSplinesInterpolateDeformationField.hdr
		${TEMP}/dmTvBSplinesInterpolateDeformationField.hdr	
	 otbBSplinesInterpolateDeformationFieldGenerator
	 ${TEMP}/dmTvBSplinesInterpolateDeformationField.hdr
)


# -------            otb::PointSetWithTransformToDeformationFieldGenerator   ----------

ADD_TEST(dmTuPointSetWithTransformToDeformationFieldGeneratorNew ${DISPARITYMAP_TESTS} 
	 otbPointSetWithTransformToDeformationFieldGeneratorNew)


# -------            otb::NearestTransformDeformationFieldGenerator   ----------

ADD_TEST(dmTuNearestTransformDeformationFieldGeneratorNew ${DISPARITYMAP_TESTS} 
	 otbNearestTransformDeformationFieldGeneratorNew)

ADD_TEST(dmTvNearestTransformDeformationFieldGenerator ${DISPARITYMAP_TESTS} 
              --compare-image ${EPSILON}
		${BASELINE}/dmTvNearestTransformDeformationField.hdr
		${TEMP}/dmTvNearestTransformDeformationField.hdr	
	 otbNearestTransformDeformationFieldGenerator
	 ${TEMP}/dmTvNearestTransformDeformationField.hdr
)


# -------  otb::NNearestTransformsLinearInterpolateDeformationFieldGenerator   ----------

ADD_TEST(dmTuNNearestTransformsLinearInterpolateDeformationFieldGeneratorNew ${DISPARITYMAP_TESTS} 
	 otbNNearestTransformsLinearInterpolateDeformationFieldGeneratorNew)

ADD_TEST(dmTvNNearestTransformsLinearInterpolateDeformationFieldGenerator ${DISPARITYMAP_TESTS} 
              --compare-image ${EPSILON}
		${BASELINE}/dmTvNNearestTransformsLinearInterpolateDeformationField.hdr
		${TEMP}/dmTvNNearestTransformsLinearInterpolateDeformationField.hdr	
	 otbNNearestTransformsLinearInterpolateDeformationFieldGenerator
	 ${TEMP}/dmTvNNearestTransformsLinearInterpolateDeformationField.hdr
)

# -------  otb::BSplinesInterpolateTransformDeformationFieldGenerator ----------

ADD_TEST(dmTuBSplinesInterpolateTransformDeformationFieldGeneratorNew ${DISPARITYMAP_TESTS} 
	 otbBSplinesInterpolateTransformDeformationFieldGeneratorNew)

ADD_TEST(dmTvBSplinesInterpolateTransformDeformationFieldGenerator ${DISPARITYMAP_TESTS} 
              --compare-image ${EPSILON}
		${BASELINE}/dmTvBSplinesInterpolateTransformDeformationFieldGenerator.hdr
		${TEMP}/dmTvBSplinesInterpolateTransformDeformationFieldGenerator.hdr	
	 otbBSplinesInterpolateTransformDeformationFieldGenerator
	 ${TEMP}/dmTvBSplinesInterpolateTransformDeformationFieldGenerator.hdr
)

# -------  Additional tests for deformation fields estimation   ----------

ADD_TEST(dmTvTranslationDeformationFieldEstimation ${DISPARITYMAP_TESTS} 
              --compare-n-images ${EPSILON} 12
	      ${TEMP}/dmTvTranslationDeformationFieldEstimationOutput_bs_df.hdr  
	      ${BASELINE}/dmTvTranslationDeformationFieldEstimationOutput_bs_df.hdr 
	      ${TEMP}/dmTvTranslationDeformationFieldEstimationOutput_bs_oi.tif
	      ${BASELINE}/dmTvTranslationDeformationFieldEstimationOutput_bs_oi.tif
	      ${TEMP}/dmTvTranslationDeformationFieldEstimationOutput_np_df.hdr
	      ${BASELINE}/dmTvTranslationDeformationFieldEstimationOutput_np_df.hdr
	      ${TEMP}/dmTvTranslationDeformationFieldEstimationOutput_np_oi.tif
	      ${BASELINE}/dmTvTranslationDeformationFieldEstimationOutput_np_oi.tif
	      ${TEMP}/dmTvTranslationDeformationFieldEstimationOutput_nnp_df.hdr  
	      ${BASELINE}/dmTvTranslationDeformationFieldEstimationOutput_nnp_df.hdr 
	      ${TEMP}/dmTvTranslationDeformationFieldEstimationOutput_nnp_oi.tif
	      ${BASELINE}/dmTvTranslationDeformationFieldEstimationOutput_nnp_oi.tif
	      ${TEMP}/dmTvTranslationDeformationFieldEstimationOutput_nt_df.hdr
	      ${BASELINE}/dmTvTranslationDeformationFieldEstimationOutput_nt_df.hdr
	      ${TEMP}/dmTvTranslationDeformationFieldEstimationOutput_nt_oi.tif
	      ${BASELINE}/dmTvTranslationDeformationFieldEstimationOutput_nt_oi.tif
	      ${TEMP}/dmTvTranslationDeformationFieldEstimationOutput_nnt_df.hdr
	      ${BASELINE}/dmTvTranslationDeformationFieldEstimationOutput_nnt_df.hdr
	      ${TEMP}/dmTvTranslationDeformationFieldEstimationOutput_nnt_oi.tif
	      ${BASELINE}/dmTvTranslationDeformationFieldEstimationOutput_nnt_oi.tif
	      ${TEMP}/dmTvTranslationDeformationFieldEstimationOutput_bst_df.hdr
	      ${BASELINE}/dmTvTranslationDeformationFieldEstimationOutput_bst_df.hdr
	      ${TEMP}/dmTvTranslationDeformationFieldEstimationOutput_bst_oi.tif
	      ${BASELINE}/dmTvTranslationDeformationFieldEstimationOutput_bst_oi.tif
	 otbTranslationDeformationFieldEstimation
	 ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub.tif
	 ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub_warped_translation.tif
	 ${TEMP}/dmTvTranslationDeformationFieldEstimationOutput
	  15 10 10 250 0.95 4 50
)

ADD_TEST(dmTvEuler2DDeformationFieldEstimation ${DISPARITYMAP_TESTS} 
              --compare-n-images ${EPSILON} 12
	      ${TEMP}/dmTvEuler2DDeformationFieldEstimationOutput_bs_df.hdr  
	      ${BASELINE}/dmTvEuler2DDeformationFieldEstimationOutput_bs_df.hdr 
	      ${TEMP}/dmTvEuler2DDeformationFieldEstimationOutput_bs_oi.tif
	      ${BASELINE}/dmTvEuler2DDeformationFieldEstimationOutput_bs_oi.tif
	      ${TEMP}/dmTvEuler2DDeformationFieldEstimationOutput_np_df.hdr
	      ${BASELINE}/dmTvEuler2DDeformationFieldEstimationOutput_np_df.hdr
	      ${TEMP}/dmTvEuler2DDeformationFieldEstimationOutput_np_oi.tif
	      ${BASELINE}/dmTvEuler2DDeformationFieldEstimationOutput_np_oi.tif
	      ${TEMP}/dmTvEuler2DDeformationFieldEstimationOutput_nnp_df.hdr  
	      ${BASELINE}/dmTvEuler2DDeformationFieldEstimationOutput_nnp_df.hdr 
	      ${TEMP}/dmTvEuler2DDeformationFieldEstimationOutput_nnp_oi.tif
	      ${BASELINE}/dmTvEuler2DDeformationFieldEstimationOutput_nnp_oi.tif
	      ${TEMP}/dmTvEuler2DDeformationFieldEstimationOutput_nt_df.hdr
	      ${BASELINE}/dmTvEuler2DDeformationFieldEstimationOutput_nt_df.hdr
	      ${TEMP}/dmTvEuler2DDeformationFieldEstimationOutput_nt_oi.tif
	      ${BASELINE}/dmTvEuler2DDeformationFieldEstimationOutput_nt_oi.tif
	      ${TEMP}/dmTvEuler2DDeformationFieldEstimationOutput_nnt_df.hdr
	      ${BASELINE}/dmTvEuler2DDeformationFieldEstimationOutput_nnt_df.hdr
	      ${TEMP}/dmTvEuler2DDeformationFieldEstimationOutput_nnt_oi.tif
	      ${BASELINE}/dmTvEuler2DDeformationFieldEstimationOutput_nnt_oi.tif
	      ${TEMP}/dmTvEuler2DDeformationFieldEstimationOutput_bst_df.hdr
	      ${BASELINE}/dmTvEuler2DDeformationFieldEstimationOutput_bst_df.hdr
	      ${TEMP}/dmTvEuler2DDeformationFieldEstimationOutput_bst_oi.tif
	      ${BASELINE}/dmTvEuler2DDeformationFieldEstimationOutput_bst_oi.tif
	 otbEuler2DDeformationFieldEstimation
	 ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub.tif
	 ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub_warped_euler2D.tif
	 ${TEMP}/dmTvEuler2DDeformationFieldEstimationOutput
	 15 10 0.01 250 0.95 4 50 128 128
)

ADD_TEST(dmTvCenteredRigidDeformationFieldEstimation ${DISPARITYMAP_TESTS} 
              --compare-n-images ${EPSILON} 12
	      ${TEMP}/dmTvCenteredRigidDeformationFieldEstimationOutput_bs_df.hdr  
	      ${BASELINE}/dmTvCenteredRigidDeformationFieldEstimationOutput_bs_df.hdr 
	      ${TEMP}/dmTvCenteredRigidDeformationFieldEstimationOutput_bs_oi.tif
	      ${BASELINE}/dmTvCenteredRigidDeformationFieldEstimationOutput_bs_oi.tif
	      ${TEMP}/dmTvCenteredRigidDeformationFieldEstimationOutput_np_df.hdr
	      ${BASELINE}/dmTvCenteredRigidDeformationFieldEstimationOutput_np_df.hdr
	      ${TEMP}/dmTvCenteredRigidDeformationFieldEstimationOutput_np_oi.tif
	      ${BASELINE}/dmTvCenteredRigidDeformationFieldEstimationOutput_np_oi.tif
	      ${TEMP}/dmTvCenteredRigidDeformationFieldEstimationOutput_nnp_df.hdr  
	      ${BASELINE}/dmTvCenteredRigidDeformationFieldEstimationOutput_nnp_df.hdr 
	      ${TEMP}/dmTvCenteredRigidDeformationFieldEstimationOutput_nnp_oi.tif
	      ${BASELINE}/dmTvCenteredRigidDeformationFieldEstimationOutput_nnp_oi.tif
	      ${TEMP}/dmTvCenteredRigidDeformationFieldEstimationOutput_nt_df.hdr
	      ${BASELINE}/dmTvCenteredRigidDeformationFieldEstimationOutput_nt_df.hdr
	      ${TEMP}/dmTvCenteredRigidDeformationFieldEstimationOutput_nt_oi.tif
	      ${BASELINE}/dmTvCenteredRigidDeformationFieldEstimationOutput_nt_oi.tif
	      ${TEMP}/dmTvCenteredRigidDeformationFieldEstimationOutput_nnt_df.hdr
	      ${BASELINE}/dmTvCenteredRigidDeformationFieldEstimationOutput_nnt_df.hdr
	      ${TEMP}/dmTvCenteredRigidDeformationFieldEstimationOutput_nnt_oi.tif
	      ${BASELINE}/dmTvCenteredRigidDeformationFieldEstimationOutput_nnt_oi.tif
	      ${TEMP}/dmTvCenteredRigidDeformationFieldEstimationOutput_bst_df.hdr
	      ${BASELINE}/dmTvCenteredRigidDeformationFieldEstimationOutput_bst_df.hdr
	      ${TEMP}/dmTvCenteredRigidDeformationFieldEstimationOutput_bst_oi.tif
	      ${BASELINE}/dmTvCenteredRigidDeformationFieldEstimationOutput_bst_oi.tif
	 otbCenteredRigidDeformationFieldEstimation
	 ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub.tif
	 ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub_warped_centered_rigid.tif
	 ${TEMP}/dmTvCenteredRigidDeformationFieldEstimationOutput
	 15 5 0.01 250 0.95 4 50 127 65 
)

ADD_TEST(dmTvSinusoidDeformationFieldEstimation ${DISPARITYMAP_TESTS} 
              --compare-n-images ${EPSILON} 12	      
	      ${TEMP}/dmTvSinusoidDeformationFieldEstimationOutput_bs_df.hdr  
	      ${BASELINE}/dmTvSinusoidDeformationFieldEstimationOutput_bs_df.hdr 
	      ${TEMP}/dmTvSinusoidDeformationFieldEstimationOutput_bs_oi.tif
	      ${BASELINE}/dmTvSinusoidDeformationFieldEstimationOutput_bs_oi.tif
	      ${TEMP}/dmTvSinusoidDeformationFieldEstimationOutput_np_df.hdr
	      ${BASELINE}/dmTvSinusoidDeformationFieldEstimationOutput_np_df.hdr
	      ${TEMP}/dmTvSinusoidDeformationFieldEstimationOutput_np_oi.tif
	      ${BASELINE}/dmTvSinusoidDeformationFieldEstimationOutput_np_oi.tif
	      ${TEMP}/dmTvSinusoidDeformationFieldEstimationOutput_nnp_df.hdr  
	      ${BASELINE}/dmTvSinusoidDeformationFieldEstimationOutput_nnp_df.hdr 
	      ${TEMP}/dmTvSinusoidDeformationFieldEstimationOutput_nnp_oi.tif
	      ${BASELINE}/dmTvSinusoidDeformationFieldEstimationOutput_nnp_oi.tif
	      ${TEMP}/dmTvSinusoidDeformationFieldEstimationOutput_nt_df.hdr
	      ${BASELINE}/dmTvSinusoidDeformationFieldEstimationOutput_nt_df.hdr
	      ${TEMP}/dmTvSinusoidDeformationFieldEstimationOutput_nt_oi.tif
	      ${BASELINE}/dmTvSinusoidDeformationFieldEstimationOutput_nt_oi.tif
	      ${TEMP}/dmTvSinusoidDeformationFieldEstimationOutput_nnt_df.hdr
	      ${BASELINE}/dmTvSinusoidDeformationFieldEstimationOutput_nnt_df.hdr
	      ${TEMP}/dmTvSinusoidDeformationFieldEstimationOutput_nnt_oi.tif
	      ${BASELINE}/dmTvSinusoidDeformationFieldEstimationOutput_nnt_oi.tif
	      ${TEMP}/dmTvSinusoidDeformationFieldEstimationOutput_bst_df.hdr
	      ${BASELINE}/dmTvSinusoidDeformationFieldEstimationOutput_bst_df.hdr
	      ${TEMP}/dmTvSinusoidDeformationFieldEstimationOutput_bst_oi.tif
	      ${BASELINE}/dmTvSinusoidDeformationFieldEstimationOutput_bst_oi.tif
	 otbTranslationDeformationFieldEstimation
	 ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub.tif
	 ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub_warped_sinus.tif
	 ${TEMP}/dmTvSinusoidDeformationFieldEstimationOutput
	  15 10 10 250 0.95 4 25
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
otbPointSetWithTransformToDeformationFieldGeneratorNew.cxx
otbNearestTransformDeformationFieldGeneratorNew.cxx
otbNearestTransformDeformationFieldGenerator.cxx
otbNNearestTransformsLinearInterpolateDeformationFieldGeneratorNew.cxx
otbNNearestTransformsLinearInterpolateDeformationFieldGenerator.cxx
otbTranslationDeformationFieldEstimation.cxx
otbEuler2DDeformationFieldEstimation.cxx
otbCenteredRigidDeformationFieldEstimation.cxx
otbBSplinesInterpolateTransformDeformationFieldGeneratorNew.cxx
otbBSplinesInterpolateTransformDeformationFieldGenerator.cxx
)

INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbDisparityMapTests otbDisparityMapTests.cxx ${BasicDisparityMap_SRCS})
TARGET_LINK_LIBRARIES(otbDisparityMapTests OTBIO OTBDisparityMap gdal ITKIO ITKAlgorithms ITKStatistics ITKCommon ITKNumerics itkvnl)

ENDIF( NOT OTB_DISABLE_CXX_TESTING )
