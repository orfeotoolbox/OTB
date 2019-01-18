#
# Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

otb_module_test()
#----------- BinaryMorphologicalOperation TESTS ----------------
otb_test_application(NAME  apTvFEBinaryMorphologicalOperation
                     APP  BinaryMorphologicalOperation
                     OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN_Mask.tif
                             -channel 1
                             -structype ball
                             -xradius 10
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
                             -xradius 10
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
							 -outleveling ${TEMP}/apTvFEMorphologicalMultiScaleDecomposition_levelingIteration_1.tif
							 -outconcave ${TEMP}/apTvFEMorphologicalMultiScaleDecomposition_concaveIteration_1.tif
							 -outconvex ${TEMP}/apTvFEMorphologicalMultiScaleDecomposition_convexIteration_1.tif
					 VALID   --compare-n-images ${NOTOL} 3
							 ${BASELINE}/apTvFEMorphologicalMultiScaleDecomposition_levelingIteration_1.tif
							 ${TEMP}/apTvFEMorphologicalMultiScaleDecomposition_levelingIteration_1.tif
							 ${BASELINE}/apTvFEMorphologicalMultiScaleDecomposition_concaveIteration_1.tif
							 ${TEMP}/apTvFEMorphologicalMultiScaleDecomposition_concaveIteration_1.tif
							 ${BASELINE}/apTvFEMorphologicalMultiScaleDecomposition_convexIteration_1.tif
							 ${TEMP}/apTvFEMorphologicalMultiScaleDecomposition_convexIteration_1.tif)


#----------- Iterative MorphologicalMultiScaleDecomposition TESTS ----------------
otb_test_application(NAME  apTvFEMorphologicalMultiScaleDecompositionIteratif
					 APP  MorphologicalMultiScaleDecomposition
					 OPTIONS -in ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
							 -channel 1
							 -levels 2
		                     -structype ball
							 -step 3
							 -radius 2
							 -outleveling ${TEMP}/apTvFEMorphologicalMultiScaleDecomposition_levelingIteration_2.tif
							 -outconcave ${TEMP}/apTvFEMorphologicalMultiScaleDecomposition_concaveIteration_2.tif
							 -outconvex ${TEMP}/apTvFEMorphologicalMultiScaleDecomposition_convexIteration_2.tif
					 VALID   --compare-n-images ${NOTOL} 3
							 ${BASELINE}/apTvFEMorphologicalMultiScaleDecomposition_levelingIteration_2.tif
							 ${TEMP}/apTvFEMorphologicalMultiScaleDecomposition_levelingIteration_2.tif
							 ${BASELINE}/apTvFEMorphologicalMultiScaleDecomposition_concaveIteration_2.tif
							 ${TEMP}/apTvFEMorphologicalMultiScaleDecomposition_concaveIteration_2.tif
							 ${BASELINE}/apTvFEMorphologicalMultiScaleDecomposition_convexIteration_2.tif
							 ${TEMP}/apTvFEMorphologicalMultiScaleDecomposition_convexIteration_2.tif)


#----------- MorphologicalClassification TESTS ----------------
otb_test_application(NAME  apTvFEMorphologicalClassification
		             APP  MorphologicalClassification
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
						     -out ${TEMP}/apTvFEMorphologicalProfilesAnalysisClassif.tif
					 VALID   --compare-image ${NOTOL}
							 ${BASELINE}/msMultiScaleConvexOrConcaveClassificationFilterOutput.tif
							 ${TEMP}/apTvFEMorphologicalProfilesAnalysisClassif.tif)
