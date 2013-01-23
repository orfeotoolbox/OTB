
IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


SET(BASICFILTERS_TESTS1 ${CXX_TEST_PATH}/otbBasicFiltersTests1)
SET(BASICFILTERS_TESTS2 ${CXX_TEST_PATH}/otbBasicFiltersTests2)
SET(BASICFILTERS_TESTS3 ${CXX_TEST_PATH}/otbBasicFiltersTests3)
SET(BASICFILTERS_TESTS4 ${CXX_TEST_PATH}/otbBasicFiltersTests4)
SET(BASICFILTERS_TESTS5 ${CXX_TEST_PATH}/otbBasicFiltersTests5)
SET(BASICFILTERS_TESTS6 ${CXX_TEST_PATH}/otbBasicFiltersTests6)
SET(BASICFILTERS_TESTS7 ${CXX_TEST_PATH}/otbBasicFiltersTests7)
SET(BASICFILTERS_TESTS8 ${CXX_TEST_PATH}/otbBasicFiltersTests8)
SET(BASICFILTERS_TESTS9 ${CXX_TEST_PATH}/otbBasicFiltersTests9)
SET(BASICFILTERS_TESTS10 ${CXX_TEST_PATH}/otbBasicFiltersTests10)
SET(BASICFILTERS_TESTS11 ${CXX_TEST_PATH}/otbBasicFiltersTests11)
SET(BASICFILTERS_TESTS12 ${CXX_TEST_PATH}/otbBasicFiltersTests12)
SET(BASICFILTERS_TESTS13 ${CXX_TEST_PATH}/otbBasicFiltersTests13)
SET(BASICFILTERS_TESTS14 ${CXX_TEST_PATH}/otbBasicFiltersTests14)
SET(BASICFILTERS_TESTS15 ${CXX_TEST_PATH}/otbBasicFiltersTests15)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbBasicFiltersTests1 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ADD_TEST(bfTvFiltreLee1CanalPoupees ${BASICFILTERS_TESTS1}
  --compare-image ${EPSILON_7}  ${BASELINE}/bfFiltreLee_05_05_04.tif
                          ${TEMP}/bfFiltreLee_05_05_04.tif
        otbLeeFilter
        ${INPUTDATA}/GomaAvant.png                  #poupees_1canal.c1.hdr
        ${TEMP}/bfFiltreLee_05_05_04.tif
        05 05 4.0)

ADD_TEST(bfTvFiltreLee ${BASICFILTERS_TESTS1}
  --compare-image ${EPSILON_7}  ${BASELINE}/bfFiltreLee_05_05_12.tif
                           ${TEMP}/bfFiltreLee_05_05_12.tif
        otbLeeFilter
        ${INPUTDATA}/GomaAvant.png    #poupees.hdr
        ${TEMP}/bfFiltreLee_05_05_12.tif
        05 05 12.0)

#
# Filtre de FROST:
#
ADD_TEST(bfTuFrostFilterNew ${BASICFILTERS_TESTS1}
         otbFrostFilterNew)

ADD_TEST(bfTvFrostFilter ${BASICFILTERS_TESTS1}
  --compare-image ${EPSILON_7}  ${BASELINE}/bfFiltreFrost_poupees_05_05_01.tif
                          ${TEMP}/bfFiltreFrost_poupees_05_05_01.tif
        otbFrostFilter
        ${INPUTDATA}/GomaAvant.png   #poupees.hdr
        ${TEMP}/bfFiltreFrost_poupees_05_05_01.tif
        05 05 0.1)

# -------            otb::PointSetFilter   ------------------------------

ADD_TEST(bfTuImageToPointSetFilterTest ${BASICFILTERS_TESTS1}
         otbImageToPointSetFilterTest)


# -------            otb::OpeningClosingMorphologicalFilter   ------------------------------

ADD_TEST(bfTuOpeningClosingFilterNew ${BASICFILTERS_TESTS1}
        otbOpeningClosingMorphologicalFilterNew )

ADD_TEST(bfTvOpeningClosingFilter ${BASICFILTERS_TESTS1}
   --compare-image ${NOTOL}  ${BASELINE}/bfFiltreOpeningClosing_1.tif
                           ${TEMP}/bfFiltreOpeningClosing_1.tif
        otbOpeningClosingMorphologicalFilter
        ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
        ${TEMP}/bfFiltreOpeningClosing_1.tif
        1)

# -------            otb::ClosingOpeningMorphologicalFilter   ------------------------------

ADD_TEST(bfTuClosingOpeningFilterNew ${BASICFILTERS_TESTS1}
        otbClosingOpeningMorphologicalFilterNew )

ADD_TEST(bfTvClosingOpeningFilter ${BASICFILTERS_TESTS1}
   --compare-image ${NOTOL}  ${BASELINE}/bfFiltreClosingOpening_1.tif
                           ${TEMP}/bfFiltreClosingOpening_1.tif
        otbClosingOpeningMorphologicalFilter
        ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
        ${TEMP}/bfFiltreClosingOpening_1.tif
        1)


# -------            otb::BinaryImageMinimalBoundingRegionCalculator   ----------------------------

ADD_TEST(bfTuBoundingRegionCalculatorNew ${BASICFILTERS_TESTS1}
         otbBinaryImageMinimalBoundingRegionCalculatorNew)



ADD_TEST(bfTvBoundingRegionCalculator ${BASICFILTERS_TESTS1}
   --compare-ascii ${NOTOL}
                    ${BASELINE_FILES}/bfBoundingRegionCalculatorOutput.txt
                    ${TEMP}/bfBoundingRegionCalculatorOutput.txt
        otbBinaryImageMinimalBoundingRegionCalculator
        4
        ${TEMP}/bfBoundingRegionCalculatorOutput.txt
        ${INPUTDATA}/rcc8_mire1.png
        ${INPUTDATA}/rcc8_mire2.png
        ${INPUTDATA}/rcc8_mire3.png
        ${INPUTDATA}/rcc8_mire4.png
        )


ADD_TEST(bfTvMeanFunctorImageTest ${BASICFILTERS_TESTS1}
  --compare-image ${NOTOL}  ${BASELINE}/bfMeanFunctorImage.tif
                           ${TEMP}/bfMeanFunctorImage.tif
  otbMeanFunctorImageTest
  ${INPUTDATA}/couleurs_extrait.png
  ${TEMP}/bfMeanFunctorImage.tif
  )


ADD_TEST(bfTvTransformPointSetFilter ${BASICFILTERS_TESTS1}
  otbTransformPointSetFilterTest)

ADD_TEST(bfTvRandomPointSetSource ${BASICFILTERS_TESTS1}
  otbRandomPointSetSourceTest)

ADD_TEST(bfTvPointSetExtractROI ${BASICFILTERS_TESTS1}
  otbPointSetExtractROITest)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbBasicFiltersTests2 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::LogPolarTransform   ----------------------------

ADD_TEST(bfTuLogPolarTransformNew ${BASICFILTERS_TESTS2}
         otbLogPolarTransformNew)

ADD_TEST(bfTvLogPolarTransform ${BASICFILTERS_TESTS2}
   --compare-ascii ${NOTOL}
                    ${BASELINE_FILES}/bfLogPolarTransformResults.txt
                    ${TEMP}/bfLogPolarTransformResults.txt
        otbLogPolarTransform
        1 1
        ${TEMP}/bfLogPolarTransformResults.txt
        5 0 0 0 10 45 0 3.14 3.14 18 1
        )

ADD_TEST(bfTvLogPolarTransformResample ${BASICFILTERS_TESTS2}
  --compare-image ${EPSILON_7}  ${BASELINE}/bfLogPolarTransformResampleOutput.hdr
                          ${TEMP}/bfLogPolarTransformResampleOutput.hdr
        otbLogPolarTransformResample
        ${INPUTDATA}/circle.hdr
        ${TEMP}/bfLogPolarTransformResampleOutput.hdr
)


# -------            otb::InverseLogPolarTransform   ----------------------------

ADD_TEST(bfTuInverseLogPolarTransformNew ${BASICFILTERS_TESTS2}
         otbInverseLogPolarTransformNew)

ADD_TEST(bfTvInverseLogPolarTransform ${BASICFILTERS_TESTS2}
     --compare-ascii ${NOTOL}
                     ${BASELINE_FILES}/bfInverseLogPolarTransformResults.txt
                     ${TEMP}/bfInverseLogPolarTransformResults.txt
        otbInverseLogPolarTransform
        1. 1.
        ${TEMP}/bfInverseLogPolarTransformResults.txt
        6 0 0 10 20 45 60 3.14 3.14 1 0 0 1
        )

ADD_TEST(bfTvInverseLogPolarTransformResample ${BASICFILTERS_TESTS2}
--compare-image ${NOTOL}  ${BASELINE}/bfInverseLogPolarTransformResampleOutput.hdr
                          ${TEMP}/bfInverseLogPolarTransformResampleOutput.hdr
        otbInverseLogPolarTransformResample
        ${BASELINE}/bfLogPolarTransformResampleOutput.hdr
        ${TEMP}/bfInverseLogPolarTransformResampleOutput.hdr
)

# -------            otb::StreamingShrinkImageFilter   ----------------------------

  ADD_TEST(bfTuStreamingShrinkImageFilterNew ${BASICFILTERS_TESTS2}
         otbStreamingShrinkImageFilterNew)

IF(OTB_DATA_USE_LARGEINPUT)

 ADD_TEST(bfTvStreamingShrinkImageFilterQBPAN ${BASICFILTERS_TESTS2}
--compare-image ${NOTOL}
                ${BASELINE}/bfTvStreamingShrinkImageFilterQBPANOutput.hdr
                ${TEMP}/bfTvStreamingShrinkImageFilterQBPANOutput.hdr
        otbStreamingShrinkImageFilter
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        ${TEMP}/bfTvStreamingShrinkImageFilterQBPANOutput.hdr
        50
)

ADD_TEST(bfTvStreamingShrinkImageFilterQBMUL ${BASICFILTERS_TESTS2}
--compare-image ${NOTOL}
                ${BASELINE}/bfTvStreamingShrinkImageFilterQBMULOutput.hdr
                ${TEMP}/bfTvStreamingShrinkImageFilterQBMULOutput.hdr
                otbStreamingShrinkImageFilter
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
        ${TEMP}/bfTvStreamingShrinkImageFilterQBMULOutput.hdr
        20
)
ENDIF(OTB_DATA_USE_LARGEINPUT)

# -------            otb::SpatialObjectToImageDrawingFilter   ----------------------------

  ADD_TEST(bfTuSpatialObjectToImageDrawingFilterNew ${BASICFILTERS_TESTS2}
         otbSpatialObjectToImageDrawingFilterNew)


 ADD_TEST(bfTvSpatialObjectToImageDrawingFilterWithoutInputImage ${BASICFILTERS_TESTS2}
 --compare-image ${NOTOL}
                ${BASELINE}/bfTvSpatialObjectDrawingNoInput.png
                ${TEMP}/bfTvSpatialObjectDrawingNoInput.png
        otbSpatialObjectToImageDrawingFilter
        ${TEMP}/bfTvSpatialObjectDrawingNoInput.png
        100
        50
)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbBasicFiltersTests3 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# -------            otb::ImportGeoInformationImageFilter   ----------------------------

  ADD_TEST(bfTuImportGeoInformationImageFilterNew ${BASICFILTERS_TESTS3}
         otbImportGeoInformationImageFilterNew)


 ADD_TEST(bfTvImportGeoInformationImageFilter ${BASICFILTERS_TESTS3}
 --compare-metadata ${NOTOL}
                    ${INPUTDATA}/HFAGeoreferenced.img
                    ${TEMP}/bfTvImportGeoInformationOutput.img
        otbImportGeoInformationImageFilter
                    ${INPUTDATA}/HFAGeoreferenced.img
                    ${TEMP}/bfTvImportGeoInformationOutput.img
)


IF(OTB_DATA_USE_LARGEINPUT)
 ADD_TEST(bfTvImportGeoInformationImageFilterWithKeywordList ${BASICFILTERS_TESTS3}
--compare-ascii ${EPSILON_7}
                ${BASELINE_FILES}/bfTvImportGeoInformationKeywordListOutput.txt
                ${TEMP}/bfTvImportGeoInformationKeywordListOutput.txt
         otbImportGeoInformationImageFilterWithKeywordList
         ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
         ${TEMP}/bfTvImportGeoInformationKeywordListOutput.txt
)
ENDIF(OTB_DATA_USE_LARGEINPUT)

# -------            otb::SpectralAngleDistanceImageFilter   ----------------------------

  ADD_TEST(bfTuSpectralAngleDistanceImageFilterNew ${BASICFILTERS_TESTS3}
         otbSpectralAngleDistanceImageFilterNew)


 ADD_TEST(bfTvSpectralAngleDistanceImageFilter ${BASICFILTERS_TESTS3}
                --compare-image ${EPSILON_7}
                    ${BASELINE}/bfTvSpectralAngleDistanceOutput.hdr
                    ${TEMP}/bfTvSpectralAngleDistanceOutput.hdr
                    otbSpectralAngleDistanceImageFilter
                    ${INPUTDATA}/qb_RoadExtract.img.hdr
                    ${TEMP}/bfTvSpectralAngleDistanceOutput.hdr
                    337 557 432 859
)

 ADD_TEST(bfTvSpectralAngleDistanceImageFilterOneChannel ${BASICFILTERS_TESTS3}
                    otbSpectralAngleDistanceImageFilter
                    ${INPUTDATA}/DeuxCercles.hdr
                    ${TEMP}/bfTvSpectralAngleDistanceOneChannelOutput.hdr
                    100
)

SET_TESTS_PROPERTIES(bfTvSpectralAngleDistanceImageFilterOneChannel PROPERTIES WILL_FAIL true)

# -------            otb::VectorRescaleIntensityImageFilter   ----------------------------

ADD_TEST(bfTuVectorRescaleIntensityImageFilterNew ${BASICFILTERS_TESTS3}
         otbVectorRescaleIntensityImageFilterNew)


ADD_TEST(bfTvVectorRescaleIntensityImageFilter ${BASICFILTERS_TESTS3}
                --compare-image ${EPSILON_7}
                    ${BASELINE}/bfTvVectorRescaleIntensityOutput.png
                    ${TEMP}/bfTvVectorRescaleIntensityOutput.png
                    otbVectorRescaleIntensityImageFilter
                    ${INPUTDATA}/qb_RoadExtract.img.hdr
                    ${TEMP}/bfTvVectorRescaleIntensityOutput.png
                    0 255
)

# -------            otb::ShiftScaleVectorImageFilter   ----------------------------

ADD_TEST(bfTuShiftScaleVectorImageFilterNew ${BASICFILTERS_TESTS3}
         otbShiftScaleVectorImageFilterNew)


ADD_TEST(bfTvShiftScaleVectorImageFilter ${BASICFILTERS_TESTS3}
                --compare-image ${EPSILON_7}
                    ${BASELINE}/bfTvShiftScaleVImageOutput.tif
                    ${TEMP}/bfTvShiftScaleVImageOutput.tif
                otbShiftScaleVectorImageFilterTest
                    ${INPUTDATA}/qb_RoadExtract.img.hdr
                ${TEMP}/bfTvShiftScaleVImageOutput.tif
)


# -------            otb::VectorImageToImageListFilter   ----------------------------

ADD_TEST(bfTuVectorImageToImageListFilterNew ${BASICFILTERS_TESTS3}
         otbVectorImageToImageListFilterNew)


ADD_TEST(bfTvVectorImageToImageListFilter ${BASICFILTERS_TESTS3}
                --compare-n-images ${EPSILON_7} 3
                    ${BASELINE}/bfTvVectorImageToImageListFilterBand1.png
                    ${TEMP}/bfTvVectorImageToImageListFilterBand1.png
                    ${BASELINE}/bfTvVectorImageToImageListFilterBand2.png
                    ${TEMP}/bfTvVectorImageToImageListFilterBand2.png
                    ${BASELINE}/bfTvVectorImageToImageListFilterBand3.png
                    ${TEMP}/bfTvVectorImageToImageListFilterBand3.png
                    otbVectorImageToImageListFilter
                    ${INPUTDATA}/poupees.png
                    ${TEMP}/bfTvVectorImageToImageListFilterBand1.png
                    ${TEMP}/bfTvVectorImageToImageListFilterBand2.png
                    ${TEMP}/bfTvVectorImageToImageListFilterBand3.png
)

ADD_TEST(bfTvVectorImageToImageListFilterIt ${BASICFILTERS_TESTS3}
                --compare-n-images ${EPSILON_7} 3
                    ${BASELINE}/bfTvVectorImageToImageListFilterBand1.png
                    ${TEMP}/bfTvVectorImageToImageListFilterItBand1.png
                    ${BASELINE}/bfTvVectorImageToImageListFilterBand2.png
                    ${TEMP}/bfTvVectorImageToImageListFilterItBand2.png
                    ${BASELINE}/bfTvVectorImageToImageListFilterBand3.png
                    ${TEMP}/bfTvVectorImageToImageListFilterItBand3.png
                    otbVectorImageToImageListFilterIterator
                    ${INPUTDATA}/poupees.png
                    ${TEMP}/bfTvVectorImageToImageListFilterIt
                    png
)
ADD_TEST(bfTvVectorImageToImageListFilterIt2 ${BASICFILTERS_TESTS3}
                --compare-n-images ${EPSILON_7} 3
                    ${BASELINE}/bfTvVectorImageToImageListFilterBand1.png
                    ${TEMP}/bfTvVectorImageToImageListFilterIt2Band1.tif
                    ${BASELINE}/bfTvVectorImageToImageListFilterBand2.png
                    ${TEMP}/bfTvVectorImageToImageListFilterIt2Band2.tif
                    ${BASELINE}/bfTvVectorImageToImageListFilterBand3.png
                    ${TEMP}/bfTvVectorImageToImageListFilterIt2Band3.tif
                    otbVectorImageToImageListFilterIterator
                    ${INPUTDATA}/poupees.png
                    ${TEMP}/bfTvVectorImageToImageListFilterIt2
                    tif
)

ADD_TEST(bfTuImageListToSingleImageFilterNew ${BASICFILTERS_TESTS3}
         otbImageListToSingleImageFilterNew)

ADD_TEST(bfTvImageListToSingleImageFilter ${BASICFILTERS_TESTS3}
                --compare-image ${NOTOL}
                    ${BASELINE}/bfTvImageListToSingleImageFilter.tif
                    ${TEMP}/bfTvImageListToSingleImageFilterOut.tif
                    otbImageListToSingleImageFilter
                    ${INPUTDATA}/poupees.png
                    ${TEMP}/bfTvImageListToSingleImageFilterOut.tif
)

#IF(OTB_DATA_USE_LARGEINPUT)
#    ADD_TEST(bfTvImageListToSingleImageFilterLarge ${BASICFILTERS_TESTS3}
#                        otbImageListToSingleImageFilter
#                        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
#                        ${TEMP}/bfTvImageListToSingleImageFilterLargeOut.tif
#    )
#ENDIF()

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbBasicFiltersTests4 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# -------            otb::ImageListToImageListApplyFilter   ----------------------------

ADD_TEST(bfTuImageListToImageListApplyFilterNew ${BASICFILTERS_TESTS4}
         otbImageListToImageListApplyFilterNew)


ADD_TEST(bfTvImageListToImageListApplyFilter ${BASICFILTERS_TESTS4}
                --compare-n-images ${EPSILON_7} 3
                    ${BASELINE}/bfTvImageListToImageListApplyFilterBand1.png
                    ${TEMP}/bfTvImageListToImageListApplyFilterBand1.png
                    ${BASELINE}/bfTvImageListToImageListApplyFilterBand2.png
                    ${TEMP}/bfTvImageListToImageListApplyFilterBand2.png
                    ${BASELINE}/bfTvImageListToImageListApplyFilterBand3.png
                    ${TEMP}/bfTvImageListToImageListApplyFilterBand3.png
                    otbImageListToImageListApplyFilter
              ${INPUTDATA}/poupees_c1.hdr
                    ${INPUTDATA}/poupees_c2.hdr
                    ${INPUTDATA}/poupees_c3.hdr
                    ${TEMP}/bfTvImageListToImageListApplyFilterBand1.png
                    ${TEMP}/bfTvImageListToImageListApplyFilterBand2.png
                    ${TEMP}/bfTvImageListToImageListApplyFilterBand3.png
)

# -------            otb::PerBandVectorImageFilter   ----------------------------

ADD_TEST(bfTuPerBandVectorImageFilterNew ${BASICFILTERS_TESTS4}
         otbPerBandVectorImageFilterNew)

ADD_TEST(bfTvPerBandVectorImageFilterWithMeanFilter ${BASICFILTERS_TESTS4}
        --compare-image ${EPSILON_7}
                        ${BASELINE}/bfTvPerBandVectorImageFilterWithMeanFilterOutput.hdr
                        ${TEMP}/bfTvPerBandVectorImageFilterWithMeanFilterOutput.hdr
        otbPerBandVectorImageFilterWithMeanFilter
                        ${INPUTDATA}/poupees.png
                        ${TEMP}/bfTvPerBandVectorImageFilterWithMeanFilterOutput.hdr
)

ADD_TEST(bfTvPerBandVectorImageFilterWithSobelFilter ${BASICFILTERS_TESTS4}
        --compare-image ${EPSILON_7}
                        ${BASELINE}/bfTvPerBandVectorImageFilterWithSobelFilterOutput.hdr
                        ${TEMP}/bfTvPerBandVectorImageFilterWithSobelFilterOutput.hdr
        otbPerBandVectorImageFilterWithSobelFilter
                        ${INPUTDATA}/poupees.png
                        ${TEMP}/bfTvPerBandVectorImageFilterWithSobelFilterOutput.hdr
)


# -------            otb::StreamingStatisticsImageFilter   ----------------------------

ADD_TEST(bfTuStreamingStatisticsImageFilterNew ${BASICFILTERS_TESTS4}
         otbStreamingStatisticsImageFilterNew)

ADD_TEST(bfTvStreamingStatisticsImageFilter ${BASICFILTERS_TESTS4}
        --compare-ascii ${NOTOL}
                    ${BASELINE_FILES}/bfStreamingStatisticsImageFilterResults.txt
                    ${TEMP}/bfStreamingStatisticsImageFilterResults.txt
         otbStreamingStatisticsImageFilter
#                ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
                ${INPUTDATA}/small_poupees_1canal.hd
                ${TEMP}/bfStreamingStatisticsImageFilterResults.txt)

# -------            otb::StreamingCompareImageFilter   ----------------------------

ADD_TEST(bfTuStreamingCompareImageFilterNew ${BASICFILTERS_TESTS4}
         otbStreamingCompareImageFilterNew)

ADD_TEST(bfTvStreamingCompareImageFilter ${BASICFILTERS_TESTS4}
        --compare-ascii ${NOTOL}
                    ${BASELINE_FILES}/bfStreamingCompareImageFilterResults.txt
                    ${TEMP}/bfStreamingCompareImageFilterResults.txt
         otbStreamingCompareImageFilter
                ${INPUTDATA}/small_poupees_1canal.hd
                ${TEMP}/bfStreamingCompareImageFilterResults.txt)

# -------            otb::VectorImageTo3DScalarImageFilter   ----------------------------

ADD_TEST(bfTuVectorImageTo3DScalarImageFilterNew ${BASICFILTERS_TESTS4}
         otbVectorImageTo3DScalarImageFilterNew)

ADD_TEST(bfTvVectorImageTo3DScalarImageFilter ${BASICFILTERS_TESTS4}
        --compare-image ${NOTOL}
                    ${BASELINE}/bfTvVectorImageTo3DScalarImageFilterOutput.png
                    ${TEMP}/bfTvVectorImageTo3DScalarImageFilterOutput.png
         otbVectorImageTo3DScalarImageFilter
                ${INPUTDATA}/poupees.png
                ${TEMP}/bfTvVectorImageTo3DScalarImageFilterOutput.png
)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbBasicFiltersTests5 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::StreamingResampleImageFilter   ----------------------------

ADD_TEST(bfTuStreamingTraitsImage ${BASICFILTERS_TESTS5}
  otbStreamingTraitsImage
  )

ADD_TEST(bfTuStreamingTraitsVectorImage ${BASICFILTERS_TESTS5}
  otbStreamingTraitsVectorImage
  )

ADD_TEST(bfTuStreamingResampleImageFilterWithAffineTransformNew ${BASICFILTERS_TESTS5}
  otbStreamingResampleImageFilterWithAffineTransform
  )

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(bfTvStreamingResampleImageFilterWithAffineTransform ${BASICFILTERS_TESTS5}
  otbStreamingResampleImageFilterWithAffineTransform
  ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
  500
  ${TEMP}/bfTvotbStreamingResampledImageWithAffineTransform.tif
  )
ENDIF(OTB_DATA_USE_LARGEINPUT)


ADD_TEST(bfTuStreamingResampleImageFilterNew ${BASICFILTERS_TESTS5}
         otbStreamingResampleImageFilterNew)

ADD_TEST(bfTvStreamingResampleImageFilter ${BASICFILTERS_TESTS5}
          --compare-image ${NOTOL}
              ${BASELINE}/bfTvStreamingResamplePoupeesTest.tif
              ${TEMP}/bfTvStreamingResamplePoupeesTest.tif
          otbStreamingResampleImageFilter
              ${INPUTDATA}/poupees.tif
              ${TEMP}/bfTvStreamingResamplePoupeesTest.tif
)

ADD_TEST(bfTvStreamingResampleImageFilterCompareWithITK ${BASICFILTERS_TESTS5}
                --compare-image ${NOTOL}
                                        ${TEMP}/bfTvStreamingResamplePoupeesTestITK.tif
                                        ${TEMP}/bfTvStreamingResamplePoupeesTestOTB.tif
                                otbStreamingResampleImageFilterCompareWithITK
                                        ${INPUTDATA}/poupees.tif
                                        3000 3000
                                        ${TEMP}/bfTvStreamingResamplePoupeesTestITK.tif
                                        ${TEMP}/bfTvStreamingResamplePoupeesTestOTB.tif
)

#ADD_TEST(StreamingResampleImageFilterCompareWithITK ${BASICFILTERS_TESTS5}
#                --compare-image ${NOTOL}
#                                        ${TEMP}/bfTvStreamingResampleSPOT5TestITK.tif
#                                        ${TEMP}/bfTvStreamingResampleSPOT5TestOTB.tif
#                                otbStreamingResampleImageFilterCompareWithITK
#                                        ${INPUTDATA}/spot5SubWithGcps.tif
#                                        1100 1100
#                                        ${TEMP}/bfTvStreamingResampleSPOT5TestITK.tif
#                                        ${TEMP}/bfTvStreamingResampleSPOT5TestOTB.tif
#)

# -------            otb::StreamingMinMaxImageFilter   ----------------------------

ADD_TEST(bfTuStreamingMinMaxImageFilterNew ${BASICFILTERS_TESTS5}
         otbStreamingMinMaxImageFilterNew)

ADD_TEST(bfTvStreamingMinMaxImageFilter ${BASICFILTERS_TESTS5}
    --compare-ascii ${NOTOL}
           ${BASELINE_FILES}/bfTvStreamingMinMaxImageFilterResults.txt
           ${TEMP}/bfTvStreamingMinMaxImageFilterResults.txt
         otbStreamingMinMaxImageFilter
        ${INPUTDATA}/couleurs_extrait.png
        ${TEMP}/bfTvStreamingMinMaxImageFilterResults.txt
        )

# -------            otb::StreamingMinMaxVectorImageFilter   ----------------------------

ADD_TEST(bfTuStreamingMinMaxVectorImageFilterNew ${BASICFILTERS_TESTS5}
         otbStreamingMinMaxVectorImageFilterNew)

ADD_TEST(bfTvStreamingMinMaxVectorImageFilter ${BASICFILTERS_TESTS5}
    --compare-ascii ${NOTOL}
           ${BASELINE_FILES}/bfTvStreamingMinMaxVectorImageFilterResults.txt
           ${TEMP}/bfTvStreamingMinMaxVectorImageFilterResults.txt
         otbStreamingMinMaxVectorImageFilter
        ${INPUTDATA}/couleurs_extrait.png
        ${TEMP}/bfTvStreamingMinMaxVectorImageFilterResults.txt
        )

# -------            otb::StreamingStatisticsVectorImageFilter   ----------------------------

ADD_TEST(bfTuStreamingStatisticsVectorImageFilterNew ${BASICFILTERS_TESTS5}
         otbStreamingStatisticsVectorImageFilterNew)

ADD_TEST(bfTvStreamingStatisticsVectorImageFilter ${BASICFILTERS_TESTS5}
        --compare-ascii ${NOTOL}
                    ${BASELINE_FILES}/bfTvStreamingStatisticsVectorImageFilterResults.txt
                    ${TEMP}/bfTvStreamingStatisticsVectorImageFilterResults.txt
         otbStreamingStatisticsVectorImageFilter
                #${LARGEINPUT}/SPOT5_SCENE01/IMAGERY.TIF
                ${INPUTDATA}/couleurs_extrait.png
                ${TEMP}/bfTvStreamingStatisticsVectorImageFilterResults.txt
                )

# -------            otb::MatrixTransposeMatrixImageFilter   ----------------------------

ADD_TEST(bfTuMatrixTransposeMatrixImageFilterNew ${BASICFILTERS_TESTS5}
         otbMatrixTransposeMatrixImageFilterNew)

ADD_TEST(bfTvMatrixTransposeMatrixImageFilter ${BASICFILTERS_TESTS5}
        --compare-ascii ${NOTOL}
                    ${BASELINE_FILES}/bfTvMatrixTransposeMatrixImageFilterResults.txt
                    ${TEMP}/bfTvMatrixTransposeMatrixImageFilterResults.txt
         otbMatrixTransposeMatrixImageFilter
                ${INPUTDATA}/multiSpect.tif
                ${INPUTDATA}/multiSpectInterp.tif
                ${TEMP}/bfTvMatrixTransposeMatrixImageFilterResults.txt
                )


# -------            otb::UnaryImageFunctorWithVectorImageFilter   ----------------------------

ADD_TEST(bfTuUnaryImageFunctorWithVectorImageFilterNew ${BASICFILTERS_TESTS5}
         otbUnaryImageFunctorWithVectorImageFilterNew)

ADD_TEST(bfTvUnaryImageFunctorWithVectorImageFilter ${BASICFILTERS_TESTS5}
     --compare-image ${NOTOL}
                          ${BASELINE}/bfTvUnaryImageFunctorWithVectorImageFilter.tif
                     ${TEMP}/bfTvUnaryImageFunctorWithVectorImageFilter.tif
         otbUnaryImageFunctorWithVectorImageFilter
                ${INPUTDATA}/poupees_sub.png
                ${TEMP}/bfTvUnaryImageFunctorWithVectorImageFilter.tif
                )

# -------            otb::UnaryFunctorImageFilter   ----------------------------

ADD_TEST(bfTuUnaryFunctorImageFilterNew ${BASICFILTERS_TESTS5}
         otbUnaryFunctorImageFilterNew)

# -------            otb::PrintableImageFilter   ------------------------------

ADD_TEST(bfTuPrintableImageFilterNew ${BASICFILTERS_TESTS5}
         otbPrintableImageFilterNew)

ADD_TEST(bfTvPrintableImageFilter ${BASICFILTERS_TESTS5}
     --compare-image ${NOTOL}
                          ${BASELINE}/bfTvPrintableImageFilter.png
                     ${TEMP}/bfTvPrintableImageFilter.png
         otbPrintableImageFilter
                ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                ${TEMP}/bfTvPrintableImageFilter.png
                )

ADD_TEST(bfTvPrintableImageFilterWithMask ${BASICFILTERS_TESTS5}
     --compare-image ${NOTOL}
                          ${BASELINE}/bfTvPrintableImageFilterWithMask.png
                     ${TEMP}/bfTvPrintableImageFilterWithMask.png
         otbPrintableImageFilterWithMask
                ${INPUTDATA}/poupees.tif
                ${INPUTDATA}/carre.png
                ${TEMP}/bfTvPrintableImageFilterWithMask.png
                0 # background
                )

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbBasicFiltersTests6 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::BSplineDecompositionImageFilter  ------------------------------

ADD_TEST(bfTuBSplineDecompositionImageFilterNew ${BASICFILTERS_TESTS6}
         otbBSplineDecompositionImageFilterNew)

ADD_TEST(bfTvBSplineDecompositionImageFilter ${BASICFILTERS_TESTS6}
     --compare-image ${EPSILON_7}
                          ${BASELINE}/bfBSplineDecompositionImageFilterOutput.tif
                     ${TEMP}/bfBSplineDecompositionImageFilterOutput.tif
         otbBSplineDecompositionImageFilter
                ${INPUTDATA}/poupees.tif
                ${TEMP}/bfBSplineDecompositionImageFilterOutput.tif
                )

# -------            otb::BSplineInterpolateImageFunction  ------------------------------

ADD_TEST(bfTuBSplineInterpolateImageFunctionNew ${BASICFILTERS_TESTS6}
         otbBSplineInterpolateImageFunctionNew)

ADD_TEST(bfTvBSplineInterpolateImageFunction ${BASICFILTERS_TESTS6}
     --compare-ascii ${EPSILON_7}
                          ${BASELINE_FILES}/bfBSplineInterpolateImageFunctionOutput.txt
                     ${TEMP}/bfBSplineInterpolateImageFunctionOutput.txt
        otbBSplineInterpolateImageFunction
                ${INPUTDATA}/poupees.tif
                ${TEMP}/bfBSplineInterpolateImageFunctionOutput.txt
                0.5 0.5
                127.33 44.9
                259.67 21.43
                12.13 61.79
                89.5 11
                128 128
                127.255 128.73
                -1 -1
                )

# -------            otb::ProlateInterpolateImageFunction  ------------------------------

ADD_TEST(bfTuProlateInterpolateImageFunctionNew ${BASICFILTERS_TESTS6}
         otbProlateInterpolateImageFunctionNew)

ADD_TEST(bfTvProlateInterpolateImageFunction ${BASICFILTERS_TESTS6}
     --compare-ascii ${NOTOL}
                          ${BASELINE_FILES}/bfProlateInterpolateImageFunctionOutput.txt
                     ${TEMP}/bfProlateInterpolateImageFunctionOutput.txt
        otbProlateInterpolateImageFunction
                ${INPUTDATA}/poupees.tif
                ${TEMP}/bfProlateInterpolateImageFunctionOutput.txt
                ${TEMP}/bfProlateInterpolateImageFunctionCosOutput.tif
                ${TEMP}/bfProlateInterpolateImageFunctionITKCosOutput.tif
                ${TEMP}/bfProlateInterpolateImageFunctionProOutput.tif
                3 # radius
                0.5 0.5
                127.33 44.9
                259.67 21.43
                12.13 61.79
                89.5 11
                128 128
                127.255 128.73
                -1 -1
                )

# -------            otb::WindowedSincInterpolateImageFunctionBase  ------------------------------
ADD_TEST(bfTuWindowedSincInterpolateImageFunctionBaseNew ${BASICFILTERS_TESTS6}
         otbWindowedSincInterpolateImageFunctionBaseNew)

# -------            otb::WindowedSincInterpolateImageGaussianFunction  ------------------------------
 ADD_TEST(bfTuWindowedSincInterpolateImageGaussianFunctionNew ${BASICFILTERS_TESTS6}
         otbWindowedSincInterpolateImageGaussianFunctionNew)

ADD_TEST(bfTvWindowedSincInterpolateImageGaussianFunction ${BASICFILTERS_TESTS6}
     --compare-ascii ${NOTOL}
                          ${BASELINE_FILES}/bfWindowedSincInterpolateImageGaussianFunctionOutput.txt
                     ${TEMP}/bfWindowedSincInterpolateImageGaussianFunctionOutput.txt
        otbWindowedSincInterpolateImageGaussianFunction
                ${INPUTDATA}/poupees.tif
                ${TEMP}/bfWindowedSincInterpolateImageGaussianFunctionOutput.txt
                3 # radius
                0.5 0.5
                127.33 44.9
                259.67 21.43
                12.13 61.79
                89.5 11
                128 128
                127.255 128.73
                -1 -1
                )


# -------            otb::WindowedSincInterpolateImageHammingFunction  ------------------------------
 ADD_TEST(bfTuWindowedSincInterpolateImageHammingFunctionNew ${BASICFILTERS_TESTS6}
         otbWindowedSincInterpolateImageHammingFunctionNew)

ADD_TEST(bfTvWindowedSincInterpolateImageHammingFunction ${BASICFILTERS_TESTS6}
     --compare-ascii ${NOTOL}
                          ${BASELINE_FILES}/bfWindowedSincInterpolateImageHammingFunctionOutput.txt
                     ${TEMP}/bfWindowedSincInterpolateImageHammingFunctionOutput.txt
        otbWindowedSincInterpolateImageHammingFunction
                ${INPUTDATA}/poupees.tif
                ${TEMP}/bfWindowedSincInterpolateImageHammingFunctionOutput.txt
                3 # radius
                0.5 0.5
                127.33 44.9
                259.67 21.43
                12.13 61.79
                89.5 11
                128 128
                127.255 128.73
                -1 -1
                )

# -------            otb::BCOInterpolateImageFunction  ------------------------------

ADD_TEST(bfTuBCOInterpolateImageFunctionNew ${BASICFILTERS_TESTS6}
  otbBCOInterpolateImageFunctionNew
  )

ADD_TEST(bfTvBCOInterpolateImageFunction ${BASICFILTERS_TESTS6}
--compare-ascii ${NOTOL}
                          ${BASELINE_FILES}/bfTvBCOInterpolateImageFunctionOutput.txt
                     ${TEMP}/bfTvBCOInterpolateImageFunctionOutput.txt
  otbBCOInterpolateImageFunction
  ${INPUTDATA}/poupees.tif
  ${TEMP}/bfTvBCOInterpolateImageFunctionOutput.txt
  3 # radius
  -0.5 # optimised bicubic
  0.5 0.5
  127.33 44.9
  259.67 21.43
  12.13 61.79
  89.5 11
  128 128
  127.255 128.73
  -1 -1
  )

ADD_TEST(bfTuBCOInterpolateImageFunctionOverVectorImageNew ${BASICFILTERS_TESTS6}
  otbBCOInterpolateImageFunctionOverVectorImageNew)

ADD_TEST(bfTvBCOInterpolateImageFunctionOverVectorImage ${BASICFILTERS_TESTS6}
--compare-ascii ${NOTOL}
                          ${BASELINE_FILES}/bfTvBCOInterpolateImageFunctionOverVectorImageOutput.txt
                     ${TEMP}/bfTvBCOInterpolateImageFunctionOverVectorImageOutput.txt
  otbBCOInterpolateImageFunctionOverVectorImage
  ${INPUTDATA}/poupees.tif
  ${TEMP}/bfTvBCOInterpolateImageFunctionOverVectorImageOutput.txt
  3 # radius
  -0.5 # optimised bicubic
  0.5 0.5
  127.33 44.9
  259.67 21.43
  12.13 61.79
  89.5 11
  128 128
  127.255 128.73
  -1 -1
  )

ADD_TEST(bfTvBCOInterpolateImageFunctionTest ${BASICFILTERS_TESTS6}
--compare-image ${EPSILON_7}
                       ${BASELINE}/bfTvBCOInterpolateImageFunctionTest.tif
                  ${TEMP}/bfTvBCOInterpolateImageFunctionTest.tif
  otbBCOInterpolateImageFunctionTest
  ${INPUTDATA}/poupees.tif
  ${TEMP}/bfTvBCOInterpolateImageFunctionTest.tif
  3 # radius
  -0.5 # optimised bicubic
  )

ADD_TEST(bfTvBCOInterpolateImageFunctionVectorImageTest ${BASICFILTERS_TESTS6}
--compare-image ${EPSILON_7}
                       ${BASELINE}/bfTvBCOInterpolateImageFunctionVectorImageTest.tif
                  ${TEMP}/bfTvBCOInterpolateImageFunctionVectorImageTest.tif
  otbBCOInterpolateImageFunctionVectorImageTest
  ${INPUTDATA}/poupees.tif
  ${TEMP}/bfTvBCOInterpolateImageFunctionVectorImageTest.tif
  4 # radius
  -0.5 # optimised bicubic
  )


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbBasicFiltersTests7 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::WindowedSincInterpolateImageCosineFunction  ------------------------------
 ADD_TEST(bfTuWindowedSincInterpolateImageCosineFunctionNew ${BASICFILTERS_TESTS7}
         otbWindowedSincInterpolateImageCosineFunctionNew)

ADD_TEST(bfTvWindowedSincInterpolateImageCosineFunction ${BASICFILTERS_TESTS7}
     --compare-ascii ${NOTOL}
                          ${BASELINE_FILES}/bfWindowedSincInterpolateImageCosineFunctionOutput.txt
                     ${TEMP}/bfWindowedSincInterpolateImageCosineFunctionOutput.txt
        otbWindowedSincInterpolateImageCosineFunction
                ${INPUTDATA}/poupees.tif
                ${TEMP}/bfWindowedSincInterpolateImageCosineFunctionOutput.txt
                3 # radius
                0.5 0.5
                127.33 44.9
                259.67 21.43
                12.13 61.79
                89.5 11
                128 128
                127.255 128.73
                -1 -1
                )

# -------            otb::WindowedSincInterpolateImageWelchFunction  ------------------------------
 ADD_TEST(bfTuWindowedSincInterpolateImageWelchFunctionNew ${BASICFILTERS_TESTS7}
         otbWindowedSincInterpolateImageWelchFunctionNew)

ADD_TEST(bfTvWindowedSincInterpolateImageWelchFunction ${BASICFILTERS_TESTS7}
     --compare-ascii ${NOTOL}
                          ${BASELINE_FILES}/bfWindowedSincInterpolateImageWelchFunctionOutput.txt
                     ${TEMP}/bfWindowedSincInterpolateImageWelchFunctionOutput.txt
        otbWindowedSincInterpolateImageWelchFunction
                ${INPUTDATA}/poupees.tif
                ${TEMP}/bfWindowedSincInterpolateImageWelchFunctionOutput.txt
                3 # radius
                0.5 0.5
                127.33 44.9
                259.67 21.43
                12.13 61.79
                89.5 11
                128 128
                127.255 128.73
                -1 -1
                )
# -------            otb::WindowedSincInterpolateImageLanczosFunction  ------------------------------
 ADD_TEST(bfTuWindowedSincInterpolateImageLanczosFunctionNew ${BASICFILTERS_TESTS7}
         otbWindowedSincInterpolateImageLanczosFunctionNew)

ADD_TEST(bfTvWindowedSincInterpolateImageLanczosFunction ${BASICFILTERS_TESTS7}
     --compare-ascii ${NOTOL}
                          ${BASELINE_FILES}/bfWindowedSincInterpolateImageLanczosFunctionOutput.txt
                     ${TEMP}/bfWindowedSincInterpolateImageLanczosFunctionOutput.txt
        otbWindowedSincInterpolateImageLanczosFunction
                ${INPUTDATA}/poupees.tif
                ${TEMP}/bfWindowedSincInterpolateImageLanczosFunctionOutput.txt
                3 # radius
                0.5 0.5
                127.33 44.9
                259.67 21.43
                12.13 61.79
                89.5 11
                128 128
                127.255 128.73
                -1 -1
                )
# -------            otb::WindowedSincInterpolateImageBlackmanFunction  ------------------------------
 ADD_TEST(bfTuWindowedSincInterpolateImageBlackmanFunctionNew ${BASICFILTERS_TESTS7}
         otbWindowedSincInterpolateImageBlackmanFunctionNew)

ADD_TEST(bfTvWindowedSincInterpolateImageBlackmanFunction ${BASICFILTERS_TESTS7}
     --compare-ascii ${NOTOL}
                          ${BASELINE_FILES}/bfWindowedSincInterpolateImageBlackmanFunctionOutput.txt
                     ${TEMP}/bfWindowedSincInterpolateImageBlackmanFunctionOutput.txt
        otbWindowedSincInterpolateImageBlackmanFunction
                ${INPUTDATA}/poupees.tif
                ${TEMP}/bfWindowedSincInterpolateImageBlackmanFunctionOutput.txt
                3 # radius
                0.5 0.5
                127.33 44.9
                259.67 21.43
                12.13 61.79
                89.5 11
                128 128
                127.255 128.73
                -1 -1
                )

ADD_TEST(bfTvProlateValidationTest ${BASICFILTERS_TESTS7}
  --compare-image ${EPSILON_7}
                       ${BASELINE}/bfTvProlateValidationTest.tif
                  ${TEMP}/bfTvProlateValidationTest.tif
        otbProlateValidationTest
                ${INPUTDATA}/poupees.tif
                ${TEMP}/bfTvProlateValidationTest.tif
                3 # radius
                0.5 # factor
                512 # size
                ${TEMP}/defaultprolatevalidationtest.tif # nearest neighborhood interpolator : NOT GENERATE IN THE TEST
                )


# -------            otb::VectorImageToAmplitudeImageFilter  ------------------------------

ADD_TEST(bfTuVectorImageToAmplitudeImageFilterNew ${BASICFILTERS_TESTS7}
otbVectorImageToAmplitudeImageFilterNew)

ADD_TEST(bfTvVectorImageToAmplitudeImageFilter ${BASICFILTERS_TESTS7}
 --compare-image ${EPSILON_7}
                       ${BASELINE}/bfTvVectorImageToAmplitudeImageFilterOutput.tif
                  ${TEMP}/bfTvVectorImageToAmplitudeImageFilterOutput.tif
otbVectorImageToAmplitudeImageFilter
${INPUTDATA}/poupees.tif
${TEMP}/bfTvVectorImageToAmplitudeImageFilterOutput.tif
)


# -------            otb::VectorImageToAmplitudeImageFilter  ------------------------------

ADD_TEST(bfTuVectorImageToAmplitudeImageFilterNew ${BASICFILTERS_TESTS7}
otbVectorImageToAmplitudeImageFilterNew)

ADD_TEST(bfTvVectorImageToAmplitudeImageFilter ${BASICFILTERS_TESTS7}
 --compare-image ${EPSILON_7}
                       ${BASELINE}/bfTvVectorImageToAmplitudeImageFilterOutput.tif
                  ${TEMP}/bfTvVectorImageToAmplitudeImageFilterOutput.tif
otbVectorImageToAmplitudeImageFilter
${INPUTDATA}/poupees.tif
${TEMP}/bfTvVectorImageToAmplitudeImageFilterOutput.tif
)

# -------            otb::VectorImageToIntensityImageFilter   ----------------------------

  ADD_TEST(bfTuVectorImageToIntensityImageFilterNew ${BASICFILTERS_TESTS7}
         otbVectorImageToIntensityImageFilterNew)


 ADD_TEST(bfTvVectorImageToIntensityImageFilter ${BASICFILTERS_TESTS7}
                --compare-image ${EPSILON_7}
                    ${BASELINE}/bfTvVectorImageToIntensityImageOutput.tif
                    ${TEMP}/bfTvVectorImageToIntensityImageOutput.tif
                    otbVectorImageToIntensityImageFilter
                    ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                    ${TEMP}/bfTvVectorImageToIntensityImageOutput.tif
)

# -------            otb::LabelizeConnectedThresholdImageFilter   ----------------------------

  ADD_TEST(bfTuLabelizeConnectedThresholdImageFilterNew ${BASICFILTERS_TESTS8}
         otbLabelizeConnectedThresholdImageFilterNew)

  ADD_TEST(bfTvLabelizeConnectedThresholdImageFilter ${BASICFILTERS_TESTS8}
                --compare-image ${EPSILON_7}
                    ${BASELINE}/bfTvLabelizeConnectedThresholdImageFilterOutput.png
                    ${TEMP}/bfTvLabelizeConnectedThresholdImageFilterOutput.png
                    otbLabelizeConnectedThresholdImageFilter
                    ${INPUTDATA}/QB_Suburb.png
                    ${TEMP}/bfTvLabelizeConnectedThresholdImageFilterOutput.png
                    170 170
                    80 80
)

# -------            otb::EuclideanDistanceWithMissingValue   ----------------------------

  ADD_TEST(bfTuEuclideanDistanceWithMissingValueNew ${BASICFILTERS_TESTS8}
         otbEuclideanDistanceWithMissingValueNew)

  ADD_TEST(bfTvEuclideanDistanceWithMissingValue ${BASICFILTERS_TESTS8}
         otbEuclideanDistanceWithMissingValue)


# -------            otb::FlexibleDistanceWithMissingValue   ----------------------------

  ADD_TEST(bfTuFlexibleDistanceWithMissingValueNew ${BASICFILTERS_TESTS8}
         otbFlexibleDistanceWithMissingValueNew)

  ADD_TEST(bfTvFlexibleDistanceWithMissingValue ${BASICFILTERS_TESTS8}
          otbFlexibleDistanceWithMissingValue
          0.000001
)

# -------            otb::LabelizeNeighborhoodConnectedImageFilter   ----------------------------

  ADD_TEST(bfTuLabelizeNeighborhoodConnectedImageFilterNew ${BASICFILTERS_TESTS8}
         otbLabelizeNeighborhoodConnectedImageFilterNew)

  ADD_TEST(bfTvLabelizeNeighborhoodConnectedImageFilter ${BASICFILTERS_TESTS8}
                --compare-image ${EPSILON_7}
                    ${BASELINE}/bfTvLabelizeNeighborhoodConnectedImageFilterOutput.png
                    ${TEMP}/bfTvLabelizeNeighborhoodConnectedImageFilterOutput.png
                    otbLabelizeNeighborhoodConnectedImageFilter
                    ${INPUTDATA}/QB_Suburb.png
                    ${TEMP}/bfTvLabelizeNeighborhoodConnectedImageFilterOutput.png
                    170 170
                    80 80
)

# -------            otb::LabelizeConfidenceConnectedImageFilter   ----------------------------

  ADD_TEST(bfTuLabelizeConfidenceConnectedImageFilterNew ${BASICFILTERS_TESTS8}
         otbLabelizeConfidenceConnectedImageFilterNew)

  ADD_TEST(bfTvLabelizeConfidenceConnectedImageFilter ${BASICFILTERS_TESTS8}
                --compare-image ${EPSILON_7}
                    ${BASELINE}/bfTvLabelizeConfidenceConnectedImageFilterOutput.png
                    ${TEMP}/bfTvLabelizeConfidenceConnectedImageFilterOutput.png
                    otbLabelizeConfidenceConnectedImageFilter
                    ${INPUTDATA}/QB_Suburb.png
                    ${TEMP}/bfTvLabelizeConfidenceConnectedImageFilterOutput.png
                    127 127 2 3 1
)

# -------            otb::ConvolutionImageFilter   ----------------------------

ADD_TEST(bfTuConvolutionImageFilterNew ${BASICFILTERS_TESTS8}
         otbConvolutionImageFilterNew)

ADD_TEST(bfTvConvolutionImageFilter ${BASICFILTERS_TESTS8}
                --compare-image ${EPSILON_7}
                    ${BASELINE}/bfTvConvolutionImageFilter.tif
                    ${TEMP}/bfTvConvolutionImageFilter.tif
                    otbConvolutionImageFilter
                    ${INPUTDATA}/QB_Suburb.png
                    ${TEMP}/bfTvConvolutionImageFilter.tif
)


# -------            otb::ScalarToRainbowRGBPixelFunctor  -----------------

ADD_TEST(bfTuScalarToRainbowRGBPixelFunctorNew ${BASICFILTERS_TESTS8}
         otbScalarToRainbowRGBPixelFunctorNew)

ADD_TEST(bfTvScalarToRainbowRGBPixelFunctor ${BASICFILTERS_TESTS8}
                --compare-image ${EPSILON_7}
                    ${BASELINE}/bfTvScalarToRainbowRGBPixelFunctor.png
                    ${TEMP}/bfTvScalarToRainbowRGBPixelFunctor.png
                    otbScalarToRainbowRGBPixelFunctor
                    ${INPUTDATA}/QB_Suburb.png
                    ${TEMP}/bfTvScalarToRainbowRGBPixelFunctor.png
)

# -------            otb::AmplitudePhaseToRGBFunctor  -----------------

ADD_TEST(bfTuAmplitudePhaseToRGBFunctorNew ${BASICFILTERS_TESTS8}
         otbAmplitudePhaseToRGBFunctorNew)

ADD_TEST(bfTvAmplitudePhaseToRGBFunctor ${BASICFILTERS_TESTS8}
                --compare-image ${EPSILON_7}
                    ${BASELINE}/bfTvAmplitudePhaseToRGBFunctor.png
                    ${TEMP}/bfTvAmplitudePhaseToRGBFunctor.png
                    otbAmplitudePhaseToRGBFunctor
                    ${INPUTDATA}/RSAT_imagery_HH.tif
                    ${TEMP}/bfTvAmplitudePhaseToRGBFunctor.png
)

# -------            otb::VarianceImageFilter  -----------------

ADD_TEST(bfTuVarianceImageFilterNew ${BASICFILTERS_TESTS8}
         otbVarianceImageFilterNew)

ADD_TEST(bfTvVarianceImageFilter ${BASICFILTERS_TESTS8}
    --compare-image ${EPSILON_7}
        ${BASELINE}/bfVarianceImageFilter.tif
        ${TEMP}/bfVarianceImageFilter.tif
        otbVarianceImageFilter
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
        ${TEMP}/bfVarianceImageFilter.tif
)

# ------             otb::BoxAndWhiskerImageFilter -------------

ADD_TEST(bfTuBoxAndWhiskerImageFilterNew ${BASICFILTERS_TESTS8}
         otbBoxAndWhiskerImageFilterNew )

ADD_TEST(bfTvBoxAndWhiskerImageFilter ${BASICFILTERS_TESTS8}
  --compare-image ${EPSILON_7}
                  ${BASELINE}/bfBoxAndWhiskerImageFilter_05_05.tif
        ${TEMP}/bfBoxAndWhiskerImageFilter_05_05.tif
            otbBoxAndWhiskerImageFilter
             ${INPUTDATA}/poupees.tif
                ${TEMP}/bfBoxAndWhiskerImageFilter_05_05.tif
                05 05)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbBasicFiltersTests9 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ADD_TEST(bfTuContinuousMinimumMaximumImageCalculatorNew ${BASICFILTERS_TESTS9}
        otbContinuousMinimumMaximumImageCalculatorNew
)

ADD_TEST(bfTvContinuousMinimumMaximumImageCalculatorTest ${BASICFILTERS_TESTS9}
        otbContinuousMinimumMaximumImageCalculatorTest
        ${INPUTDATA}/QB_Suburb.png )

ADD_TEST(bfTuMeanShiftImageFilterNew ${BASICFILTERS_TESTS9}
        otbMeanShiftImageFilterNew )

ADD_TEST(bfTuMeanShiftSmoothingImageFilterNew ${BASICFILTERS_TESTS9}
        otbMeanShiftImageFilterNew )


ADD_TEST(bfTvMeanShiftImageFilter ${BASICFILTERS_TESTS9}
 --compare-n-images ${EPSILON_7} 4
                ${BASELINE}/bfMeanShiftImageFilterOutput.tif
                ${TEMP}/bfMeanShiftImageFilterOutput.tif
                ${BASELINE}/bfMeanShiftImageFilterClusteredOutput.tif
                ${TEMP}/bfMeanShiftImageFilterClusteredOutput.tif
                ${BASELINE}/bfMeanShiftImageFilterLabeledClusteredOutput.tif
                ${TEMP}/bfMeanShiftImageFilterLabeledClusteredOutput.tif
                ${BASELINE}/bfMeanShiftImageFilterClusterBoundariesOutput.tif
                ${TEMP}/bfMeanShiftImageFilterClusterBoundariesOutput.tif
        otbMeanShiftImageFilter
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/bfMeanShiftImageFilterOutput.tif
        ${TEMP}/bfMeanShiftImageFilterClusteredOutput.tif
        ${TEMP}/bfMeanShiftImageFilterLabeledClusteredOutput.tif
        ${TEMP}/bfMeanShiftImageFilterClusterBoundariesOutput.tif
        16 16 10 1.0
        )


 ADD_TEST(bfTvMeanShiftImageFilterEDISON ${BASICFILTERS_TESTS9}
       --compare-image ${EPSILON_7}
        ${BASELINE}/bfTvMeanShiftFilterSpectralOutputOptim.tif
        ${TEMP}/bfTvMeanShiftImageFilterOutputOptim.tif
        otbMeanShiftImageFilter
        ${INPUTDATA}/MeanShiftTest.tif
        ${TEMP}/bfTvMeanShiftImageFilterOutputOptim.tif
        ${TEMP}/bfTvMeanShiftImageFilterClusteredOutputOptim.tif
        ${TEMP}/bfTvMeanShiftImageFilterLabeledClusteredOutputOptim.tif
        ${TEMP}/bfTvMeanShiftImageFilterClusterBoundariesOutpuOptim.tif
        2 10 10 0.1
        )


 ADD_TEST(bfTvMeanShiftSmoothingImageFilterOptim ${BASICFILTERS_TESTS9}
        --compare-image ${EPSILON_7}
        ${BASELINE}/bfTvMeanShiftFilterSpectralOutputOptim.tif
        ${TEMP}/bfTvMeanShiftSmoothingImageFilterSpectralOutputOptim.tif
        otbMeanShiftSmoothingImageFilter
        ${INPUTDATA}/MeanShiftTest.tif
        ${TEMP}/bfTvMeanShiftSmoothingImageFilterSpatialOutputOptim.tif
        ${TEMP}/bfTvMeanShiftSmoothingImageFilterSpectralOutputOptim.tif
        ${TEMP}/bfTvMeanShiftSmoothingImageFilterIterationOutputOptim.tif
        ${TEMP}/bfTvMeanShiftSmoothingImageFilterLabelOutputOptim.tif
        2 10 0.1 10 1
       )


 ADD_TEST(bfTvMeanShiftSmoothingImageFilterNonOptim ${BASICFILTERS_TESTS9}
        --compare-image ${EPSILON_7}
        ${BASELINE}/bfTvMeanShiftFilterSpectralOutputNonOptim.tif
        ${TEMP}/bfTvMeanShiftSmoothingImageFilterSpectralOutputNonOptim.tif
        otbMeanShiftSmoothingImageFilter
        ${INPUTDATA}/MeanShiftTest.tif
        ${TEMP}/bfTvMeanShiftSmoothingImageFilterSpatialOutputNonOptim.tif
        ${TEMP}/bfTvMeanShiftSmoothingImageFilterSpectralOutputNonOptim.tif
        ${TEMP}/bfTvMeanShiftSmoothingImageFilterIterationOutputNonOptim.tif
        ${TEMP}/bfTvMeanShiftSmoothingImageFilterLabelOutputNonOptim.tif
        2 10 0.1 10 0
       )

ADD_TEST(bfTvMeanShiftImageFilterQBSuburb ${BASICFILTERS_TESTS9}
        otbMeanShiftImageFilter
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/bfMeanShiftImageFilterOutput_QBSuburb.tif
        ${TEMP}/bfMeanShiftImageFilterClusteredOutput_QBSuburb.tif
        ${TEMP}/bfMeanShiftImageFilterLabeledClusteredOutput_QBSuburb.tif
        ${TEMP}/bfMeanShiftImageFilterClusterBoundariesOutput_QBSuburb.tif
        4 50 10 1.0
        )

ADD_TEST(bfTuMeanShiftSmoothingImageFilterQBSuburbOptim ${BASICFILTERS_TESTS9}
        otbMeanShiftSmoothingImageFilter
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/bfMeanShiftSmoothingImageFilterSpatialOutput_QBSuburbOptim.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterSpectralOutput_QBSuburbOptim.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterIterationOutput_QBSuburbOptim.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterLabelOutput_QBSuburbOptim.tif
        4 25 0.1 100 1
        )

 ADD_TEST(bfTvMeanShiftSmoothingImageFilterQBSuburbNonOptim ${BASICFILTERS_TESTS9}
        --compare-n-images ${EPSILON_7} 3
        ${BASELINE}/bfMeanShiftSmoothingImageFilterSpatialOutput_QBSuburbNonOptim.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterSpatialOutput_QBSuburbNonOptim.tif
        ${BASELINE}/bfMeanShiftSmoothingImageFilterSpectralOutput_QBSuburbNonOptim.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterSpectralOutput_QBSuburbNonOptim.tif
        ${BASELINE}/bfMeanShiftSmoothingImageFilterIterationOutput_QBSuburbNonOptim.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterIterationOutput_QBSuburbNonOptim.tif
        otbMeanShiftSmoothingImageFilter
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/bfMeanShiftSmoothingImageFilterSpatialOutput_QBSuburbNonOptim.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterSpectralOutput_QBSuburbNonOptim.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterIterationOutput_QBSuburbNonOptim.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterLabelOutput_QBSuburbNonOptim.tif
        4 25 0.1 100 0
        )


ADD_TEST(bfTuMeanShiftVectorImageFilterROIQBMul4 ${BASICFILTERS_TESTS9}
        otbMeanShiftVectorImageFilter
        ${INPUTDATA}/ROI_QB_MUL_4.tif
        ${TEMP}/bfMeanShiftImageFilterOutput_ROIQBMul4.tif
        ${TEMP}/bfMeanShiftImageFilterClusteredOutput_ROIQBMul4.tif
        ${TEMP}/bfMeanShiftImageFilterLabeledClusteredOutput_ROIQBMul4.tif
        ${TEMP}/bfMeanShiftImageFilterClusterBoundariesOutput_ROIQBMul4.tif
        4 50 100 1.0
        )




ADD_TEST(bfTuMeanShiftSmoothingImageFilterROIQBMul4 ${BASICFILTERS_TESTS9}
        otbMeanShiftSmoothingImageFilter
        ${INPUTDATA}/ROI_QB_MUL_4.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterSpatialOutput_ROIQBMul4.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterSpectralOutput_ROIQBMul4.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterIterationOutput_ROIQBMul4.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterLabelOutput_ROIQBMul4.tif
        4 50 0.1 100
        )

ADD_TEST(bfTuMeanShiftVectorImageFilterQBRoad ${BASICFILTERS_TESTS9}
        otbMeanShiftVectorImageFilter
        ${INPUTDATA}/qb_RoadExtract.img
        ${TEMP}/bfMeanShiftImageFilterOutput_QBRoad.tif
        ${TEMP}/bfMeanShiftImageFilterClusteredOutput_QBRoad.tif
        ${TEMP}/bfMeanShiftImageFilterLabeledClusteredOutput_QBRoad.tif
        ${TEMP}/bfMeanShiftImageFilterClusterBoundariesOutput_QBRoad.tif
        4 30 100 1.0
        )


ADD_TEST(bfTuMeanShiftSmoothingImageFilterQBRoad ${BASICFILTERS_TESTS9}
        otbMeanShiftSmoothingImageFilter
        ${INPUTDATA}/qb_RoadExtract.img
        ${TEMP}/bfMeanShiftSmoothingImageFilterSpatialOutput_QBRoad.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterSpectralOutput_QBRoad.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterIterationOutput_QBRoad.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterLabelOutput_QBRoad.tif
        4 30 0.1 100
        )


ADD_TEST(bfTvMeanShiftVectorImageFilterQBPAN ${BASICFILTERS_TESTS9}
        otbMeanShiftVectorImageFilter
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
        ${TEMP}/bfMeanShiftImageFilterOutput_PAN.tif
        ${TEMP}/bfMeanShiftImageFilterClusteredOutputValid_PAN.tif
        ${TEMP}/bfMeanShiftImageFilterLabeledClusteredOutputValid_PAN.tif
        ${TEMP}/bfMeanShiftImageFilterClusterBoundariesOutputValid_PAN.tif
        5 15 100 1.0
        )


ADD_TEST(bfTvMeanShiftSmoothingImageFilterQBPAN ${BASICFILTERS_TESTS9}
        otbMeanShiftSmoothingImageFilter
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterSpatialOutput_PAN.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterSpectralOutput_PAN.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterIterationOutput_PAN.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterLabelOutput_PAN.tif
        5 15 1.0 100
        )

ADD_TEST(bfTuMeanShiftVectorImageFilterSPOT5 ${BASICFILTERS_TESTS9}
        otbMeanShiftVectorImageFilter
        ${INPUTDATA}/SPOT5_EXTRACTS/Arcachon/Arcachon_extrait_3852_3319_546_542.tif
        ${TEMP}/bfMeanShiftImageFilterOutput_SPOT5.tif
        ${TEMP}/bfMeanShiftImageFilterClusteredOutput_SPOT5.tif
        ${TEMP}/bfMeanShiftImageFilterLabeledClusteredOutput_SPOT5.tif
        ${TEMP}/bfMeanShiftImageFilterClusterBoundariesOutput_SPOT5.tif
        4 10 100 1.0
        )


ADD_TEST(bfTuMeanShiftSmoothingImageFilterSPOT5 ${BASICFILTERS_TESTS9}
        otbMeanShiftSmoothingImageFilter
        ${INPUTDATA}/SPOT5_EXTRACTS/Arcachon/Arcachon_extrait_3852_3319_546_542.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterSpatialOutput_SPOT5.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterSpectralOutput_SPOT5.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterIterationOutput_SPOT5.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterLabelOutput_SPOT5.tif
        4 10 0.1 100
        )

ADD_TEST(bfTvMeanShiftSmoothingImageFilterThreadingNonOpt ${BASICFILTERS_TESTS9}
        --compare-image ${EPSILON_7}
        ${TEMP}/bfMeanShiftSmoothingImageFilter2SingleThreading_SPOT5.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilter2MultiThreading_SPOT5.tif
        otbMeanShiftSmoothingImageFilterThreading
        ${INPUTDATA}/SPOT5_EXTRACTS/Arcachon/Arcachon_extrait_3852_3319_546_542.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilter2SingleThreading_SPOT5.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilter2MultiThreading_SPOT5.tif
        4 10 0
        )

#ADD_TEST(bfTvMeanShiftImageFilter2ThreadingOpt ${BASICFILTERS_TESTS9}
#        --compare-image ${EPSILON_7}
#        ${TEMP}/bfMeanShiftSmoothingImageFilter2SingleThreading_SPOT5.tif
#        ${TEMP}/bfMeanShiftSmoothingImageFilter2MultiThreading_SPOT5.tif
#        otbMeanShiftImageFilter2Threading
#        ${INPUTDATA}/SPOT5_EXTRACTS/Arcachon/Arcachon_extrait_3852_3319_546_542.tif
#        ${TEMP}/bfMeanShiftSmoothingImageFilter2SingleThreading_SPOT5.tif
#        ${TEMP}/bfMeanShiftSmoothingImageFilter2MultiThreading_SPOT5.tif
#        4 10 1
#        )


ADD_TEST(bfTvLabelImageRegionMergingFilterQB_Suburb ${BASICFILTERS_TESTS9}
        --compare-n-images ${EPSILON_7} 2
        ${BASELINE}/bfLabelImageRegionMergingFilterMergeOutput_QB_Suburb.tif
        ${TEMP}/bfLabelImageRegionMergingFilterMergeOutput_QB_Suburb.tif
      	${BASELINE}/bfLabelImageRegionMergingFilterClusteredOutput_QB_Suburb.tif
       	${TEMP}/bfLabelImageRegionMergingFilterClusteredOutput_QB_Suburb.tif
        otbLabelImageRegionMergingFilter
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/bfLabelImageRegionMergingFilterSpectralOutput_QB_Suburb.tif
        ${TEMP}/bfLabelImageRegionMergingFilterLabelOutput_QB_Suburb.tif
        ${TEMP}/bfLabelImageRegionMergingFilterMergeOutput_QB_Suburb.tif
        ${TEMP}/bfLabelImageRegionMergingFilterClusteredOutput_QB_Suburb.tif
        4 25 0.1 100
        )
        
ADD_TEST(bfTvLabelImageRegionPruningFilterQB_Suburb ${BASICFILTERS_TESTS9}
        --compare-n-images ${EPSILON_7} 2
        ${BASELINE}/bfLabelImageRegionPruningFilterMergeOutput_QB_Suburb.tif
        ${TEMP}/bfLabelImageRegionPruningFilterMergeOutput_QB_Suburb.tif
      	${BASELINE}/bfLabelImageRegionPruningFilterClusteredOutput_QB_Suburb.tif
       	${TEMP}/bfLabelImageRegionPruningFilterClusteredOutput_QB_Suburb.tif
        otbLabelImageRegionPruningFilter
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/bfLabelImageRegionPruningFilterSpectralOutput_QB_Suburb.tif
        ${TEMP}/bfLabelImageRegionPruningFilterLabelOutput_QB_Suburb.tif
        ${TEMP}/bfLabelImageRegionPruningFilterMergeOutput_QB_Suburb.tif
        ${TEMP}/bfLabelImageRegionPruningFilterClusteredOutput_QB_Suburb.tif
        4 25 0.1 100 10
        )        

ADD_TEST(bfTvLabelImageRegionMergingFilterSPOT5 ${BASICFILTERS_TESTS9}
        --compare-n-images ${EPSILON_7} 2
        ${BASELINE}/bfLabelImageRegionMergingFilterMergeOutput_SPOT5.tif
        ${TEMP}/bfLabelImageRegionMergingFilterMergeOutput_SPOT5.tif
        ${BASELINE}/bfLabelImageRegionMergingFilterClusteredOutput_SPOT5.tif
        ${TEMP}/bfLabelImageRegionMergingFilterClusteredOutput_SPOT5.tif
        otbLabelImageRegionMergingFilter
        ${INPUTDATA}/SPOT5_EXTRACTS/Arcachon/Arcachon_extrait_3852_3319_546_542.tif
        ${TEMP}/bfLabelImageRegionMergingFilterSpectralOutput_SPOT5.tif
        ${TEMP}/bfLabelImageRegionMergingFilterLabelOutput_SPOT5.tif
        ${TEMP}/bfLabelImageRegionMergingFilterMergeOutput_SPOT5.tif
        ${TEMP}/bfLabelImageRegionMergingFilterClusteredOutput_SPOT5.tif
        4 25 0.1 100
        )

ADD_TEST(bfTvLabelImageRegionPruningFilterSPOT5 ${BASICFILTERS_TESTS9}
        --compare-n-images ${EPSILON_7} 2
        ${BASELINE}/bfLabelImageRegionPruningFilterMergeOutput_SPOT5.tif
        ${TEMP}/bfLabelImageRegionPruningFilterMergeOutput_SPOT5.tif
        ${BASELINE}/bfLabelImageRegionPruningFilterClusteredOutput_SPOT5.tif
        ${TEMP}/bfLabelImageRegionPruningFilterClusteredOutput_SPOT5.tif
        otbLabelImageRegionPruningFilter
        ${INPUTDATA}/SPOT5_EXTRACTS/Arcachon/Arcachon_extrait_3852_3319_546_542.tif
        ${TEMP}/bfLabelImageRegionPruningFilterSpectralOutput_SPOT5.tif
        ${TEMP}/bfLabelImageRegionPruningFilterLabelOutput_SPOT5.tif
        ${TEMP}/bfLabelImageRegionPruningFilterMergeOutput_SPOT5.tif
        ${TEMP}/bfLabelImageRegionPruningFilterClusteredOutput_SPOT5.tif
        4 25 0.1 100 20
        )

IF(OTB_DATA_USE_LARGEINPUT)
#
#ADD_TEST(bfTvMeanShiftImageFilterValidLargeInput ${BASICFILTERS_TESTS9}
#        otbMeanShiftImageFilter
#        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
#        ${TEMP}/bfMeanShiftImageFilterOutputValidLargeInput.tif
#        ${TEMP}/bfMeanShiftImageFilterClusteredOutputValidLargeInput.tif
#        ${TEMP}/bfMeanShiftImageFilterLabeledClusteredOutputValidLargeInput.tif
#        ${TEMP}/bfMeanShiftImageFilterClusterBoundariesOutputValidLargeInput.tif
#        9 50 10 1.0
#        )

#ADD_TEST(bfTvMeanShiftImageFilter2LargeInput ${BASICFILTERS_TESTS9}
#       otbMeanShiftImageFilter2
#        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
#        ${TEMP}/bfMeanShiftImageSpatialOutputLargeInput.tif
#        ${TEMP}/bfMeanShiftImageFilterSpectralOutputLargeInput.tif
#        9 9 10 10 0.001 10
#        )

ENDIF(OTB_DATA_USE_LARGEINPUT)


ADD_TEST(bfTuMeanShiftVectorImageFilterNew ${BASICFILTERS_TESTS9}
        otbMeanShiftVectorImageFilterNew )

ADD_TEST(bfTvMeanShiftVectorImageFilter ${BASICFILTERS_TESTS9}
--compare-n-images ${EPSILON_7} 4
                ${BASELINE}/bfMeanShiftVectorImageFilterOutput.tif
                ${TEMP}/bfMeanShiftVectorImageFilterOutput.tif
                ${BASELINE}/bfMeanShiftVectorImageFilterClusteredOutput.tif
                ${TEMP}/bfMeanShiftVectorImageFilterClusteredOutput.tif
                ${BASELINE}/bfMeanShiftVectorImageFilterLabeledClusteredOutput.tif
                ${TEMP}/bfMeanShiftVectorImageFilterLabeledClusteredOutput.tif
                ${BASELINE}/bfMeanShiftVectorImageFilterClusterBoundariesOutput.tif
                ${TEMP}/bfMeanShiftVectorImageFilterClusterBoundariesOutput.tif
        otbMeanShiftVectorImageFilter
        ${INPUTDATA}/qb_RoadExtract2sub200x200.tif
        ${TEMP}/bfMeanShiftVectorImageFilterOutput.tif
        ${TEMP}/bfMeanShiftVectorImageFilterClusteredOutput.tif
        ${TEMP}/bfMeanShiftVectorImageFilterLabeledClusteredOutput.tif
        ${TEMP}/bfMeanShiftVectorImageFilterClusterBoundariesOutput.tif
        16 16 10 1.0
        )

ADD_TEST(bfTuFunctionToImageFilterNew ${BASICFILTERS_TESTS9}
        otbFunctionToImageFilterNew
)

ADD_TEST(bfTvFunctionToImageFilterTest ${BASICFILTERS_TESTS9}
        --compare-image ${EPSILON_7}
            ${BASELINE}/bfTvFunctionToImageFilterTest.tif
            ${TEMP}/bfTvFunctionToImageFilterTest.tif
        otbFunctionToImageFilter
            ${INPUTDATA}/poupees_1canal.hd
            ${TEMP}/bfTvFunctionToImageFilterTest.tif
            2 # radius
)

# -------            otb::BinaryImageDensityFunction   ----------------------------
ADD_TEST(bfTuBinaryImageDensityFunctionNew  ${BASICFILTERS_TESTS9}
        otbBinaryImageDensityFunctionNew
)

ADD_TEST(bfTvBinaryImageDensityFunction ${BASICFILTERS_TESTS9}
--compare-ascii ${NOTOL}
             ${BASELINE_FILES}/bfTvBinaryImageDensityFunctionOutputAscii.txt
            ${TEMP}/bfTvBinaryImageDensityFunctionOutputAscii.txt
         otbBinaryImageDensityFunction
         ${INPUTDATA}/scene.png
         ${TEMP}/bfTvBinaryImageDensityFunctionOutputAscii.txt
         )

# -------            otb::BinaryImageToImageDensityFilter   ----------------------------
ADD_TEST(bfTuBinaryImageToImageDensityFilterNew  ${BASICFILTERS_TESTS9}
        otbBinaryImageToDensityImageFilterNew
)

ADD_TEST(bfTvBinaryImageToDensityImageFilter ${BASICFILTERS_TESTS9}
        --compare-image ${EPSILON_7}
            ${BASELINE}/bfTvBinaryImageToDensityImageFilterOutputImage.tif
            ${TEMP}/bfTvBinaryImageToDensityImageFilterOutputImage.tif
        otbBinaryImageToDensityImageFilter
            ${INPUTDATA}/scene.png
            ${TEMP}/bfTvBinaryImageToDensityImageFilterOutputImage.tif
            2 # radius
)

# -------            otb::EdgeDensityImageyFilter   ----------------------------
ADD_TEST(bfTuEdgeDensityImageFilterNew  ${BASICFILTERS_TESTS9}
        otbEdgeDensityImageFilterNew
)

ADD_TEST(bfTvEdgeDensityImageFilter ${BASICFILTERS_TESTS9}
        --compare-image ${EPSILON_7}
            ${BASELINE}/bfTvEdgeDensityImageFilterOutputImage.tif
            ${TEMP}/bfTvEdgeDensityImageFilterOutputImage.tif
        otbEdgeDensityImageFilter
            ${INPUTDATA}/scene.png
            ${TEMP}/bfTvEdgeDensityImageFilterOutputImage.tif
            1 # radius
            15 3  1. 0.01  #Canny Parameters

)



# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbBasicFiltersTests10 ~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::GaborFilterGenerator   ----------------------------

ADD_TEST(bfTuGaborFilterGeneratorNew ${BASICFILTERS_TESTS10}
        otbGaborFilterGeneratorNew
         )

ADD_TEST(bfTvGaborFilterGenerator ${BASICFILTERS_TESTS10}
        --compare-image ${EPSILON_7}
            ${BASELINE}/bfGaborFilterGeneratorTest.tif
            ${TEMP}/bfGaborFilterGeneratorTest.tif
         otbGaborFilterGenerator
         ${TEMP}/bfGaborFilterGeneratorTest.tif
         64 64 #Radius
         0.02 0.025 # a b
         -45 # theta
         0.0125 0.0125 #u0 v0
         0
)

# -------            otb::OverlapSaveConvolutionImageFilter   ----------------------------

IF(USE_FFTWD)

  ADD_TEST(bfTuOverlapSaveConvolutionImageFilterNew ${BASICFILTERS_TESTS10}
         otbOverlapSaveConvolutionImageFilterNew)

  ADD_TEST(bfTvOverlapSaveConvolutionImageFilter ${BASICFILTERS_TESTS10}
                --compare-image ${EPSILON_7}
                    ${BASELINE}/bfTvConvolutionImageFilter.tif
                    ${TEMP}/bfTvOverlapSaveConvolutionImageFilter.tif
                    otbOverlapSaveConvolutionImageFilter
                    ${INPUTDATA}/QB_Suburb.png
                    ${TEMP}/bfTvOverlapSaveConvolutionImageFilter.tif
)

 ADD_TEST(bfTvCompareOverlapSaveAndClassicalConvolutionWithGaborFilter ${BASICFILTERS_TESTS10}
                --compare-image ${EPSILON_7}
                    ${TEMP}/bfTvCompareConvolutionOutput.tif
                    ${TEMP}/bfTvCompareOSConvolutionoutput.tif
                    otbCompareOverlapSaveAndClassicalConvolutionWithGaborFilter
                    ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub.tif
                    ${TEMP}/bfTvCompareConvolutionOutput.tif
                    ${TEMP}/bfTvCompareOSConvolutionoutput.tif
                    64 64 #Radius
                    0.02 0.025 # a b
                    -45 # theta
                    0.0125 0.0125 #u0 v0
                    0
)

ENDIF(USE_FFTWD)


ADD_TEST(bfTvPolygonCompacityFunctor ${BASICFILTERS_TESTS10}
        --compare-ascii ${NOTOL}
            ${BASELINE_FILES}/bfPolygonCompacityFunctorTest.txt
            ${TEMP}/bfPolygonCompacityFunctorTest.txt
         otbPolygonCompacityFunctor
         ${INPUTDATA}/polygon.png
         ${TEMP}/bfPolygonCompacityFunctorTest.txt
)

ADD_TEST(bfTvPathLengthFunctor ${BASICFILTERS_TESTS10}
        --compare-ascii ${NOTOL}
            ${BASELINE_FILES}/bfPathLengthFunctorTest.txt
            ${TEMP}/bfPathLengthFunctorTest.txt
         otbPathLengthFunctor
         ${INPUTDATA}/polygon.png
         ${TEMP}/bfPathLengthFunctorTest.txt
)

ADD_TEST(bfTvClosePathFunctor ${BASICFILTERS_TESTS10}
        --compare-ascii ${NOTOL}
            ${BASELINE_FILES}/bfClosePathFunctorTest.txt
            ${TEMP}/bfClosePathFunctorTest.txt
         otbClosePathFunctor
         ${INPUTDATA}/polygon.png
         ${TEMP}/bfClosePathFunctorTest.txt
)

# -------            otb::PersistentVectorizationImageFilter   ----------------------------

ADD_TEST(bfTuPersistentVectorizationImageFilterNew ${BASICFILTERS_TESTS10}
        otbPersistentVectorizationFilterNew
         )

ADD_TEST(bfTvPersistentVectorizationImageFilter ${BASICFILTERS_TESTS10}
--compare-ascii ${NOTOL}
            ${BASELINE_FILES}/bfTvPersistentVectorizationImageFilterOutput.txt
            ${TEMP}/bfTvPersistentVectorizationImageFilterOutput.txt
         otbPersistentVectorizationFilter
         ${INPUTDATA}/Seg1InputForRCC8Graph.tif
         ${TEMP}/bfTvPersistentVectorizationImageFilterOutput.txt
)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbBasicFiltersTests11 ~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------     Combination ExtractROI and StremingResample   ----------------------------

ADD_TEST(bfTvExtractROIResample ${BASICFILTERS_TESTS11}
--compare-image ${NOTOL}
            ${BASELINE}/bfTvExtractROIResample.tif
            ${TEMP}/bfTvExtractROIResample.tif
         otbExtractROIResample
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/bfTvExtractROIResample.tif
         0
)

ADD_TEST(bfTvExtractROIResample2 ${BASICFILTERS_TESTS11}
--compare-image ${NOTOL}
            ${BASELINE}/bfTvExtractROIResample2.tif
            ${TEMP}/bfTvExtractROIResample2.tif
         otbExtractROIResample
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/bfTvExtractROIResample2.tif
         1
)
# -------    otbPointSetFunction   ----------------------------
#ADD_TEST(bfTuPointSetFunctionNew ${BASICFILTERS_TESTS11}
#         otbPointSetFunctionNew
#         )



# -------    otbPointSetDensityFunction   ----------------------------
ADD_TEST(bfTuPointSetDensityFunctionNew ${BASICFILTERS_TESTS11}
         otbPointSetDensityFunctionNew
         )


ADD_TEST(bfTvPointSetDensityFunctionTest ${BASICFILTERS_TESTS11}
--compare-ascii ${NOTOL}
             ${BASELINE_FILES}/bfTvPointSetDensityFunctionOutputAscii.txt
            ${TEMP}/bfTvPointSetDensityFunctionOutputAscii.txt
         otbPointSetDensityFunctionTest
         ${TEMP}/bfTvPointSetDensityFunctionOutputAscii.txt
         )

# -------    otbPointSetDensityGaussianFunction   ----------------------------
ADD_TEST(bfTuPointSetDensityGaussianFunctionNew ${BASICFILTERS_TESTS11}
     otbPointSetDensityGaussianFunctionNew
     )


ADD_TEST(bfTvPointSetDensityGaussianFunctionTest ${BASICFILTERS_TESTS11}
--compare-ascii ${NOTOL}
         ${BASELINE_FILES}/bfTvPointSetDensityGaussianFunctionOutputAscii.txt
        ${TEMP}/bfTvPointSetDensityGaussianFunctionOutputAscii.txt
     otbPointSetDensityGaussianFunctionTest
     ${TEMP}/bfTvPointSetDensityGaussianFunctionOutputAscii.txt
     )

# -------    otbPointSetDensityEpanechnikovFunction   ----------------------------
ADD_TEST(bfTuPointSetDensityEpanechnikovFunctionNew ${BASICFILTERS_TESTS11}
     otbPointSetDensityEpanechnikovFunctionNew
     )


ADD_TEST(bfTvPointSetDensityEpanechnikovFunctionTest ${BASICFILTERS_TESTS11}
--compare-ascii ${NOTOL}
         ${BASELINE_FILES}/bfTvPointSetDensityEpanechnikovFunctionOutputAscii.txt
        ${TEMP}/bfTvPointSetDensityEpanechnikovFunctionOutputAscii.txt
     otbPointSetDensityEpanechnikovFunctionTest
     ${TEMP}/bfTvPointSetDensityEpanechnikovFunctionOutputAscii.txt
     )

#------------ otbPointSetToDensityImageFilter ---------------------

ADD_TEST(bfTuPointSetToDensityImageFilterNew ${BASICFILTERS_TESTS11}
         otbPointSetToDensityImageFilterNew)



ADD_TEST(bfTvPointSetToDensityImageFilterTest ${BASICFILTERS_TESTS11}
--compare-image ${NOTOL}
            ${BASELINE}/bfTvPointSetToDensityImageFilterOutputImage.tif
            ${TEMP}/bfTvPointSetToDensityImageFilterOutputImage.tif
            otbPointSetToDensityImageFilterTest
         ${INPUTDATA}/QB_Suburb.png
         ${TEMP}/bfTvPointSetToDensityImageFilterOutputImage.tif
         3 5 10
)

# -------    otbKeyPointDensityImageFilter   ----------------------------
ADD_TEST(bfTuKeyPointDensityImageFilterNew ${BASICFILTERS_TESTS11}
         otbKeyPointDensityImageFilterNew
         )

ADD_TEST(bfTvKeyPointDensityImageFilterOutputImage ${BASICFILTERS_TESTS11}
--compare-image ${NOTOL}
            ${BASELINE}/bfTvKeyPointDensityImageFilterOutputImage.tif
            ${TEMP}/bfTvKeyPointDensityImageFilterOutputImage.tif
            otbKeyPointDensityImageFilterTest
         ${INPUTDATA}/QB_Suburb.png
         ${TEMP}/bfTvKeyPointDensityImageFilterOutputImage.tif
         3 5 10
)

ADD_TEST(bfTvFunctionWithNeighborhoodToImageFilterNew ${BASICFILTERS_TESTS11}
        otbFunctionWithNeighborhoodToImageFilterNew
)

ADD_TEST(bfTvFunctionWithNeighborhoodToImageFilter ${BASICFILTERS_TESTS11}
        --compare-image ${EPSILON_7}
            ${BASELINE}/bfTvFunctionWithNeighborhoodToImageFilterTest.tif
            ${TEMP}/bfTvFunctionWithNeighborhoodToImageFilterTest.tif
        otbFunctionWithNeighborhoodToImageFilter
            ${INPUTDATA}/poupees_1canal.hd
        ${TEMP}/bfTvFunctionWithNeighborhoodToImageFilterTest.tif
            2 # radius
)


ADD_TEST(bfTuStreamingInnerProductVectorImageFilterNew ${BASICFILTERS_TESTS11}
        otbStreamingInnerProductVectorImageFilterNew
)

ADD_TEST(bfTvStreamingInnerProductVectorImageFilter ${BASICFILTERS_TESTS11}
      --compare-ascii 0.000001
      ${BASELINE_FILES}/bfStreamingInnerProductVectorImageFilterResults.txt
      ${TEMP}/bfStreamingInnerProductVectorImageFilterResults.txt
      otbStreamingInnerProductVectorImageFilter
      ${INPUTDATA}/poupees_sub_3c.png
      ${TEMP}/bfStreamingInnerProductVectorImageFilterResults.txt
      1 #center data
)
ADD_TEST(bfTvStreamingInnerProductVectorImageFilterDisableCenterData ${BASICFILTERS_TESTS11}
      --compare-ascii 0.000001
      ${BASELINE_FILES}/bfStreamingInnerProductVectorImageFilterResultsDisableCenterData.txt
      ${TEMP}/bfStreamingInnerProductVectorImageFilterResultsDisableCenterData.txt
      otbStreamingInnerProductVectorImageFilter
      ${INPUTDATA}/poupees_sub_3c.png
      ${TEMP}/bfStreamingInnerProductVectorImageFilterResultsDisableCenterData.txt
      0 #center data
)

ADD_TEST(bfTuEstimateInnerProductPCAImageFilterNew ${BASICFILTERS_TESTS11}
        otbEstimateInnerProductPCAImageFilterNew )


ADD_TEST(bfTvEstimateInnerProductPCAImageFilter ${BASICFILTERS_TESTS11}
    --compare-image ${EPSILON_7}
    ${BASELINE}/bfEstimateInnerProductPCAImageFilter.tif
    ${TEMP}/bfEstimateInnerProductPCAImageFilter.tif
    otbEstimateInnerProductPCAImageFilter
    ${INPUTDATA}/poupees_sub_3c.png
    ${TEMP}/bfEstimateInnerProductPCAImageFilter.tif
    3
    1 #center data
)

ADD_TEST(bfTvEstimateInnerProductPCAImageFilterDisableCenterData ${BASICFILTERS_TESTS11}
    --compare-image ${EPSILON_7}
    ${BASELINE}/bfEstimateInnerProductPCAImageFilterDisableCenterData.tif
    ${TEMP}/bfEstimateInnerProductPCAImageFilterDisableCenterData.tif
    otbEstimateInnerProductPCAImageFilter
    ${INPUTDATA}/poupees_sub_3c.png
    ${TEMP}/bfEstimateInnerProductPCAImageFilterDisableCenterData.tif
    3
    0 #center data
)

ADD_TEST(bfTvEstimateInnerProductPCAImageFilterCP1 ${BASICFILTERS_TESTS11}
    --compare-image ${EPSILON_7}
    ${BASELINE}/bfEstimateInnerProductPCAImageFilter_CP1.tif
    ${TEMP}/bfEstimateInnerProductPCAImageFilter_CP1.tif
    otbEstimateInnerProductPCAImageFilter
    ${INPUTDATA}/poupees_sub_3c.png
    ${TEMP}/bfEstimateInnerProductPCAImageFilter_CP1.tif
    1
    1 #center data
)
ADD_TEST(bfTvEstimateInnerProductPCAImageFilterCP12 ${BASICFILTERS_TESTS11}
    --compare-image ${EPSILON_7}
    ${BASELINE}/bfEstimateInnerProductPCAImageFilter_CP12.tif
    ${TEMP}/bfEstimateInnerProductPCAImageFilter_CP12.tif
    otbEstimateInnerProductPCAImageFilter
    ${INPUTDATA}/poupees_sub_3c.png
    ${TEMP}/bfEstimateInnerProductPCAImageFilter_CP12.tif
    2
    1 #center data
)

ADD_TEST(bfTuNormalizeInnerProductPCAImageFilterNew ${BASICFILTERS_TESTS11}
        otbNormalizeInnerProductPCAImageFilterNew )

ADD_TEST(bfTvNormalizeInnerProductPCAImageFilter ${BASICFILTERS_TESTS11}
    --compare-image ${EPSILON_12}
    ${BASELINE}/bfNormalizeInnerProductPCAImageFilter.tif
    ${TEMP}/bfNormalizeInnerProductPCAImageFilter.tif
    otbNormalizeInnerProductPCAImageFilter
    ${INPUTDATA}/poupees_sub_3c.png
    ${TEMP}/bfNormalizeInnerProductPCAImageFilter.tif
)

ADD_TEST(bfTuInnerProductPCAImageFilterNew ${BASICFILTERS_TESTS11}
        otbInnerProductPCAImageFilterNew )


ADD_TEST(bfTvInnerProductPCAImageFilter ${BASICFILTERS_TESTS11}
    --compare-image ${EPSILON_7}
    ${BASELINE}/bfInnerProductPCAImageFilter.tif
    ${TEMP}/bfInnerProductPCAImageFilter.tif
    otbInnerProductPCAImageFilter
    ${INPUTDATA}/poupees_sub_3c.png
    ${TEMP}/bfInnerProductPCAImageFilter.tif
    0 # Flag generate Mean component ?
    1 #center data
    3 # Nb Of Principal Components
    1 # use unbiased estimator
    ${TEMP}/bfInnerProductPCAImageFilter_pc1.tif
    ${TEMP}/bfInnerProductPCAImageFilter_pc2.tif
    ${TEMP}/bfInnerProductPCAImageFilter_pc3.tif
)

ADD_TEST(bfTvInnerProductPCAImageFilterDisableCenterData ${BASICFILTERS_TESTS11}
    --compare-image ${EPSILON_7}
    ${BASELINE}/bfInnerProductPCAImageFilterDisableCenterData.tif
    ${TEMP}/bfInnerProductPCAImageFilterDisableCenterData.tif
    otbInnerProductPCAImageFilter
    ${INPUTDATA}/poupees_sub_3c.png
    ${TEMP}/bfInnerProductPCAImageFilterDisableCenterData.tif
    0 # Flag generate Mean component ?
    0 #center data
    3 # Nb Of Principal Components
    1 # use unbiased estimator
    ${TEMP}/bfInnerProductPCAImageFilterDisableCenterData_pc1.tif
    ${TEMP}/bfInnerProductPCAImageFilterDisableCenterData_pc2.tif
    ${TEMP}/bfInnerProductPCAImageFilterDisableCenterData_pc3.tif
)

ADD_TEST(bfTvInnerProductPCAImageFilter_PC1 ${BASICFILTERS_TESTS11}
    --compare-image ${EPSILON_7}
    ${BASELINE}/bfInnerProductPCAImageFilter_PC1.tif
    ${TEMP}/bfInnerProductPCAImageFilter_PC1.tif
    otbInnerProductPCAImageFilter
    ${INPUTDATA}/poupees_sub_3c.png
    ${TEMP}/bfInnerProductPCAImageFilter_PC1.tif
    0 # Flag generate Mean component ?
    1 #center data
    1 # Nb Of Principal Components
    1 # use unbiased estimator
    ${TEMP}/bfInnerProductPCAImageFilter_PC1_1.tif
)
ADD_TEST(bfTvInnerProductPCAImageFilter_PC12 ${BASICFILTERS_TESTS11}
    --compare-image ${EPSILON_7}
    ${BASELINE}/bfInnerProductPCAImageFilter_PC12.tif
    ${TEMP}/bfInnerProductPCAImageFilter_PC12.tif
    otbInnerProductPCAImageFilter
    ${INPUTDATA}/poupees_sub_3c.png
    ${TEMP}/bfInnerProductPCAImageFilter_PC12.tif
    0 # Flag generate Mean component ?
    1 #center data
    2 # Nb Of Principal Components
    1 # use unbiased estimator
    ${TEMP}/bfInnerProductPCAImageFilter_PC12_1.tif
    ${TEMP}/bfInnerProductPCAImageFilter_PC12_2.tif
)

ADD_TEST(bfTvInnerProductPCAImageFilterAllsOutputs ${BASICFILTERS_TESTS11}
    --compare-image ${EPSILON_7}
    ${BASELINE}/bfInnerProductPCAImageFilterAllsOutputs.tif
    ${TEMP}/bfInnerProductPCAImageFilterAllsOutputs.tif
    otbInnerProductPCAImageFilter
    ${INPUTDATA}/poupees_sub_3c.png
    ${TEMP}/bfInnerProductPCAImageFilterAllsOutputs.tif
    1 # Flag generate Mean component ?
    1 #center data
    3 # Nb Of Principal Components
    1 # use unbiased estimator (default mode)
    ${TEMP}/bfInnerProductPCAImageFilterAllsOutputs_pc1.tif
    ${TEMP}/bfInnerProductPCAImageFilterAllsOutputs_pc2.tif
    ${TEMP}/bfInnerProductPCAImageFilterAllsOutputs_pc3.tif
    ${TEMP}/bfInnerProductPCAImageFilterAllsOutputs_MEAN.tif
)

ADD_TEST(bfTvInnerProductPCAImageFilterAllsOutputsWithoutUnbiasedEstimator ${BASICFILTERS_TESTS11}
    --compare-image ${EPSILON_7}
    ${BASELINE}/bfInnerProductPCAImageFilterAllsOutputs.tif
    ${TEMP}/bfInnerProductPCAImageFilterAllsOutputs.tif
    otbInnerProductPCAImageFilter
    ${INPUTDATA}/poupees_sub_3c.png
    ${TEMP}/bfInnerProductPCAImageFilterAllsOutputs.tif
    1 # Flag generate Mean component ?
    1 #center data
    3 # Nb Of Principal Components
    0 # do not use unbiased estimator (use in bfTvItkImagePCAShapeModelEstimatorTest to compare results with ITK PCA filter)
    ${TEMP}/bfInnerProductPCAImageFilterAllsOutputsWithoutUnbiasedEstimator_pc1.tif
    ${TEMP}/bfInnerProductPCAImageFilterAllsOutputsWithoutUnbiasedEstimator_pc2.tif
    ${TEMP}/bfInnerProductPCAImageFilterAllsOutputsWithoutUnbiasedEstimator_pc3.tif
    ${TEMP}/bfInnerProductPCAImageFilterAllsOutputsWithoutUnbiasedEstimator_MEAN.tif
)

# -------    Check PCA Implementation with ITK ImagePCAShapeModelEstimator class ----------------------------
ADD_TEST(bfTvItkImagePCAShapeModelEstimatorTest ${BASICFILTERS_TESTS11}
    --compare-n-images ${EPSILON_7} 4
    ${TEMP}/bfITKimagePCAShapeModelEstimatorTest_eigen_vectors1.tif
    ${TEMP}/bfInnerProductPCAImageFilterAllsOutputsWithoutUnbiasedEstimator_pc1.tif
    ${TEMP}/bfITKimagePCAShapeModelEstimatorTest_eigen_vectors2.tif
    ${TEMP}/bfInnerProductPCAImageFilterAllsOutputsWithoutUnbiasedEstimator_pc2.tif
    ${TEMP}/bfITKimagePCAShapeModelEstimatorTest_eigen_vectors3.tif
    ${TEMP}/bfInnerProductPCAImageFilterAllsOutputsWithoutUnbiasedEstimator_pc3.tif
    ${TEMP}/bfITKimagePCAShapeModelEstimatorTest_mean.tif
    ${TEMP}/bfInnerProductPCAImageFilterAllsOutputsWithoutUnbiasedEstimator_MEAN.tif
    otbImagePCAShapeModelEstimatorTest
    ${INPUTDATA}/poupees_sub_c1.png
    ${INPUTDATA}/poupees_sub_c2.png
    ${INPUTDATA}/poupees_sub_c3.png
    ${TEMP}/bfITKimagePCAShapeModelEstimatorTest_mean.tif
    ${TEMP}/bfITKimagePCAShapeModelEstimatorTest_eigen_vectors1.tif
    ${TEMP}/bfITKimagePCAShapeModelEstimatorTest_eigen_vectors2.tif
    ${TEMP}/bfITKimagePCAShapeModelEstimatorTest_eigen_vectors3.tif
)
SET_TESTS_PROPERTIES(bfTvItkImagePCAShapeModelEstimatorTest PROPERTIES DEPENDS bfTvInnerProductPCAImageFilterAllsOutputsWithoutUnbiasedEstimator)

# -------  otb::ListSampleToVariableDimensionHistogramGenerator ----------------------------

ADD_TEST(bfTuListSampleToVariableDimensionHistogramGeneratorNew ${BASICFILTERS_TESTS11}
        otbListSampleToVariableDimensionHistogramGeneratorNew)

ADD_TEST(bfTvListSampleToVariableDimensionHistogramGenerator ${BASICFILTERS_TESTS11}
--compare-ascii ${NOTOL}
        ${BASELINE_FILES}/bfTvListSampleToVariableDimensionHistogramGeneratorOutput.txt
        ${TEMP}/bfTvListSampleToVariableDimensionHistogramGeneratorOutput.txt
        otbListSampleToVariableDimensionHistogramGenerator
        ${INPUTDATA}/couleurs.tif
        ${TEMP}/bfTvListSampleToVariableDimensionHistogramGeneratorOutput.txt
        10 1
)

# -------  otb::ListSampleToVariableDimensionHistogramGenerator ----------------------------

ADD_TEST(bfTuListSampleToHistogramListGeneratorNew ${BASICFILTERS_TESTS11}
        otbListSampleToHistogramListGeneratorNew)

ADD_TEST(bfTvListSampleToHistogramListGenerator ${BASICFILTERS_TESTS11}
--compare-ascii ${NOTOL}
        ${BASELINE_FILES}/bfTvListSampleToVariableDimensionHistogramGeneratorOutput.txt
        ${TEMP}/bfTvListSampleToHistogramListGeneratorOutput.txt
        otbListSampleToHistogramListGenerator
        ${INPUTDATA}/couleurs.tif
        ${TEMP}/bfTvListSampleToHistogramListGeneratorOutput.txt
        10 1
)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbBasicFiltersTests12 ~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------     otbEdgeDetectorImageFilter   ----------------------------
ADD_TEST(bfTuEdgeDetectorImageFilterNew ${BASICFILTERS_TESTS12}
         otbEdgeDetectorImageFilterNew
         )

ADD_TEST(bfTvEdgeDetectorImageFilter ${BASICFILTERS_TESTS12}
--compare-image ${NOTOL}
            ${BASELINE}/bfTvEdgeDetectorImageFilter.tif
            ${TEMP}/bfTvEdgeDetectorImageFilter.tif
         otbEdgeDetectorImageFilter
         ${INPUTDATA}/poupees_sub_c1.png
         ${TEMP}/bfTvEdgeDetectorImageFilter.tif
         0   # lower threshold
         255 # upper  threshold
)

# -------     otbChangeLabelImageFilter   ----------------------------
ADD_TEST(bfTuChangeLabelImageFilterNew ${BASICFILTERS_TESTS12}
   otbChangeLabelImageFilterNew
   )

ADD_TEST(bfTvChangeLabelImageFilter ${BASICFILTERS_TESTS12}
--compare-image ${NOTOL}
      ${BASELINE}/bfChangeLabelImageFilter.tif
      ${TEMP}/bfChangeLabelImageFilter.tif
   otbChangeLabelImageFilter
 ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
 #  ${INPUTDATA}/poupees_sub_c1.png
  ${TEMP}/bfChangeLabelImageFilter.tif
 # 255   # Mask value
  150   # lower threshold
  255   # upper  threshold
)

# -----  otbImageAndVectorImageOperationFilter  --------------------------------------
ADD_TEST(bfTuImageAndVectorImageOperationFilterNew ${BASICFILTERS_TESTS12}
        otbImageAndVectorImageOperationFilterNew)

ADD_TEST(bfTvImageAndVectorImageOperationFilterTest ${BASICFILTERS_TESTS12}
        --compare-n-images ${NOTOL} 4
                ${TEMP}/bfTvImageAndVectorImageOperationFilterTestAdd.tif
                ${BASELINE}/bfTvImageAndVectorImageOperationFilterTestAdd.tif
                ${TEMP}/bfTvImageAndVectorImageOperationFilterTestSub.tif
                ${BASELINE}/bfTvImageAndVectorImageOperationFilterTestSub.tif
                ${TEMP}/bfTvImageAndVectorImageOperationFilterTestMul.tif
                ${BASELINE}/bfTvImageAndVectorImageOperationFilterTestMul.tif
                ${TEMP}/bfTvImageAndVectorImageOperationFilterTestDiv.tif
                ${BASELINE}/bfTvImageAndVectorImageOperationFilterTestDiv.tif
        otbImageAndVectorImageOperationFilterTest
                ${INPUTDATA}/checkerboard_128_128.png
                ${INPUTDATA}/poupees_sub.png
                ${TEMP}/bfTvImageAndVectorImageOperationFilterTestAdd.tif
                ${TEMP}/bfTvImageAndVectorImageOperationFilterTestSub.tif
                ${TEMP}/bfTvImageAndVectorImageOperationFilterTestMul.tif
                ${TEMP}/bfTvImageAndVectorImageOperationFilterTestDiv.tif
)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbBasicFiltersTests13 ~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# ----------------------------     otbNaryParserFunctorImageFilter   ----------------------------
ADD_TEST(bfTuBandMathImageFilter ${BASICFILTERS_TESTS13}
  otbBandMathImageFilterNew)

ADD_TEST(bfTvBandMathImageFilter ${BASICFILTERS_TESTS13}
  otbBandMathImageFilter)

ADD_TEST(bfTvBandMathImageFilterWithIdx ${BASICFILTERS_TESTS13}
  otbBandMathImageFilterWithIdx
  ${TEMP}/bfTvBandMathImageFilterWithIdx1.tif
  ${TEMP}/bfTvBandMathImageFilterWithIdx2.tif
)

ADD_TEST(bfTvComplexToIntensityFilterTest ${BASICFILTERS_TESTS13}
  otbComplexToIntensityFilterTest)

ADD_TEST(bfTvRealAndImaginaryImageToComplexImageFilterTest ${BASICFILTERS_TESTS13}
  otbRealAndImaginaryImageToComplexImageFilterTest
  ${INPUTDATA}/GomaAvant.png
  ${INPUTDATA}/GomaApres.png
)

ADD_TEST(bfTvRealImageToComplexImageFilterTest ${BASICFILTERS_TESTS13}
  otbRealImageToComplexImageFilterTest
  ${INPUTDATA}/GomaAvant.png
)

ADD_TEST(bfTvImaginaryImageToComplexImageFilterTest ${BASICFILTERS_TESTS13}
  otbImaginaryImageToComplexImageFilterTest
  ${INPUTDATA}/GomaAvant.png
)

ADD_TEST(bfTuComplexToVectorImageCastFilterNew ${BASICFILTERS_TESTS13}
  otbComplexToVectorImageCastFilterNew
)

ADD_TEST(bfTuComplexToVectorImageCastFilterTest ${BASICFILTERS_TESTS13}
  --compare-n-images ${NOTOL} 2
  ${TEMP}/bfTvComplexToVectorImageCastFilterTest.tif
  ${BASELINE}/bfTvComplexToVectorImageCastFilterTest.tif
  ${TEMP}/bfTvVectorComplexToVectorImageCastFilterTest.tif
  ${BASELINE}/bfTvComplexToVectorImageCastFilterTest.tif
  otbComplexToVectorImageCastFilterTest
    ${INPUTDATA}/monobandComplexDouble
    ${TEMP}/bfTvComplexToVectorImageCastFilterTest.tif
    ${TEMP}/bfTvVectorComplexToVectorImageCastFilterTest.tif
)

ADD_TEST(bfTvotbLog10ThresholdedImageFilterTest ${BASICFILTERS_TESTS13}
  --compare-ascii ${NOTOL}
  ${TEMP}/bfTvLog10ThresholdedImageFilterTest.txt
  ${BASELINE_FILES}/bfTvLog10ThresholdedImageFilter.txt
  otbLog10ThresholdedImageFilterTest
    ${TEMP}/bfTvLog10ThresholdedImageFilterTest.txt
)


ADD_TEST(bfTuMatrixImageFilterNew ${BASICFILTERS_TESTS13}
  otbMatrixImageFilterNew
)

ADD_TEST(bfTvMatrixImageFilterTest_1 ${BASICFILTERS_TESTS13}
  --compare-image ${NOTOL}
  ${TEMP}/bfTvMatrixImageFilterTest_1.tif
  ${BASELINE}/bfTvMatrixImageFilterTest_1.tif
  otbMatrixImageFilterTest
    ${INPUTDATA}/poupees.tif
    ${TEMP}/bfTvMatrixImageFilterTest_1.tif
    1
)

ADD_TEST(bfTvMatrixImageFilterTest_2 ${BASICFILTERS_TESTS13}
  --compare-image ${NOTOL}
  ${TEMP}/bfTvMatrixImageFilterTest_2.tif
  ${BASELINE}/bfTvMatrixImageFilterTest_2.tif
  otbMatrixImageFilterTest
    ${INPUTDATA}/poupees.tif
    ${TEMP}/bfTvMatrixImageFilterTest_2.tif
    2
)


ADD_TEST(bfTvMatrixImageFilterTest_3 ${BASICFILTERS_TESTS13}
  --compare-image ${NOTOL}
  ${TEMP}/bfTvMatrixImageFilterTest_3.tif
  ${BASELINE}/bfTvMatrixImageFilterTest_3.tif
  otbMatrixImageFilterTest
    ${INPUTDATA}/poupees.tif
    ${TEMP}/bfTvMatrixImageFilterTest_3.tif
    3
)



ADD_TEST(bfTuConcatenateScalarValueImageFilterNew ${BASICFILTERS_TESTS13}
  otbConcatenateScalarValueImageFilterNew )

ADD_TEST(bfTvConcatenateScalarValueImageFilterTest ${BASICFILTERS_TESTS13}
  otbConcatenateScalarValueImageFilterTest )

# ----------------------------     otbMaskMuParserFilter   ----------------------------

ADD_TEST(bfTuMaskMuParserFilterNew ${BASICFILTERS_TESTS13}
  otbMaskMuParserFilterNew
)

ADD_TEST(bfTvMaskMuParserFilterTest ${BASICFILTERS_TESTS13}
  --compare-image ${NOTOL}
  ${TEMP}/bfTvMaskMuParserFilterTest.tif
  ${BASELINE}/bfTvMaskMuParserFilterTest.tif
        otbMaskMuParserFilterTest
    ${INPUTDATA}/ROI_QB_MUL_4.tif
    ${TEMP}/bfTvMaskMuParserFilterTest.tif
    "(b1>100)*(b2>120)"
)

ADD_TEST(bfTuHorizontalSobelVectorImageFilterNew
         ${BASICFILTERS_TESTS13}
         otbHorizontalSobelVectorImageFilterNewTest)

ADD_TEST(bfTvHorizontalSobelVectorImageFilter
         ${BASICFILTERS_TESTS13}
           --compare-image ${EPSILON_7}
             ${BASELINE}/bfTvHorizontalSobelVectorImageFilter.tif
             ${TEMP}/bfTvHorizontalSobelVectorImageFilter.tif
         otbHorizontalSobelVectorImageFilterTest
		 -in ${INPUTDATA}/cupriteSubHsi.tif
		 -out ${TEMP}/bfTvHorizontalSobelVectorImageFilter.tif)

ADD_TEST(bfTuVerticalSobelVectorImageFilterNew
         ${BASICFILTERS_TESTS13}
         otbVerticalSobelVectorImageFilterNewTest)

ADD_TEST(bfTvVerticalSobelVectorImageFilter
         ${BASICFILTERS_TESTS13}
           --compare-image ${EPSILON_7}
             ${BASELINE}/bfTvVerticalSobelVectorImageFilter.tif
             ${TEMP}/bfTvVerticalSobelVectorImageFilter.tif
         otbVerticalSobelVectorImageFilterTest
		 -in ${INPUTDATA}/cupriteSubHsi.tif
		 -out ${TEMP}/bfTvVerticalSobelVectorImageFilter.tif)

ADD_TEST(bfTuSobelVectorImageFilterNew
         ${BASICFILTERS_TESTS13}
         otbSobelVectorImageFilterNewTest)

ADD_TEST(bfTvSobelVectorImageFilter
         ${BASICFILTERS_TESTS13}
           --compare-image ${EPSILON_7}
             ${BASELINE}/bfTvSobelVectorImageFilter.tif
             ${TEMP}/bfTvSobelVectorImageFilter.tif
         otbSobelVectorImageFilterTest
		 -in ${INPUTDATA}/cupriteSubHsi.tif
		 -out ${TEMP}/bfTvSobelVectorImageFilter.tif)

ADD_TEST(bfTuLocalGradientVectorImageFilterNew
         ${BASICFILTERS_TESTS13}
         otbLocalGradientVectorImageFilterNewTest)

ADD_TEST(bfTvLocalGradientVectorImageFilter
         ${BASICFILTERS_TESTS13}
           --compare-image ${EPSILON_7}
             ${BASELINE}/bfTvLocalGradientVectorImageFilter.tif
             ${TEMP}/bfTvLocalGradientVectorImageFilter.tif
         otbLocalGradientVectorImageFilterTest
		 -in ${INPUTDATA}/cupriteSubHsi.tif
		 -out ${TEMP}/bfTvLocalGradientVectorImageFilter.tif)

ADD_TEST(bfTuLocalActivityVectorImageFilterNew
         ${BASICFILTERS_TESTS13}
         otbLocalActivityVectorImageFilterNewTest)

ADD_TEST(bfTvLocalActivityVectorImageFilter
         ${BASICFILTERS_TESTS13}
           --compare-image ${EPSILON_7}
             ${BASELINE}/bfTvLocalActivityVectorImageFilter.tif
             ${TEMP}/bfTvLocalActivityVectorImageFilter.tif
         otbLocalActivityVectorImageFilterTest
		 -in ${INPUTDATA}/cupriteSubHsi.tif
		 -out ${TEMP}/bfTvLocalActivityVectorImageFilter.tif)

ADD_TEST(bfTuNormalizeVectorImageFilter
         ${BASICFILTERS_TESTS13}
		 otbNormalizeVectorImageFilterNewTest)

ADD_TEST(bfTvNormalizeVectorImageFilter
         ${BASICFILTERS_TESTS13}
           --compare-image ${EPSILON_7}
             ${BASELINE}/bfTvNormalizeVectorImageFilter.tif
             ${TEMP}/bfTvNormalizeVectorImageFilter.tif
		 otbNormalizeVectorImageFilterTest
		 -in ${INPUTDATA}/cupriteSubHsi.tif
		 -out ${TEMP}/bfTvNormalizeVectorImageFilter.tif)


ADD_TEST(bfTuNormalizeVectorImageFilter
         ${BASICFILTERS_TESTS13}
     otbNormalizeVectorImageFilterNewTest)

ADD_TEST(bfTvNormalizeVectorImageFilter
         ${BASICFILTERS_TESTS13}
           --compare-image ${EPSILON_7}
             ${BASELINE}/bfTvNormalizeVectorImageFilter.tif
             ${TEMP}/bfTvNormalizeVectorImageFilter.tif
     otbNormalizeVectorImageFilterTest
     -in ${INPUTDATA}/cupriteSubHsi.tif
     -out ${TEMP}/bfTvNormalizeVectorImageFilter.tif)



ADD_TEST(bfTuProjectiveProjectionNew
         ${BASICFILTERS_TESTS13}
         otbProjectiveProjectionNew)

ADD_TEST(bfTvProjectiveProjectionTestHighSNR
         ${BASICFILTERS_TESTS13}
           --compare-image ${EPSILON_7}
             ${BASELINE}/bfTvProjectiveProjectionTestHighSNR.tif
             ${TEMP}/bfTvProjectiveProjectionTestHighSNR.tif
     otbProjectiveProjectionTestHighSNR
     ${INPUTDATA}/cupriteSubHsi.tif
     3
     ${TEMP}/bfTvProjectiveProjectionTestHighSNR.tif)


ADD_TEST(bfTuVectorImageToMatrixNew
         ${BASICFILTERS_TESTS13}
         otbVectorImageToMatrixNewTest)

ADD_TEST(bfTvVectorImageToMatrix
         ${BASICFILTERS_TESTS13}
         otbVectorImageToMatrixTest)

### PCA ###

ADD_TEST(bfTuPCAImageFilterNew
         ${BASICFILTERS_TESTS13}
         otbPCAImageFilterNewTest)

# Test input->PCA->inverse PCA
ADD_TEST(bfTvPCAImageFilter1
         ${BASICFILTERS_TESTS13}
         --compare-image ${EPSILON_7}
             ${INPUTDATA}/cupriteSubHsi.tif
             ${TEMP}/bfTvPCAImageFilter1Inv.tif
         otbPCAImageFilterTest
         -in ${INPUTDATA}/cupriteSubHsi.tif
         -out ${TEMP}/bfTvPCAImageFilter1.tif
         -inv ${TEMP}/bfTvPCAImageFilter1Inv.tif)

ADD_TEST(bfTvPCAImageFilter2
         ${BASICFILTERS_TESTS13}
         --compare-image ${EPSILON_7}
             ${INPUTDATA}/cupriteSubHsi.tif
             ${TEMP}/bfTvPCAImageFilter2Inv.tif
         otbPCAImageFilterTest
         -in ${INPUTDATA}/cupriteSubHsi.tif
         -out ${TEMP}/bfTvPCAImageFilter2.tif
         -inv ${TEMP}/bfTvPCAImageFilter2Inv.tif
         -norm)

# Test the direct transform
ADD_TEST(bfTvPCAImageFilter3
         ${BASICFILTERS_TESTS13}
         --compare-n-images ${EPSILON_7} 2
             ${BASELINE}/bfTvPCAImageFilter3.tif
             ${TEMP}/bfTvPCAImageFilter3.tif
             ${BASELINE}/bfTvPCAImageFilter3Inv.tif
             ${TEMP}/bfTvPCAImageFilter3Inv.tif
         otbPCAImageFilterTest
         -in ${INPUTDATA}/cupriteSubHsi.tif
         -out ${TEMP}/bfTvPCAImageFilter3.tif
         -inv ${TEMP}/bfTvPCAImageFilter3Inv.tif
         )

# Test the direct transform
ADD_TEST(bfTvPCAImageFilter3Norm
         ${BASICFILTERS_TESTS13}
         --compare-n-images ${EPSILON_7} 2
             ${BASELINE}/bfTvPCAImageFilter3Norm.tif
             ${TEMP}/bfTvPCAImageFilter3Norm.tif
             ${BASELINE}/bfTvPCAImageFilter3InvNorm.tif
             ${TEMP}/bfTvPCAImageFilter3InvNorm.tif
         otbPCAImageFilterTest
         -in ${INPUTDATA}/cupriteSubHsi.tif
         -out ${TEMP}/bfTvPCAImageFilter3Norm.tif
         -inv ${TEMP}/bfTvPCAImageFilter3InvNorm.tif
         -norm)


# Test the direct transform, reducing dimensionnality
ADD_TEST(bfTvPCAImageFilter4Norm
         ${BASICFILTERS_TESTS13}
         --compare-n-images ${EPSILON_7} 2
             ${BASELINE}/bfTvPCAImageFilter4Norm.tif
             ${TEMP}/bfTvPCAImageFilter4Norm.tif
             ${BASELINE}/bfTvPCAImageFilter4InvNorm.tif
             ${TEMP}/bfTvPCAImageFilter4InvNorm.tif
         otbPCAImageFilterTest
         -in ${INPUTDATA}/cupriteSubHsi.tif
         -out ${TEMP}/bfTvPCAImageFilter4Norm.tif
         -inv ${TEMP}/bfTvPCAImageFilter4InvNorm.tif
         -norm
         -n 4)

# Test the direct transform, reducing dimensionnality
ADD_TEST(bfTvPCAImageFilter4
         ${BASICFILTERS_TESTS13}
         --compare-n-images ${EPSILON_7} 2
             ${BASELINE}/bfTvPCAImageFilter4.tif
             ${TEMP}/bfTvPCAImageFilter4.tif
             ${BASELINE}/bfTvPCAImageFilter4Inv.tif
             ${TEMP}/bfTvPCAImageFilter4Inv.tif
         otbPCAImageFilterTest
         -in ${INPUTDATA}/cupriteSubHsi.tif
         -out ${TEMP}/bfTvPCAImageFilter4.tif
         -inv ${TEMP}/bfTvPCAImageFilter4Inv.tif
         -n 4)



### NAPCA ###

ADD_TEST(bfTuNAPCAImageFilterNew
         ${BASICFILTERS_TESTS13}
         otbNAPCAImageFilterNewTest)

# Test input->NAPCA->inverse NAPCA
ADD_TEST(bfTvNAPCAImageFilter1
         ${BASICFILTERS_TESTS13}
         --compare-image ${EPSILON_7}
             ${INPUTDATA}/cupriteSubHsi.tif
             ${TEMP}/bfTvNAPCAImageFilter1Inv.tif
         otbNAPCAImageFilterTest
         -in ${INPUTDATA}/cupriteSubHsi.tif
         -out ${TEMP}/bfTvNAPCAImageFilter1.tif
         -inv ${TEMP}/bfTvNAPCAImageFilter1Inv.tif)

ADD_TEST(bfTvNAPCAImageFilter2
         ${BASICFILTERS_TESTS13}
         --compare-image ${EPSILON_7}
             ${INPUTDATA}/cupriteSubHsi.tif
             ${TEMP}/bfTvNAPCAImageFilter2Inv.tif
         otbNAPCAImageFilterTest
         -in ${INPUTDATA}/cupriteSubHsi.tif
         -out ${TEMP}/bfTvNAPCAImageFilter2.tif
         -inv ${TEMP}/bfTvNAPCAImageFilter2Inv.tif
         -norm)

# Test the direct transform
ADD_TEST(bfTvNAPCAImageFilter3
         ${BASICFILTERS_TESTS13}
         --compare-n-images ${EPSILON_7} 2
             ${BASELINE}/bfTvNAPCAImageFilter3.tif
             ${TEMP}/bfTvNAPCAImageFilter3.tif
             ${BASELINE}/bfTvNAPCAImageFilter3Inv.tif
             ${TEMP}/bfTvNAPCAImageFilter3Inv.tif
         otbNAPCAImageFilterTest
         -in ${INPUTDATA}/cupriteSubHsi.tif
         -out ${TEMP}/bfTvNAPCAImageFilter3.tif
         -inv ${TEMP}/bfTvNAPCAImageFilter3Inv.tif
         -norm)


# Test the direct transform, reducing dimensionnality
ADD_TEST(bfTvNAPCAImageFilter4
         ${BASICFILTERS_TESTS13}
         --compare-n-images ${EPSILON_7} 2
             ${BASELINE}/bfTvNAPCAImageFilter4.tif
             ${TEMP}/bfTvNAPCAImageFilter4.tif
             ${BASELINE}/bfTvNAPCAImageFilter4Inv.tif
             ${TEMP}/bfTvNAPCAImageFilter4Inv.tif
         otbNAPCAImageFilterTest
         -in ${INPUTDATA}/cupriteSubHsi.tif
         -out ${TEMP}/bfTvNAPCAImageFilter4.tif
         -inv ${TEMP}/bfTvNAPCAImageFilter4Inv.tif
         -norm
         -n 4)


### MNF ###

# Test the direct transform, reducing dimensionnality
ADD_TEST(bfTvMNFImageFilter4
         ${BASICFILTERS_TESTS13}
         --compare-n-images ${EPSILON_7} 2
             ${BASELINE}/bfTvMNFImageFilter4.tif
             ${TEMP}/bfTvMNFImageFilter4.tif
             ${BASELINE}/bfTvMNFImageFilter4Inv.tif
             ${TEMP}/bfTvMNFImageFilter4Inv.tif
         otbMNFImageFilterTest
         -in ${INPUTDATA}/cupriteSubHsi.tif
         -out ${TEMP}/bfTvMNFImageFilter4.tif
         -inv ${TEMP}/bfTvMNFImageFilter4Inv.tif
         -norm
         -n 4)


# Test input->MNF->inverse MNF
ADD_TEST(bfTvMNFImageFilter1
         ${BASICFILTERS_TESTS13}
         --compare-image ${EPSILON_7}
             ${INPUTDATA}/cupriteSubHsi.tif
             ${TEMP}/bfTvMNFImageFilter1Inv.tif
         otbMNFImageFilterTest
         -in ${INPUTDATA}/cupriteSubHsi.tif
         -out ${TEMP}/bfTvMNFImageFilter1.tif
         -inv ${TEMP}/bfTvMNFImageFilter1Inv.tif)

ADD_TEST(bfTvMNFImageFilter2
         ${BASICFILTERS_TESTS13}
         --compare-image ${EPSILON_7}
             ${INPUTDATA}/cupriteSubHsi.tif
             ${TEMP}/bfTvMNFImageFilter2Inv.tif
         otbMNFImageFilterTest
         -in ${INPUTDATA}/cupriteSubHsi.tif
         -out ${TEMP}/bfTvMNFImageFilter2.tif
         -inv ${TEMP}/bfTvMNFImageFilter2Inv.tif
         -norm)

# Test the direct transform
ADD_TEST(bfTvMNFImageFilter3
         ${BASICFILTERS_TESTS13}
         --compare-n-images ${EPSILON_7} 2
             ${BASELINE}/bfTvMNFImageFilter3.tif
             ${TEMP}/bfTvMNFImageFilter3.tif
             ${BASELINE}/bfTvMNFImageFilter3Inv.tif
             ${TEMP}/bfTvMNFImageFilter3Inv.tif
         otbMNFImageFilterTest
         -in ${INPUTDATA}/cupriteSubHsi.tif
         -out ${TEMP}/bfTvMNFImageFilter3.tif
         -inv ${TEMP}/bfTvMNFImageFilter3Inv.tif
         -norm)


# Test the direct transform, reducing dimensionnality
ADD_TEST(bfTvMNFImageFilter4
         ${BASICFILTERS_TESTS13}
         --compare-n-images ${EPSILON_7} 2
             ${BASELINE}/bfTvMNFImageFilter4.tif
             ${TEMP}/bfTvMNFImageFilter4.tif
             ${BASELINE}/bfTvMNFImageFilter4Inv.tif
             ${TEMP}/bfTvMNFImageFilter4Inv.tif
         otbMNFImageFilterTest
         -in ${INPUTDATA}/cupriteSubHsi.tif
         -out ${TEMP}/bfTvMNFImageFilter4.tif
         -inv ${TEMP}/bfTvMNFImageFilter4Inv.tif
         -norm
         -n 4)

### FastICA ###
ADD_TEST(bfTuFastICAInternalOptimizerVectorImageFilterNew
     ${BASICFILTERS_TESTS13}
     otbFastICAInternalOptimizerVectorImageFilterNewTest)

ADD_TEST(bfTuFastICAImageFilterNew
    ${BASICFILTERS_TESTS13}
    otbFastICAImageFilterNewTest)

ADD_TEST(bfTvFastICAImageFilter
         ${BASICFILTERS_TESTS13}
           --compare-n-images ${EPSILON_7} 2
             ${BASELINE}/hyTvFastICAImageFilter.tif
             ${TEMP}/hyTvFastICAImageFilter.tif
             ${BASELINE}/hyTvFastICAImageFilterInv.tif
             ${TEMP}/hyTvFastICAImageFilterInv.tif
          otbFastICAImageFilterTest
          -in ${INPUTDATA}/cupriteSubHsi.tif
          -inv ${TEMP}/hyTvFastICAImageFilterInv.tif
          -out ${TEMP}/hyTvFastICAImageFilter.tif)

### SparceUnmixing related class

ADD_TEST(bfTuAngularProjectionImageFilterNew
	${BASICFILTERS_TESTS13}
	otbAngularProjectionImageFilterNew)

#ADD_TEST(bfTvAngularProjectionImageFilter
#	${BASICFILTERS_TESTS13}
#		--compare-image ${EPSILON_7}
#		${INPUTDATA}/xx
#		${TEMP}/yy
#		otbAngularProjectionImageFilterTest
#		-in ${INPUTDATA}/xx ${INPUTDATA}/yy
#		-out ${TEMP}/zz)

ADD_TEST(bfTuAngularProjectionSetImageFilterNew
	${BASICFILTERS_TESTS13}
	otbAngularProjectionSetImageFilterNew)

#ADD_TEST(bfTvAngularProjectionSetImageFilter
#	${BASICFILTERS_TESTS13}
#		--compare-image ${EPSILON_7}
#		${INPUTDATA}/xx
#		${TEMP}/yy
#		otbAngularProjectionSetImageFilterTest
#		-in ${INPUTDATA}/xx ${INPUTDATA}/yy ${INPUTDATA}/zz ${INPUTDATA}/zzz
#		-out ${TEMP}/zz)

ADD_TEST(bfTuSparseWvltToAngleMapperListFilterNew
	${BASICFILTERS_TESTS13}
	otbSparseWvltToAngleMapperListFilterNew)

#ADD_TEST(bfTvSparseWvltToAngleMapperListFilter
#	${BASICFILTERS_TESTS13}
#	otbSparseWvltToAngleMapperListFilterTest
#	-in ${INPUTDATA}/xx ${INPUTDATA}/yy
#	-th 10)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbBasicFiltersTests14 ~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# ----------------------------     otbConnectedComponentMuParserFunctor   ----------------------------

ADD_TEST(bfTvConnectedComponentMuParserFunctorTest ${BASICFILTERS_TESTS14}
  --compare-image ${NOTOL}
  ${TEMP}/bfTvConnectedComponentMuParserFunctorTest_1.tif
  ${BASELINE}/bfTvConnectedComponentMuParserFunctorTest_1.tif
        otbConnectedComponentMuParserFunctorTest
    ${INPUTDATA}/ROI_QB_MUL_4.tif
    ${TEMP}/bfTvConnectedComponentMuParserFunctorTest_1.tif
    "distance<40"
)



ADD_TEST(bfTvConnectedComponentMuParserFunctorTestMask ${BASICFILTERS_TESTS14}
  --compare-image ${NOTOL}
  ${TEMP}/bfTvConnectedComponentMuParserFunctorTest_2.tif
  ${BASELINE}/bfTvConnectedComponentMuParserFunctorTest_2.tif
        otbConnectedComponentMuParserFunctorTest
    ${INPUTDATA}/ROI_QB_MUL_4.tif
    ${TEMP}/bfTvConnectedComponentMuParserFunctorTest_2.tif
    "distance<40"
    ${INPUTDATA}/ROI_QB_MUL_4_Mask.tif
)

# ----------------------------     otbLabelObjectOpeningMuParserFilter   ----------------------------


ADD_TEST(bfTuLabelObjectOpeningMuParserFilterNew ${BASICFILTERS_TESTS14}
  otbLabelObjectOpeningMuParserFilterNew
)


ADD_TEST(bfTvLabelObjectOpeningMuParserFilterTest ${BASICFILTERS_TESTS14}
  --compare-image ${NOTOL}
  ${TEMP}/bfTvLabelObjectOpeningMuParserFilterTest.tif
  ${BASELINE}/bfTvLabelObjectOpeningMuParserFilterTest.tif
        otbLabelObjectOpeningMuParserFilterTest
    ${INPUTDATA}/ROI_QB_MUL_4.tif
    ${INPUTDATA}/ROI_QB_MUL_4_Label.tif
    ${TEMP}/bfTvLabelObjectOpeningMuParserFilterTest.tif
    "SHAPE_Elongation>8"
)

# -----------------------  otbVectorDataToRandomLineGenerator  -----------------------

ADD_TEST(bfTuVectorDataToRandomLineGeneratorNew ${BASICFILTERS_TESTS14}
  otbVectorDataToRandomLineGeneratorNew
)

ADD_TEST(bfTuVectorDataToRandomLineGenerator ${BASICFILTERS_TESTS14}
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/bfTvVectorDataToRandomLineGeneratorOutput.shp
  ${TEMP}/bfTvVectorDataToRandomLineGeneratorOutput.shp
  otbVectorDataToRandomLineGenerator
  ${INPUTDATA}/ObjectReco/Boats/maur_B010202_01Polygons.shp
  ${TEMP}/bfTvVectorDataToRandomLineGeneratorOutput.shp
)


# -----------------------  otbConcatenateVectorDataFilter  -----------------------

ADD_TEST(bfTuConcatenateVectorDataFilterNew ${BASICFILTERS_TESTS14}
  otbConcatenateVectorDataFilterNew
)

ADD_TEST(bfTvConcatenateVectorDataFilter ${BASICFILTERS_TESTS14}
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/otbConcatenateVectorDataFilter_Output.shp
  ${TEMP}/otbConcatenateVectorDataFilter_Output.shp
  otbConcatenateVectorDataFilter
  -in  ${INPUTDATA}/france_coastline.shp
       ${INPUTDATA}/waterways.shp
  -out ${TEMP}/otbConcatenateVectorDataFilter_Output.shp
)



# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbBasicFiltersTests15 ~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -----------------------  otbNDVIDataNodeFeatureFunction  -----------------------

ADD_TEST(bfTuNDVIDataNodeFeatureFunctionNew ${BASICFILTERS_TESTS15}
  otbNDVIDataNodeFeatureFunctionNew
)

ADD_TEST(bfTvNDVIDataNodeFeatureFunction_Line ${BASICFILTERS_TESTS15}
 --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/bfTvNDVIDataNodeFeatureFunctionOutput_Line.shp
  ${TEMP}/bfTvNDVIDataNodeFeatureFunctionOutput_Line.shp
  otbNDVIDataNodeFeatureFunction
          ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE_ROADS.shp
        ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE.TIF
        ${INPUTDATA}/DEM/srtm_directory
        ${TEMP}/bfTvNDVIDataNodeFeatureFunctionOutput_Line.shp
        0
)

ADD_TEST(bfTvNDVIDataNodeFeatureFunction_Polygon ${BASICFILTERS_TESTS15}
 --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/bfTvNDVIDataNodeFeatureFunctionOutput_Polygon.shp
  ${TEMP}/bfTvNDVIDataNodeFeatureFunctionOutput_Polygon.shp
  otbNDVIDataNodeFeatureFunction
          ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE_BUILDINGS.shp
        ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE.TIF
        ${INPUTDATA}/DEM/srtm_directory
        ${TEMP}/bfTvNDVIDataNodeFeatureFunctionOutput_Polygon.shp
        0
)


ADD_TEST(bfTvParserConditionDataNodeFeatureFunction_Line_NDVI ${BASICFILTERS_TESTS15}
 --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/bfTvNDVIDataNodeFeatureFunctionOutput_Line.shp
  ${TEMP}/bfTvParserConditionDataNodeFeatureFunction_Line_NDVI.shp
  otbParserConditionDataNodeFeatureFunction
    ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE_ROADS.shp
    ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE.TIF
    ${INPUTDATA}/DEM/srtm_directory
    ${TEMP}/bfTvParserConditionDataNodeFeatureFunction_Line_NDVI.shp
    "ndvi(b3,b4) > 0.047"
    0
)

ADD_TEST(bfTvParserConditionDataNodeFeatureFunction_Polygon_NDVI ${BASICFILTERS_TESTS15}
 --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/bfTvNDVIDataNodeFeatureFunctionOutput_Polygon.shp
  ${TEMP}/bfTvParserConditionDataNodeFeatureFunction_Polygon_NDVI.shp
  otbParserConditionDataNodeFeatureFunction
    ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE_BUILDINGS.shp
    ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE.TIF
    ${INPUTDATA}/DEM/srtm_directory
    ${TEMP}/bfTvParserConditionDataNodeFeatureFunction_Polygon_NDVI.shp
    "ndvi(b3,b4) > 0.047"
    0
)

# ------------------  otbSpectralAngleDataNodeFeatureFunction   ------------------
ADD_TEST(bfTuSpectralAngleDataNodeFeatureFunctionNew ${BASICFILTERS_TESTS15}
  otbSpectralAngleDataNodeFeatureFunctionNew
)

ADD_TEST(bfTvSpectralAngleDataNodeFeatureFunction_Line ${BASICFILTERS_TESTS15}
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/bfTvSpectralAngleDataNodeFeatureFunctionOutput_Line.shp
  ${TEMP}/bfTvSpectralAngleDataNodeFeatureFunctionOutput_Line.shp
         otbSpectralAngleDataNodeFeatureFunction
          ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE_ROADS.shp
        ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE.TIF
        ${INPUTDATA}/DEM/srtm_directory
        ${TEMP}/bfTvSpectralAngleDataNodeFeatureFunctionOutput_Line.shp
        0
)

ADD_TEST(bfTvSpectralAngleDataNodeFeatureFunction_Polygon ${BASICFILTERS_TESTS15}
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/bfTvSpectralAngleDataNodeFeatureFunctionOutput_Polygon.shp
  ${TEMP}/bfTvSpectralAngleDataNodeFeatureFunctionOutput_Polygon.shp
         otbSpectralAngleDataNodeFeatureFunction
          ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE_BUILDINGS.shp
          ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE.TIF
          ${INPUTDATA}/DEM/srtm_directory
          ${TEMP}/bfTvSpectralAngleDataNodeFeatureFunctionOutput_Polygon.shp
          0 )

# ------------------  otbRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction   ------------------
ADD_TEST(bfTuRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunctionNew ${BASICFILTERS_TESTS15}
  otbRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunctionNew
)

ADD_TEST(bfTvRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction_Line ${BASICFILTERS_TESTS15}
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/bfTvRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunctionOutput_Line.shp
  ${TEMP}/bfTvRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunctionOutput_Line.shp
  otbRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction
    ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE_ROADS.shp
    ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE.TIF
    ${INPUTDATA}/DEM/srtm_directory
    ${TEMP}/bfTvRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunctionOutput_Line.shp
    0 )

ADD_TEST(bfTvRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction_Polygon ${BASICFILTERS_TESTS15}
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/bfTvRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunctionOutput_Polygon.shp
  ${TEMP}/bfTvRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunctionOutput_Polygon.shp
  otbRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction
    ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE_BUILDINGS.shp
    ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE.TIF
    ${INPUTDATA}/DEM/srtm_directory
    ${TEMP}/bfTvRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunctionOutput_Polygon.shp
    0 )

# ---------------------  otDBOverlapDataNodeFeatureFunction  ---------------------
ADD_TEST(bfTuDBOverlapDataNodeFeatureFunctionNew ${BASICFILTERS_TESTS15}
  otbDBOverlapDataNodeFeatureFunctionNew
)

ADD_TEST(bfTvDBOverlapDataNodeFeatureFunction ${BASICFILTERS_TESTS15}
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/bfTvDBOverlapDataNodeFeatureFunctionOutput.shp
  ${TEMP}/bfTvDBOverlapDataNodeFeatureFunctionOutput.shp
         otbDBOverlapDataNodeFeatureFunction
          ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE_ROADS.shp
        ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE_BUILDINGS.shp
        ${TEMP}/bfTvDBOverlapDataNodeFeatureFunctionOutput.shp
        0
)

# ---------------------  otbMaximumAutocorrelationFactorImageFilter  ---------------------
ADD_TEST(bfTuMaximumAutocorrelationFactorImageFilterNew ${BASICFILTERS_TESTS15}
  otbMaximumAutocorrelationFactorImageFilterNew
)

ADD_TEST(bfTvMaximumAutocorrelationFactorImageFilter ${BASICFILTERS_TESTS15}
   --compare-image 0.0001
   ${BASELINE}/bfTvMaximumAutocorrelationFactorImageFilterOutput.tif
   ${TEMP}/bfTvMaximumAutocorrelationFactorImageFilterOutput.tif
   otbMaximumAutocorrelationFactorImageFilter
   ${INPUTDATA}/cupriteSubHsi.tif
   ${TEMP}/bfTvMaximumAutocorrelationFactorImageFilterOutput.tif
)

# ---------------------    otbThresholdVectorImageFilter---------------------
ADD_TEST(bfTuThresholdVectorImageFilterNew ${BASICFILTERS_TESTS15}
  otbThresholdVectorImageFilterNew
)

ADD_TEST(bfTvThresholdVectorImageFilterTest ${BASICFILTERS_TESTS15}
   --compare-image 0.0001
   ${BASELINE}/bfTvThresholdVectorImageFilterTest_Output.tif
   ${TEMP}/bfTvThresholdVectorImageFilterTest_Output.tif
   otbThresholdVectorImageFilterTest
   ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500.tif
   ${TEMP}/bfTvThresholdVectorImageFilterTest_Output.tif
)

# ---------------------    otbClampVectorImageFilter---------------------
ADD_TEST(bfTuClampVectorImageFilterNew ${BASICFILTERS_TESTS15}
  otbClampVectorImageFilterNew
)

ADD_TEST(bfTvClampVectorImageFilterTest ${BASICFILTERS_TESTS15}
   --compare-image 0.0001
   ${BASELINE}/bfTvClampVectorImageFilterTest_Output.tif
   ${TEMP}/bfTvClampVectorImageFilterTest_Output.tif
   otbClampVectorImageFilterTest
   ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500.tif
   ${TEMP}/bfTvClampVectorImageFilterTest_Output.tif
)

# ---------------------    otbClampImageFilter---------------------
ADD_TEST(bfTuClampImageFilterNew ${BASICFILTERS_TESTS15}
  otbClampImageFilterNew
)

ADD_TEST(bfTvClampImageFilterTest ${BASICFILTERS_TESTS15}
   --compare-image 0.0001
   ${BASELINE}/bfTvClampImageFilterTest_Output.tif
   ${TEMP}/bfTvClampImageFilterTest_Output.tif
   otbClampImageFilterTest
   ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
   ${TEMP}/bfTvClampImageFilterTest_Output.tif
)


#-------- TileImageFilter --------------------------
ADD_TEST(bfTuTileImageFilterNew ${BASICFILTERS_TESTS15}
  otbTileImageFilterNew
)


ADD_TEST(bfTvTileImageFilter ${BASICFILTERS_TESTS15}
  --compare-image ${NOTOL}
   ${BASELINE}/bfTvTileImageFilterOutput.tif
   ${TEMP}/bfTvTileImageFilterOutput.tif
  otbTileImageFilter
  2 2
  ${TEMP}/bfTvTileImageFilterOutput.tif
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${INPUTDATA}/ROI_QB_MUL_4.tif
)


# A enrichir
SET(BasicFilters_SRCS1
otbBasicFiltersTests1.cxx
otbLeeFilter.cxx
#otbFrostFilterTest.cxx
otbFrostFilterNew.cxx
otbFrostFilter.cxx
otbImageToPointSetFilterTest.cxx
otbOpeningClosingMorphologicalFilterNew.cxx
otbOpeningClosingMorphologicalFilter.cxx
otbClosingOpeningMorphologicalFilterNew.cxx
otbClosingOpeningMorphologicalFilter.cxx
otbBinaryImageMinimalBoundingRegionCalculatorNew.cxx
otbBinaryImageMinimalBoundingRegionCalculator.cxx
otbMeanFunctorImageTest.cxx
otbTransformPointSetFilterTest.cxx
otbRandomPointSetSourceTest.cxx
otbPointSetExtractROITest.cxx
)

# A enrichir
SET(BasicFilters_SRCS2
otbBasicFiltersTests2.cxx
otbLogPolarTransformNew.cxx
otbLogPolarTransform.cxx
otbLogPolarTransformResample.cxx
otbInverseLogPolarTransformNew.cxx
otbInverseLogPolarTransform.cxx
otbInverseLogPolarTransformResample.cxx
otbStreamingShrinkImageFilterNew.cxx
otbStreamingShrinkImageFilter.cxx
otbSpatialObjectToImageDrawingFilterNew.cxx
otbSpatialObjectToImageDrawingFilter.cxx
)

# A enrichir
SET(BasicFilters_SRCS3
otbBasicFiltersTests3.cxx
otbImportGeoInformationImageFilterNew.cxx
otbImportGeoInformationImageFilter.cxx
otbImportGeoInformationImageFilterWithKeywordList.cxx
otbSpectralAngleDistanceImageFilterNew.cxx
otbSpectralAngleDistanceImageFilter.cxx
otbVectorRescaleIntensityImageFilterNew.cxx
otbVectorRescaleIntensityImageFilter.cxx
otbVectorImageToImageListFilterNew.cxx
otbVectorImageToImageListFilter.cxx
otbImageListToSingleImageFilterTest.cxx
otbShiftScaleVectorImageFilterNew.cxx
otbShiftScaleVectorImageFilterTest.cxx
)

# A enrichir
SET(BasicFilters_SRCS4
otbBasicFiltersTests4.cxx
otbImageListToImageListApplyFilterNew.cxx
otbImageListToImageListApplyFilter.cxx
otbPerBandVectorImageFilterNew.cxx
otbPerBandVectorImageFilterWithMeanFilter.cxx
otbPerBandVectorImageFilterWithSobelFilter.cxx
otbStreamingStatisticsImageFilterNew.cxx
otbStreamingStatisticsImageFilter.cxx
otbStreamingCompareImageFilter.cxx
otbVectorImageTo3DScalarImageFilterNew.cxx
otbVectorImageTo3DScalarImageFilter.cxx
)

# A enrichir
SET(BasicFilters_SRCS5
otbBasicFiltersTests5.cxx
otbStreamingTraits.cxx
otbStreamingResampleImageFilterNew.cxx
otbStreamingResampleImageFilter.cxx
otbStreamingResampleImageFilterWithAffineTransform.cxx
otbStreamingResampleImageFilterCompareWithITK.cxx
otbStreamingMinMaxImageFilterNew.cxx
otbStreamingMinMaxImageFilter.cxx
otbStreamingMinMaxVectorImageFilterNew.cxx
otbStreamingMinMaxVectorImageFilter.cxx
otbStreamingStatisticsVectorImageFilterNew.cxx
otbStreamingStatisticsVectorImageFilter.cxx
otbMatrixTransposeMatrixImageFilterNew.cxx
otbMatrixTransposeMatrixImageFilter.cxx
otbUnaryImageFunctorWithVectorImageFilterNew.cxx
otbUnaryImageFunctorWithVectorImageFilter.cxx
otbUnaryFunctorImageFilterNew.cxx
otbPrintableImageFilterNew.cxx
otbPrintableImageFilter.cxx
otbPrintableImageFilterWithMask.cxx

)

# A enrichir
SET(BasicFilters_SRCS6
otbBasicFiltersTests6.cxx
otbBSplineDecompositionImageFilterNew.cxx
otbBSplineDecompositionImageFilter.cxx
otbBSplineInterpolateImageFunctionNew.cxx
otbBSplineInterpolateImageFunction.cxx
otbProlateInterpolateImageFunctionNew.cxx
otbProlateInterpolateImageFunction.cxx
otbWindowedSincInterpolateImageFunctionBaseNew.cxx
otbWindowedSincInterpolateImageGaussianFunctionNew.cxx
otbWindowedSincInterpolateImageGaussianFunction.cxx
otbWindowedSincInterpolateImageHammingFunctionNew.cxx
otbWindowedSincInterpolateImageHammingFunction.cxx
otbBCOInterpolateImageFunction.cxx
)

# A enrichir
SET(BasicFilters_SRCS7
otbBasicFiltersTests7.cxx
otbWindowedSincInterpolateImageCosineFunctionNew.cxx
otbWindowedSincInterpolateImageCosineFunction.cxx
otbWindowedSincInterpolateImageWelchFunctionNew.cxx
otbWindowedSincInterpolateImageWelchFunction.cxx
otbWindowedSincInterpolateImageLanczosFunctionNew.cxx
otbWindowedSincInterpolateImageLanczosFunction.cxx
otbWindowedSincInterpolateImageBlackmanFunctionNew.cxx
otbWindowedSincInterpolateImageBlackmanFunction.cxx
otbProlateValidationTest.cxx
otbVectorImageToAmplitudeImageFilterNew.cxx
otbVectorImageToAmplitudeImageFilter.cxx
otbVectorImageToIntensityImageFilterNew.cxx
otbVectorImageToIntensityImageFilter.cxx
)

# A enrichir
SET(BasicFilters_SRCS8
otbBasicFiltersTests8.cxx
otbLabelizeConnectedThresholdImageFilterNew.cxx
otbLabelizeConnectedThresholdImageFilter.cxx
otbLabelizeNeighborhoodConnectedImageFilterNew.cxx
otbLabelizeNeighborhoodConnectedImageFilter.cxx
otbLabelizeConfidenceConnectedImageFilterNew.cxx
otbLabelizeConfidenceConnectedImageFilter.cxx
otbEuclideanDistanceWithMissingValueNew.cxx
otbEuclideanDistanceWithMissingValue.cxx
otbConvolutionImageFilterNew.cxx
otbConvolutionImageFilter.cxx
otbScalarToRainbowRGBPixelFunctorNew.cxx
otbScalarToRainbowRGBPixelFunctor.cxx
otbAmplitudePhaseToRGBFunctorNew.cxx
otbAmplitudePhaseToRGBFunctor.cxx
otbVarianceImageFilterNew.cxx
otbVarianceImageFilter.cxx
otbBoxAndWhiskerImageFilterNew.cxx
otbBoxAndWhiskerImageFilter.cxx
otbFlexibleDistanceWithMissingValueNew.cxx
otbFlexibleDistanceWithMissingValue.cxx
)

# A enrichir
SET(BasicFilters_SRCS9
otbBasicFiltersTests9.cxx
otbContinuousMinimumMaximumImageCalculatorNew.cxx
otbContinuousMinimumMaximumImageCalculatorTest.cxx
otbMeanShiftImageFilterNew.cxx
otbMeanShiftImageFilter.cxx
otbMeanShiftSmoothingImageFilterNew.cxx
otbMeanShiftSmoothingImageFilter.cxx
otbMeanShiftSmoothingImageFilterThreading.cxx
otbMeanShiftVectorImageFilterNew.cxx
otbMeanShiftVectorImageFilter.cxx
otbLabelImageRegionMergingFilter.cxx
otbLabelImageRegionPruningFilter.cxx
otbFunctionToImageFilterNew.cxx
otbFunctionToImageFilter.cxx
otbBinaryImageDensityFunctionNew.cxx
otbBinaryImageDensityFunction.cxx
otbBinaryImageToDensityImageFilterNew.cxx
otbBinaryImageToDensityImageFilter.cxx
otbEdgeDensityImageFilterNew.cxx
otbEdgeDensityImageFilter.cxx
)

# A enrichir
SET(BasicFilters_SRCS10
otbBasicFiltersTests10.cxx
otbGaborFilterGeneratorNew.cxx
otbGaborFilterGenerator.cxx
otbPolygonCompacityFunctor.cxx
otbPathLengthFunctor.cxx
otbClosePathFunctor.cxx
otbPersistentVectorizationFilterNew.cxx
otbPersistentVectorizationFilter.cxx
)

IF(USE_FFTWD)
SET(BasicFilters_SRCS10
${BasicFilters_SRCS10}
otbOverlapSaveConvolutionImageFilterNew.cxx
otbOverlapSaveConvolutionImageFilter.cxx
otbCompareOverlapSaveAndClassicalConvolutionWithGaborFilter.cxx
)
ENDIF(USE_FFTWD)

SET(BasicFilters_SRCS11
otbBasicFiltersTests11.cxx
otbExtractROIResample.cxx
#otbPointSetFunctionNew.cxx
otbPointSetDensityFunctionNew.cxx
otbPointSetDensityFunctionTest.cxx
otbPointSetDensityGaussianFunctionNew.cxx
otbPointSetDensityGaussianFunctionTest.cxx
otbPointSetDensityEpanechnikovFunctionNew.cxx
otbPointSetDensityEpanechnikovFunctionTest.cxx
otbPointSetToDensityImageFilterNew.cxx
otbPointSetToDensityImageFilterTest.cxx
otbKeyPointDensityImageFilterNew.cxx
otbKeyPointDensityImageFilterTest.cxx
otbImagePCAShapeModelEstimatorTest.cxx
otbFunctionWithNeighborhoodToImageFilterNew.cxx
otbFunctionWithNeighborhoodToImageFilter.cxx
otbStreamingInnerProductVectorImageFilterNew.cxx
otbStreamingInnerProductVectorImageFilter.cxx
otbEstimateInnerProductPCAImageFilterNew.cxx
otbEstimateInnerProductPCAImageFilter.cxx
otbNormalizeInnerProductPCAImageFilterNew.cxx
otbNormalizeInnerProductPCAImageFilter.cxx
otbInnerProductPCAImageFilter.cxx
otbInnerProductPCAImageFilterNew.cxx
otbListSampleToVariableDimensionHistogramGeneratorNew.cxx
otbListSampleToVariableDimensionHistogramGenerator.cxx
otbListSampleToHistogramListGeneratorNew.cxx
otbListSampleToHistogramListGenerator.cxx
)

SET(BasicFilters_SRCS12
otbBasicFiltersTests12.cxx
otbEdgeDetectorImageFilter.cxx
otbEdgeDetectorImageFilterNew.cxx
otbChangeLabelImageFilterNew.cxx
otbChangeLabelImageFilter.cxx
otbImageAndVectorImageOperationFilterNew.cxx
otbImageAndVectorImageOperationFilterTest.cxx
)

SET(BasicFilters_SRCS13
otbBasicFiltersTests13.cxx
otbBandMathImageFilter.cxx
otbComplexToIntensityFilterTest.cxx
otbRealAndImaginaryImageToComplexImageFilterTest.cxx
otbRealImageToComplexImageFilterTest.cxx
otbImaginaryImageToComplexImageFilterTest.cxx
otbComplexToVectorImageCastFilter.cxx
otbLog10ThresholdedImageFilterTest.cxx
otbMatrixImageFilterTest.cxx
otbMaskMuParserFilterTest.cxx
otbMaskMuParserFilterNew.cxx
otbProjectiveProjection.cxx
otbNormalizeVectorImageFilter.cxx
otbHorizontalSobelVectorImageFilter.cxx
otbVerticalSobelVectorImageFilter.cxx
otbSobelVectorImageFilter.cxx
otbLocalActivityVectorImageFilter.cxx
otbLocalGradientVectorImageFilter.cxx
otbPCAImageFilter.cxx
otbMNFImageFilter.cxx
otbNAPCAImageFilter.cxx
otbFastICAImageFilter.cxx
otbVectorImageToMatrixImageFilter.cxx
otbConcatenateScalarValueImageFilterTest.cxx
otbAngularProjectionImageFilterNew.cxx
otbAngularProjectionImageFilter.cxx
otbAngularProjectionSetImageFilterNew.cxx
otbAngularProjectionSetImageFilter.cxx
otbSparseWvltToAngleMapperListFilterNew.cxx
otbSparseWvltToAngleMapperListFilter.cxx
)

SET(BasicFilters_SRCS14
otbBasicFiltersTests14.cxx
otbConnectedComponentMuParserFunctorTest.cxx
otbLabelObjectOpeningMuParserFilterNew.cxx
otbLabelObjectOpeningMuParserFilterTest.cxx
otbVectorDataToRandomLineGenerator.cxx
otbConcatenateVectorDataFilter.cxx
)
SET(BasicFilters_SRCS15
otbBasicFiltersTests15.cxx
otbNDVIDataNodeFeatureFunction.cxx
otbSpectralAngleDataNodeFeatureFunction.cxx
otbDBOverlapDataNodeFeatureFunction.cxx
otbRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction.cxx
otbParserConditionDataNodeFeatureFunction.cxx
otbMaximumAutocorrelationFactorImageFilter.cxx
otbThresholdVectorImageFilter.cxx
otbClampVectorImageFilter.cxx
otbClampImageFilter.cxx
otbTileImageFilter.cxx
)

OTB_ADD_EXECUTABLE(otbBasicFiltersTests1 "${BasicFilters_SRCS1}" "OTBBasicFilters;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbBasicFiltersTests2 "${BasicFilters_SRCS2}" "OTBBasicFilters;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbBasicFiltersTests3 "${BasicFilters_SRCS3}" "OTBBasicFilters;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbBasicFiltersTests4 "${BasicFilters_SRCS4}" "OTBBasicFilters;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbBasicFiltersTests5 "${BasicFilters_SRCS5}" "OTBBasicFilters;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbBasicFiltersTests6 "${BasicFilters_SRCS6}" "OTBBasicFilters;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbBasicFiltersTests7 "${BasicFilters_SRCS7}" "OTBBasicFilters;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbBasicFiltersTests8 "${BasicFilters_SRCS8}" "OTBBasicFilters;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbBasicFiltersTests9 "${BasicFilters_SRCS9}" "OTBBasicFilters;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbBasicFiltersTests10 "${BasicFilters_SRCS10}" "OTBBasicFilters;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbBasicFiltersTests11 "${BasicFilters_SRCS11}" "OTBBasicFilters;ITKAlgorithms;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbBasicFiltersTests12 "${BasicFilters_SRCS12}" "OTBBasicFilters;ITKAlgorithms;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbBasicFiltersTests13 "${BasicFilters_SRCS13}" "OTBBasicFilters;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbBasicFiltersTests14 "${BasicFilters_SRCS14}" "OTBBasicFilters;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbBasicFiltersTests15 "${BasicFilters_SRCS15}" "OTBBasicFilters;OTBIO;OTBTesting;OTBOGRAdapters")

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
