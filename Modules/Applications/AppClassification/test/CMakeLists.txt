#
# Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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
#----------- ComputeOGRLayersFeaturesStatistics TESTS ----------------
otb_test_application(NAME apTvClComputeOGRLayersFeaturesStatistics
  APP  ComputeOGRLayersFeaturesStatistics
  OPTIONS -inshp ${INPUTDATA}/Classification/apTvClLabeledVector.shp
  -feat meanB0 meanB1 meanB2 meanB3 varB0 varB1 varB2 varB3
  -outstats ${TEMP}/apTvClComputeOGRLayersFeaturesStatistics.xml
  VALID   --compare-ascii ${NOTOL}
  ${OTBAPP_BASELINE_FILES}/apTvClComputeOGRLayersFeaturesStatistics.xml
  ${TEMP}/apTvClComputeOGRLayersFeaturesStatistics.xml)

#----------- SOMClassification TESTS ----------------
otb_test_application(NAME apTvClSOMClassificationSmall
  APP  SOMClassification
  OPTIONS -in ${INPUTDATA}/poupees_sub.png
  -rand 121212
  -out ${TEMP}/apTvClSOMClassificationSmall.tif uint16
  VALID   --compare-image ${NOTOL}
  ${BASELINE}/apTvClSOMClassificationSmall.tif
  ${TEMP}/apTvClSOMClassificationSmall.tif)

otb_test_application(NAME apTvClSOMClassificationFull
  APP  SOMClassification
  OPTIONS -in  ${INPUTDATA}/poupees_sub.png
  -out ${TEMP}/apTvClSOMClassificationFull.tif uint16
  -vm  ${INPUTDATA}/poupees_sub_c1.png
  -tp  0.8
  -ts  13000
  -som ${TEMP}/apTvClSOMClassificationMap.tif
  -sx  30
  -sy  30
  -nx  9
  -ny  9
  -ni  5
  -bi  1.0
  -bf  0.1
  -iv  0
  -rand 121212
  VALID   --compare-n-images ${NOTOL} 2
  ${BASELINE}/apTvClSOMClassificationFull.tif
  ${TEMP}/apTvClSOMClassificationFull.tif
  ${BASELINE}/apTvClSOMClassificationMap.tif
  ${TEMP}/apTvClSOMClassificationMap.tif)


#----------- ImageClassifier TESTS ----------------

# Input file types
set(raster_input_format ".tif")
set(vector_input_format ".sqlite")
set(stat_input_format ".xml")

# Input file depending on classifier
set(libsvm_input "")
set(svm_input "")
set(boost_input "Water_Vegetation")
set(dt_input "")
set(ann_input "")
set(bayes_input "")
set(rf_input "")
set(knn_input "")

# Output type
set(raster_output_format ".tif")
set(raster_output_option "uint8")

# Output model type depending on classifier
set(libsvm_output_format ".libsvm")
set(svm_output_format ".svm")
set(boost_output_format ".boost")
set(dt_output_format ".dt")
set(ann_output_format ".ann")
set(bayes_output_format ".bayes")
set(rf_output_format ".rf")
set(knn_output_format ".knn")
set(sharkrf_output_format ".txt")
set(sharkkm_output_format ".txt")

# Training algorithms parameters
set(libsvm_parameters "-classifier.libsvm.opt" "true" "-classifier.libsvm.prob" "true")
#set(svm_parameters "-classifier.svm.opt" "true")
set(boost_parameters "")
set(dt_parameters "")
set(ann_parameters "-classifier.ann.sizes" "100")
set(bayes_parameters "")
set(rf_parameters "")
set(knn_parameters "")
set(sharkrf_parameters "")
set(sharkkm_parameters "")

# Validation depending on mode
set(ascii_comparison --compare-ascii ${EPSILON_6})
set(ascii_confmat_comparison --compare-ascii 2)
set(raster_comparison --compare-image ${NOTOL})
set(raster_comparison_two --compare-n-images ${NOTOL} 2)
set(raster_comparison_three --compare-n-images ${NOTOL} 3)

# Reference ffiles depending on modes
set(ascii_ref_path ${OTBAPP_BASELINE_FILES})
set(raster_ref_path ${OTBAPP_BASELINE})

set(classifierList)
#if(OTB_USE_LIBSVM)
#list(APPEND classifierList "LIBSVM")
#endif()
if(OTB_USE_OPENCV)
  #list(APPEND classifierList "SVM" "BOOST" "DT" "ANN" "BAYES" "RF" "KNN")
  list(APPEND classifierList "BOOST" "DT" "ANN" "BAYES" "RF" "KNN")
endif()
if(OTB_USE_SHARK)
  list(APPEND classifierList "SHARKRF" "SHARKKM")
endif()

set(classifier_with_confmap "LIBSVM" "BOOST" "KNN" "ANN" "RF")
set(classifier_with_probamap "SHARKRF")

# This is a black list for classifier that can not have a baseline
# because they are using randomness and seed can not be set
set(classifier_without_baseline "")

# Loop on classifiers
foreach(classifier ${classifierList})
  string(TOLOWER ${classifier} lclassifier)

  # Derive output file name
  set(OUTMODELFILE cl${classifier}_ModelQB1${${lclassifier}_output_format})

  # Derive confusion matrix file name
  set(OUTCONFMATFILE cl${classifier}_ConfMatQB1${${lclassifier}_output_format})


  set(OUTRASTER cl${classifier}LabeledImageQB1${raster_output_format})
  set(OUTCONFMAP cl${classifier}ConfidenceMapQB1${raster_output_format})
  set(OUTPROBAMAP cl${classifier}ProbabilityMapQB1${raster_output_format})

  list(FIND classifier_without_baseline ${classifier} _classifier_has_baseline)
  if(${_classifier_has_baseline} EQUAL -1)
    set(valid ${ascii_confmat_comparison} ${ascii_ref_path}/${OUTCONFMATFILE} ${TEMP}/${OUTCONFMATFILE})
  else()
    set(valid "")
  endif()

  otb_test_application(
    NAME     apTvClTrainMethod${classifier}ImagesClassifierQB1
    APP      TrainImagesClassifier
    OPTIONS  -io.il ${INPUTDATA}/Classification/QB_1_ortho${raster_input_format}
    -io.vd ${INPUTDATA}/Classification/VectorData_${${lclassifier}_input}QB1_utm31n${vector_input_format}
    -io.imstat ${INPUTDATA}/Classification/clImageStatisticsQB1${stat_input_format}
    -classifier ${lclassifier}
    ${${lclassifier}_parameters}
    -io.out ${TEMP}/${OUTMODELFILE}
    -sample.vfn class
    -rand 121212
    -outxml ${TEMP}/cl${classifier}_OutXML1.xml
    -io.confmatout ${TEMP}/${OUTCONFMATFILE}
    VALID    ${valid}
  )

  set_tests_properties(apTvClTrainMethod${classifier}ImagesClassifierQB1 PROPERTIES
    RESOURCE_LOCK ${INPUTDATA}/Classification/VectorData_${${lclassifier}_input}QB1_utm31n${vector_input_format})

  if(${_classifier_has_baseline} EQUAL -1)
    set(valid ${ascii_confmat_comparison} ${ascii_ref_path}/${OUTCONFMATFILE} ${TEMP}/${OUTCONFMATFILE})
  else()
    set(valid "")
  endif()

  otb_test_application(
    NAME     apTvClTrainMethod${classifier}ImagesClassifierQB1_InXML1
    APP      TrainImagesClassifier
    OPTIONS  -inxml ${TEMP}/cl${classifier}_OutXML1.xml
    -io.out ${TEMP}/OutXML1_${OUTMODELFILE}
    -io.confmatout ${TEMP}/${OUTCONFMATFILE}
    VALID    ${valid}
  )

  set_tests_properties(apTvClTrainMethod${classifier}ImagesClassifierQB1_InXML1 PROPERTIES DEPENDS apTvClTrainMethod${classifier}ImagesClassifierQB1)

  list(FIND classifier_with_confmap ${classifier} _classifier_has_confmap)
  list(FIND classifier_with_probamap ${classifier} _classifier_has_probamap)
  if(${_classifier_has_confmap} EQUAL -1)
    otb_test_application(
      NAME     apTvClMethod${classifier}ImageClassifierQB1
      APP      ImageClassifier
      OPTIONS  -in ${INPUTDATA}/Classification/QB_1_ortho${raster_input_format}
      -model ${INPUTDATA}/Classification/${OUTMODELFILE}
      -imstat ${INPUTDATA}/Classification/clImageStatisticsQB1${stat_input_format}
      -out ${TEMP}/${OUTRASTER} ${raster_output_option}

      VALID    ${raster_comparison}
      ${raster_ref_path}/${OUTRASTER}
      ${TEMP}/${OUTRASTER}
    )
  else()
    if(${_classifier_has_probamap} EQUAL -1)
      otb_test_application(
        NAME     apTvClMethod${classifier}ImageClassifierQB1
        APP      ImageClassifier
        OPTIONS  -in ${INPUTDATA}/Classification/QB_1_ortho${raster_input_format}
        -model ${INPUTDATA}/Classification/${OUTMODELFILE}
        -imstat ${INPUTDATA}/Classification/clImageStatisticsQB1${stat_input_format}
        -out ${TEMP}/${OUTRASTER} ${raster_output_option}
        -confmap ${TEMP}/${OUTCONFMAP}

        VALID    ${raster_comparison_two}
        ${raster_ref_path}/${OUTRASTER}
        ${TEMP}/${OUTRASTER}
        ${raster_ref_path}/${OUTCONFMAP}
        ${TEMP}/${OUTCONFMAP}
        )
    else()
      message(${classifier})
      otb_test_application(
        NAME     apTvClMethod${classifier}ImageClassifierQB1
        APP      ImageClassifier
        OPTIONS  -in ${INPUTDATA}/Classification/QB_1_ortho${raster_input_format}
        -model ${INPUTDATA}/Classification/${OUTMODELFILE}
        -imstat ${INPUTDATA}/Classification/clImageStatisticsQB1${stat_input_format}
        -out ${TEMP}/${OUTRASTER} ${raster_output_option}
        -confmap ${TEMP}/${OUTCONFMAP}
        -nbclasses 4
        -probamap ${TEMP}/${OUTPROBAMAP}

        VALID    ${raster_comparison_three}
        ${raster_ref_path}/${OUTRASTER}
        ${TEMP}/${OUTRASTER}
        ${raster_ref_path}/${OUTCONFMAP}
        ${TEMP}/${OUTCONFMAP}
        ${raster_ref_path}/${OUTPROBAMAP}
        ${TEMP}/${OUTPROBAMAP}
        )
    endif()
  endif()

endforeach()

#----------- LIBSVM Classifier TESTS ----------------

if(OTB_USE_LIBSVM)
  otb_test_application(NAME apTvClImageSVMClassifierQB2
    APP  ImageClassifier
    OPTIONS -in      ${INPUTDATA}/Classification/QB_2_ortho.tif
    -imstat  ${INPUTDATA}/Classification/clImageStatisticsQB1.xml
    -model   ${INPUTDATA}/Classification/clsvmModelQB1.svm
    -out     ${TEMP}/clLabeledImageQB2.tif
    VALID   --compare-image ${NOTOL}
    ${OTBAPP_BASELINE}/clLabeledImageQB2.tif
    ${TEMP}/clLabeledImageQB2.tif)

  otb_test_application(NAME apTvClImageSVMClassifierQB3
    APP  ImageClassifier
    OPTIONS -in      ${INPUTDATA}/Classification/QB_3_ortho.tif
    -imstat  ${INPUTDATA}/Classification/clImageStatisticsQB1.xml
    -model   ${INPUTDATA}/Classification/clsvmModelQB1.svm
    -out     ${TEMP}/clLabeledImageQB3.tif
    VALID   --compare-image ${NOTOL}
    ${OTBAPP_BASELINE}/clLabeledImageQB3.tif
    ${TEMP}/clLabeledImageQB3.tif)

  otb_test_application(NAME apTvClImageSVMClassifierQB1
    APP  ImageClassifier
    OPTIONS -in      ${INPUTDATA}/Classification/QB_1_ortho.tif
    -imstat  ${INPUTDATA}/Classification/clImageStatisticsQB1.xml
    -model   ${INPUTDATA}/Classification/clsvmModelQB1.svm
    -out     ${TEMP}/clLabeledImageQB1.tif
    VALID   --compare-image ${NOTOL}
    ${OTBAPP_BASELINE}/clLabeledImageQB1.tif
    ${TEMP}/clLabeledImageQB1.tif)

  otb_test_application(NAME apTvClImageSVMClassifierQB456_6
    APP  ImageClassifier
    OPTIONS -in      ${INPUTDATA}/Classification/QB_6_extract.tif
    -mask    ${INPUTDATA}/Classification/QB_6_mask.tif
    -imstat  ${INPUTDATA}/Classification/clImageStatisticsQB456.xml
    -model   ${INPUTDATA}/Classification/clsvmModelQB456.svm
    -out     ${TEMP}/clLabeledImageQB456_6.tif
    VALID   --compare-image ${NOTOL}
    ${OTBAPP_BASELINE}/clLabeledImageQB456_6.tif
    ${TEMP}/clLabeledImageQB456_6.tif)

  otb_test_application(NAME apTvClImageSVMClassifierQB456_4
    APP  ImageClassifier
    OPTIONS -in      ${INPUTDATA}/Classification/QB_4_extract.tif
    -imstat  ${INPUTDATA}/Classification/clImageStatisticsQB456.xml
    -model   ${INPUTDATA}/Classification/clsvmModelQB456.svm
    -out     ${TEMP}/clLabeledImageQB456_4.tif
    VALID   --compare-image ${NOTOL}
    ${OTBAPP_BASELINE}/clLabeledImageQB456_4.tif
    ${TEMP}/clLabeledImageQB456_4.tif)

  otb_test_application(NAME apTvClImageSVMClassifierQB456_5
    APP  ImageClassifier
    OPTIONS -in      ${INPUTDATA}/Classification/QB_5_extract.tif
    -imstat  ${INPUTDATA}/Classification/clImageStatisticsQB456.xml
    -model   ${INPUTDATA}/Classification/clsvmModelQB456.svm
    -out     ${TEMP}/clLabeledImageQB456_5.tif
    VALID   --compare-image ${NOTOL}
    ${OTBAPP_BASELINE}/clLabeledImageQB456_5.tif
    ${TEMP}/clLabeledImageQB456_5.tif)

  otb_test_application(NAME apTvClImageSVMClassifierQB123_6
    APP  ImageClassifier
    OPTIONS -in      ${INPUTDATA}/Classification/QB_6_extract.tif
    -imstat  ${INPUTDATA}/Classification/clImageStatisticsQB123.xml
    -model   ${INPUTDATA}/Classification/clsvmModelQB123.svm
    -out     ${TEMP}/clLabeledImageQB123_6.tif
    VALID   --compare-image ${NOTOL}
    ${OTBAPP_BASELINE}/clLabeledImageQB123_6.tif
    ${TEMP}/clLabeledImageQB123_6.tif)

  otb_test_application(NAME apTvClImageSVMClassifierQB456_1
    APP  ImageClassifier
    OPTIONS -in      ${INPUTDATA}/Classification/QB_1_ortho.tif
    -imstat  ${INPUTDATA}/Classification/clImageStatisticsQB456.xml
    -model   ${INPUTDATA}/Classification/clsvmModelQB456.svm
    -out     ${TEMP}/clLabeledImageQB456_1.tif
    VALID   --compare-image ${NOTOL}
    ${OTBAPP_BASELINE}/clLabeledImageQB456_1.tif
    ${TEMP}/clLabeledImageQB456_1.tif)

  otb_test_application(NAME apTvClImageSVMClassifierQB123_3
    APP  ImageClassifier
    OPTIONS -in      ${INPUTDATA}/Classification/QB_3_ortho.tif
    -imstat  ${INPUTDATA}/Classification/clImageStatisticsQB123.xml
    -model   ${INPUTDATA}/Classification/clsvmModelQB123.svm
    -out     ${TEMP}/clLabeledImageQB123_3.tif
    VALID   --compare-image ${NOTOL}
    ${OTBAPP_BASELINE}/clLabeledImageQB123_3.tif
    ${TEMP}/clLabeledImageQB123_3.tif)

  otb_test_application(NAME apTvClImageSVMClassifierQB123_2
    APP  ImageClassifier
    OPTIONS -in      ${INPUTDATA}/Classification/QB_2_ortho.tif
    -imstat  ${INPUTDATA}/Classification/clImageStatisticsQB123.xml
    -model   ${INPUTDATA}/Classification/clsvmModelQB123.svm
    -out     ${TEMP}/clLabeledImageQB123_2.tif
    VALID   --compare-image ${NOTOL}
    ${OTBAPP_BASELINE}/clLabeledImageQB123_2.tif
    ${TEMP}/clLabeledImageQB123_2.tif)

  otb_test_application(NAME apTvClImageSVMClassifierQB123_1
    APP  ImageClassifier
    OPTIONS -in      ${INPUTDATA}/Classification/QB_1_ortho.tif
    -imstat  ${INPUTDATA}/Classification/clImageStatisticsQB123.xml
    -model   ${INPUTDATA}/Classification/clsvmModelQB123.svm
    -out     ${TEMP}/clLabeledImageQB123_1.tif
    VALID   --compare-image ${NOTOL}
    ${OTBAPP_BASELINE}/clLabeledImageQB123_1.tif
    ${TEMP}/clLabeledImageQB123_1.tif)

endif()

#----------- ComputeConfusionMatrix TESTS ----------------
otb_test_application(NAME apTvComputeConfusionMatrixV
  APP  ComputeConfusionMatrix
  OPTIONS -in ${OTBAPP_BASELINE}/clLabeledImageQB123_1.tif
  -ref vector
  -ref.vector.in ${INPUTDATA}/Classification/VectorData_QB1_ter_utm31n.sqlite
  -ref.vector.field class
  -out ${TEMP}/apTvComputeConfusionMatrixTconfusionVOut.csv
  VALID   --compare-ascii ${NOTOL}
  ${OTBAPP_BASELINE_FILES}/apTvComputeConfusionMatrixTconfusionVOut.csv
  ${TEMP}/apTvComputeConfusionMatrixTconfusionVOut.csv)

otb_test_application(NAME apTvComputeConfusionMatrixExtraReferenceLabelsV
  APP  ComputeConfusionMatrix
  OPTIONS -in ${INPUTDATA}/Classification/QB_1_ortho_C7.tif
  -ref vector
  -ref.vector.in ${INPUTDATA}/Classification/VectorData_QB1_ter_utm31n.sqlite
  -ref.vector.field class
  -out ${TEMP}/apTvComputeConfusionMatrixExtraRefLabelsVOut.csv
  VALID   --compare-ascii ${NOTOL}
  ${OTBAPP_BASELINE_FILES}/apTvComputeConfusionMatrixExtraRefLabelsVOut.csv
  ${TEMP}/apTvComputeConfusionMatrixExtraRefLabelsVOut.csv)

otb_test_application(NAME apTvComputeConfusionMatrixExtraReferenceLabelsR
  APP  ComputeConfusionMatrix
  OPTIONS -in ${INPUTDATA}/Classification/QB_1_ortho_C7.tif
  -ref raster
  -ref.raster.in ${INPUTDATA}/Classification/clLabeledImageQB456_1_NoData_255.tif
  -ref.raster.nodata 255
  -nodatalabel 255
  -out ${TEMP}/apTvComputeConfusionMatrixExtraRefLabelsROut.csv
  VALID   --compare-ascii ${NOTOL}
  ${OTBAPP_BASELINE_FILES}/apTvComputeConfusionMatrixExtraRefLabelsROut.csv
  ${TEMP}/apTvComputeConfusionMatrixExtraRefLabelsROut.csv)

otb_test_application(NAME apTvComputeConfusionMatrixR
  APP  ComputeConfusionMatrix
  OPTIONS -in ${OTBAPP_BASELINE}/clLabeledImageQB123_1.tif
  -ref raster
  -ref.raster.in ${INPUTDATA}/Classification/clLabeledImageQB456_1_NoData_255.tif
  -ref.raster.nodata 255
  -nodatalabel 255
  -out ${TEMP}/apTvComputeConfusionMatrixTconfusionROut.csv
  VALID   --compare-ascii ${NOTOL}
  ${OTBAPP_BASELINE_FILES}/apTvComputeConfusionMatrixTconfusionROut.csv
  ${TEMP}/apTvComputeConfusionMatrixTconfusionROut.csv)

otb_test_application(NAME apTvComputeConfusionMatrixExtraProducedLabelsR
  APP  ComputeConfusionMatrix
  OPTIONS -in ${INPUTDATA}/Classification/clLabeledImageQB456_1_NoData_255.tif
  -ref raster
  -ref.raster.in ${INPUTDATA}/Classification/QB_1_ortho_C8.tif
  -ref.raster.nodata 255
  -nodatalabel 255
  -out ${TEMP}/apTvComputeConfusionMatrixExtraProdLabelsROut.csv
  VALID   --compare-ascii ${NOTOL}
  ${OTBAPP_BASELINE_FILES}/apTvComputeConfusionMatrixExtraProdLabelsROut.csv
  ${TEMP}/apTvComputeConfusionMatrixExtraProdLabelsROut.csv)

#----------- ComputeContingencyTable TESTS ----------------

otb_test_application(NAME apTvComputeContingencyTableExtraProducedLabelsR
  APP  ComputeConfusionMatrix
  OPTIONS -in ${INPUTDATA}/Classification/clLabeledImageQB456_1_NoData_255.tif
  -ref raster
  -ref.raster.in ${INPUTDATA}/Classification/QB_1_ortho_C8.tif
  -ref.raster.nodata 255
  -nodatalabel 255
  -format contingencytable
  -out ${TEMP}/apTvComputeContingencyTableExtraProdLabelsROut.csv
  VALID   --compare-ascii ${NOTOL}
  ${OTBAPP_BASELINE_FILES}/apTvComputeContingencyTableExtraProdLabelsROut.csv
  ${TEMP}/apTvComputeContingencyTableExtraProdLabelsROut.csv)

#----------- FusionOfClassifications TESTS ----------------
otb_test_application(NAME apTvFusionOfClassificationsDSPrecision6Inputs
  APP  FusionOfClassifications
  OPTIONS -il  ${INPUTDATA}/Classification/QB_1_ortho_C1.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C2.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C3.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C4.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C5.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C6.tif
  -method dempstershafer
  -method.dempstershafer.cmfl ${INPUTDATA}/Classification/QB_1_ortho_C1.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C2.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C3.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C4.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C5.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C6.csv
  -method.dempstershafer.mob precision
  -nodatalabel 10
  -undecidedlabel 7
  -out ${TEMP}/apTvFusionOfClassificationsDS6InputsPrecisionOutput.tif uint8
  VALID   --compare-image ${NOTOL}
  ${BASELINE}/QB_1_ortho_DS_FUSED_PRECISION.tif
  ${TEMP}/apTvFusionOfClassificationsDS6InputsPrecisionOutput.tif)

otb_test_application(NAME apTvFusionOfClassificationsMV3Inputs
  APP  FusionOfClassifications
  OPTIONS -il ${INPUTDATA}/Classification/clLabeledImageQB123_1.tif
  ${INPUTDATA}/Classification/clLabeledImageQB456_1.tif
  ${INPUTDATA}/Classification/clLabeledImageQB1.tif
  -method majorityvoting
  -undecidedlabel 100
  -out ${TEMP}/apTvFusionOfClassificationsMV3InputsOutput.tif uint16
  VALID   --compare-image ${NOTOL}
  ${OTBAPP_BASELINE}/apTvFusionOfClassifications3InputsOutput.tif
  ${TEMP}/apTvFusionOfClassificationsMV3InputsOutput.tif)

otb_test_application(NAME apTvFusionOfClassificationsDSKappa6Inputs
  APP  FusionOfClassifications
  OPTIONS -il  ${INPUTDATA}/Classification/QB_1_ortho_C1.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C2.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C3.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C4.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C5.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C6.tif
  -method dempstershafer
  -method.dempstershafer.cmfl ${INPUTDATA}/Classification/QB_1_ortho_C1.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C2.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C3.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C4.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C5.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C6.csv
  -method.dempstershafer.mob kappa
  -nodatalabel 10
  -undecidedlabel 7
  -out ${TEMP}/apTvFusionOfClassificationsDS6InputsKappaOutput.tif uint8
  VALID   --compare-image ${NOTOL}
  ${BASELINE}/QB_1_ortho_DS_FUSED_KAPPA.tif
  ${TEMP}/apTvFusionOfClassificationsDS6InputsKappaOutput.tif)

otb_test_application(NAME apTvFusionOfClassificationsDSRecall6Inputs
  APP  FusionOfClassifications
  OPTIONS -il  ${INPUTDATA}/Classification/QB_1_ortho_C1.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C2.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C3.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C4.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C5.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C6.tif
  -method dempstershafer
  -method.dempstershafer.cmfl ${INPUTDATA}/Classification/QB_1_ortho_C1.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C2.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C3.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C4.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C5.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C6.csv
  -method.dempstershafer.mob recall
  -nodatalabel 10
  -undecidedlabel 7
  -out ${TEMP}/apTvFusionOfClassificationsDS6InputsRecallOutput.tif uint8
  VALID   --compare-image ${NOTOL}
  ${BASELINE}/QB_1_ortho_DS_FUSED_RECALL.tif
  ${TEMP}/apTvFusionOfClassificationsDS6InputsRecallOutput.tif)

otb_test_application(NAME apTvFusionOfClassificationsDSAccuracy6Inputs
  APP  FusionOfClassifications
  OPTIONS -il  ${INPUTDATA}/Classification/QB_1_ortho_C1.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C2.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C3.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C4.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C5.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C6.tif
  -method dempstershafer
  -method.dempstershafer.cmfl ${INPUTDATA}/Classification/QB_1_ortho_C1.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C2.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C3.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C4.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C5.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C6.csv
  -method.dempstershafer.mob accuracy
  -nodatalabel 10
  -undecidedlabel 7
  -out ${TEMP}/apTvFusionOfClassificationsDS6InputsAccuracyOutput.tif uint8
  VALID   --compare-image ${NOTOL}
  ${BASELINE}/QB_1_ortho_DS_FUSED_ACCURACY.tif
  ${TEMP}/apTvFusionOfClassificationsDS6InputsAccuracyOutput.tif)

otb_test_application(NAME apTvFusionOfClassificationsMV2Inputs
  APP  FusionOfClassifications
  OPTIONS -il ${INPUTDATA}/Classification/clLabeledImageQB123_1.tif
  ${INPUTDATA}/Classification/clLabeledImageQB456_1.tif
  -method majorityvoting
  -undecidedlabel 100
  -out ${TEMP}/apTvFusionOfClassificationsMV2InputsOutput.tif uint16
  VALID   --compare-image ${NOTOL}
  ${OTBAPP_BASELINE}/apTvFusionOfClassifications2InputsOutput.tif
  ${TEMP}/apTvFusionOfClassificationsMV2InputsOutput.tif)

#----------- KMeansClassification TESTS ----------------
if(OTB_USE_SHARK)
  otb_test_application(NAME apTvClKMeansImageClassification_composite
    APP  KMeansClassification
    OPTIONS -in ${INPUTDATA}/qb_RoadExtract.img
    -vm ${INPUTDATA}/qb_RoadExtract_mask_binary.png
    -ts 30000
    -nc 5
    -maxit 10000
    -sampler periodic
    -rand 121212
    -nodatalabel 255
    -centroids.out ${TEMP}/apTvClKMeansImageClassificationFilterOutMeans.txt
    -out ${TEMP}/apTvClKMeansImageClassificationFilterOutput.tif uint8
    -cleanup 0
    VALID   --compare-image ${NOTOL}
    ${OTBAPP_BASELINE}/apTvClKMeansImageClassificationFilterOutput.tif
    ${TEMP}/apTvClKMeansImageClassificationFilterOutput.tif )
endif()

if(OTB_USE_SHARK)
  otb_test_application(NAME apTvClKMeansImageClassification_inputCentroids
    APP  KMeansClassification
    OPTIONS -in ${INPUTDATA}/qb_RoadExtract.img
    -ts 30000
    -nc 5
    -maxit 10000
    -sampler periodic
    -nodatalabel 255
    -rand 121212
    -centroids.in ${INPUTDATA}/Classification/KMeansInputCentroids.txt
    -out ${TEMP}/apTvClKMeansImageClassificationInputCentroids.tif uint8
    -cleanup 0
    
    VALID   --compare-image ${NOTOL}
    ${OTBAPP_BASELINE}/apTvClKMeansImageClassificationInputCentroids.tif
    ${TEMP}/apTvClKMeansImageClassificationInputCentroids.tif )
endif()

#----------- TrainImagesClassifier TESTS ----------------
if(OTB_USE_LIBSVM)
  otb_test_application(NAME apTvClTrainSVMImagesClassifierQB1_allOpt_InXML
    APP  TrainImagesClassifier
    OPTIONS
    -inxml ${TEMP}/clsvmModelQB1_allOpt.xml
    -io.out ${TEMP}/clsvmModelQB1_allOpt_InXML.svm
    -io.confmatout ${TEMP}/clsvmConfMatQB1_allOpt_InXML.svm
    VALID   ${ascii_confmat_comparison}
    ${OTBAPP_BASELINE_FILES}/clsvmConfMatQB1_allOpt.svm
    ${TEMP}/clsvmConfMatQB1_allOpt_InXML.svm)

  set_tests_properties(apTvClTrainSVMImagesClassifierQB1_allOpt_InXML PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB1_allOpt)

  otb_test_application(NAME apTvClTrainSVMImagesClassifierQB123
    APP  TrainImagesClassifier
    OPTIONS -io.il ${INPUTDATA}/Classification/QB_1_ortho.tif
    ${INPUTDATA}/Classification/QB_2_ortho.tif
    ${INPUTDATA}/Classification/QB_3_ortho.tif
    -io.vd ${INPUTDATA}/Classification/VectorData_QB1_utm31n.sqlite
    ${INPUTDATA}/Classification/VectorData_QB2_utm31n.sqlite
    ${INPUTDATA}/Classification/VectorData_QB3_utm31n.sqlite
    -io.imstat ${INPUTDATA}/Classification/clImageStatisticsQB123.xml
    -sample.vfn class
    -classifier libsvm
    -classifier.libsvm.opt true
    -io.out ${TEMP}/clsvmModelQB123.svm
    -rand 121212)
  # RK: 09/2016. same reason explained for leTvSVMImageModelEstimatorTrainWithOptimization test
  #                       VALID   --compare-ascii ${NOTOL}
  #                               ${OTBAPP_BASELINE_FILES}/clsvmModelQB123.svm
  #                               ${TEMP}/clsvmModelQB123.svm

  otb_test_application(NAME apTvClTrainSVMImagesClassifierQB1
    APP  TrainImagesClassifier
    OPTIONS -io.il ${INPUTDATA}/Classification/QB_1_ortho.tif
    -io.vd ${INPUTDATA}/Classification/VectorData_QB1_utm31n.sqlite
    -io.imstat ${INPUTDATA}/Classification/clImageStatisticsQB1.xml
    -classifier libsvm
    -classifier.libsvm.opt true
    -sample.vfn class
    -io.out ${TEMP}/clsvmModelQB1.svm
    -io.confmatout ${TEMP}/clsvmConfMatQB1.svm
    -rand 121212
    VALID   ${ascii_confmat_comparison}
    ${OTBAPP_BASELINE_FILES}/clsvmConfMatQB1.svm
    ${TEMP}/clsvmConfMatQB1.svm)

  otb_test_application(NAME apTvClTrainSVMImagesClassifierQB456
    APP  TrainImagesClassifier
    OPTIONS -io.il ${INPUTDATA}/Classification/QB_4_extract.tif
    ${INPUTDATA}/Classification/QB_5_extract.tif
    ${INPUTDATA}/Classification/QB_6_extract.tif
    -io.vd ${INPUTDATA}/Classification/VectorData_QB4_utm31n.sqlite
    ${INPUTDATA}/Classification/VectorData_QB5_utm31n.sqlite
    ${INPUTDATA}/Classification/VectorData_QB6_utm31n.sqlite
    -io.imstat ${INPUTDATA}/Classification/clImageStatisticsQB456.xml
    -sample.vfn class
    -classifier libsvm
    -classifier.libsvm.opt true
    -io.out ${TEMP}/clsvmModelQB456.svm
    -rand 121212)

  # RK: 09/2016. same reason explained for leTvSVMImageModelEstimatorTrainWithOptimization test
  #                       VALID   --compare-ascii ${NOTOL}
  #                               ${OTBAPP_BASELINE_FILES}/clsvmModelQB456.svm
  #                               ${TEMP}/clsvmModelQB456.svm

  otb_test_application(NAME apTvClTrainSVMImagesClassifierQB1_allOpt
    APP  TrainImagesClassifier
    OPTIONS -io.il ${INPUTDATA}/Classification/QB_1_ortho.tif
    -io.vd ${INPUTDATA}/Classification/VectorData_QB1_utm31n.sqlite
    -io.imstat ${INPUTDATA}/Classification/clImageStatisticsQB1.xml
    -classifier libsvm
    -sample.mv 100
    -sample.mt 100
    -sample.vtr 0.5
    -sample.vfn class
    -classifier.libsvm.opt true
    -rand 121212
    -io.out ${TEMP}/clsvmModelQB1_allOpt.svm
    -io.confmatout ${TEMP}/clsvmConfMatQB1_allOpt.svm
    -outxml ${TEMP}/clsvmModelQB1_allOpt.xml
    VALID   ${ascii_confmat_comparison}
    ${OTBAPP_BASELINE_FILES}/clsvmConfMatQB1_allOpt.svm
    ${TEMP}/clsvmConfMatQB1_allOpt.svm)
endif()

#----------- ClassificationMapRegularization TESTS ----------------
otb_test_application(NAME apTvClassificationMapRegularization
  APP  ClassificationMapRegularization
  OPTIONS -io.in  ${OTBAPP_BASELINE}/clLabeledImageQB123_1.tif
  -io.out ${TEMP}/clLabeledImageQB123_1_CMR_r2_nodl_10_undl_7.tif
  -ip.radius 2
  -ip.suvbool true
  -ip.nodatalabel 10
  -ip.undecidedlabel 7
  VALID   --compare-image ${NOTOL}
  ${OTBAPP_BASELINE}/clLabeledImageQB123_1_CMR_r2_nodl_10_undl_7.tif
  ${TEMP}/clLabeledImageQB123_1_CMR_r2_nodl_10_undl_7.tif)


#----------- OGRLayerClassifier TESTS ----------------

#----------- VectorClassifier TESTS ----------------
if(OTB_USE_OPENCV)
  otb_test_application(NAME apTvClSVMVectorClassifier
    APP  VectorClassifier
    OPTIONS -in ${INPUTDATA}/Classification/apTvClLabeledVector.shp
            -instat ${OTBAPP_BASELINE_FILES}/apTvClComputeOGRLayersFeaturesStatistics.xml
            -model ${OTBAPP_BASELINE_FILES}/apTvClModel.svm
            -out ${TEMP}/apTvClSVMLabeledVector.shp
            -feat meanB0 meanB1 meanB2 meanB3 varB0 varB1 varB2 varB3
            -cfield class
    VALID   --compare-ogr ${NOTOL}
            ${OTBAPP_BASELINE_FILES}/apTvClSVMLabeledVector.shp
            ${TEMP}/apTvClSVMLabeledVector.shp)
endif()

#----------- VectorRegression TESTS ----------------
if(OTB_USE_OPENCV)
  otb_test_application(NAME apTvClDTVectorRegression
    APP  VectorRegression
    OPTIONS -in ${INPUTDATA}/Classification/apTvClSampleExtractionOut.sqlite
            -model ${INPUTDATA}/Classification/apTvClDTVectorRegression.dt
            -out ${TEMP}/apTvClDTVectorRegression.sqlite
            -feat value_0 value_1 value_2 value_3
            -cfield predicted
    VALID   --compare-ogr ${NOTOL}
            ${OTBAPP_BASELINE_FILES}/apTvClDTVectorRegression.sqlite
            ${TEMP}/apTvClDTVectorRegression.sqlite)
endif()

#----------- ComputeImagesStatistics TESTS ----------------
otb_test_application(NAME apTvClComputeImagesStatisticsQB1
  APP  ComputeImagesStatistics
  OPTIONS -il ${INPUTDATA}/Classification/QB_1_ortho.tif
  -out ${TEMP}/apTvClEstimateImageStatisticsQB1.xml
  VALID   --compare-ascii ${NOTOL}
  ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB1.xml
  ${TEMP}/apTvClEstimateImageStatisticsQB1.xml)

otb_test_application(NAME apTvClComputeImagesStatisticsQB456
  APP  ComputeImagesStatistics
  OPTIONS -il ${INPUTDATA}/Classification/QB_4_extract.tif
  ${INPUTDATA}/Classification/QB_5_extract.tif
  ${INPUTDATA}/Classification/QB_6_extract.tif
  -out ${TEMP}/apTvClEstimateImageStatisticsQB456.xml
  VALID   --compare-ascii ${NOTOL}
  ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB456.xml
  ${TEMP}/apTvClEstimateImageStatisticsQB456.xml)

otb_test_application(NAME apTvClComputeImagesStatisticsQB123
  APP  ComputeImagesStatistics
  OPTIONS -il ${INPUTDATA}/Classification/QB_1_ortho.tif
  ${INPUTDATA}/Classification/QB_2_ortho.tif
  ${INPUTDATA}/Classification/QB_3_ortho.tif
  -out ${TEMP}/apTvClEstimateImageStatisticsQB123.xml
  VALID   --compare-ascii ${NOTOL}
  ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB123.xml
  ${TEMP}/apTvClEstimateImageStatisticsQB123.xml)

if(OTB_USE_OPENCV)
  #----------- TrainRegression TESTS ----------------
  # y = 0.01*x^2 + 1.5*x - 300
  otb_test_application(NAME apTvClTrainRegressionTest_monovar
    APP  TrainRegression
    OPTIONS -io.il ${INPUTDATA}/QB_Toulouse_Ortho_regression.tif
    -io.imstat ${INPUTDATA}/QB_Toulouse_Ortho_regression.xml
    -io.out ${TEMP}/apTvClTrainRegressionTest_monovar.rf
    -sample.mt 20000
    -sample.mv 20000
    -sample.vtr 0.5
    -rand 121212
    -classifier rf
    -classifier.rf.ra 0.0001
    -classifier.rf.max 6
    -classifier.rf.acc 0.0005
    TESTENVOPTIONS ${TEMP}/apTvClTrainRegressionTest_monovar.txt
    VALID    ${ascii_comparison}
    ${OTBAPP_BASELINE_FILES}/apTvClTrainRegressionTest_monovar.txt
    ${TEMP}/apTvClTrainRegressionTest_monovar.txt)

  #----------- ImageRegression TESTS ----------------
  otb_test_application(NAME apTvClImageRegressionTest_monovar
    APP ImageRegression
    OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
    -model ${OTBAPP_BASELINE_FILES}/apTvClTrainRegressionTest_monovar.rf
    -imstat ${INPUTDATA}/QB_Toulouse_Ortho_regression.xml
    -out ${TEMP}/apTvClImageRegressionTest_monovar.tif
    VALID    --compare-image 1
    ${OTBAPP_BASELINE}/apTvClImageRegressionTest_monovar.tif
    ${TEMP}/apTvClImageRegressionTest_monovar.tif)

endif()

#----------- PolygonClassStatistics TESTS ----------------
otb_test_application(NAME apTvClPolygonClassStatisticsTest
  APP PolygonClassStatistics
  OPTIONS -in ${INPUTDATA}/Classification/QB_1_ortho.tif
  -vec ${INPUTDATA}/Classification/VectorData_QB1_utm31n.sqlite
  -field class
  -out ${TEMP}/apTvClPolygonClassStatisticsOut.xml
  VALID   --compare-ascii ${NOTOL}
  ${OTBAPP_BASELINE_FILES}/apTvClPolygonClassStatisticsOut.xml
  ${TEMP}/apTvClPolygonClassStatisticsOut.xml)

#----------- SampleSelection TESTS -----------------------
otb_test_application(NAME apTvClSampleSelection
  APP SampleSelection
  OPTIONS -in ${INPUTDATA}/Classification/QB_1_ortho.tif
  -vec ${INPUTDATA}/Classification/VectorData_QB1_utm31n.sqlite
  -field class
  -out ${TEMP}/apTvClSampleSelectionOut.sqlite
  -instats ${OTBAPP_BASELINE_FILES}/apTvClPolygonClassStatisticsOut.xml
  -outrates ${TEMP}/apTvClSampleSelectionOutRates.txt
  -strategy byclass
  -strategy.byclass.in ${INPUTDATA}/Classification/sampling_required.csv
  -sampler periodic
  VALID   --compare-ogr ${NOTOL}
  ${OTBAPP_BASELINE_FILES}/apTvClSampleSelectionOut.sqlite
  ${TEMP}/apTvClSampleSelectionOut.sqlite)

#----------- SampleExtraction TESTS -----------------------
otb_test_application(NAME apTvClSampleExtraction
  APP SampleExtraction
  OPTIONS -in ${INPUTDATA}/Classification/QB_1_ortho.tif
  -vec ${INPUTDATA}/Classification/apTvClSampleSelectionOut.sqlite
  -field class
  -out ${TEMP}/apTvClSampleExtractionOut.sqlite
  VALID   --compare-ogr ${NOTOL}
  ${OTBAPP_BASELINE_FILES}/apTvClSampleExtractionOut.sqlite
  ${TEMP}/apTvClSampleExtractionOut.sqlite)

#----------- TrainVectorClassifier TESTS ----------------
if(OTB_USE_OPENCV)
  otb_test_application(NAME apTvClTrainVectorClassifier
    APP  TrainVectorClassifier
    OPTIONS -io.vd ${INPUTDATA}/Classification/apTvClSampleExtractionOut.sqlite
    -feat value_0 value_1 value_2 value_3
    -cfield class
    -classifier rf
    -io.confmatout ${TEMP}/apTvClTrainVectorClassifierConfMat.txt
    -io.out ${TEMP}/apTvClTrainVectorClassifierModel.rf
    VALID   ${ascii_comparison}
    ${OTBAPP_BASELINE_FILES}/apTvClTrainVectorClassifierModel.rf
    ${TEMP}/apTvClTrainVectorClassifierModel.rf)
endif()

#----------- TrainVectorRegression TESTS ----------------
if(OTB_USE_OPENCV)
  otb_test_application(NAME apTvClTrainVectorRegression
    APP  TrainVectorRegression
    OPTIONS -io.vd ${INPUTDATA}/Classification/apTvClSampleExtractionOut.sqlite
    -feat value_0 value_1 value_2 value_3
    -cfield class
    -classifier rf
    -io.out ${TEMP}/apTvClTrainVectorRegressionModel.rf
    -io.mse ${TEMP}/apTvClTrainVectorRegressionModel.txt
    TESTENVOPTIONS ${TEMP}/apTvClTrainVectorRegressionModel.txt
    VALID   ${ascii_comparison}
    ${OTBAPP_BASELINE_FILES}/apTvClTrainVectorRegressionModel.txt
    ${TEMP}/apTvClTrainVectorRegressionModel.txt)
endif()

#----------- TrainImagesRegression TESTS ----------------
if(OTB_USE_OPENCV)
  otb_test_application(NAME apTvClTrainImagesRegression
    APP  TrainImagesRegression
    OPTIONS 
    -io.il ${INPUTDATA}/Classification/QB_1_ortho.tif?&bands=0,1,2
    -io.ip ${INPUTDATA}/Classification/QB_1_ortho.tif?&bands=3
    -io.vd ${INPUTDATA}/Classification/VectorData_QB1_utm31n.sqlite
    -sample.nt 50
    -sample.ratio 0
    -classifier rf
    -io.out ${TEMP}/apTvClTrainImagesRegressionModel.rf
    -io.mse ${TEMP}/apTvClTrainImagesRegressionModel.txt
    TESTENVOPTIONS ${TEMP}/apTvClTrainImagesRegressionModel.txt
    VALID   ${ascii_comparison}
    ${OTBAPP_BASELINE_FILES}/apTvClTrainImagesRegressionModel.txt
    ${TEMP}/apTvClTrainImagesRegressionModel.txt)
endif()

if(OTB_USE_OPENCV)
  otb_test_application(NAME apTvClTrainImagesRegressionNoVD
    APP  TrainImagesRegression
    OPTIONS 
    -io.il ${INPUTDATA}/Classification/QB_1_ortho.tif?&bands=0,1,2
    -io.ip ${INPUTDATA}/Classification/QB_1_ortho.tif?&bands=3
    -sample.nt 50
    -sample.ratio 0.5
    -classifier rf
    -io.out ${TEMP}/apTvClTrainImagesRegressionNoVD.rf
    -io.mse ${TEMP}/apTvClTrainImagesRegressionNoVD.txt
    TESTENVOPTIONS ${TEMP}/apTvClTrainImagesRegressionNoVD.txt
    VALID   ${ascii_comparison}
    ${OTBAPP_BASELINE_FILES}/apTvClTrainImagesRegressionNoVD.txt
    ${TEMP}/apTvClTrainImagesRegressionNoVD.txt)
endif()

#----------- TrainVectorClassifier unsupervised TESTS ----------------
if(OTB_USE_SHARK)
  otb_test_application(NAME apTvClTrainVectorUnsupervised
    APP  TrainVectorClassifier
    OPTIONS -io.vd ${INPUTDATA}/Classification/apTvClSampleExtractionOut.sqlite
    -feat value_0 value_1 value_2 value_3
    -classifier sharkkm
    -io.out ${TEMP}/apTvClTrainVectorClusteringModel.txt)

  otb_test_application(NAME apTvClTrainVectorUnsupervisedWithClass
    APP  TrainVectorClassifier
    OPTIONS -io.vd ${INPUTDATA}/Classification/apTvClSampleExtractionOut.sqlite
    -feat value_0 value_1 value_2 value_3
    -cfield class
    -classifier sharkkm
    -io.out ${TEMP}/apTvClTrainVectorClusteringModelWithClass.txt)
endif()

#------------ MultiImageSamplingRate TESTS ----------------
otb_test_application(
  NAME apTvClMultiImageSamplingRate
  APP MultiImageSamplingRate
  OPTIONS -il ${INPUTDATA}/Classification/vector_stats_QB1.xml
  ${INPUTDATA}/Classification/vector_stats_QB2.xml
  ${INPUTDATA}/Classification/vector_stats_QB3.xml
  -out ${TEMP}/apTvClMultiImageSamplingRate_out.csv
  -strategy constant
  -strategy.constant.nb "300"
  -mim proportional
  VALID   --compare-n-ascii ${NOTOL} 3
  ${OTBAPP_BASELINE_FILES}/apTvClMultiImageSamplingRate_out_1.csv
  ${TEMP}/apTvClMultiImageSamplingRate_out_1.csv
  ${OTBAPP_BASELINE_FILES}/apTvClMultiImageSamplingRate_out_2.csv
  ${TEMP}/apTvClMultiImageSamplingRate_out_2.csv
  ${OTBAPP_BASELINE_FILES}/apTvClMultiImageSamplingRate_out_3.csv
  ${TEMP}/apTvClMultiImageSamplingRate_out_3.csv
)

#------------ SampleAgmentation TESTS ----------------
otb_test_application(NAME apTvClSampleAugmentationReplicate
  APP  SampleAugmentation
  OPTIONS -in ${INPUTDATA}/Classification/apTvClSampleExtractionOut.sqlite
  -field class
  -label 3
  -samples 100
  -out ${TEMP}/apTvClSampleAugmentationReplicate.sqlite
  -exclude originfid
  -strategy replicate
  )

otb_test_application(NAME apTvClSampleAugmentationJitter
  APP  SampleAugmentation
  OPTIONS -in ${INPUTDATA}/Classification/apTvClSampleExtractionOut.sqlite
  -field class
  -label 3
  -samples 100
  -out ${TEMP}/apTvClSampleAugmentationJitter.sqlite
  -exclude originfid
  -strategy jitter
  -strategy.jitter.stdfactor 10
  )

otb_test_application(NAME apTvClSampleAugmentationSmote
  APP  SampleAugmentation
  OPTIONS -in ${INPUTDATA}/Classification/apTvClSampleExtractionOut.sqlite
  -field class
  -label 3
  -samples 100
  -out ${TEMP}/apTvClSampleAugmentationSmote.sqlite
  -exclude originfid
  -strategy smote
  -strategy.smote.neighbors 5
  )

#----------- ZonalStatistics TESTS ----------------
otb_test_application(NAME apTvClZonalStatisticsVec
  APP  ZonalStatistics
  OPTIONS -inzone vector -inzone.vector.in ${INPUTDATA}/Classification/VectorData_QB1_ter_utm31n.sqlite -out.vector.filename ${TEMP}/apTvClVectorData_QB1_ter_with_stats.sqlite -in ${INPUTDATA}/Classification/QB_1_ortho.tif -inzone.vector.reproject 1
  VALID   --compare-ogr ${EPSILON_9}
  ${OTBAPP_BASELINE_FILES}/apTvClVectorData_QB1_ter_with_stats.sqlite
  ${TEMP}/apTvClVectorData_QB1_ter_with_stats.sqlite)

otb_test_application(NAME apTvClZonalStatisticsImg
  APP  ZonalStatistics
  OPTIONS -inzone labelimage -inzone.labelimage.in ${INPUTDATA}/Classification/VectorData_QB1_ter.tif -out.xml.filename ${TEMP}/apTvClVectorData_QB1_ter_stats.xml -in ${INPUTDATA}/Classification/QB_1_ortho.tif -out xml
  VALID   --compare-ascii ${NOTOL}
  ${OTBAPP_BASELINE_FILES}/apTvClVectorData_QB1_ter_stats.xml
  ${TEMP}/apTvClVectorData_QB1_ter_stats.xml)

otb_test_application(NAME apTvClZonalStatisticsImgNoData
  APP  ZonalStatistics
  OPTIONS -inzone labelimage -inzone.labelimage.in ${INPUTDATA}/Classification/VectorData_QB1_ter.tif -out.xml.filename ${TEMP}/apTvClVectorData_QB1_ter_stats_nodata.xml -in ${INPUTDATA}/Classification/QB_1_ortho.tif -inzone.labelimage.nodata -1  -out xml
  VALID   --compare-ascii ${NOTOL}
  ${OTBAPP_BASELINE_FILES}/apTvClVectorData_QB1_ter_stats_nodata.xml
  ${TEMP}/apTvClVectorData_QB1_ter_stats_nodata.xml)

  
otb_test_application(NAME apTvClZonalStatisticsInVecOutRaster
  APP  ZonalStatistics
  OPTIONS 
  -in ${INPUTDATA}/Classification/QB_1_ortho.tif 
  -inzone vector 
  -inzone.vector.in ${INPUTDATA}/Classification/VectorData_QB1_ter_utm31n.sqlite
  -inzone.vector.reproject 1
  -out raster  
  -out.raster.filename ${TEMP}/apTvClZonalStats_QB1_ter_with_stats_invec.tif
  -out.raster.bv -1  
  VALID    --compare-image 1
  ${OTBAPP_BASELINE}/apTvClZonalStats_QB1_ter_with_stats_invec.tif
  ${TEMP}/apTvClZonalStats_QB1_ter_with_stats_invec.tif
  )

otb_test_application(NAME apTvClZonalStatisticsInRasterOutRaster
  APP  ZonalStatistics
  OPTIONS 
  -in ${INPUTDATA}/Classification/QB_1_ortho.tif 
  -inzone labelimage 
  -inzone.labelimage.in ${INPUTDATA}/Classification/VectorData_QB1_ter.tif
  -inzone.labelimage.nodata -1
  -out raster  
  -out.raster.filename ${TEMP}/apTvClZonalStats_QB1_ter_with_stats_inraster.tif  
  VALID    --compare-image 1
  ${OTBAPP_BASELINE}/apTvClZonalStats_QB1_ter_with_stats_inraster.tif
  ${TEMP}/apTvClZonalStats_QB1_ter_with_stats_inraster.tif
  )
