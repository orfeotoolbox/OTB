
#--- EstimateImageStatistics ---#
# QB1 chain

OTB_TEST_APPLICATION(NAME apTvClComputeImagesStatisticsQB1
                     APP  ComputeImagesStatistics
                     OPTIONS -il ${INPUTDATA}/Classification/QB_1_ortho.tif
                             -out ${TEMP}/apTvClEstimateImageStatisticsQB1.xml
                     VALID   --compare-ascii ${NOTOL}
                             ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB1.xml
                             ${TEMP}/apTvClEstimateImageStatisticsQB1.xml)


IF(OTB_USE_OPENCV)

  # Input file types
  SET(raster_input_format ".tif")
  SET(vector_input_format ".shp")
  SET(stat_input_format ".xml")
  
  # Input file depending on classifier
  SET(libsvm_input "")
  SET(svm_input "")
  SET(boost_input "Water_Vegetation")
  SET(dt_input "")
  SET(gbt_input "")
  SET(ann_input "")
  SET(bayes_input "")
  SET(rf_input "")
  SET(knn_input "")
  
  # Output type
  SET(raster_output_format ".tif")
  SET(raster_output_option "uint8")
  
  # Output model type depending on classifier
  SET(libsvm_output_format ".libsvm")
  SET(svm_output_format ".svm")
  SET(boost_output_format ".boost")
  SET(dt_output_format ".dt")
  SET(gbt_output_format ".gbt")
  SET(ann_output_format ".ann")
  SET(bayes_output_format ".bayes")
  SET(rf_output_format ".rf")
  SET(knn_output_format ".knn")
  
  # Training algorithms parameters
  SET(libsvm_parameters "-classifier.libsvm.opt true")
  SET(svm_parameters "-classifier.svm.opt true")
  SET(boost_parameters "")
  SET(dt_parameters "")
  SET(gbt_parameters "")
  SET(ann_parameters "-classifier.ann.sizes 100")
  SET(bayes_parameters "")
  SET(rf_parameters "")
  SET(knn_parameters "")
  
  # Validation depending on mode 
  SET(ascii_comparison --compare-ascii ${NOTOL})
  SET(raster_comparison --compare-image ${NOTOL})
  
  # Reference ffiles depending on modes
  SET(ascii_ref_path ${OTBAPP_BASELINE_FILES})
  SET(raster_ref_path ${OTBAPP_BASELINE})
  
  # Loop on classifiers
  FOREACH(classifier "LIBSVM" "SVM" "BOOST" "DT" "GBT" "ANN" "BAYES" "RF" "KNN")
  STRING(TOLOWER ${classifier} lclassifier)

    # Derive output file name
    SET(OUTMODELFILE cl${classifier}_ModelQB1${${lclassifier}_output_format})
    SET(OUTRASTER cl${classifier}LabeledImageQB1${raster_output_format})
    
    OTB_TEST_APPLICATION(
                         NAME     apTvClTrainMethod${classifier}ImagesClassifierQB1
                         APP      TrainImagesClassifier
                         OPTIONS  -io.il ${INPUTDATA}/Classification/QB_1_ortho${raster_input_format}
                                  -io.vd ${INPUTDATA}/Classification/VectorData_${${lclassifier}_input}QB1${vector_input_format}
                                  -io.imstat ${ascii_ref_path}/clImageStatisticsQB1${stat_input_format}
                                  -classifier ${lclassifier}
                                  ${${lclassifier}_parameters}
                                  -io.out ${TEMP}/${OUTMODELFILE}
                                  -rand 121212

                         VALID    ${ascii_comparison}
                                  ${ascii_ref_path}/${OUTMODELFILE}
                                  ${TEMP}/${OUTMODELFILE}
                         )

    OTB_TEST_APPLICATION(
                         NAME     apTvClTrainMethod${classifier}ImagesClassifierQB1_OutXML1
                         APP      TrainImagesClassifier
                         OPTIONS  -io.il ${INPUTDATA}/Classification/QB_1_ortho${raster_input_format}
                                  -io.vd ${INPUTDATA}/Classification/VectorData_${${lclassifier}_input}QB1${vector_input_format}
                                  -io.imstat ${ascii_ref_path}/clImageStatisticsQB1${stat_input_format}
                                  -classifier ${lclassifier}
                                  ${${lclassifier}_parameters}
                                  -io.out ${TEMP}/OutXML1_${OUTMODELFILE}
                                  -rand 121212
                                  -outxml ${TEMP}/cl${classifier}_OutXML1.xml

                         VALID    ${ascii_comparison}
                                  ${ascii_ref_path}/${OUTMODELFILE}
                                  ${TEMP}/OutXML1_${OUTMODELFILE}
                         )

    OTB_TEST_APPLICATION(
                         NAME     apTvClTrainMethod${classifier}ImagesClassifierQB1_InXML1
                         APP      TrainImagesClassifier
                         OPTIONS  -inxml ${TEMP}/cl${classifier}_OutXML1.xml
                                  -io.il ${INPUTDATA}/Classification/QB_1_ortho${raster_input_format}
                                  -io.vd ${INPUTDATA}/Classification/VectorData_${${lclassifier}_input}QB1${vector_input_format}
                                  -io.imstat ${ascii_ref_path}/clImageStatisticsQB1${stat_input_format}
                                  -io.out ${TEMP}/OutXML2_${OUTMODELFILE}

                         VALID    ${ascii_comparison}
                                  ${ascii_ref_path}/${OUTMODELFILE}
                                  ${TEMP}/OutXML2_${OUTMODELFILE}
                         )

                       #SET_TESTS_PROPERTIES(apTvClTrainMethod${classifier}ImagesClassifierQB1_InXML1 PROPERTIES DEPENDS apTvClTrainMethod${classifier}ImagesClassifierQB1_OutXML1)

    OTB_TEST_APPLICATION(
                         NAME     apTvClMethod${classifier}ImageClassifierQB1
                         APP      ImageClassifier
                         OPTIONS  -in ${INPUTDATA}/Classification/QB_1_ortho${raster_input_format}
                                  -model ${ascii_ref_path}/${OUTMODELFILE}
                                  -imstat ${ascii_ref_path}/clImageStatisticsQB1${stat_input_format}
                                  -out ${TEMP}/${OUTRASTER} ${raster_output_option}

                         VALID    ${raster_comparison}
                                  ${raster_ref_path}/${OUTRASTER}
                                  ${TEMP}/${OUTRASTER}
                         )

  ENDFOREACH(classifier)
ENDIF()

OTB_TEST_APPLICATION(NAME apTvClTrainSVMImagesClassifierQB1
                      APP  TrainImagesClassifier
                      OPTIONS -io.il ${INPUTDATA}/Classification/QB_1_ortho.tif
                              -io.vd ${INPUTDATA}/Classification/VectorData_QB1.shp
                              -io.imstat ${TEMP}/apTvClEstimateImageStatisticsQB1.xml
                              -classifier libsvm
                              -classifier.libsvm.opt true
                              -io.out ${TEMP}/clsvmModelQB1.svm
                              -rand 121212
                      VALID   --compare-ascii ${NOTOL}
                              ${OTBAPP_BASELINE_FILES}/clsvmModelQB1.svm
                              ${TEMP}/clsvmModelQB1.svm)

SET_TESTS_PROPERTIES(apTvClTrainSVMImagesClassifierQB1 PROPERTIES DEPENDS apTvClComputeImagesStatisticsQB1)

OTB_TEST_APPLICATION(NAME apTvClTrainSVMImagesClassifierQB1_OutXML
                      APP  TrainImagesClassifier
                      OPTIONS -io.il ${INPUTDATA}/Classification/QB_1_ortho.tif
                              -io.vd ${INPUTDATA}/Classification/VectorData_QB1.shp
                              -io.imstat ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB1.xml
                              -classifier libsvm
                              -classifier.libsvm.opt true
                              -io.out ${TEMP}/clsvmModelQB1_OutXML.svm
                              -rand 121212
                              -outxml ${TEMP}/clsvmModelQB1_OutXML.xml
                      VALID   --compare-ascii ${NOTOL}
                              ${OTBAPP_BASELINE_FILES}/clsvmModelQB1.svm
                              ${TEMP}/clsvmModelQB1_OutXML.svm)

#SET_TESTS_PROPERTIES(apTvClTrainSVMImagesClassifierQB1_OutXML PROPERTIES DEPENDS apTvClComputeImagesStatisticsQB1)
#because ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB1.xml == ${TEMP}/apTvClEstimateImageStatisticsQB1.xml

OTB_TEST_APPLICATION(NAME apTvClTrainSVMImagesClassifierQB1_allOpt
                      APP  TrainImagesClassifier
                      OPTIONS -io.il ${INPUTDATA}/Classification/QB_1_ortho.tif
                              -io.vd ${INPUTDATA}/Classification/VectorData_QB1.shp
                              -io.imstat ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB1.xml
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
  
OTB_TEST_APPLICATION(NAME apTvClTrainSVMImagesClassifierQB1_allOpt_InXML
                      APP  TrainImagesClassifier
                      OPTIONS 
                              -inxml ${TEMP}/clsvmModelQB1_OutXML.xml
                              -io.il ${INPUTDATA}/Classification/QB_1_ortho.tif
                              -io.vd ${INPUTDATA}/Classification/VectorData_QB1.shp
                              -io.imstat ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB1.xml
                              -sample.mv 100
                              -sample.mt 100
                              -sample.vtr 0.5
                              -classifier.libsvm.opt true
                              -rand 121212
                              -io.out ${TEMP}/clsvmModelQB1_allOpt_InXML.svm
                      VALID   --compare-ascii ${NOTOL}
                              ${OTBAPP_BASELINE_FILES}/clsvmModelQB1.svm
                              ${TEMP}/clsvmModelQB1_allOpt_InXML.svm)

#SET_TESTS_PROPERTIES(apTvClTrainSVMImagesClassifierQB1_allOpt_InXML PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB1_OutXML)

OTB_TEST_APPLICATION(NAME apTvClImageSVMClassifierQB1
                      APP  ImageClassifier
                      OPTIONS -in      ${INPUTDATA}/Classification/QB_1_ortho.tif
                              -imstat  ${TEMP}/apTvClEstimateImageStatisticsQB1.xml
                              -model   ${TEMP}/clsvmModelQB1.svm
                              -out     ${TEMP}/clLabeledImageQB1.tif
                      VALID   --compare-image ${NOTOL}
                              ${OTBAPP_BASELINE}/clLabeledImageQB1.tif
                              ${TEMP}/clLabeledImageQB1.tif)

SET_TESTS_PROPERTIES(apTvClImageSVMClassifierQB1 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB1)


OTB_TEST_APPLICATION(NAME apTvClImageSVMClassifierQB2
                      APP  ImageClassifier
                      OPTIONS -in      ${INPUTDATA}/Classification/QB_2_ortho.tif
                              -imstat  ${TEMP}/apTvClEstimateImageStatisticsQB1.xml
                              -model   ${TEMP}/clsvmModelQB1.svm
                              -out     ${TEMP}/clLabeledImageQB2.tif
                      VALID   --compare-image ${NOTOL}
                              ${OTBAPP_BASELINE}/clLabeledImageQB2.tif
                              ${TEMP}/clLabeledImageQB2.tif)

SET_TESTS_PROPERTIES(apTvClImageSVMClassifierQB2 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB1)


OTB_TEST_APPLICATION(NAME apTvClImageSVMClassifierQB3
                      APP  ImageClassifier
                      OPTIONS -in      ${INPUTDATA}/Classification/QB_3_ortho.tif
                              -imstat  ${TEMP}/apTvClEstimateImageStatisticsQB1.xml
                              -model   ${TEMP}/clsvmModelQB1.svm
                              -out     ${TEMP}/clLabeledImageQB3.tif
                      VALID   --compare-image ${NOTOL}
                              ${OTBAPP_BASELINE}/clLabeledImageQB3.tif
                              ${TEMP}/clLabeledImageQB3.tif)

SET_TESTS_PROPERTIES(apTvClImageSVMClassifierQB3 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB1)

#--------
# QB multi chain

OTB_TEST_APPLICATION(NAME apTvClComputeImagesStatisticsQB123
                     APP  ComputeImagesStatistics
                     OPTIONS -il ${INPUTDATA}/Classification/QB_1_ortho.tif
                                 ${INPUTDATA}/Classification/QB_2_ortho.tif
                                 ${INPUTDATA}/Classification/QB_3_ortho.tif
                             -out ${TEMP}/apTvClEstimateImageStatisticsQB123.xml
                     VALID   --compare-ascii ${NOTOL}
                             ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB123.xml
                             ${TEMP}/apTvClEstimateImageStatisticsQB123.xml)


IF(OTB_USE_OPENCV)
  OTB_TEST_APPLICATION(NAME apTvClTrainSVMImagesClassifierQB123
                       APP  TrainImagesClassifier
                       OPTIONS -io.il ${INPUTDATA}/Classification/QB_1_ortho.tif
                                      ${INPUTDATA}/Classification/QB_2_ortho.tif
                                      ${INPUTDATA}/Classification/QB_3_ortho.tif
                               -io.vd ${INPUTDATA}/Classification/VectorData_QB1.shp
                                      ${INPUTDATA}/Classification/VectorData_QB2.shp
                                      ${INPUTDATA}/Classification/VectorData_QB3.shp
                               -io.imstat ${TEMP}/apTvClEstimateImageStatisticsQB123.xml
                               -classifier libsvm
                               -classifier.libsvm.opt true
                               -io.out ${TEMP}/clsvmModelQB123.svm
                               -rand 121212
                       VALID   --compare-ascii ${NOTOL}
                               ${OTBAPP_BASELINE_FILES}/clsvmModelQB123.svm
                               ${TEMP}/clsvmModelQB123.svm)
  
  SET_TESTS_PROPERTIES(apTvClTrainSVMImagesClassifierQB123 PROPERTIES DEPENDS apTvClComputeImagesStatisticsQB123)
  
  
  OTB_TEST_APPLICATION(NAME apTvClImageSVMClassifierQB123_1
                       APP  ImageClassifier
                       OPTIONS -in      ${INPUTDATA}/Classification/QB_1_ortho.tif
                               -imstat  ${TEMP}/apTvClEstimateImageStatisticsQB123.xml
                               -model   ${TEMP}/clsvmModelQB123.svm
                               -out     ${TEMP}/clLabeledImageQB123_1.tif
                       VALID   --compare-image ${NOTOL}
                               ${OTBAPP_BASELINE}/clLabeledImageQB123_1.tif
                               ${TEMP}/clLabeledImageQB123_1.tif)
  
  SET_TESTS_PROPERTIES(apTvClImageSVMClassifierQB123_1 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB123)
ENDIF()
  
  
OTB_TEST_APPLICATION(NAME apTvClLabeledImageColorMappingQB123_1
                     APP  ColorMapping
                     OPTIONS -in      ${OTBAPP_BASELINE}/clLabeledImageQB123_1.tif
                             -method  custom
                             -method.custom.lut ${INPUTDATA}/Classification/ColorTable.txt
                             -out     ${TEMP}/clLabeledFancyImageQB123_1.tif
                     VALID   --compare-image ${NOTOL}
                             ${OTBAPP_BASELINE}/clLabeledFancyImageQB123_1.tif
                             ${TEMP}/clLabeledFancyImageQB123_1.tif)

#SET_TESTS_PROPERTIES(apTvClLabeledImageColorMappingQB123_1 PROPERTIES DEPENDS apTvClImageSVMClassifierQB123_1)
  

IF(OTB_USE_OPENCV)
  OTB_TEST_APPLICATION(NAME apTvClImageSVMClassifierQB123_2
                       APP  ImageClassifier
                       OPTIONS -in      ${INPUTDATA}/Classification/QB_2_ortho.tif
                               -imstat  ${TEMP}/apTvClEstimateImageStatisticsQB123.xml
                               -model   ${TEMP}/clsvmModelQB123.svm
                               -out     ${TEMP}/clLabeledImageQB123_2.tif
                       VALID   --compare-image ${NOTOL}
                               ${OTBAPP_BASELINE}/clLabeledImageQB123_2.tif
                               ${TEMP}/clLabeledImageQB123_2.tif)
  
  SET_TESTS_PROPERTIES(apTvClImageSVMClassifierQB123_2 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB123)
  
  
  OTB_TEST_APPLICATION(NAME apTvClImageSVMClassifierQB123_3
                       APP  ImageClassifier
                       OPTIONS -in      ${INPUTDATA}/Classification/QB_3_ortho.tif
                               -imstat  ${TEMP}/apTvClEstimateImageStatisticsQB123.xml
                               -model   ${TEMP}/clsvmModelQB123.svm
                               -out     ${TEMP}/clLabeledImageQB123_3.tif
                       VALID   --compare-image ${NOTOL}
                               ${OTBAPP_BASELINE}/clLabeledImageQB123_3.tif
                               ${TEMP}/clLabeledImageQB123_3.tif)
  
  SET_TESTS_PROPERTIES(apTvClImageSVMClassifierQB123_3 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB123)
  
  
  OTB_TEST_APPLICATION(NAME apTvClImageSVMClassifierQB123_6
                       APP  ImageClassifier
                       OPTIONS -in      ${INPUTDATA}/Classification/QB_6_extract.tif
                               -imstat  ${TEMP}/apTvClEstimateImageStatisticsQB123.xml
                               -model   ${TEMP}/clsvmModelQB123.svm
                               -out     ${TEMP}/clLabeledImageQB123_6.tif
                       VALID   --compare-image ${NOTOL}
                               ${OTBAPP_BASELINE}/clLabeledImageQB123_6.tif
                               ${TEMP}/clLabeledImageQB123_6.tif)
  
  SET_TESTS_PROPERTIES(apTvClImageSVMClassifierQB123_6 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB123)
ENDIF()


OTB_TEST_APPLICATION(NAME apTvClComputeImagesStatisticsQB456
                     APP  ComputeImagesStatistics
                     OPTIONS -il ${INPUTDATA}/Classification/QB_4_extract.tif
                                 ${INPUTDATA}/Classification/QB_5_extract.tif
                                 ${INPUTDATA}/Classification/QB_6_extract.tif
                             -out ${TEMP}/apTvClEstimateImageStatisticsQB456.xml
                     VALID   --compare-ascii ${NOTOL}
                             ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB456.xml
                             ${TEMP}/apTvClEstimateImageStatisticsQB456.xml)


IF(OTB_USE_OPENCV)
  OTB_TEST_APPLICATION(NAME apTvClTrainSVMImagesClassifierQB456
                       APP  TrainImagesClassifier
                       OPTIONS -io.il ${INPUTDATA}/Classification/QB_4_extract.tif
                                      ${INPUTDATA}/Classification/QB_5_extract.tif
                                      ${INPUTDATA}/Classification/QB_6_extract.tif
                               -io.vd ${INPUTDATA}/Classification/VectorData_QB4.shp
                                      ${INPUTDATA}/Classification/VectorData_QB5.shp
                                      ${INPUTDATA}/Classification/VectorData_QB6.shp
                               -io.imstat ${TEMP}/apTvClEstimateImageStatisticsQB456.xml
                               -classifier libsvm
                               -classifier.libsvm.opt true
                               -io.out ${TEMP}/clsvmModelQB456.svm
                               -rand 121212
                       VALID   --compare-ascii ${NOTOL}
                               ${OTBAPP_BASELINE_FILES}/clsvmModelQB456.svm
                               ${TEMP}/clsvmModelQB456.svm)
  
  SET_TESTS_PROPERTIES(apTvClTrainSVMImagesClassifierQB456 PROPERTIES DEPENDS apTvClComputeImagesStatisticsQB456)
  
  
  OTB_TEST_APPLICATION(NAME apTvClImageSVMClassifierQB456_4
                       APP  ImageClassifier
                       OPTIONS -in      ${INPUTDATA}/Classification/QB_4_extract.tif
                               -imstat  ${TEMP}/apTvClEstimateImageStatisticsQB456.xml
                               -model   ${TEMP}/clsvmModelQB456.svm
                               -out     ${TEMP}/clLabeledImageQB456_4.tif
                       VALID   --compare-image ${NOTOL}
                               ${OTBAPP_BASELINE}/clLabeledImageQB456_4.tif
                               ${TEMP}/clLabeledImageQB456_4.tif)
  
  SET_TESTS_PROPERTIES(apTvClImageSVMClassifierQB456_4 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB456)
  
  
  OTB_TEST_APPLICATION(NAME apTvClImageSVMClassifierQB456_5
                       APP  ImageClassifier
                       OPTIONS -in      ${INPUTDATA}/Classification/QB_5_extract.tif
                               -imstat  ${TEMP}/apTvClEstimateImageStatisticsQB456.xml
                               -model   ${TEMP}/clsvmModelQB456.svm
                               -out     ${TEMP}/clLabeledImageQB456_5.tif
                       VALID   --compare-image ${NOTOL}
                               ${OTBAPP_BASELINE}/clLabeledImageQB456_5.tif
                               ${TEMP}/clLabeledImageQB456_5.tif)
  
  SET_TESTS_PROPERTIES(apTvClImageSVMClassifierQB456_5 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB456)
  
  
  OTB_TEST_APPLICATION(NAME apTvClImageSVMClassifierQB456_6
                       APP  ImageClassifier
                       OPTIONS -in      ${INPUTDATA}/Classification/QB_6_extract.tif
                               -mask    ${INPUTDATA}/Classification/QB_6_mask.tif
                               -imstat  ${TEMP}/apTvClEstimateImageStatisticsQB456.xml
                               -model   ${TEMP}/clsvmModelQB456.svm
                               -out     ${TEMP}/clLabeledImageQB456_6.tif
                       VALID   --compare-image ${NOTOL}
                               ${OTBAPP_BASELINE}/clLabeledImageQB456_6.tif
                               ${TEMP}/clLabeledImageQB456_6.tif)
  
  SET_TESTS_PROPERTIES(apTvClImageSVMClassifierQB456_6 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB456)
  
  
  OTB_TEST_APPLICATION(NAME apTvClImageSVMClassifierQB456_1
                       APP  ImageClassifier
                       OPTIONS -in      ${INPUTDATA}/Classification/QB_1_ortho.tif
                               -imstat  ${TEMP}/apTvClEstimateImageStatisticsQB456.xml
                               -model   ${TEMP}/clsvmModelQB456.svm
                               -out     ${TEMP}/clLabeledImageQB456_1.tif
                       VALID   --compare-image ${NOTOL}
                               ${OTBAPP_BASELINE}/clLabeledImageQB456_1.tif
                               ${TEMP}/clLabeledImageQB456_1.tif)
  
  SET_TESTS_PROPERTIES(apTvClImageSVMClassifierQB456_1 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB456)
ENDIF()


OTB_TEST_APPLICATION(NAME apTvClKMeansImageClassification
                     APP  KMeansClassification
                     OPTIONS -in ${INPUTDATA}/qb_RoadExtract.img
                             -vm ${INPUTDATA}/qb_RoadExtract_mask.png
                             -ts 30000
                             -nc 5
                             -maxit 10000
                             -ct 0.0000001
                             -rand 121212
                             -out ${TEMP}/apTvClKMeansImageClassificationFilterOuptut.tif
                     VALID   --compare-image ${NOTOL}
                             ${OTBAPP_BASELINE}/apTvClKMeansImageClassificationFilterOuptut.tif
                             ${TEMP}/apTvClKMeansImageClassificationFilterOuptut.tif )


# --- SOMClassification ---
OTB_TEST_APPLICATION(NAME apTvClSOMClassificationSmall
                     APP  SOMClassification
                     OPTIONS -in ${INPUTDATA}/poupees_sub.png
                             -rand 121212
                             -out ${TEMP}/apTvClSOMClassificationSmall.tif uint16
                     VALID   --compare-image ${NOTOL}
                             ${BASELINE}/apTvClSOMClassificationSmall.tif
                             ${TEMP}/apTvClSOMClassificationSmall.tif)


OTB_TEST_APPLICATION(NAME apTvClSOMClassificationFull
                     APP  SOMClassification
                     OPTIONS -in  ${INPUTDATA}/poupees_sub.png
                             -out ${TEMP}/apTvClSOMClassificationFull.tif uint16
                             -vm  ${INPUTDATA}/poupees_sub_c1.png
                             -tp  0.8
                             -ts  13000
                             -sl  32
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


# --- Fusion of classifications ---
OTB_TEST_APPLICATION(NAME apTvFusionOfClassificationsMV2Inputs
                     APP  FusionOfClassifications
                     OPTIONS -il ${OTBAPP_BASELINE}/clLabeledImageQB123_1.tif
                                 ${OTBAPP_BASELINE}/clLabeledImageQB456_1.tif
                             -method majorityvoting
                             -undecidedlabel 100
                             -out ${TEMP}/apTvFusionOfClassificationsMV2InputsOutput.tif uint16
                     VALID   --compare-image ${NOTOL}
                     ${OTBAPP_BASELINE}/apTvFusionOfClassifications2InputsOutput.tif
                     ${TEMP}/apTvFusionOfClassificationsMV2InputsOutput.tif)


OTB_TEST_APPLICATION(NAME apTvFusionOfClassificationsMV3Inputs
                     APP  FusionOfClassifications
                     OPTIONS -il ${OTBAPP_BASELINE}/clLabeledImageQB123_1.tif
                                 ${OTBAPP_BASELINE}/clLabeledImageQB456_1.tif
                                 ${OTBAPP_BASELINE}/clLabeledImageQB1.tif
                             -method majorityvoting
                             -undecidedlabel 100
                             -out ${TEMP}/apTvFusionOfClassificationsMV3InputsOutput.tif uint16
                     VALID   --compare-image ${NOTOL}
                     ${OTBAPP_BASELINE}/apTvFusionOfClassifications3InputsOutput.tif
                     ${TEMP}/apTvFusionOfClassificationsMV3InputsOutput.tif)


OTB_TEST_APPLICATION(NAME apTvFusionOfClassificationsDSPrecision6Inputs
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


OTB_TEST_APPLICATION(NAME apTvFusionOfClassificationsDSRecall6Inputs
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


OTB_TEST_APPLICATION(NAME apTvFusionOfClassificationsDSAccuracy6Inputs
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


OTB_TEST_APPLICATION(NAME apTvFusionOfClassificationsDSKappa6Inputs
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


# --- Compute confusion matrix ---
OTB_TEST_APPLICATION(NAME apTvComputeConfusionMatrixV
                     APP  ComputeConfusionMatrix
                     OPTIONS -in ${OTBAPP_BASELINE}/clLabeledImageQB123_1.tif
                             -ref vector
                             -ref.vector.in ${INPUTDATA}/Classification/VectorData_QB1_ter.shp
                             -ref.vector.field Class
                             -out ${TEMP}/apTvComputeConfusionMatrixTconfusionVOut.csv
                     VALID   --compare-ascii ${NOTOL}
                     ${OTBAPP_BASELINE_FILES}/apTvComputeConfusionMatrixTconfusionVOut.csv
                     ${TEMP}/apTvComputeConfusionMatrixTconfusionVOut.csv)


OTB_TEST_APPLICATION(NAME apTvComputeConfusionMatrixR
                     APP  ComputeConfusionMatrix
                     OPTIONS -in ${OTBAPP_BASELINE}/clLabeledImageQB123_1.tif
                             -ref raster
                             -ref.raster.in ${INPUTDATA}/Classification/clLabeledImageQB456_1_NoData_255.tif
                             -nodatalabel 255
                             -out ${TEMP}/apTvComputeConfusionMatrixTconfusionROut.csv
                     VALID   --compare-ascii ${NOTOL}
                     ${OTBAPP_BASELINE_FILES}/apTvComputeConfusionMatrixTconfusionROut.csv
                     ${TEMP}/apTvComputeConfusionMatrixTconfusionROut.csv)


OTB_TEST_APPLICATION(NAME apTvComputeConfusionMatrixExtraReferenceLabelsV
                     APP  ComputeConfusionMatrix
                     OPTIONS -in ${INPUTDATA}/Classification/QB_1_ortho_C7.tif
                             -ref vector
                             -ref.vector.in ${INPUTDATA}/Classification/VectorData_QB1_ter.shp
                             -ref.vector.field Class
                             -out ${TEMP}/apTvComputeConfusionMatrixExtraRefLabelsVOut.csv
                     VALID   --compare-ascii ${NOTOL}
                     ${OTBAPP_BASELINE_FILES}/apTvComputeConfusionMatrixExtraRefLabelsVOut.csv
                     ${TEMP}/apTvComputeConfusionMatrixExtraRefLabelsVOut.csv)


OTB_TEST_APPLICATION(NAME apTvComputeConfusionMatrixExtraReferenceLabelsR
                     APP  ComputeConfusionMatrix
                     OPTIONS -in ${INPUTDATA}/Classification/QB_1_ortho_C7.tif
                             -ref raster
                             -ref.raster.in ${INPUTDATA}/Classification/clLabeledImageQB456_1_NoData_255.tif
                             -nodatalabel 255
                             -out ${TEMP}/apTvComputeConfusionMatrixExtraRefLabelsROut.csv
                     VALID   --compare-ascii ${NOTOL}
                     ${OTBAPP_BASELINE_FILES}/apTvComputeConfusionMatrixExtraRefLabelsROut.csv
                     ${TEMP}/apTvComputeConfusionMatrixExtraRefLabelsROut.csv)


OTB_TEST_APPLICATION(NAME apTvComputeConfusionMatrixExtraProducedLabelsR
                     APP  ComputeConfusionMatrix
                     OPTIONS -in ${INPUTDATA}/Classification/clLabeledImageQB456_1_NoData_255.tif
                             -ref raster
                             -ref.raster.in ${INPUTDATA}/Classification/QB_1_ortho_C8.tif
                             -nodatalabel 255
                             -out ${TEMP}/apTvComputeConfusionMatrixExtraProdLabelsROut.csv
                     VALID   --compare-ascii ${NOTOL}
                     ${OTBAPP_BASELINE_FILES}/apTvComputeConfusionMatrixExtraProdLabelsROut.csv
                     ${TEMP}/apTvComputeConfusionMatrixExtraProdLabelsROut.csv)


#OTB_TEST_APPLICATION(NAME apTvComputeConfusionMatrixI
#                     APP  ComputeConfusionMatrix
#                     OPTIONS -in ${OTBAPP_BASELINE}/clLabeledImageQB123_1.tif
#                             -ref raster
#                             -ref.raster.in ${OTBAPP_BASELINE}/clLabeledImageQB456_1.tif
#                             -labels 4
#                             -nodata 255
#                             -out ${TEMP}/apTvComputeConfusionMatrixTconfusionROut.txt
#                     VALID   --compare-ascii ${NOTOL}
#                     ${OTBAPP_BASELINE}/apTvComputeConfusionMatrixTconfusionROut.txt
#                     ${TEMP}/apTvComputeConfusionMatrixTconfusionROut.txt)


OTB_TEST_APPLICATION(NAME apTvClassificationMapRegularization
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

