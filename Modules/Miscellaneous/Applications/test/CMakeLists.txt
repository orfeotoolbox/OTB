#
# Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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
#----------- LocalStatisticExtraction TESTS ----------------
otb_test_application(NAME  apTvFELocalStatisticExtraction
                     APP  LocalStatisticExtraction
                     OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                             -channel 1
                             -radius 3
                             -out ${TEMP}/apTvFELocalStatisticExtraction.tif
                     VALID   --compare-image ${NOTOL}
                   			 ${BASELINE}/feTvRadiometricMomentsImageFilter.tif
                 		     ${TEMP}/apTvFELocalStatisticExtraction.tif)
#------------ DomainTransform TESTS -----------------------
otb_test_application(NAME apTvDomainTransform_wav_db20_fwd
APP  DomainTransform
OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
-mode wavelet
-mode.wavelet.form db20
-mode.wavelet.nlevels 2
-direction forward
-out ${TEMP}/apTvDomainTransform_wav_db20_fwd.tif
)

otb_test_application(NAME apTvDomainTransform_wav_haar_inv
APP  DomainTransform
OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
-mode wavelet
-mode.wavelet.form haar
-direction inverse
-out ${TEMP}/apTvDomainTransform_wav_haar_inv.tif
)

# otb_test_application(NAME apTvDomainTransform_fft_shift_fwd
# APP  DomainTransform
# OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
# -mode fft
# -mode.fft.shift true
# -direction forward
# -out ${TEMP}/apTvDomainTransform_fft_shift_fwd.tif
# )


# otb_test_application(NAME apTvDomainTransform_fft_inv
# APP  DomainTransform
# OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
# -mode fft
# -direction inverse
# -out ${TEMP}/apTvDomainTransform_fft_shift_inv.tif
# )

#----------------------Dimensionnality reduction TESTS-------------------
set(algos som)

set(som_params
-algorithm som
-algorithm.som.s 10 10
-algorithm.som.n 3 3
-algorithm.som.ni 10)

if(OTB_USE_SHARK)
  list(APPEND classifierList ae pca)

  set(ae_params
  -algorithm autoencoder
  -algorithm.autoencoder.nbneuron 8
  -algorithm.autoencoder.regularization 0.01
  -algorithm.autoencoder.noise 0
  -algorithm.autoencoder.rho 0
  -algorithm.autoencoder.beta 0)

  set(pca_params
  -algorithm pca
  -algorithm.pca.dim 8)
endif()

foreach(algo ${algos})
  string(TOUPPER ${algo} ualgo)
  #------------------ TrainDimensionalityReduction TESTS------------------------
  otb_test_application(NAME apTvDrTrainDimensionalityReduction${ualgo}
    APP TrainDimensionalityReduction
    OPTIONS -io.vd ${INPUTDATA}/cuprite_samples.sqlite
            -io.out ${TEMP}/cuprite_DRModel.${algo}
            -io.stats ${INPUTDATA}/cupriteStats.xml
            -feat value_0 value_1 value_2 value_3 value_4 value_5 value_6 value_7 value_8 value_9
            ${${algo}_params})

  #------------------ ImageDimensionalityReduction TESTS------------------------
  otb_test_application(NAME apTvDrImageDimensionalityReduction${ualgo}
    APP ImageDimensionalityReduction
    OPTIONS -in ${INPUTDATA}/cupriteSubHsi.tif
            -model ${TEMP}/cuprite_DRModel.${algo}
            -imstat ${INPUTDATA}/cupriteStats.xml
            -out ${TEMP}/cupriteReduced_${algo}.tif)

  set_tests_properties( apTvDrImageDimensionalityReduction${ualgo}
    PROPERTIES DEPENDS apTvDrTrainDimensionalityReduction${ualgo})

  #------------------ VectorDimensionalityReduction TESTS-----------------------
  otb_test_application(NAME apTvDrVectorDimensionalityReduction${ualgo}
    APP VectorDimensionalityReduction
    OPTIONS -in ${INPUTDATA}/cuprite_samples.sqlite
            -model ${TEMP}/cuprite_DRModel.${algo}
            -instat ${INPUTDATA}/cupriteStats.xml
            -out ${TEMP}/cupriteReduced_${algo}.sqlite
            -feat value_0 value_1 value_2 value_3 value_4 value_5 value_6 value_7 value_8 value_9)

  set_tests_properties( apTvDrVectorDimensionalityReduction${ualgo}
    PROPERTIES DEPENDS apTvDrTrainDimensionalityReduction${ualgo})
endforeach()

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
