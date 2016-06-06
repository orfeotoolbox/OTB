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
                             -som ${TEMP}/apTvClSOMClassificationMap.hdr
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
                             ${BASELINE}/apTvClSOMClassificationMap.hdr
                             ${TEMP}/apTvClSOMClassificationMap.hdr)


#----------- ImageClassifier TESTS ----------------

# Input file types
set(raster_input_format ".tif")
set(vector_input_format ".shp")
set(stat_input_format ".xml")

# Input file depending on classifier
set(libsvm_input "")
set(svm_input "")
set(boost_input "Water_Vegetation")
set(dt_input "")
set(gbt_input "")
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
set(gbt_output_format ".gbt")
set(ann_output_format ".ann")
set(bayes_output_format ".bayes")
set(rf_output_format ".rf")
set(knn_output_format ".knn")

# Training algorithms parameters
set(libsvm_parameters "-classifier.libsvm.opt" "true" "-classifier.libsvm.prob" "true")
#set(svm_parameters "-classifier.svm.opt" "true")
set(boost_parameters "")
set(dt_parameters "")
set(gbt_parameters "")
set(ann_parameters "-classifier.ann.sizes" "100")
set(bayes_parameters "")
set(rf_parameters "")
set(knn_parameters "")

# Validation depending on mode
set(ascii_comparison --compare-ascii ${NOTOL})
set(raster_comparison --compare-image ${NOTOL})
set(raster_comparison_two --compare-n-images ${NOTOL} 2)

# Reference ffiles depending on modes
set(ascii_ref_path ${OTBAPP_BASELINE_FILES})
set(raster_ref_path ${OTBAPP_BASELINE})

set(classifierList)
#if(OTB_USE_LIBSVM)
  #list(APPEND classifierList "LIBSVM")
#endif()
if(OTB_USE_OPENCV)
  #list(APPEND classifierList "SVM" "BOOST" "DT" "GBT" "ANN" "BAYES" "RF" "KNN")
  list(APPEND classifierList "BOOST" "DT" "GBT" "ANN" "BAYES" "RF" "KNN")
endif()
set(classifier_with_confmap "LIBSVM" "BOOST" "KNN" "ANN" "RF")

# Loop on classifiers
foreach(classifier ${classifierList})
  string(TOLOWER ${classifier} lclassifier)

  # Derive output file name
  set(OUTMODELFILE cl${classifier}_ModelQB1${${lclassifier}_output_format})
  set(OUTRASTER cl${classifier}LabeledImageQB1${raster_output_format})
  set(OUTCONFMAP cl${classifier}ConfidenceMapQB1${raster_output_format})

  otb_test_application(
                       NAME     apTvClTrainMethod${classifier}ImagesClassifierQB1
                       APP      TrainImagesClassifier
                       OPTIONS  -io.il ${INPUTDATA}/Classification/QB_1_ortho${raster_input_format}
                                -io.vd ${INPUTDATA}/Classification/VectorData_${${lclassifier}_input}QB1${vector_input_format}
                                -io.imstat ${INPUTDATA}/Classification/clImageStatisticsQB1${stat_input_format}
                                -classifier ${lclassifier}
                                ${${lclassifier}_parameters}
                                -io.out ${TEMP}/${OUTMODELFILE}
                                -rand 121212

                       VALID    ${ascii_comparison}
                                ${ascii_ref_path}/${OUTMODELFILE}
                                ${TEMP}/${OUTMODELFILE}
                       )

  otb_test_application(
                       NAME     apTvClTrainMethod${classifier}ImagesClassifierQB1_OutXML1
                       APP      TrainImagesClassifier
                       OPTIONS  -io.il ${INPUTDATA}/Classification/QB_1_ortho${raster_input_format}
                                -io.vd ${INPUTDATA}/Classification/VectorData_${${lclassifier}_input}QB1${vector_input_format}
                                -io.imstat ${INPUTDATA}/Classification/clImageStatisticsQB1${stat_input_format}
                                -classifier ${lclassifier}
                                ${${lclassifier}_parameters}
                                -io.out ${TEMP}/OutXML1_${OUTMODELFILE}
                                -rand 121212
                                -outxml ${TEMP}/cl${classifier}_OutXML1.xml

                       VALID    ${ascii_comparison}
                                ${ascii_ref_path}/${OUTMODELFILE}
                                ${TEMP}/OutXML1_${OUTMODELFILE}
                       )

  otb_test_application(
                       NAME     apTvClTrainMethod${classifier}ImagesClassifierQB1_InXML1
                       APP      TrainImagesClassifier
                       OPTIONS  -inxml ${INPUTDATA}/cl${classifier}_OutXML1.xml
                                -io.il ${INPUTDATA}/Classification/QB_1_ortho${raster_input_format}
                                -io.vd ${INPUTDATA}/Classification/VectorData_${${lclassifier}_input}QB1${vector_input_format}
                                -io.imstat ${INPUTDATA}/Classification/clImageStatisticsQB1${stat_input_format}
                                -io.out ${TEMP}/OutXML2_${OUTMODELFILE}

                       VALID    ${ascii_comparison}
                                ${ascii_ref_path}/${OUTMODELFILE}
                                ${TEMP}/OutXML2_${OUTMODELFILE}
                       )

                     #set_tests_properties(apTvClTrainMethod${classifier}ImagesClassifierQB1_InXML1 PROPERTIES DEPENDS apTvClTrainMethod${classifier}ImagesClassifierQB1_OutXML1)

  list(FIND classifier_with_confmap ${classifier} _classifier_has_confmap)
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
  endif()

endforeach()

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

#----------- TrainOGRLayersClassifier TESTS ----------------
otb_test_application(NAME apTvClTrainVectorClassifier
                     APP  TrainVectorClassifier
                     OPTIONS -io.vd ${INPUTDATA}/Classification/apTvClLabeledVector.shp
                             -feat meanB0 meanB1 meanB2 meanB3 varB0 varB1 varB2 varB3
                             -io.stats ${TEMP}/apTvClComputeOGRLayersFeaturesStatistics.xml
                             -classifier libsvm
                             -classifier.libsvm.c 1
                             -classifier.libsvm.k linear
                             -classifier.libsvm.opt true
                             -io.out ${TEMP}/apTvClTrainVectorSVMModel.svm
                     VALID   --compare-ascii ${NOTOL}
                             ${OTBAPP_BASELINE_FILES}/apTvClTrainVectorSVMModel.svm
                             ${TEMP}/apTvClTrainVectorSVMModel.svm)

set_tests_properties(apTvClTrainVectorClassifier PROPERTIES DEPENDS ComputeOGRLayersFeaturesStatistics)

#----------- ComputeConfusionMatrix TESTS ----------------
otb_test_application(NAME apTvComputeConfusionMatrixV
                     APP  ComputeConfusionMatrix
                     OPTIONS -in ${OTBAPP_BASELINE}/clLabeledImageQB123_1.tif
                             -ref vector
                             -ref.vector.in ${INPUTDATA}/Classification/VectorData_QB1_ter.shp
                             -ref.vector.field Class
                             -out ${TEMP}/apTvComputeConfusionMatrixTconfusionVOut.csv
                     VALID   --compare-ascii ${NOTOL}
                     ${OTBAPP_BASELINE_FILES}/apTvComputeConfusionMatrixTconfusionVOut.csv
                     ${TEMP}/apTvComputeConfusionMatrixTconfusionVOut.csv)

otb_test_application(NAME apTvComputeConfusionMatrixExtraReferenceLabelsV
                     APP  ComputeConfusionMatrix
                     OPTIONS -in ${INPUTDATA}/Classification/QB_1_ortho_C7.tif
                             -ref vector
                             -ref.vector.in ${INPUTDATA}/Classification/VectorData_QB1_ter.shp
                             -ref.vector.field Class
                             -out ${TEMP}/apTvComputeConfusionMatrixExtraRefLabelsVOut.csv
                     VALID   --compare-ascii ${NOTOL}
                     ${OTBAPP_BASELINE_FILES}/apTvComputeConfusionMatrixExtraRefLabelsVOut.csv
                     ${TEMP}/apTvComputeConfusionMatrixExtraRefLabelsVOut.csv)

otb_test_application(NAME apTvComputeConfusionMatrixExtraReferenceLabelsR
                     APP  ComputeConfusionMatrix
                     OPTIONS -in ${INPUTDATA}/Classification/QB_1_ortho_C7.tif
                             -ref raster
                             -ref.raster.in ${INPUTDATA}/Classification/clLabeledImageQB456_1_NoData_255.tif
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
                             -nodatalabel 255
                             -out ${TEMP}/apTvComputeConfusionMatrixExtraProdLabelsROut.csv
                     VALID   --compare-ascii ${NOTOL}
                     ${OTBAPP_BASELINE_FILES}/apTvComputeConfusionMatrixExtraProdLabelsROut.csv
                     ${TEMP}/apTvComputeConfusionMatrixExtraProdLabelsROut.csv)


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


#----------- ComputePolylineFeatureFromImage TESTS ----------------
otb_test_application(NAME apTvCdbComputePolylineFeatureFromImage_LI_NONDVI_wr
                     APP  ComputePolylineFeatureFromImage
                     OPTIONS -in LARGEINPUT{DEMPSTER-SHAFER/NDVI.TIF}
                             -vd LARGEINPUT{DEMPSTER-SHAFER/wrongroads.shp}
                             -expr  "(b1 > 0.4)"
                             -field "NONDVI"
                             -out ${TEMP}/apTvCdbComputePolylineFeatureFromImage_LI_NONDVI_wr.shp)

# FIXME cascade dependencies to largeinput repository
if(OTB_DATA_USE_LARGEINPUT)
otb_test_application(NAME apTvCdbComputePolylineFeatureFromImage_LI_ROADSA_RoadExtractionApplication
                     APP  ComputePolylineFeatureFromImage
                     OPTIONS -in LARGEINPUT{DEMPSTER-SHAFER/SpectralAngleRoad.TIF}
                             -vd ${TEMP}/apTvCdbComputePolylineFeatureFromImage_LI_NONDVI_RoadExtractionApplication.shp
                             -expr  "(b1 > 0.25)"
                             -field "ROADSA"
                             -out ${TEMP}/apTvCdbComputePolylineFeatureFromImage_LI_ROADSA_RoadExtractionApplication.shp)

set_tests_properties(apTvCdbComputePolylineFeatureFromImage_LI_ROADSA_RoadExtractionApplication PROPERTIES DEPENDS apTvCdbComputePolylineFeatureFromImage_LI_NONDVI_RoadExtractionApplication)

otb_test_application(NAME apTvCdbComputePolylineFeatureFromImage_LI_ROADSA_wr
                     APP  ComputePolylineFeatureFromImage
                     OPTIONS -in LARGEINPUT{DEMPSTER-SHAFER/SpectralAngleRoad.TIF}
                             -vd ${TEMP}/apTvCdbComputePolylineFeatureFromImage_LI_NONDVI_wr.shp
                             -expr  "(b1 > 0.25)"
                             -field "ROADSA"
                             -out ${TEMP}/apTvCdbComputePolylineFeatureFromImage_LI_ROADSA_wr.shp)

set_tests_properties(apTvCdbComputePolylineFeatureFromImage_LI_ROADSA_wr PROPERTIES DEPENDS apTvCdbComputePolylineFeatureFromImage_LI_NONDVI_wr)

otb_test_application(NAME apTvCdbComputePolylineFeatureFromImage_LI_NOBUIL_wr
                     APP  ComputePolylineFeatureFromImage
                     OPTIONS -in LARGEINPUT{DEMPSTER-SHAFER/BUILDINGS.TIF}
                             -vd ${TEMP}/apTvCdbComputePolylineFeatureFromImage_LI_ROADSA_wr.shp
                             -expr  "(b1 == 0)"
                             -field "NOBUIL"
                             -out ${TEMP}/apTvCdbComputePolylineFeatureFromImage_LI_NOBUIL_wr.shp
                     VALID   --compare-ogr 0.0
                             ${OTBAPP_BASELINE_FILES}/cdbTvComputePolylineFeatureFromImage_LI_NOBUIL_wr.shp
                             ${TEMP}/apTvCdbComputePolylineFeatureFromImage_LI_NOBUIL_wr.shp)

set_tests_properties(apTvCdbComputePolylineFeatureFromImage_LI_NOBUIL_wr PROPERTIES DEPENDS apTvCdbComputePolylineFeatureFromImage_LI_ROADSA_wr)

otb_test_application(NAME apTvCdbComputePolylineFeatureFromImage_LI_NONDVI_RoadExtractionApplication
                     APP  ComputePolylineFeatureFromImage
                     OPTIONS -in LARGEINPUT{DEMPSTER-SHAFER/NDVI.TIF}
                             -vd LARGEINPUT{DEMPSTER-SHAFER/road_extraction.shp}
                             -expr  "(b1 > 0.4)"
                             -field "NONDVI"
                             -out ${TEMP}/apTvCdbComputePolylineFeatureFromImage_LI_NONDVI_RoadExtractionApplication.shp)

otb_test_application(NAME apTvCdbComputePolylineFeatureFromImage_LI_NONDVI_gt
                     APP  ComputePolylineFeatureFromImage
                     OPTIONS -in LARGEINPUT{DEMPSTER-SHAFER/NDVI.TIF}
                             -vd LARGEINPUT{DEMPSTER-SHAFER/roads_ground_truth.shp}
                             -expr  "(b1 > 0.4)"
                             -field NONDVI
                             -out ${TEMP}/apTvCdbComputePolylineFeatureFromImage.shp)

otb_test_application(NAME apTvCdbComputePolylineFeatureFromImage_LI_ROADSA_gt
                     APP  ComputePolylineFeatureFromImage
                     OPTIONS -in LARGEINPUT{DEMPSTER-SHAFER/SpectralAngleRoad.TIF}
                             -vd ${TEMP}/apTvCdbComputePolylineFeatureFromImage.shp
                             -expr  "(b1 > 0.25)"
                             -field "ROADSA"
                             -out ${TEMP}/apTvCdbComputePolylineFeatureFromImage_LI_ROADSA_gt.shp)

set_tests_properties(apTvCdbComputePolylineFeatureFromImage_LI_ROADSA_gt PROPERTIES DEPENDS apTvCdbComputePolylineFeatureFromImage_LI_NONDVI_gt)

otb_test_application(NAME apTvCdbComputePolylineFeatureFromImage_LI_NOBUIL_gt
                     APP  ComputePolylineFeatureFromImage
                     OPTIONS -in LARGEINPUT{DEMPSTER-SHAFER/BUILDINGS.TIF}
                             -vd ${TEMP}/apTvCdbComputePolylineFeatureFromImage_LI_ROADSA_gt.shp
                             -expr  "(b1 == 0)"
                             -field "NOBUIL"
                             -out ${TEMP}/apTvCdbComputePolylineFeatureFromImage_LI_NOBUIL_gt.shp
                     VALID   --compare-ogr 0.0
                             ${OTBAPP_BASELINE_FILES}/cdbTvComputePolylineFeatureFromImage_LI_NOBUIL_gt.shp
                             ${TEMP}/apTvCdbComputePolylineFeatureFromImage_LI_NOBUIL_gt.shp)

set_tests_properties(apTvCdbComputePolylineFeatureFromImage_LI_NOBUIL_gt PROPERTIES DEPENDS apTvCdbComputePolylineFeatureFromImage_LI_ROADSA_gt)

otb_test_application(NAME apTvCdbComputePolylineFeatureFromImage_LI_NOBUIL_RoadExtractionApplication
                     APP  ComputePolylineFeatureFromImage
                     OPTIONS -in LARGEINPUT{DEMPSTER-SHAFER/BUILDINGS.TIF}
                             -vd ${TEMP}/apTvCdbComputePolylineFeatureFromImage_LI_ROADSA_RoadExtractionApplication.shp
                             -expr  "(b1 == 0)"
                             -field "NOBUIL"
                             -out ${TEMP}/apTvCdbComputePolylineFeatureFromImage_LI_NOBUIL_RoadExtractionApplication.shp
                     VALID   --compare-ogr 0.0
                             ${OTBAPP_BASELINE_FILES}/cdbTvComputePolylineFeatureFromImage_LI_NOBUIL_RoadExtractionApplication.shp
                             ${TEMP}/apTvCdbComputePolylineFeatureFromImage_LI_NOBUIL_RoadExtractionApplication.shp)

set_tests_properties(apTvCdbComputePolylineFeatureFromImage_LI_NOBUIL_RoadExtractionApplication PROPERTIES DEPENDS apTvCdbComputePolylineFeatureFromImage_LI_ROADSA_RoadExtractionApplication)
endif()

#----------- KMeansClassification TESTS ----------------
otb_test_application(NAME apTvClKMeansImageClassification
                     APP  KMeansClassification
                     OPTIONS -in ${INPUTDATA}/qb_RoadExtract.img
                             -vm ${INPUTDATA}/qb_RoadExtract_mask.png
                             -ts 30000
                             -nc 5
                             -maxit 10000
                             -ct 0.0000001
                             -rand 121212
                             -out ${TEMP}/apTvClKMeansImageClassificationFilterOutput.tif
                     VALID   --compare-image ${NOTOL}
                             ${OTBAPP_BASELINE}/apTvClKMeansImageClassificationFilterOutput.tif
                             ${TEMP}/apTvClKMeansImageClassificationFilterOutput.tif )


#----------- TrainImagesClassifier TESTS ----------------
if(OTB_USE_LIBSVM)
otb_test_application(NAME apTvClTrainSVMImagesClassifierQB1_allOpt_InXML
                      APP  TrainImagesClassifier
                      OPTIONS
                              -inxml ${INPUTDATA}/clsvmModelQB1_OutXML.xml
                              -io.il ${INPUTDATA}/Classification/QB_1_ortho.tif
                              -io.vd ${INPUTDATA}/Classification/VectorData_QB1.shp
                              -io.imstat ${INPUTDATA}/Classification/clImageStatisticsQB1.xml
                              -sample.mv 100
                              -sample.mt 100
                              -sample.vtr 0.5
                              -classifier.libsvm.opt true
                              -rand 121212
                              -io.out ${TEMP}/clsvmModelQB1_allOpt_InXML.svm
                      VALID   --compare-ascii ${NOTOL}
                              ${OTBAPP_BASELINE_FILES}/clsvmModelQB1.svm
                              ${TEMP}/clsvmModelQB1_allOpt_InXML.svm)

otb_test_application(NAME apTvClTrainSVMImagesClassifierQB1_OutXML
                      APP  TrainImagesClassifier
                      OPTIONS -io.il ${INPUTDATA}/Classification/QB_1_ortho.tif
                              -io.vd ${INPUTDATA}/Classification/VectorData_QB1.shp
                              -io.imstat ${INPUTDATA}/Classification/clImageStatisticsQB1.xml
                              -classifier libsvm
                              -classifier.libsvm.opt true
                              -io.out ${TEMP}/clsvmModelQB1_OutXML.svm
                              -rand 121212
                              -outxml ${TEMP}/clsvmModelQB1_OutXML.xml
                      VALID   --compare-ascii ${NOTOL}
                              ${OTBAPP_BASELINE_FILES}/clsvmModelQB1.svm
                              ${TEMP}/clsvmModelQB1_OutXML.svm)

  otb_test_application(NAME apTvClTrainSVMImagesClassifierQB123
                       APP  TrainImagesClassifier
                       OPTIONS -io.il ${INPUTDATA}/Classification/QB_1_ortho.tif
                                      ${INPUTDATA}/Classification/QB_2_ortho.tif
                                      ${INPUTDATA}/Classification/QB_3_ortho.tif
                               -io.vd ${INPUTDATA}/Classification/VectorData_QB1.shp
                                      ${INPUTDATA}/Classification/VectorData_QB2.shp
                                      ${INPUTDATA}/Classification/VectorData_QB3.shp
                               -io.imstat ${INPUTDATA}/Classification/clImageStatisticsQB123.xml
                               -classifier libsvm
                               -classifier.libsvm.opt true
                               -io.out ${TEMP}/clsvmModelQB123.svm
                               -rand 121212
                       VALID   --compare-ascii ${NOTOL}
                               ${OTBAPP_BASELINE_FILES}/clsvmModelQB123.svm
                               ${TEMP}/clsvmModelQB123.svm)

otb_test_application(NAME apTvClTrainSVMImagesClassifierQB1
                      APP  TrainImagesClassifier
                      OPTIONS -io.il ${INPUTDATA}/Classification/QB_1_ortho.tif
                              -io.vd ${INPUTDATA}/Classification/VectorData_QB1.shp
                              -io.imstat ${INPUTDATA}/Classification/clImageStatisticsQB1.xml
                              -classifier libsvm
                              -classifier.libsvm.opt true
                              -io.out ${TEMP}/clsvmModelQB1.svm
                              -rand 121212
                      VALID   --compare-ascii ${NOTOL}
                              ${OTBAPP_BASELINE_FILES}/clsvmModelQB1.svm
                              ${TEMP}/clsvmModelQB1.svm)

  otb_test_application(NAME apTvClTrainSVMImagesClassifierQB456
                       APP  TrainImagesClassifier
                       OPTIONS -io.il ${INPUTDATA}/Classification/QB_4_extract.tif
                                      ${INPUTDATA}/Classification/QB_5_extract.tif
                                      ${INPUTDATA}/Classification/QB_6_extract.tif
                               -io.vd ${INPUTDATA}/Classification/VectorData_QB4.shp
                                      ${INPUTDATA}/Classification/VectorData_QB5.shp
                                      ${INPUTDATA}/Classification/VectorData_QB6.shp
                               -io.imstat ${INPUTDATA}/Classification/clImageStatisticsQB456.xml
                               -classifier libsvm
                               -classifier.libsvm.opt true
                               -io.out ${TEMP}/clsvmModelQB456.svm
                               -rand 121212
                       VALID   --compare-ascii ${NOTOL}
                               ${OTBAPP_BASELINE_FILES}/clsvmModelQB456.svm
                               ${TEMP}/clsvmModelQB456.svm)

otb_test_application(NAME apTvClTrainSVMImagesClassifierQB1_allOpt
                      APP  TrainImagesClassifier
                      OPTIONS -io.il ${INPUTDATA}/Classification/QB_1_ortho.tif
                              -io.vd ${INPUTDATA}/Classification/VectorData_QB1.shp
                              -io.imstat ${INPUTDATA}/Classification/clImageStatisticsQB1.xml
                              -classifier libsvm
                              -sample.mv 100
                              -sample.mt 100
                              -sample.vtr 0.5
                              -classifier.libsvm.opt true
                              -rand 121212
                              -io.out ${TEMP}/clsvmModelQB1_allOpt.svm
                      VALID   --compare-ascii ${NOTOL}
                              ${OTBAPP_BASELINE_FILES}/clsvmModelQB1.svm
                              ${TEMP}/clsvmModelQB1_allOpt.svm)
endif()

#----------- DSFuzzyModelEstimation TESTS ----------------
otb_test_application(NAME apTvCdbDSFuzzyModelEstimation_LI_autoInit
                     APP  DSFuzzyModelEstimation
                     OPTIONS -psin ${OTBAPP_BASELINE_FILES}/cdbTvComputePolylineFeatureFromImage_LI_NOBUIL_gt.shp
                             -nsin ${OTBAPP_BASELINE_FILES}/cdbTvComputePolylineFeatureFromImage_LI_NOBUIL_wr.shp
                             -belsup "ROADSA"
                             -plasup "NONDVI" "ROADSA" "NOBUIL"
                             -desclist "NONDVI" "ROADSA" "NOBUIL"
                             -maxnbit 4
                             -optobs true
                             -out ${TEMP}/apTvCdbDSFuzzyModelEstimatorOutput_LI_autoInit.xml)

otb_test_application(NAME apTvCdbDSFuzzyModelEstimation_LI
                     APP  DSFuzzyModelEstimation
                     OPTIONS -psin ${OTBAPP_BASELINE_FILES}/cdbTvComputePolylineFeatureFromImage_LI_NOBUIL_gt.shp
                             -nsin ${OTBAPP_BASELINE_FILES}/cdbTvComputePolylineFeatureFromImage_LI_NOBUIL_wr.shp
                             -belsup "ROADSA"
                             -plasup "NONDVI" "ROADSA" "NOBUIL"
                             -initmod ${OTB_DATA_ROOT}/Input/Dempster-Shafer/DSFuzzyModel_Init.xml
                             -maxnbit 4
                             -optobs true
                             -out ${TEMP}/apTvCdbDSFuzzyModelEstimatorOutput_LI.xml)


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


#----------- VectorDataDSValidation TESTS ----------------
otb_test_application(NAME cdbTvVectorDataDSValidationGroundTruth_LI
                     APP  VectorDataDSValidation
                     OPTIONS -in ${OTBAPP_BASELINE_FILES}/cdbTvComputePolylineFeatureFromImage_LI_NOBUIL_gt.shp
                             -belsup "ROADSA"
                             -plasup "NONDVI" "ROADSA" "NOBUIL"
                             -descmod ${OTB_DATA_ROOT}/Input/Dempster-Shafer/DSFuzzyModel.xml
                             -out ${TEMP}/cdbTvVectorDataDSValidationOutpout_LI_gt.shp
                    VALID    --compare-ogr 0.0
                             ${OTBAPP_BASELINE_FILES}/cdbTvVectorDataDSValidationOutpout_LI_gt.shp
                             ${TEMP}/cdbTvVectorDataDSValidationOutpout_LI_gt.shp)

otb_test_application(NAME apTvCdbVectorDataDSValidationRoadExtractionApplication_LI
                     APP  VectorDataDSValidation
                     OPTIONS -in ${OTBAPP_BASELINE_FILES}/cdbTvComputePolylineFeatureFromImage_LI_NOBUIL_RoadExtractionApplication.shp
                             -belsup "ROADSA"
                             -plasup "NONDVI" "ROADSA" "NOBUIL"
                             -descmod ${OTB_DATA_ROOT}/Input/Dempster-Shafer/DSFuzzyModel.xml
                             -out ${TEMP}/apTvCdbVectorDataDSValidationOutpout_LI_RoadExtractionApplication.shp)

otb_test_application(NAME cdbTvVectorDataDSValidationWrongRoads_LI
                     APP  VectorDataDSValidation
                     OPTIONS -in ${OTBAPP_BASELINE_FILES}/cdbTvComputePolylineFeatureFromImage_LI_NOBUIL_wr.shp
                             -belsup "ROADSA"
                             -plasup "NONDVI" "ROADSA" "NOBUIL"
                             -descmod ${OTB_DATA_ROOT}/Input/Dempster-Shafer/DSFuzzyModel.xml
                             -out ${TEMP}/cdbTvVectorDataDSValidationOutpout_LI_wr.shp
                    VALID    --compare-ogr 0.0
                             ${OTBAPP_BASELINE_FILES}/cdbTvVectorDataDSValidationOutpout_LI_wr.shp
                             ${TEMP}/cdbTvVectorDataDSValidationOutpout_LI_wr.shp)

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
                    VALID    --compare-ascii ${NOTOL}
                             ${OTBAPP_BASELINE_FILES}/apTvClTrainRegressionTest_monovar.rf
                             ${TEMP}/apTvClTrainRegressionTest_monovar.rf)

#----------- PredictRegression TESTS ----------------
otb_test_application(NAME apTvClPredictRegressionTest_monovar
                     APP PredictRegression
                     OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                             -model ${OTBAPP_BASELINE_FILES}/apTvClTrainRegressionTest_monovar.rf
                             -imstat ${INPUTDATA}/QB_Toulouse_Ortho_regression.xml
                             -out ${TEMP}/apTvClPredictRegressionTest_monovar.tif
                    VALID    --compare-image 1
                             ${OTBAPP_BASELINE}/apTvClPredictRegressionTest_monovar.tif
                             ${TEMP}/apTvClPredictRegressionTest_monovar.tif)

endif()

#----------- PolygonClassStatistics TESTS ----------------
otb_test_application(NAME apTvClPolygonClassStatisticsTest
                     APP PolygonClassStatistics
                     OPTIONS -in ${INPUTDATA}/cthead1_flip.tif
                             -vec ${INPUTDATA}/variousVectors.sqlite
                             -field label
                             -out ${TEMP}/apTvClPolygonClassStatisticsOut.xml
                     VALID   --compare-ascii ${NOTOL}
                             ${OTBAPP_BASELINE_FILES}/apTvClPolygonClassStatisticsOut.xml
                             ${TEMP}/apTvClPolygonClassStatisticsOut.xml)
