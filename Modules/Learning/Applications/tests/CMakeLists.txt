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


#----------- ComputeImagesStatistics TESTS ----------------
otb_test_application(NAME apTvClComputeImagesStatisticsQB1
  APP  ComputeImagesStatistics
  OPTIONS -il ${INPUTDATA}/Classification/QB_1_ortho.tif
  -out.xml ${TEMP}/apTvClEstimateImageStatisticsQB1.xml
  VALID   --compare-ascii ${NOTOL}
  ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB1.xml
  ${TEMP}/apTvClEstimateImageStatisticsQB1.xml)

otb_test_application(NAME apTvClComputeImagesStatisticsQB456
  APP  ComputeImagesStatistics
  OPTIONS -il ${INPUTDATA}/Classification/QB_4_extract.tif
  ${INPUTDATA}/Classification/QB_5_extract.tif
  ${INPUTDATA}/Classification/QB_6_extract.tif
  -out.xml ${TEMP}/apTvClEstimateImageStatisticsQB456.xml
  VALID   --compare-ascii ${NOTOL}
  ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB456.xml
  ${TEMP}/apTvClEstimateImageStatisticsQB456.xml)

otb_test_application(NAME apTvClComputeImagesStatisticsQB123
  APP  ComputeImagesStatistics
  OPTIONS -il ${INPUTDATA}/Classification/QB_1_ortho.tif
  ${INPUTDATA}/Classification/QB_2_ortho.tif
  ${INPUTDATA}/Classification/QB_3_ortho.tif
  -out.xml ${TEMP}/apTvClEstimateImageStatisticsQB123.xml
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
