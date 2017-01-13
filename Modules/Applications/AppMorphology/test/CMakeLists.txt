otb_module_test()
#----------- BinaryMorphologicalOperation TESTS ----------------
otb_test_application(NAME  apTvFEBinaryMorphologicalOperation
                     APP  BinaryMorphologicalOperation
                     OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN_Mask.tif
                             -channel 1
                             -structype ball
                             -structype.ball.xradius 10
                             -filter opening
                             -filter.opening.foreval 1.0
                             -filter.opening.backval 0.0
                             -out ${TEMP}/apTvFEBinaryMorphologicalOperation.tif
                     VALID   --compare-image ${NOTOL}
                   			 ${BASELINE}/apTvFEBinaryMorphologicalOperation.tif
                 		     ${TEMP}/apTvFEBinaryMorphologicalOperation.tif)


#----------- GrayScaleMorphologicalOperation TESTS ----------------
otb_test_application(NAME  apTvFEGrayScaleMorphologicalOperation
                     APP  GrayScaleMorphologicalOperation
                     OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                             -channel 1
                             -structype ball
                             -structype.ball.xradius 10
                             -filter opening
                             -out ${TEMP}/apTvFEGrayScaleMorphologicalOperation.tif
                     VALID   --compare-image ${NOTOL}
                   			 ${BASELINE}/apTvFEGrayScaleMorphologicalOperation.tif
                 		     ${TEMP}/apTvFEGrayScaleMorphologicalOperation.tif)


#----------- MorphologicalMultiScaleDecomposition TESTS ----------------
otb_test_application(NAME  apTvFEMorphologicalMultiScaleDecomposition
					 APP  MorphologicalMultiScaleDecomposition
					 OPTIONS -in ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
							 -channel 1
							 -radius 4
		 					 -structype ball
							 -out ${TEMP}/apTvFEMorphologicalMultiScaleDecomposition_Iteration_1.tif
					 VALID   --compare-image ${NOTOL}
							 ${BASELINE}/apTvFEMorphologicalMultiScaleDecomposition_Iteration_1.tif
							 ${TEMP}/apTvFEMorphologicalMultiScaleDecomposition_Iteration_1.tif)


#----------- Iterative MorphologicalMultiScaleDecomposition TESTS ----------------
otb_test_application(NAME  apTvFEMorphologicalMultiScaleDecompositionIteratif
					 APP  MorphologicalMultiScaleDecomposition
					 OPTIONS -in ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
							 -channel 1
							 -levels 2
		                     -structype ball
							 -step 3
							 -radius 2
							 -out ${TEMP}/apTvFEMorphologicalMultiScaleDecomposition_Iteration_2.tif
					 VALID   --compare-image ${NOTOL}
							 ${BASELINE}/apTvFEMorphologicalMultiScaleDecomposition_Iteration_2.tif
							 ${TEMP}/apTvFEMorphologicalMultiScaleDecomposition_Iteration_2.tif)


#----------- MorphologicalProfilesClassification TESTS ----------------
otb_test_application(NAME  apTvFEMorphologicalProfilesClassification
		             APP  MorphologicalProfilesClassification
		             OPTIONS -in ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
			                 -channel 1
		                     -structype ball
		                     -radius 5
							 -sigma 0.5
		                     -out ${TEMP}/apTvFEMorphologicalProfilesClassification.tif
		             VALID   --compare-image ${NOTOL}
							 ${BASELINE}/apTvFEMorphologicalProfilesClassification.tif
					         ${TEMP}/apTvFEMorphologicalProfilesClassification.tif)



#----------- Opening MorphologicalProfilesAnalysis TESTS ----------------
otb_test_application(NAME  apTvFEMorphologicalProfilesAnalysisOpening
					 APP  MorphologicalProfilesAnalysis
					 OPTIONS -in ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
		                     -channel 1
		                     -structype ball
		                     -profile closing
		                     -size 4
		                     -radius 1
		                     -step 1
							 -out ${TEMP}/apTvFEMorphologicalProfilesOpeningAnalysis.tif)

#----------- Closing MorphologicalProfilesAnalysis TESTS ----------------
otb_test_application(NAME  apTvFEMorphologicalProfilesAnalysisClosing
					 APP  MorphologicalProfilesAnalysis
					 OPTIONS -in ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
					         -channel 1
					         -structype ball
					         -profile closing
					         -size 4
					         -radius 1
					         -step 1
					         -out ${TEMP}/apTvFEMorphologicalProfilesClosingAnalysis.tif
					 VALID   --compare-image ${NOTOL}
					         ${BASELINE}/apTvFEMorphologicalProfilesClosingAnalysis.tif
					         ${TEMP}/apTvFEMorphologicalProfilesClosingAnalysis.tif)

#----------- Classfication MorphologicalProfilesAnalysis TESTS ----------------
otb_test_application(NAME  apTvFEMorphologicalProfilesAnalysisClassification
					 APP  MorphologicalProfilesAnalysis
					 OPTIONS -in ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
						     -channel 1
						     -structype ball
						     -profile classification
						     -size 5
						     -radius 1
						     -step 1
							 -profile.classification.sigma 1
						     -out ${TEMP}/apTvFEMorphologicalProfilesClosingAnalysis.tif
					 VALID   --compare-image ${NOTOL}
							 ${BASELINE}/msMultiScaleConvexOrConcaveClassificationFilterOutput.tif
					 		 ${TEMP}/apTvFEMorphologicalProfilesClosingAnalysis.tif)
