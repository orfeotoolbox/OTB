
if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


set(BASICFILTERS_TESTS1 ${CXX_TEST_PATH}/otbBasicFiltersTests1)
set(BASICFILTERS_TESTS2 ${CXX_TEST_PATH}/otbBasicFiltersTests2)
set(BASICFILTERS_TESTS3 ${CXX_TEST_PATH}/otbBasicFiltersTests3)
set(BASICFILTERS_TESTS4 ${CXX_TEST_PATH}/otbBasicFiltersTests4)
set(BASICFILTERS_TESTS5 ${CXX_TEST_PATH}/otbBasicFiltersTests5)
set(BASICFILTERS_TESTS6 ${CXX_TEST_PATH}/otbBasicFiltersTests6)
set(BASICFILTERS_TESTS7 ${CXX_TEST_PATH}/otbBasicFiltersTests7)
set(BASICFILTERS_TESTS8 ${CXX_TEST_PATH}/otbBasicFiltersTests8)
set(BASICFILTERS_TESTS9 ${CXX_TEST_PATH}/otbBasicFiltersTests9)
set(BASICFILTERS_TESTS10 ${CXX_TEST_PATH}/otbBasicFiltersTests10)
set(BASICFILTERS_TESTS11 ${CXX_TEST_PATH}/otbBasicFiltersTests11)
set(BASICFILTERS_TESTS12 ${CXX_TEST_PATH}/otbBasicFiltersTests12)
set(BASICFILTERS_TESTS13 ${CXX_TEST_PATH}/otbBasicFiltersTests13)
set(BASICFILTERS_TESTS14 ${CXX_TEST_PATH}/otbBasicFiltersTests14)
set(BASICFILTERS_TESTS15 ${CXX_TEST_PATH}/otbBasicFiltersTests15)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbBasicFiltersTests1 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

add_test(bfTvFiltreLee1CanalPoupees ${BASICFILTERS_TESTS1}
  --compare-image ${EPSILON_7}  ${BASELINE}/bfFiltreLee_05_05_04.tif
                          ${TEMP}/bfFiltreLee_05_05_04.tif
        otbLeeFilter
        ${INPUTDATA}/GomaAvant.png                  #poupees_1canal.c1.hdr
        ${TEMP}/bfFiltreLee_05_05_04.tif
        05 05 4.0)

add_test(bfTvFiltreLee ${BASICFILTERS_TESTS1}
  --compare-image ${EPSILON_7}  ${BASELINE}/bfFiltreLee_05_05_12.tif
                           ${TEMP}/bfFiltreLee_05_05_12.tif
        otbLeeFilter
        ${INPUTDATA}/GomaAvant.png    #poupees.hdr
        ${TEMP}/bfFiltreLee_05_05_12.tif
        05 05 12.0)

#
# Filtre de FROST:
#
add_test(bfTuFrostFilterNew ${BASICFILTERS_TESTS1}
         otbFrostFilterNew)

add_test(bfTvFrostFilter ${BASICFILTERS_TESTS1}
  --compare-image ${EPSILON_7}  ${BASELINE}/bfFiltreFrost_poupees_05_05_01.tif
                          ${TEMP}/bfFiltreFrost_poupees_05_05_01.tif
        otbFrostFilter
        ${INPUTDATA}/GomaAvant.png   #poupees.hdr
        ${TEMP}/bfFiltreFrost_poupees_05_05_01.tif
        05 05 0.1)

# -------            otb::PointSetFilter   ------------------------------

add_test(bfTuImageToPointSetFilterTest ${BASICFILTERS_TESTS1}
         otbImageToPointSetFilterTest)


# -------            otb::OpeningClosingMorphologicalFilter   ------------------------------

add_test(bfTuOpeningClosingFilterNew ${BASICFILTERS_TESTS1}
        otbOpeningClosingMorphologicalFilterNew )

add_test(bfTvOpeningClosingFilter ${BASICFILTERS_TESTS1}
   --compare-image ${NOTOL}  ${BASELINE}/bfFiltreOpeningClosing_1.tif
                           ${TEMP}/bfFiltreOpeningClosing_1.tif
        otbOpeningClosingMorphologicalFilter
        ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
        ${TEMP}/bfFiltreOpeningClosing_1.tif
        1)

# -------            otb::ClosingOpeningMorphologicalFilter   ------------------------------

add_test(bfTuClosingOpeningFilterNew ${BASICFILTERS_TESTS1}
        otbClosingOpeningMorphologicalFilterNew )

add_test(bfTvClosingOpeningFilter ${BASICFILTERS_TESTS1}
   --compare-image ${NOTOL}  ${BASELINE}/bfFiltreClosingOpening_1.tif
                           ${TEMP}/bfFiltreClosingOpening_1.tif
        otbClosingOpeningMorphologicalFilter
        ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
        ${TEMP}/bfFiltreClosingOpening_1.tif
        1)


# -------            otb::BinaryImageMinimalBoundingRegionCalculator   ----------------------------

add_test(bfTuBoundingRegionCalculatorNew ${BASICFILTERS_TESTS1}
         otbBinaryImageMinimalBoundingRegionCalculatorNew)



add_test(bfTvBoundingRegionCalculator ${BASICFILTERS_TESTS1}
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


add_test(bfTvMeanFunctorImageTest ${BASICFILTERS_TESTS1}
  --compare-image ${NOTOL}  ${BASELINE}/bfMeanFunctorImage.tif
                           ${TEMP}/bfMeanFunctorImage.tif
  otbMeanFunctorImageTest
  ${INPUTDATA}/couleurs_extrait.png
  ${TEMP}/bfMeanFunctorImage.tif
  )


add_test(bfTvTransformPointSetFilter ${BASICFILTERS_TESTS1}
  otbTransformPointSetFilterTest)

add_test(bfTvRandomPointSetSource ${BASICFILTERS_TESTS1}
  otbRandomPointSetSourceTest)

add_test(bfTvPointSetExtractROI ${BASICFILTERS_TESTS1}
  otbPointSetExtractROITest)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbBasicFiltersTests2 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::LogPolarTransform   ----------------------------

add_test(bfTuLogPolarTransformNew ${BASICFILTERS_TESTS2}
         otbLogPolarTransformNew)

add_test(bfTvLogPolarTransform ${BASICFILTERS_TESTS2}
   --compare-ascii ${NOTOL}
                    ${BASELINE_FILES}/bfLogPolarTransformResults.txt
                    ${TEMP}/bfLogPolarTransformResults.txt
        otbLogPolarTransform
        1 1
        ${TEMP}/bfLogPolarTransformResults.txt
        5 0 0 0 10 45 0 3.14 3.14 18 1
        )

add_test(bfTvLogPolarTransformResample ${BASICFILTERS_TESTS2}
  --compare-image ${EPSILON_7}  ${BASELINE}/bfLogPolarTransformResampleOutput.hdr
                          ${TEMP}/bfLogPolarTransformResampleOutput.hdr
        otbLogPolarTransformResample
        ${INPUTDATA}/circle.hdr
        ${TEMP}/bfLogPolarTransformResampleOutput.hdr
)


# -------            otb::InverseLogPolarTransform   ----------------------------

add_test(bfTuInverseLogPolarTransformNew ${BASICFILTERS_TESTS2}
         otbInverseLogPolarTransformNew)

add_test(bfTvInverseLogPolarTransform ${BASICFILTERS_TESTS2}
     --compare-ascii ${NOTOL}
                     ${BASELINE_FILES}/bfInverseLogPolarTransformResults.txt
                     ${TEMP}/bfInverseLogPolarTransformResults.txt
        otbInverseLogPolarTransform
        1. 1.
        ${TEMP}/bfInverseLogPolarTransformResults.txt
        6 0 0 10 20 45 60 3.14 3.14 1 0 0 1
        )

add_test(bfTvInverseLogPolarTransformResample ${BASICFILTERS_TESTS2}
--compare-image ${NOTOL}  ${BASELINE}/bfInverseLogPolarTransformResampleOutput.hdr
                          ${TEMP}/bfInverseLogPolarTransformResampleOutput.hdr
        otbInverseLogPolarTransformResample
        ${BASELINE}/bfLogPolarTransformResampleOutput.hdr
        ${TEMP}/bfInverseLogPolarTransformResampleOutput.hdr
)

# -------            otb::StreamingShrinkImageFilter   ----------------------------

  add_test(bfTuStreamingShrinkImageFilterNew ${BASICFILTERS_TESTS2}
         otbStreamingShrinkImageFilterNew)

if(OTB_DATA_USE_LARGEINPUT)

 add_test(bfTvStreamingShrinkImageFilterQBPAN ${BASICFILTERS_TESTS2}
--compare-image ${NOTOL}
                ${BASELINE}/bfTvStreamingShrinkImageFilterQBPANOutput.hdr
                ${TEMP}/bfTvStreamingShrinkImageFilterQBPANOutput.hdr
        otbStreamingShrinkImageFilter
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        ${TEMP}/bfTvStreamingShrinkImageFilterQBPANOutput.hdr
        50
)

add_test(bfTvStreamingShrinkImageFilterQBMUL ${BASICFILTERS_TESTS2}
--compare-image ${NOTOL}
                ${BASELINE}/bfTvStreamingShrinkImageFilterQBMULOutput.hdr
                ${TEMP}/bfTvStreamingShrinkImageFilterQBMULOutput.hdr
                otbStreamingShrinkImageFilter
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
        ${TEMP}/bfTvStreamingShrinkImageFilterQBMULOutput.hdr
        20
)
endif()

# -------            otb::SpatialObjectToImageDrawingFilter   ----------------------------

  add_test(bfTuSpatialObjectToImageDrawingFilterNew ${BASICFILTERS_TESTS2}
         otbSpatialObjectToImageDrawingFilterNew)


 add_test(bfTvSpatialObjectToImageDrawingFilterWithoutInputImage ${BASICFILTERS_TESTS2}
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

  add_test(bfTuImportGeoInformationImageFilterNew ${BASICFILTERS_TESTS3}
         otbImportGeoInformationImageFilterNew)


 add_test(bfTvImportGeoInformationImageFilter ${BASICFILTERS_TESTS3}
 --compare-metadata ${NOTOL}
                    ${INPUTDATA}/HFAGeoreferenced.img
                    ${TEMP}/bfTvImportGeoInformationOutput.img
        otbImportGeoInformationImageFilter
                    ${INPUTDATA}/HFAGeoreferenced.img
                    ${TEMP}/bfTvImportGeoInformationOutput.img
)


if(OTB_DATA_USE_LARGEINPUT)
 add_test(bfTvImportGeoInformationImageFilterWithKeywordList ${BASICFILTERS_TESTS3}
--compare-ascii ${EPSILON_7}
                ${BASELINE_FILES}/bfTvImportGeoInformationKeywordListOutput.txt
                ${TEMP}/bfTvImportGeoInformationKeywordListOutput.txt
         otbImportGeoInformationImageFilterWithKeywordList
         ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
         ${TEMP}/bfTvImportGeoInformationKeywordListOutput.txt
)
endif()

# -------            otb::SpectralAngleDistanceImageFilter   ----------------------------

  add_test(bfTuSpectralAngleDistanceImageFilterNew ${BASICFILTERS_TESTS3}
         otbSpectralAngleDistanceImageFilterNew)


 add_test(bfTvSpectralAngleDistanceImageFilter ${BASICFILTERS_TESTS3}
                --compare-image ${EPSILON_7}
                    ${BASELINE}/bfTvSpectralAngleDistanceOutput.hdr
                    ${TEMP}/bfTvSpectralAngleDistanceOutput.hdr
                    otbSpectralAngleDistanceImageFilter
                    ${INPUTDATA}/qb_RoadExtract.img.hdr
                    ${TEMP}/bfTvSpectralAngleDistanceOutput.hdr
                    337 557 432 859
)

 add_test(bfTvSpectralAngleDistanceImageFilterOneChannel ${BASICFILTERS_TESTS3}
                    otbSpectralAngleDistanceImageFilter
                    ${INPUTDATA}/DeuxCercles.hdr
                    ${TEMP}/bfTvSpectralAngleDistanceOneChannelOutput.hdr
                    100
)

set_tests_properties(bfTvSpectralAngleDistanceImageFilterOneChannel PROPERTIES WILL_FAIL true)

# -------            otb::VectorRescaleIntensityImageFilter   ----------------------------

add_test(bfTuVectorRescaleIntensityImageFilterNew ${BASICFILTERS_TESTS3}
         otbVectorRescaleIntensityImageFilterNew)


add_test(bfTvVectorRescaleIntensityImageFilter ${BASICFILTERS_TESTS3}
                --compare-image ${EPSILON_7}
                    ${BASELINE}/bfTvVectorRescaleIntensityOutput.png
                    ${TEMP}/bfTvVectorRescaleIntensityOutput.png
                    otbVectorRescaleIntensityImageFilter
                    ${INPUTDATA}/qb_RoadExtract.img.hdr
                    ${TEMP}/bfTvVectorRescaleIntensityOutput.png
                    0 255
)

# -------            otb::ShiftScaleVectorImageFilter   ----------------------------

add_test(bfTuShiftScaleVectorImageFilterNew ${BASICFILTERS_TESTS3}
         otbShiftScaleVectorImageFilterNew)


add_test(bfTvShiftScaleVectorImageFilter ${BASICFILTERS_TESTS3}
                --compare-image ${EPSILON_7}
                    ${BASELINE}/bfTvShiftScaleVImageOutput.tif
                    ${TEMP}/bfTvShiftScaleVImageOutput.tif
                otbShiftScaleVectorImageFilterTest
                    ${INPUTDATA}/qb_RoadExtract.img.hdr
                ${TEMP}/bfTvShiftScaleVImageOutput.tif
)


# -------            otb::VectorImageToImageListFilter   ----------------------------

add_test(bfTuVectorImageToImageListFilterNew ${BASICFILTERS_TESTS3}
         otbVectorImageToImageListFilterNew)


add_test(bfTvVectorImageToImageListFilter ${BASICFILTERS_TESTS3}
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

add_test(bfTvVectorImageToImageListFilterIt ${BASICFILTERS_TESTS3}
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
add_test(bfTvVectorImageToImageListFilterIt2 ${BASICFILTERS_TESTS3}
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

add_test(bfTuImageListToSingleImageFilterNew ${BASICFILTERS_TESTS3}
         otbImageListToSingleImageFilterNew)

add_test(bfTvImageListToSingleImageFilter ${BASICFILTERS_TESTS3}
                --compare-image ${NOTOL}
                    ${BASELINE}/bfTvImageListToSingleImageFilter.tif
                    ${TEMP}/bfTvImageListToSingleImageFilterOut.tif
                    otbImageListToSingleImageFilter
                    ${INPUTDATA}/poupees.png
                    ${TEMP}/bfTvImageListToSingleImageFilterOut.tif
)

#if(OTB_DATA_USE_LARGEINPUT)
#    add_test(bfTvImageListToSingleImageFilterLarge ${BASICFILTERS_TESTS3}
#                        otbImageListToSingleImageFilter
#                        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
#                        ${TEMP}/bfTvImageListToSingleImageFilterLargeOut.tif
#    )
#endif()

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbBasicFiltersTests4 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# -------            otb::ImageListToImageListApplyFilter   ----------------------------

add_test(bfTuImageListToImageListApplyFilterNew ${BASICFILTERS_TESTS4}
         otbImageListToImageListApplyFilterNew)


add_test(bfTvImageListToImageListApplyFilter ${BASICFILTERS_TESTS4}
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

add_test(bfTuPerBandVectorImageFilterNew ${BASICFILTERS_TESTS4}
         otbPerBandVectorImageFilterNew)

add_test(bfTvPerBandVectorImageFilterWithMeanFilter ${BASICFILTERS_TESTS4}
        --compare-image ${EPSILON_7}
                        ${BASELINE}/bfTvPerBandVectorImageFilterWithMeanFilterOutput.hdr
                        ${TEMP}/bfTvPerBandVectorImageFilterWithMeanFilterOutput.hdr
        otbPerBandVectorImageFilterWithMeanFilter
                        ${INPUTDATA}/poupees.png
                        ${TEMP}/bfTvPerBandVectorImageFilterWithMeanFilterOutput.hdr
)

add_test(bfTvPerBandVectorImageFilterWithSobelFilter ${BASICFILTERS_TESTS4}
        --compare-image ${EPSILON_7}
                        ${BASELINE}/bfTvPerBandVectorImageFilterWithSobelFilterOutput.hdr
                        ${TEMP}/bfTvPerBandVectorImageFilterWithSobelFilterOutput.hdr
        otbPerBandVectorImageFilterWithSobelFilter
                        ${INPUTDATA}/poupees.png
                        ${TEMP}/bfTvPerBandVectorImageFilterWithSobelFilterOutput.hdr
)


# -------            otb::StreamingStatisticsImageFilter   ----------------------------

add_test(bfTuStreamingStatisticsImageFilterNew ${BASICFILTERS_TESTS4}
         otbStreamingStatisticsImageFilterNew)

add_test(bfTvStreamingStatisticsImageFilter ${BASICFILTERS_TESTS4}
        --compare-ascii ${NOTOL}
                    ${BASELINE_FILES}/bfStreamingStatisticsImageFilterResults.txt
                    ${TEMP}/bfStreamingStatisticsImageFilterResults.txt
         otbStreamingStatisticsImageFilter
#                ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
                ${INPUTDATA}/small_poupees_1canal.hd
                ${TEMP}/bfStreamingStatisticsImageFilterResults.txt)

add_test(bfTvStreamingStatisticsImageFilterWithNaNs ${BASICFILTERS_TESTS4}
        --compare-ascii ${NOTOL}
                    ${BASELINE_FILES}/bfStreamingStatisticsImageFilterWithNaNsResults.txt
                    ${TEMP}/bfStreamingStatisticsImageFilterWithNaNsResults.txt
         otbStreamingStatisticsImageFilter
                ${INPUTDATA}/small_poupees_1canalWithNaNs.TIF
                ${TEMP}/bfStreamingStatisticsImageFilterWithNaNsResults.txt)

add_test(bfTvStreamingStatisticsImageFilterWithBckGrdVal ${BASICFILTERS_TESTS4}
        --compare-ascii ${NOTOL}
                    ${BASELINE_FILES}/bfStreamingStatisticsImageFilterWithBckGrdValResults.txt
                    ${TEMP}/bfStreamingStatisticsImageFilterWithBckGrdValResults.txt
         otbStreamingStatisticsImageFilter
                ${INPUTDATA}/small_poupees_1canalWithNaNs.TIF
                ${TEMP}/bfStreamingStatisticsImageFilterWithBckGrdValResults.txt
                0 )


foreach(inputFileType "UINT8" "UINT16" "INT16" "UINT32" "INT32" "FLOAT" "DOUBLE")
  foreach(sizeType "LARGE" "SMALL")
    string(TOLOWER ${sizeType} lsizeType)

    if( sizeType STREQUAL "LARGE" )
      set(size "2048")
    else()
      set(size "256")
    endif()

    # Derive output file names
    set(OUTFILENAME RGBSquares${inputFileType}_${lsizeType}.tif)
    set(OUTLABELFILENAME RGBSquares${inputFileType}_${lsizeType}_Labels.tif)

    set(writeOutputImagesRq "0")

    add_test(bfTvStreamingStatisticsMapFromLabelImageFilterTest${inputFileType}${lsizeType} ${BASICFILTERS_TESTS4}
         otbStreamingStatisticsMapFromLabelImageFilterTest
               ${inputFileType}
               ${size}
               ${size}
               ${writeOutputImagesRq}
               ${TEMP}/${OUTFILENAME}
               ${TEMP}/${OUTLABELFILENAME}
    )
  endforeach()
endforeach()

# -------            otb::StreamingCompareImageFilter   ----------------------------

add_test(bfTuStreamingCompareImageFilterNew ${BASICFILTERS_TESTS4}
         otbStreamingCompareImageFilterNew)

add_test(bfTvStreamingCompareImageFilter ${BASICFILTERS_TESTS4}
        --compare-ascii ${NOTOL}
                    ${BASELINE_FILES}/bfStreamingCompareImageFilterResults.txt
                    ${TEMP}/bfStreamingCompareImageFilterResults.txt
         otbStreamingCompareImageFilter
                ${INPUTDATA}/small_poupees_1canal.hd
                ${TEMP}/bfStreamingCompareImageFilterResults.txt)

# -------            otb::VectorImageTo3DScalarImageFilter   ----------------------------

add_test(bfTuVectorImageTo3DScalarImageFilterNew ${BASICFILTERS_TESTS4}
         otbVectorImageTo3DScalarImageFilterNew)

add_test(bfTvVectorImageTo3DScalarImageFilter ${BASICFILTERS_TESTS4}
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

add_test(bfTuStreamingTraitsImage ${BASICFILTERS_TESTS5}
  otbStreamingTraitsImage
  )

add_test(bfTuStreamingTraitsVectorImage ${BASICFILTERS_TESTS5}
  otbStreamingTraitsVectorImage
  )

add_test(bfTuStreamingResampleImageFilterWithAffineTransformNew ${BASICFILTERS_TESTS5}
  otbStreamingResampleImageFilterWithAffineTransform
  )

if(OTB_DATA_USE_LARGEINPUT)
add_test(bfTvStreamingResampleImageFilterWithAffineTransform ${BASICFILTERS_TESTS5}
  otbStreamingResampleImageFilterWithAffineTransform
  ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
  500
  ${TEMP}/bfTvotbStreamingResampledImageWithAffineTransform.tif
  )
endif()


add_test(bfTuStreamingResampleImageFilterNew ${BASICFILTERS_TESTS5}
         otbStreamingResampleImageFilterNew)

add_test(bfTvStreamingResampleImageFilter ${BASICFILTERS_TESTS5}
          --compare-image ${NOTOL}
              ${BASELINE}/bfTvStreamingResamplePoupeesTest.tif
              ${TEMP}/bfTvStreamingResamplePoupeesTest.tif
          otbStreamingResampleImageFilter
              ${INPUTDATA}/poupees.tif
              ${TEMP}/bfTvStreamingResamplePoupeesTest.tif
)

add_test(bfTvStreamingResampleImageFilterCompareWithITK ${BASICFILTERS_TESTS5}
                --compare-image ${NOTOL}
                                        ${TEMP}/bfTvStreamingResamplePoupeesTestITK.tif
                                        ${TEMP}/bfTvStreamingResamplePoupeesTestOTB.tif
                                otbStreamingResampleImageFilterCompareWithITK
                                        ${INPUTDATA}/poupees.tif
                                        3000 3000
                                        ${TEMP}/bfTvStreamingResamplePoupeesTestITK.tif
                                        ${TEMP}/bfTvStreamingResamplePoupeesTestOTB.tif
)

#add_test(StreamingResampleImageFilterCompareWithITK ${BASICFILTERS_TESTS5}
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

add_test(bfTuStreamingMinMaxImageFilterNew ${BASICFILTERS_TESTS5}
         otbStreamingMinMaxImageFilterNew)

add_test(bfTvStreamingMinMaxImageFilter ${BASICFILTERS_TESTS5}
    --compare-ascii ${NOTOL}
           ${BASELINE_FILES}/bfTvStreamingMinMaxImageFilterResults.txt
           ${TEMP}/bfTvStreamingMinMaxImageFilterResults.txt
         otbStreamingMinMaxImageFilter
        ${INPUTDATA}/couleurs_extrait.png
        ${TEMP}/bfTvStreamingMinMaxImageFilterResults.txt
        )

# -------            otb::StreamingMinMaxVectorImageFilter   ----------------------------

add_test(bfTuStreamingMinMaxVectorImageFilterNew ${BASICFILTERS_TESTS5}
         otbStreamingMinMaxVectorImageFilterNew)

add_test(bfTvStreamingMinMaxVectorImageFilter ${BASICFILTERS_TESTS5}
    --compare-ascii ${NOTOL}
           ${BASELINE_FILES}/bfTvStreamingMinMaxVectorImageFilterResults.txt
           ${TEMP}/bfTvStreamingMinMaxVectorImageFilterResults.txt
         otbStreamingMinMaxVectorImageFilter
        ${INPUTDATA}/couleurs_extrait.png
        ${TEMP}/bfTvStreamingMinMaxVectorImageFilterResults.txt
        )

# -------            otb::StreamingStatisticsVectorImageFilter   ----------------------------

add_test(bfTuStreamingStatisticsVectorImageFilterNew ${BASICFILTERS_TESTS5}
         otbStreamingStatisticsVectorImageFilterNew)

add_test(bfTvStreamingStatisticsVectorImageFilter ${BASICFILTERS_TESTS5}
        --compare-ascii ${NOTOL}
                    ${BASELINE_FILES}/bfTvStreamingStatisticsVectorImageFilterResults.txt
                    ${TEMP}/bfTvStreamingStatisticsVectorImageFilterResults.txt
         otbStreamingStatisticsVectorImageFilter
                #${LARGEINPUT}/SPOT5_SCENE01/IMAGERY.TIF
                ${INPUTDATA}/couleurs_extrait.png
                ${TEMP}/bfTvStreamingStatisticsVectorImageFilterResults.txt
                )

add_test(bfTvStreamingStatisticsVectorImageFilterWithNaNs ${BASICFILTERS_TESTS5}
        --compare-ascii ${NOTOL}
                    ${BASELINE_FILES}/bfTvStreamingStatisticsVectorImageFilterWithNaNsResults.txt
                    ${TEMP}/bfTvStreamingStatisticsVectorImageFilterWithNaNsResults.txt
         otbStreamingStatisticsVectorImageFilter
                ${INPUTDATA}/small_poupees_WithNaNs.TIF
                ${TEMP}/bfTvStreamingStatisticsVectorImageFilterWithNaNsResults.txt
                )

add_test(bfTvStreamingStatisticsVectorImageFilterWithBckGrdVal ${BASICFILTERS_TESTS5}
        --compare-ascii ${NOTOL}
                    ${BASELINE_FILES}/bfTvStreamingStatisticsVectorImageFilterWithBckGrdValResults.txt
                    ${TEMP}/bfTvStreamingStatisticsVectorImageFilterWithBckGrdValResults.txt
         otbStreamingStatisticsVectorImageFilter
                ${INPUTDATA}/small_poupees_WithNaNs.TIF
                ${TEMP}/bfTvStreamingStatisticsVectorImageFilterWithBckGrdValResults.txt
                0
                )


# -------            otb::MatrixTransposeMatrixImageFilter   ----------------------------

add_test(bfTuMatrixTransposeMatrixImageFilterNew ${BASICFILTERS_TESTS5}
         otbMatrixTransposeMatrixImageFilterNew)

add_test(bfTvMatrixTransposeMatrixImageFilter ${BASICFILTERS_TESTS5}
        --compare-ascii ${NOTOL}
                    ${BASELINE_FILES}/bfTvMatrixTransposeMatrixImageFilterResults.txt
                    ${TEMP}/bfTvMatrixTransposeMatrixImageFilterResults.txt
         otbMatrixTransposeMatrixImageFilter
                ${INPUTDATA}/multiSpect.tif
                ${INPUTDATA}/multiSpectInterp.tif
                ${TEMP}/bfTvMatrixTransposeMatrixImageFilterResults.txt
                )


# -------            otb::UnaryImageFunctorWithVectorImageFilter   ----------------------------

add_test(bfTuUnaryImageFunctorWithVectorImageFilterNew ${BASICFILTERS_TESTS5}
         otbUnaryImageFunctorWithVectorImageFilterNew)

add_test(bfTvUnaryImageFunctorWithVectorImageFilter ${BASICFILTERS_TESTS5}
     --compare-image ${NOTOL}
                          ${BASELINE}/bfTvUnaryImageFunctorWithVectorImageFilter.tif
                     ${TEMP}/bfTvUnaryImageFunctorWithVectorImageFilter.tif
         otbUnaryImageFunctorWithVectorImageFilter
                ${INPUTDATA}/poupees_sub.png
                ${TEMP}/bfTvUnaryImageFunctorWithVectorImageFilter.tif
                )

# -------            otb::UnaryFunctorImageFilter   ----------------------------

add_test(bfTuUnaryFunctorImageFilterNew ${BASICFILTERS_TESTS5}
         otbUnaryFunctorImageFilterNew)

# -------            otb::PrintableImageFilter   ------------------------------

add_test(bfTuPrintableImageFilterNew ${BASICFILTERS_TESTS5}
         otbPrintableImageFilterNew)

add_test(bfTvPrintableImageFilter ${BASICFILTERS_TESTS5}
     --compare-image ${NOTOL}
                          ${BASELINE}/bfTvPrintableImageFilter.png
                     ${TEMP}/bfTvPrintableImageFilter.png
         otbPrintableImageFilter
                ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                ${TEMP}/bfTvPrintableImageFilter.png
                )

add_test(bfTvPrintableImageFilterWithMask ${BASICFILTERS_TESTS5}
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

add_test(bfTuBSplineDecompositionImageFilterNew ${BASICFILTERS_TESTS6}
         otbBSplineDecompositionImageFilterNew)

add_test(bfTvBSplineDecompositionImageFilter ${BASICFILTERS_TESTS6}
     --compare-image ${EPSILON_7}
                          ${BASELINE}/bfBSplineDecompositionImageFilterOutput.tif
                     ${TEMP}/bfBSplineDecompositionImageFilterOutput.tif
         otbBSplineDecompositionImageFilter
                ${INPUTDATA}/poupees.tif
                ${TEMP}/bfBSplineDecompositionImageFilterOutput.tif
                )

# -------            otb::BSplineInterpolateImageFunction  ------------------------------

add_test(bfTuBSplineInterpolateImageFunctionNew ${BASICFILTERS_TESTS6}
         otbBSplineInterpolateImageFunctionNew)

add_test(bfTvBSplineInterpolateImageFunction ${BASICFILTERS_TESTS6}
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

add_test(bfTuProlateInterpolateImageFunctionNew ${BASICFILTERS_TESTS6}
         otbProlateInterpolateImageFunctionNew)

add_test(bfTvProlateInterpolateImageFunction ${BASICFILTERS_TESTS6}
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
add_test(bfTuWindowedSincInterpolateImageFunctionBaseNew ${BASICFILTERS_TESTS6}
         otbWindowedSincInterpolateImageFunctionBaseNew)

# -------            otb::WindowedSincInterpolateImageGaussianFunction  ------------------------------
 add_test(bfTuWindowedSincInterpolateImageGaussianFunctionNew ${BASICFILTERS_TESTS6}
         otbWindowedSincInterpolateImageGaussianFunctionNew)

add_test(bfTvWindowedSincInterpolateImageGaussianFunction ${BASICFILTERS_TESTS6}
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
 add_test(bfTuWindowedSincInterpolateImageHammingFunctionNew ${BASICFILTERS_TESTS6}
         otbWindowedSincInterpolateImageHammingFunctionNew)

add_test(bfTvWindowedSincInterpolateImageHammingFunction ${BASICFILTERS_TESTS6}
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

add_test(bfTuBCOInterpolateImageFunctionNew ${BASICFILTERS_TESTS6}
  otbBCOInterpolateImageFunctionNew
  )

add_test(bfTvBCOInterpolateImageFunction ${BASICFILTERS_TESTS6}
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

add_test(bfTuBCOInterpolateImageFunctionOverVectorImageNew ${BASICFILTERS_TESTS6}
  otbBCOInterpolateImageFunctionOverVectorImageNew)

add_test(bfTvBCOInterpolateImageFunctionOverVectorImage ${BASICFILTERS_TESTS6}
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

add_test(bfTvBCOInterpolateImageFunctionTest ${BASICFILTERS_TESTS6}
--compare-image ${EPSILON_7}
                       ${BASELINE}/bfTvBCOInterpolateImageFunctionTest.tif
                  ${TEMP}/bfTvBCOInterpolateImageFunctionTest.tif
  otbBCOInterpolateImageFunctionTest
  ${INPUTDATA}/poupees.tif
  ${TEMP}/bfTvBCOInterpolateImageFunctionTest.tif
  3 # radius
  -0.5 # optimised bicubic
  )

add_test(bfTvBCOInterpolateImageFunctionVectorImageTest ${BASICFILTERS_TESTS6}
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
 add_test(bfTuWindowedSincInterpolateImageCosineFunctionNew ${BASICFILTERS_TESTS7}
         otbWindowedSincInterpolateImageCosineFunctionNew)

add_test(bfTvWindowedSincInterpolateImageCosineFunction ${BASICFILTERS_TESTS7}
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
 add_test(bfTuWindowedSincInterpolateImageWelchFunctionNew ${BASICFILTERS_TESTS7}
         otbWindowedSincInterpolateImageWelchFunctionNew)

add_test(bfTvWindowedSincInterpolateImageWelchFunction ${BASICFILTERS_TESTS7}
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
 add_test(bfTuWindowedSincInterpolateImageLanczosFunctionNew ${BASICFILTERS_TESTS7}
         otbWindowedSincInterpolateImageLanczosFunctionNew)

add_test(bfTvWindowedSincInterpolateImageLanczosFunction ${BASICFILTERS_TESTS7}
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
 add_test(bfTuWindowedSincInterpolateImageBlackmanFunctionNew ${BASICFILTERS_TESTS7}
         otbWindowedSincInterpolateImageBlackmanFunctionNew)

add_test(bfTvWindowedSincInterpolateImageBlackmanFunction ${BASICFILTERS_TESTS7}
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

add_test(bfTvProlateValidationTest ${BASICFILTERS_TESTS7}
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

add_test(bfTuVectorImageToAmplitudeImageFilterNew ${BASICFILTERS_TESTS7}
otbVectorImageToAmplitudeImageFilterNew)

add_test(bfTvVectorImageToAmplitudeImageFilter ${BASICFILTERS_TESTS7}
 --compare-image ${EPSILON_7}
                       ${BASELINE}/bfTvVectorImageToAmplitudeImageFilterOutput.tif
                  ${TEMP}/bfTvVectorImageToAmplitudeImageFilterOutput.tif
otbVectorImageToAmplitudeImageFilter
${INPUTDATA}/poupees.tif
${TEMP}/bfTvVectorImageToAmplitudeImageFilterOutput.tif
)


# -------            otb::VectorImageToAmplitudeImageFilter  ------------------------------

add_test(bfTuVectorImageToAmplitudeImageFilterNew ${BASICFILTERS_TESTS7}
otbVectorImageToAmplitudeImageFilterNew)

add_test(bfTvVectorImageToAmplitudeImageFilter ${BASICFILTERS_TESTS7}
 --compare-image ${EPSILON_7}
                       ${BASELINE}/bfTvVectorImageToAmplitudeImageFilterOutput.tif
                  ${TEMP}/bfTvVectorImageToAmplitudeImageFilterOutput.tif
otbVectorImageToAmplitudeImageFilter
${INPUTDATA}/poupees.tif
${TEMP}/bfTvVectorImageToAmplitudeImageFilterOutput.tif
)

# -------            otb::VectorImageToIntensityImageFilter   ----------------------------

  add_test(bfTuVectorImageToIntensityImageFilterNew ${BASICFILTERS_TESTS7}
         otbVectorImageToIntensityImageFilterNew)


 add_test(bfTvVectorImageToIntensityImageFilter ${BASICFILTERS_TESTS7}
                --compare-image ${EPSILON_7}
                    ${BASELINE}/bfTvVectorImageToIntensityImageOutput.tif
                    ${TEMP}/bfTvVectorImageToIntensityImageOutput.tif
                    otbVectorImageToIntensityImageFilter
                    ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                    ${TEMP}/bfTvVectorImageToIntensityImageOutput.tif
)

#------------ otb::AmplitudeFuntor ----------------------

add_test(bfTvAmplitudeFunctorTest ${BASICFILTERS_TESTS7}
        otbAmplitudeFunctorTest
)

#------------ otb::AmplitudeFuntor ----------------------

add_test(bfTvPhaseFunctorTest ${BASICFILTERS_TESTS7}
        otbPhaseFunctorTest
)

# -------            otb::LabelizeConnectedThresholdImageFilter   ----------------------------

  add_test(bfTuLabelizeConnectedThresholdImageFilterNew ${BASICFILTERS_TESTS8}
         otbLabelizeConnectedThresholdImageFilterNew)

  add_test(bfTvLabelizeConnectedThresholdImageFilter ${BASICFILTERS_TESTS8}
                --compare-image ${EPSILON_7}
                    ${BASELINE}/bfTvLabelizeConnectedThresholdImageFilterOutput.png
                    ${TEMP}/bfTvLabelizeConnectedThresholdImageFilterOutput.png
                    otbLabelizeConnectedThresholdImageFilter
                    ${INPUTDATA}/QB_Suburb.png
                    ${TEMP}/bfTvLabelizeConnectedThresholdImageFilterOutput.png
                    170 170
                    80 80
)

# -------            otb::EuclideanDistanceMetricWithMissingValue   ----------------------------

  add_test(bfTuEuclideanDistanceMetricWithMissingValueNew ${BASICFILTERS_TESTS8}
         otbEuclideanDistanceMetricWithMissingValueNew)

  add_test(bfTvEuclideanDistanceMetricWithMissingValue ${BASICFILTERS_TESTS8}
         otbEuclideanDistanceMetricWithMissingValue)


# -------            otb::FlexibleDistanceWithMissingValue   ----------------------------

  add_test(bfTuFlexibleDistanceWithMissingValueNew ${BASICFILTERS_TESTS8}
         otbFlexibleDistanceWithMissingValueNew)

  add_test(bfTvFlexibleDistanceWithMissingValue ${BASICFILTERS_TESTS8}
          otbFlexibleDistanceWithMissingValue
          0.000001
)

# -------            otb::LabelizeNeighborhoodConnectedImageFilter   ----------------------------

  add_test(bfTuLabelizeNeighborhoodConnectedImageFilterNew ${BASICFILTERS_TESTS8}
         otbLabelizeNeighborhoodConnectedImageFilterNew)

  add_test(bfTvLabelizeNeighborhoodConnectedImageFilter ${BASICFILTERS_TESTS8}
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

  add_test(bfTuLabelizeConfidenceConnectedImageFilterNew ${BASICFILTERS_TESTS8}
         otbLabelizeConfidenceConnectedImageFilterNew)

  add_test(bfTvLabelizeConfidenceConnectedImageFilter ${BASICFILTERS_TESTS8}
                --compare-image ${EPSILON_7}
                    ${BASELINE}/bfTvLabelizeConfidenceConnectedImageFilterOutput.png
                    ${TEMP}/bfTvLabelizeConfidenceConnectedImageFilterOutput.png
                    otbLabelizeConfidenceConnectedImageFilter
                    ${INPUTDATA}/QB_Suburb.png
                    ${TEMP}/bfTvLabelizeConfidenceConnectedImageFilterOutput.png
                    127 127 2 3 1
)

# -------            otb::ConvolutionImageFilter   ----------------------------

add_test(bfTuConvolutionImageFilterNew ${BASICFILTERS_TESTS8}
         otbConvolutionImageFilterNew)

add_test(bfTvConvolutionImageFilter ${BASICFILTERS_TESTS8}
                --compare-image ${EPSILON_7}
                    ${BASELINE}/bfTvConvolutionImageFilter.tif
                    ${TEMP}/bfTvConvolutionImageFilter.tif
                    otbConvolutionImageFilter
                    ${INPUTDATA}/QB_Suburb.png
                    ${TEMP}/bfTvConvolutionImageFilter.tif
)


# -------            otb::ScalarToRainbowRGBPixelFunctor  -----------------

add_test(bfTuScalarToRainbowRGBPixelFunctorNew ${BASICFILTERS_TESTS8}
         otbScalarToRainbowRGBPixelFunctorNew)

add_test(bfTvScalarToRainbowRGBPixelFunctor ${BASICFILTERS_TESTS8}
                --compare-image ${EPSILON_7}
                    ${BASELINE}/bfTvScalarToRainbowRGBPixelFunctor.png
                    ${TEMP}/bfTvScalarToRainbowRGBPixelFunctor.png
                    otbScalarToRainbowRGBPixelFunctor
                    ${INPUTDATA}/QB_Suburb.png
                    ${TEMP}/bfTvScalarToRainbowRGBPixelFunctor.png
)

# -------            otb::AmplitudePhaseToRGBFunctor  -----------------

add_test(bfTuAmplitudePhaseToRGBFunctorNew ${BASICFILTERS_TESTS8}
         otbAmplitudePhaseToRGBFunctorNew)

add_test(bfTvAmplitudePhaseToRGBFunctor ${BASICFILTERS_TESTS8}
                --compare-image ${EPSILON_7}
                    ${BASELINE}/bfTvAmplitudePhaseToRGBFunctor.png
                    ${TEMP}/bfTvAmplitudePhaseToRGBFunctor.png
                    otbAmplitudePhaseToRGBFunctor
                    ${INPUTDATA}/RSAT_imagery_HH.tif
                    ${TEMP}/bfTvAmplitudePhaseToRGBFunctor.png
)

# -------            otb::VarianceImageFilter  -----------------

add_test(bfTuVarianceImageFilterNew ${BASICFILTERS_TESTS8}
         otbVarianceImageFilterNew)

add_test(bfTvVarianceImageFilter ${BASICFILTERS_TESTS8}
    --compare-image ${EPSILON_7}
        ${BASELINE}/bfVarianceImageFilter.tif
        ${TEMP}/bfVarianceImageFilter.tif
        otbVarianceImageFilter
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
        ${TEMP}/bfVarianceImageFilter.tif
)

# ------             otb::BoxAndWhiskerImageFilter -------------

add_test(bfTuBoxAndWhiskerImageFilterNew ${BASICFILTERS_TESTS8}
         otbBoxAndWhiskerImageFilterNew )

add_test(bfTvBoxAndWhiskerImageFilter ${BASICFILTERS_TESTS8}
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

add_test(bfTuContinuousMinimumMaximumImageCalculatorNew ${BASICFILTERS_TESTS9}
        otbContinuousMinimumMaximumImageCalculatorNew
)

add_test(bfTvContinuousMinimumMaximumImageCalculatorTest ${BASICFILTERS_TESTS9}
        otbContinuousMinimumMaximumImageCalculatorTest
        ${INPUTDATA}/QB_Suburb.png )

add_test(bfTuMeanShiftImageFilterNew ${BASICFILTERS_TESTS9}
        otbMeanShiftImageFilterNew )

add_test(bfTuMeanShiftSmoothingImageFilterNew ${BASICFILTERS_TESTS9}
        otbMeanShiftImageFilterNew )


add_test(bfTvMeanShiftImageFilter ${BASICFILTERS_TESTS9}
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


 add_test(bfTvMeanShiftImageFilterEDISON ${BASICFILTERS_TESTS9}
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


 add_test(bfTvMeanShiftSmoothingImageFilterOptim ${BASICFILTERS_TESTS9}
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


 add_test(bfTvMeanShiftSmoothingImageFilterNonOptim ${BASICFILTERS_TESTS9}
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

add_test(bfTvMeanShiftImageFilterQBSuburb ${BASICFILTERS_TESTS9}
        otbMeanShiftImageFilter
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/bfMeanShiftImageFilterOutput_QBSuburb.tif
        ${TEMP}/bfMeanShiftImageFilterClusteredOutput_QBSuburb.tif
        ${TEMP}/bfMeanShiftImageFilterLabeledClusteredOutput_QBSuburb.tif
        ${TEMP}/bfMeanShiftImageFilterClusterBoundariesOutput_QBSuburb.tif
        4 50 10 1.0
        )

add_test(bfTuMeanShiftSmoothingImageFilterQBSuburbOptim ${BASICFILTERS_TESTS9}
        otbMeanShiftSmoothingImageFilter
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/bfMeanShiftSmoothingImageFilterSpatialOutput_QBSuburbOptim.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterSpectralOutput_QBSuburbOptim.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterIterationOutput_QBSuburbOptim.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterLabelOutput_QBSuburbOptim.tif
        4 25 0.1 100 1
        )

 add_test(bfTvMeanShiftSmoothingImageFilterQBSuburbNonOptim ${BASICFILTERS_TESTS9}
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


add_test(bfTuMeanShiftVectorImageFilterROIQBMul4 ${BASICFILTERS_TESTS9}
        otbMeanShiftVectorImageFilter
        ${INPUTDATA}/ROI_QB_MUL_4.tif
        ${TEMP}/bfMeanShiftImageFilterOutput_ROIQBMul4.tif
        ${TEMP}/bfMeanShiftImageFilterClusteredOutput_ROIQBMul4.tif
        ${TEMP}/bfMeanShiftImageFilterLabeledClusteredOutput_ROIQBMul4.tif
        ${TEMP}/bfMeanShiftImageFilterClusterBoundariesOutput_ROIQBMul4.tif
        4 50 100 1.0
        )




add_test(bfTuMeanShiftSmoothingImageFilterROIQBMul4 ${BASICFILTERS_TESTS9}
        otbMeanShiftSmoothingImageFilter
        ${INPUTDATA}/ROI_QB_MUL_4.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterSpatialOutput_ROIQBMul4.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterSpectralOutput_ROIQBMul4.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterIterationOutput_ROIQBMul4.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterLabelOutput_ROIQBMul4.tif
        4 50 0.1 100
        )

 #TOLERANCE SHOULD BE SET TO NOTOL
 ## Spatial Stability
 add_test(bfTuMeanShiftSmoothingImageFilterSpatialStability ${BASICFILTERS_TESTS9}
        otbMeanShiftSmoothingImageFilterSpatialStability
        ${INPUTDATA}/ROI_QB_MUL_4.tif
        4 50 0.1 40
        100 100 512 512
        )






add_test(bfTuMeanShiftVectorImageFilterQBRoad ${BASICFILTERS_TESTS9}
        otbMeanShiftVectorImageFilter
        ${INPUTDATA}/qb_RoadExtract.img
        ${TEMP}/bfMeanShiftImageFilterOutput_QBRoad.tif
        ${TEMP}/bfMeanShiftImageFilterClusteredOutput_QBRoad.tif
        ${TEMP}/bfMeanShiftImageFilterLabeledClusteredOutput_QBRoad.tif
        ${TEMP}/bfMeanShiftImageFilterClusterBoundariesOutput_QBRoad.tif
        4 30 100 1.0
        )


add_test(bfTuMeanShiftSmoothingImageFilterQBRoad ${BASICFILTERS_TESTS9}
        otbMeanShiftSmoothingImageFilter
        ${INPUTDATA}/qb_RoadExtract.img
        ${TEMP}/bfMeanShiftSmoothingImageFilterSpatialOutput_QBRoad.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterSpectralOutput_QBRoad.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterIterationOutput_QBRoad.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterLabelOutput_QBRoad.tif
        4 30 0.1 100
        )


add_test(bfTvMeanShiftVectorImageFilterQBPAN ${BASICFILTERS_TESTS9}
        otbMeanShiftVectorImageFilter
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
        ${TEMP}/bfMeanShiftImageFilterOutput_PAN.tif
        ${TEMP}/bfMeanShiftImageFilterClusteredOutputValid_PAN.tif
        ${TEMP}/bfMeanShiftImageFilterLabeledClusteredOutputValid_PAN.tif
        ${TEMP}/bfMeanShiftImageFilterClusterBoundariesOutputValid_PAN.tif
        5 15 100 1.0
        )


add_test(bfTvMeanShiftSmoothingImageFilterQBPAN ${BASICFILTERS_TESTS9}
        otbMeanShiftSmoothingImageFilter
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterSpatialOutput_PAN.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterSpectralOutput_PAN.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterIterationOutput_PAN.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterLabelOutput_PAN.tif
        5 15 1.0 100
        )

add_test(bfTuMeanShiftVectorImageFilterSPOT5 ${BASICFILTERS_TESTS9}
        otbMeanShiftVectorImageFilter
        ${INPUTDATA}/SPOT5_EXTRACTS/Arcachon/Arcachon_extrait_3852_3319_546_542.tif
        ${TEMP}/bfMeanShiftImageFilterOutput_SPOT5.tif
        ${TEMP}/bfMeanShiftImageFilterClusteredOutput_SPOT5.tif
        ${TEMP}/bfMeanShiftImageFilterLabeledClusteredOutput_SPOT5.tif
        ${TEMP}/bfMeanShiftImageFilterClusterBoundariesOutput_SPOT5.tif
        4 10 100 1.0
        )


add_test(bfTuMeanShiftSmoothingImageFilterSPOT5 ${BASICFILTERS_TESTS9}
        otbMeanShiftSmoothingImageFilter
        ${INPUTDATA}/SPOT5_EXTRACTS/Arcachon/Arcachon_extrait_3852_3319_546_542.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterSpatialOutput_SPOT5.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterSpectralOutput_SPOT5.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterIterationOutput_SPOT5.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilterLabelOutput_SPOT5.tif
        4 10 0.1 100
        )

add_test(bfTvMeanShiftSmoothingImageFilterThreadingNonOpt ${BASICFILTERS_TESTS9}
        --compare-image ${EPSILON_7}
        ${TEMP}/bfMeanShiftSmoothingImageFilter2SingleThreading_SPOT5.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilter2MultiThreading_SPOT5.tif
        otbMeanShiftSmoothingImageFilterThreading
        ${INPUTDATA}/SPOT5_EXTRACTS/Arcachon/Arcachon_extrait_3852_3319_546_542.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilter2SingleThreading_SPOT5.tif
        ${TEMP}/bfMeanShiftSmoothingImageFilter2MultiThreading_SPOT5.tif
        4 10 0
        )

#add_test(bfTvMeanShiftImageFilter2ThreadingOpt ${BASICFILTERS_TESTS9}
#        --compare-image ${EPSILON_7}
#        ${TEMP}/bfMeanShiftSmoothingImageFilter2SingleThreading_SPOT5.tif
#        ${TEMP}/bfMeanShiftSmoothingImageFilter2MultiThreading_SPOT5.tif
#        otbMeanShiftImageFilter2Threading
#        ${INPUTDATA}/SPOT5_EXTRACTS/Arcachon/Arcachon_extrait_3852_3319_546_542.tif
#        ${TEMP}/bfMeanShiftSmoothingImageFilter2SingleThreading_SPOT5.tif
#        ${TEMP}/bfMeanShiftSmoothingImageFilter2MultiThreading_SPOT5.tif
#        4 10 1
#        )


add_test(bfTvLabelImageRegionMergingFilterQB_Suburb ${BASICFILTERS_TESTS9}
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

add_test(bfTvLabelImageRegionPruningFilterQB_Suburb ${BASICFILTERS_TESTS9}
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

add_test(bfTvLabelImageRegionMergingFilterSPOT5 ${BASICFILTERS_TESTS9}
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

add_test(bfTvLabelImageRegionPruningFilterSPOT5 ${BASICFILTERS_TESTS9}
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

if(OTB_DATA_USE_LARGEINPUT)
#
#add_test(bfTvMeanShiftImageFilterValidLargeInput ${BASICFILTERS_TESTS9}
#        otbMeanShiftImageFilter
#        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
#        ${TEMP}/bfMeanShiftImageFilterOutputValidLargeInput.tif
#        ${TEMP}/bfMeanShiftImageFilterClusteredOutputValidLargeInput.tif
#        ${TEMP}/bfMeanShiftImageFilterLabeledClusteredOutputValidLargeInput.tif
#        ${TEMP}/bfMeanShiftImageFilterClusterBoundariesOutputValidLargeInput.tif
#        9 50 10 1.0
#        )

#add_test(bfTvMeanShiftImageFilter2LargeInput ${BASICFILTERS_TESTS9}
#       otbMeanShiftImageFilter2
#        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
#        ${TEMP}/bfMeanShiftImageSpatialOutputLargeInput.tif
#        ${TEMP}/bfMeanShiftImageFilterSpectralOutputLargeInput.tif
#        9 9 10 10 0.001 10
#        )

endif()


add_test(bfTuMeanShiftVectorImageFilterNew ${BASICFILTERS_TESTS9}
        otbMeanShiftVectorImageFilterNew )

add_test(bfTvMeanShiftVectorImageFilter ${BASICFILTERS_TESTS9}
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

add_test(bfTuFunctionToImageFilterNew ${BASICFILTERS_TESTS9}
        otbFunctionToImageFilterNew
)

add_test(bfTvFunctionToImageFilterTest ${BASICFILTERS_TESTS9}
        --compare-image ${EPSILON_7}
            ${BASELINE}/bfTvFunctionToImageFilterTest.tif
            ${TEMP}/bfTvFunctionToImageFilterTest.tif
        otbFunctionToImageFilter
            ${INPUTDATA}/poupees_1canal.hd
            ${TEMP}/bfTvFunctionToImageFilterTest.tif
            2 # radius
)

# -------            otb::BinaryImageDensityFunction   ----------------------------
add_test(bfTuBinaryImageDensityFunctionNew  ${BASICFILTERS_TESTS9}
        otbBinaryImageDensityFunctionNew
)

add_test(bfTvBinaryImageDensityFunction ${BASICFILTERS_TESTS9}
--compare-ascii ${NOTOL}
             ${BASELINE_FILES}/bfTvBinaryImageDensityFunctionOutputAscii.txt
            ${TEMP}/bfTvBinaryImageDensityFunctionOutputAscii.txt
         otbBinaryImageDensityFunction
         ${INPUTDATA}/scene.png
         ${TEMP}/bfTvBinaryImageDensityFunctionOutputAscii.txt
         )

# -------            otb::BinaryImageToImageDensityFilter   ----------------------------
add_test(bfTuBinaryImageToImageDensityFilterNew  ${BASICFILTERS_TESTS9}
        otbBinaryImageToDensityImageFilterNew
)

add_test(bfTvBinaryImageToDensityImageFilter ${BASICFILTERS_TESTS9}
        --compare-image ${EPSILON_7}
            ${BASELINE}/bfTvBinaryImageToDensityImageFilterOutputImage.tif
            ${TEMP}/bfTvBinaryImageToDensityImageFilterOutputImage.tif
        otbBinaryImageToDensityImageFilter
            ${INPUTDATA}/scene.png
            ${TEMP}/bfTvBinaryImageToDensityImageFilterOutputImage.tif
            2 # radius
)

# -------            otb::EdgeDensityImageyFilter   ----------------------------
add_test(bfTuEdgeDensityImageFilterNew  ${BASICFILTERS_TESTS9}
        otbEdgeDensityImageFilterNew
)

add_test(bfTvEdgeDensityImageFilter ${BASICFILTERS_TESTS9}
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

add_test(bfTuGaborFilterGeneratorNew ${BASICFILTERS_TESTS10}
        otbGaborFilterGeneratorNew
         )

add_test(bfTvGaborFilterGenerator ${BASICFILTERS_TESTS10}
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

if(ITK_USE_FFTWD)

  add_test(bfTuOverlapSaveConvolutionImageFilterNew ${BASICFILTERS_TESTS10}
         otbOverlapSaveConvolutionImageFilterNew)

  add_test(bfTvOverlapSaveConvolutionImageFilter ${BASICFILTERS_TESTS10}
                --compare-image ${EPSILON_7}
                    ${BASELINE}/bfTvConvolutionImageFilter.tif
                    ${TEMP}/bfTvOverlapSaveConvolutionImageFilter.tif
                    otbOverlapSaveConvolutionImageFilter
                    ${INPUTDATA}/QB_Suburb.png
                    ${TEMP}/bfTvOverlapSaveConvolutionImageFilter.tif
)

 add_test(bfTvCompareOverlapSaveAndClassicalConvolutionWithGaborFilter ${BASICFILTERS_TESTS10}
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

endif()


add_test(bfTvPolygonCompacityFunctor ${BASICFILTERS_TESTS10}
        --compare-ascii ${NOTOL}
            ${BASELINE_FILES}/bfPolygonCompacityFunctorTest.txt
            ${TEMP}/bfPolygonCompacityFunctorTest.txt
         otbPolygonCompacityFunctor
         ${INPUTDATA}/polygon.png
         ${TEMP}/bfPolygonCompacityFunctorTest.txt
)

add_test(bfTvPathLengthFunctor ${BASICFILTERS_TESTS10}
        --compare-ascii ${NOTOL}
            ${BASELINE_FILES}/bfPathLengthFunctorTest.txt
            ${TEMP}/bfPathLengthFunctorTest.txt
         otbPathLengthFunctor
         ${INPUTDATA}/polygon.png
         ${TEMP}/bfPathLengthFunctorTest.txt
)

add_test(bfTvClosePathFunctor ${BASICFILTERS_TESTS10}
        --compare-ascii ${NOTOL}
            ${BASELINE_FILES}/bfClosePathFunctorTest.txt
            ${TEMP}/bfClosePathFunctorTest.txt
         otbClosePathFunctor
         ${INPUTDATA}/polygon.png
         ${TEMP}/bfClosePathFunctorTest.txt
)

# -------            otb::PersistentVectorizationImageFilter   ----------------------------

add_test(bfTuPersistentVectorizationImageFilterNew ${BASICFILTERS_TESTS10}
        otbPersistentVectorizationFilterNew
         )

add_test(bfTvPersistentVectorizationImageFilter ${BASICFILTERS_TESTS10}
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

add_test(bfTvExtractROIResample ${BASICFILTERS_TESTS11}
--compare-image ${NOTOL}
            ${BASELINE}/bfTvExtractROIResample.tif
            ${TEMP}/bfTvExtractROIResample.tif
         otbExtractROIResample
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/bfTvExtractROIResample.tif
         0
)

add_test(bfTvExtractROIResample2 ${BASICFILTERS_TESTS11}
--compare-image ${NOTOL}
            ${BASELINE}/bfTvExtractROIResample2.tif
            ${TEMP}/bfTvExtractROIResample2.tif
         otbExtractROIResample
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/bfTvExtractROIResample2.tif
         1
)
# -------    otbPointSetFunction   ----------------------------
#add_test(bfTuPointSetFunctionNew ${BASICFILTERS_TESTS11}
#         otbPointSetFunctionNew
#         )



# -------    otbPointSetDensityFunction   ----------------------------
add_test(bfTuPointSetDensityFunctionNew ${BASICFILTERS_TESTS11}
         otbPointSetDensityFunctionNew
         )


add_test(bfTvPointSetDensityFunctionTest ${BASICFILTERS_TESTS11}
--compare-ascii ${NOTOL}
             ${BASELINE_FILES}/bfTvPointSetDensityFunctionOutputAscii.txt
            ${TEMP}/bfTvPointSetDensityFunctionOutputAscii.txt
         otbPointSetDensityFunctionTest
         ${TEMP}/bfTvPointSetDensityFunctionOutputAscii.txt
         )

# -------    otbPointSetDensityGaussianFunction   ----------------------------
add_test(bfTuPointSetDensityGaussianFunctionNew ${BASICFILTERS_TESTS11}
     otbPointSetDensityGaussianFunctionNew
     )


add_test(bfTvPointSetDensityGaussianFunctionTest ${BASICFILTERS_TESTS11}
--compare-ascii ${NOTOL}
         ${BASELINE_FILES}/bfTvPointSetDensityGaussianFunctionOutputAscii.txt
        ${TEMP}/bfTvPointSetDensityGaussianFunctionOutputAscii.txt
     otbPointSetDensityGaussianFunctionTest
     ${TEMP}/bfTvPointSetDensityGaussianFunctionOutputAscii.txt
     )

# -------    otbPointSetDensityEpanechnikovFunction   ----------------------------
add_test(bfTuPointSetDensityEpanechnikovFunctionNew ${BASICFILTERS_TESTS11}
     otbPointSetDensityEpanechnikovFunctionNew
     )


add_test(bfTvPointSetDensityEpanechnikovFunctionTest ${BASICFILTERS_TESTS11}
--compare-ascii ${NOTOL}
         ${BASELINE_FILES}/bfTvPointSetDensityEpanechnikovFunctionOutputAscii.txt
        ${TEMP}/bfTvPointSetDensityEpanechnikovFunctionOutputAscii.txt
     otbPointSetDensityEpanechnikovFunctionTest
     ${TEMP}/bfTvPointSetDensityEpanechnikovFunctionOutputAscii.txt
     )

#------------ otbPointSetToDensityImageFilter ---------------------

add_test(bfTuPointSetToDensityImageFilterNew ${BASICFILTERS_TESTS11}
         otbPointSetToDensityImageFilterNew)



add_test(bfTvPointSetToDensityImageFilterTest ${BASICFILTERS_TESTS11}
--compare-image ${NOTOL}
            ${BASELINE}/bfTvPointSetToDensityImageFilterOutputImage.tif
            ${TEMP}/bfTvPointSetToDensityImageFilterOutputImage.tif
            otbPointSetToDensityImageFilterTest
         ${INPUTDATA}/QB_Suburb.png
         ${TEMP}/bfTvPointSetToDensityImageFilterOutputImage.tif
         3 5 10
)

# -------    otbKeyPointDensityImageFilter   ----------------------------
add_test(bfTuKeyPointDensityImageFilterNew ${BASICFILTERS_TESTS11}
         otbKeyPointDensityImageFilterNew
         )

add_test(bfTvKeyPointDensityImageFilterOutputImage ${BASICFILTERS_TESTS11}
--compare-image ${NOTOL}
            ${BASELINE}/bfTvKeyPointDensityImageFilterOutputImage.tif
            ${TEMP}/bfTvKeyPointDensityImageFilterOutputImage.tif
            otbKeyPointDensityImageFilterTest
         ${INPUTDATA}/QB_Suburb.png
         ${TEMP}/bfTvKeyPointDensityImageFilterOutputImage.tif
         3 5 10
)

add_test(bfTvFunctionWithNeighborhoodToImageFilterNew ${BASICFILTERS_TESTS11}
        otbFunctionWithNeighborhoodToImageFilterNew
)

add_test(bfTvFunctionWithNeighborhoodToImageFilter ${BASICFILTERS_TESTS11}
        --compare-image ${EPSILON_7}
            ${BASELINE}/bfTvFunctionWithNeighborhoodToImageFilterTest.tif
            ${TEMP}/bfTvFunctionWithNeighborhoodToImageFilterTest.tif
        otbFunctionWithNeighborhoodToImageFilter
            ${INPUTDATA}/poupees_1canal.hd
        ${TEMP}/bfTvFunctionWithNeighborhoodToImageFilterTest.tif
            2 # radius
)


add_test(bfTuStreamingInnerProductVectorImageFilterNew ${BASICFILTERS_TESTS11}
        otbStreamingInnerProductVectorImageFilterNew
)

add_test(bfTvStreamingInnerProductVectorImageFilter ${BASICFILTERS_TESTS11}
      --compare-ascii 0.000001
      ${BASELINE_FILES}/bfStreamingInnerProductVectorImageFilterResults.txt
      ${TEMP}/bfStreamingInnerProductVectorImageFilterResults.txt
      otbStreamingInnerProductVectorImageFilter
      ${INPUTDATA}/poupees_sub_3c.png
      ${TEMP}/bfStreamingInnerProductVectorImageFilterResults.txt
      1 #center data
)
add_test(bfTvStreamingInnerProductVectorImageFilterDisableCenterData ${BASICFILTERS_TESTS11}
      --compare-ascii 0.000001
      ${BASELINE_FILES}/bfStreamingInnerProductVectorImageFilterResultsDisableCenterData.txt
      ${TEMP}/bfStreamingInnerProductVectorImageFilterResultsDisableCenterData.txt
      otbStreamingInnerProductVectorImageFilter
      ${INPUTDATA}/poupees_sub_3c.png
      ${TEMP}/bfStreamingInnerProductVectorImageFilterResultsDisableCenterData.txt
      0 #center data
)

add_test(bfTuEstimateInnerProductPCAImageFilterNew ${BASICFILTERS_TESTS11}
        otbEstimateInnerProductPCAImageFilterNew )


add_test(bfTvEstimateInnerProductPCAImageFilter ${BASICFILTERS_TESTS11}
    --compare-image ${EPSILON_7}
    ${BASELINE}/bfEstimateInnerProductPCAImageFilter.tif
    ${TEMP}/bfEstimateInnerProductPCAImageFilter.tif
    otbEstimateInnerProductPCAImageFilter
    ${INPUTDATA}/poupees_sub_3c.png
    ${TEMP}/bfEstimateInnerProductPCAImageFilter.tif
    3
    1 #center data
)

add_test(bfTvEstimateInnerProductPCAImageFilterDisableCenterData ${BASICFILTERS_TESTS11}
    --compare-image ${EPSILON_7}
    ${BASELINE}/bfEstimateInnerProductPCAImageFilterDisableCenterData.tif
    ${TEMP}/bfEstimateInnerProductPCAImageFilterDisableCenterData.tif
    otbEstimateInnerProductPCAImageFilter
    ${INPUTDATA}/poupees_sub_3c.png
    ${TEMP}/bfEstimateInnerProductPCAImageFilterDisableCenterData.tif
    3
    0 #center data
)

add_test(bfTvEstimateInnerProductPCAImageFilterCP1 ${BASICFILTERS_TESTS11}
    --compare-image ${EPSILON_7}
    ${BASELINE}/bfEstimateInnerProductPCAImageFilter_CP1.tif
    ${TEMP}/bfEstimateInnerProductPCAImageFilter_CP1.tif
    otbEstimateInnerProductPCAImageFilter
    ${INPUTDATA}/poupees_sub_3c.png
    ${TEMP}/bfEstimateInnerProductPCAImageFilter_CP1.tif
    1
    1 #center data
)
add_test(bfTvEstimateInnerProductPCAImageFilterCP12 ${BASICFILTERS_TESTS11}
    --compare-image ${EPSILON_7}
    ${BASELINE}/bfEstimateInnerProductPCAImageFilter_CP12.tif
    ${TEMP}/bfEstimateInnerProductPCAImageFilter_CP12.tif
    otbEstimateInnerProductPCAImageFilter
    ${INPUTDATA}/poupees_sub_3c.png
    ${TEMP}/bfEstimateInnerProductPCAImageFilter_CP12.tif
    2
    1 #center data
)

add_test(bfTuNormalizeInnerProductPCAImageFilterNew ${BASICFILTERS_TESTS11}
        otbNormalizeInnerProductPCAImageFilterNew )

add_test(bfTvNormalizeInnerProductPCAImageFilter ${BASICFILTERS_TESTS11}
    --compare-image ${EPSILON_12}
    ${BASELINE}/bfNormalizeInnerProductPCAImageFilter.tif
    ${TEMP}/bfNormalizeInnerProductPCAImageFilter.tif
    otbNormalizeInnerProductPCAImageFilter
    ${INPUTDATA}/poupees_sub_3c.png
    ${TEMP}/bfNormalizeInnerProductPCAImageFilter.tif
)

add_test(bfTuInnerProductPCAImageFilterNew ${BASICFILTERS_TESTS11}
        otbInnerProductPCAImageFilterNew )


add_test(bfTvInnerProductPCAImageFilter ${BASICFILTERS_TESTS11}
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

add_test(bfTvInnerProductPCAImageFilterDisableCenterData ${BASICFILTERS_TESTS11}
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

add_test(bfTvInnerProductPCAImageFilter_PC1 ${BASICFILTERS_TESTS11}
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
add_test(bfTvInnerProductPCAImageFilter_PC12 ${BASICFILTERS_TESTS11}
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

add_test(bfTvInnerProductPCAImageFilterAllsOutputs ${BASICFILTERS_TESTS11}
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

add_test(bfTvInnerProductPCAImageFilterAllsOutputsWithoutUnbiasedEstimator ${BASICFILTERS_TESTS11}
    --compare-image ${EPSILON_7}
    ${BASELINE}/bfInnerProductPCAImageFilterAllsOutputs.tif
    ${TEMP}/bfInnerProductPCAImageFilterAllsOutputsWithoutUnbiasedEstimator.tif
    otbInnerProductPCAImageFilter
    ${INPUTDATA}/poupees_sub_3c.png
    ${TEMP}/bfInnerProductPCAImageFilterAllsOutputsWithoutUnbiasedEstimator.tif
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
add_test(bfTvItkImagePCAShapeModelEstimatorTest ${BASICFILTERS_TESTS11}
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
set_tests_properties(bfTvItkImagePCAShapeModelEstimatorTest PROPERTIES DEPENDS bfTvInnerProductPCAImageFilterAllsOutputsWithoutUnbiasedEstimator)

# -------  otb::ListSampleToVariableDimensionHistogramGenerator ----------------------------

add_test(bfTuListSampleToVariableDimensionHistogramGeneratorNew ${BASICFILTERS_TESTS11}
        otbListSampleToVariableDimensionHistogramGeneratorNew)

add_test(bfTvListSampleToVariableDimensionHistogramGenerator ${BASICFILTERS_TESTS11}
--compare-ascii ${NOTOL}
        ${BASELINE_FILES}/bfTvListSampleToVariableDimensionHistogramGeneratorOutput.txt
        ${TEMP}/bfTvListSampleToVariableDimensionHistogramGeneratorOutput.txt
        otbListSampleToVariableDimensionHistogramGenerator
        ${INPUTDATA}/couleurs.tif
        ${TEMP}/bfTvListSampleToVariableDimensionHistogramGeneratorOutput.txt
        10 1
)

# -------  otb::ListSampleToVariableDimensionHistogramGenerator ----------------------------

add_test(bfTuListSampleToHistogramListGeneratorNew ${BASICFILTERS_TESTS11}
        otbListSampleToHistogramListGeneratorNew)

add_test(bfTvListSampleToHistogramListGenerator ${BASICFILTERS_TESTS11}
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
add_test(bfTuEdgeDetectorImageFilterNew ${BASICFILTERS_TESTS12}
         otbEdgeDetectorImageFilterNew
         )

add_test(bfTvEdgeDetectorImageFilter ${BASICFILTERS_TESTS12}
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
add_test(bfTuChangeLabelImageFilterNew ${BASICFILTERS_TESTS12}
   otbChangeLabelImageFilterNew
   )

add_test(bfTvChangeLabelImageFilter ${BASICFILTERS_TESTS12}
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
add_test(bfTuImageAndVectorImageOperationFilterNew ${BASICFILTERS_TESTS12}
        otbImageAndVectorImageOperationFilterNew)

add_test(bfTvImageAndVectorImageOperationFilterTest ${BASICFILTERS_TESTS12}
        --compare-n-images ${NOTOL} 4
                ${BASELINE}/bfTvImageAndVectorImageOperationFilterTestAdd.tif
                ${TEMP}/bfTvImageAndVectorImageOperationFilterTestAdd.tif
                ${BASELINE}/bfTvImageAndVectorImageOperationFilterTestSub.tif
                ${TEMP}/bfTvImageAndVectorImageOperationFilterTestSub.tif
                ${BASELINE}/bfTvImageAndVectorImageOperationFilterTestMul.tif
                ${TEMP}/bfTvImageAndVectorImageOperationFilterTestMul.tif
                ${BASELINE}/bfTvImageAndVectorImageOperationFilterTestDiv.tif
                ${TEMP}/bfTvImageAndVectorImageOperationFilterTestDiv.tif
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
add_test(bfTuBandMathImageFilter ${BASICFILTERS_TESTS13}
  otbBandMathImageFilterNew)

add_test(bfTuBandMathImageFilterX ${BASICFILTERS_TESTS13}
  otbBandMathImageFilterXNew)

add_test(bfTvBandMathImageFilter ${BASICFILTERS_TESTS13}
  otbBandMathImageFilter)

add_test(bfTvBandMathImageFilterX ${BASICFILTERS_TESTS13}
  otbBandMathImageFilterX)

add_test(bfTvBandMathImageFilterWithIdx ${BASICFILTERS_TESTS13}
  otbBandMathImageFilterWithIdx
  ${TEMP}/bfTvBandMathImageFilterWithIdx1.tif
  ${TEMP}/bfTvBandMathImageFilterWithIdx2.tif
)

add_test(bfTvBandMathImageFilterXWithIdx ${BASICFILTERS_TESTS13}
  otbBandMathImageFilterXWithIdx
  ${TEMP}/bfTvBandMathImageFilterWithIdx1.tif
  ${TEMP}/bfTvBandMathImageFilterWithIdx2.tif
)

add_test(bfTvComplexToIntensityFilterTest ${BASICFILTERS_TESTS13}
  otbComplexToIntensityFilterTest)

add_test(bfTvRealAndImaginaryImageToComplexImageFilterTest ${BASICFILTERS_TESTS13}
  otbRealAndImaginaryImageToComplexImageFilterTest
  ${INPUTDATA}/GomaAvant.png
  ${INPUTDATA}/GomaApres.png
)

add_test(bfTvRealImageToComplexImageFilterTest ${BASICFILTERS_TESTS13}
  otbRealImageToComplexImageFilterTest
  ${INPUTDATA}/GomaAvant.png
)

add_test(bfTvImaginaryImageToComplexImageFilterTest ${BASICFILTERS_TESTS13}
  otbImaginaryImageToComplexImageFilterTest
  ${INPUTDATA}/GomaAvant.png
)

add_test(bfTuComplexToVectorImageCastFilterNew ${BASICFILTERS_TESTS13}
  otbComplexToVectorImageCastFilterNew
)

add_test(bfTuComplexToVectorImageCastFilterTest ${BASICFILTERS_TESTS13}
  --compare-n-images ${NOTOL} 2
  ${BASELINE}/bfTvComplexToVectorImageCastFilterTest.tif
  ${TEMP}/bfTvComplexToVectorImageCastFilterTest.tif
  ${BASELINE}/bfTvComplexToVectorImageCastFilterTest.tif
  ${TEMP}/bfTvVectorComplexToVectorImageCastFilterTest.tif
  otbComplexToVectorImageCastFilterTest
    ${INPUTDATA}/monobandComplexDouble
    ${TEMP}/bfTvComplexToVectorImageCastFilterTest.tif
    ${TEMP}/bfTvVectorComplexToVectorImageCastFilterTest.tif
)

add_test(bfTvotbLog10ThresholdedImageFilterTest ${BASICFILTERS_TESTS13}
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfTvLog10ThresholdedImageFilter.txt
  ${TEMP}/bfTvLog10ThresholdedImageFilterTest.txt
  otbLog10ThresholdedImageFilterTest
    ${TEMP}/bfTvLog10ThresholdedImageFilterTest.txt
)


add_test(bfTuMatrixImageFilterNew ${BASICFILTERS_TESTS13}
  otbMatrixImageFilterNew
)

add_test(bfTvMatrixImageFilterTest_1 ${BASICFILTERS_TESTS13}
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvMatrixImageFilterTest_1.tif
  ${TEMP}/bfTvMatrixImageFilterTest_1.tif
  otbMatrixImageFilterTest
    ${INPUTDATA}/poupees.tif
    ${TEMP}/bfTvMatrixImageFilterTest_1.tif
    1
)

add_test(bfTvMatrixImageFilterTest_2 ${BASICFILTERS_TESTS13}
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvMatrixImageFilterTest_2.tif
  ${TEMP}/bfTvMatrixImageFilterTest_2.tif
  otbMatrixImageFilterTest
    ${INPUTDATA}/poupees.tif
    ${TEMP}/bfTvMatrixImageFilterTest_2.tif
    2
)


add_test(bfTvMatrixImageFilterTest_3 ${BASICFILTERS_TESTS13}
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvMatrixImageFilterTest_3.tif
  ${TEMP}/bfTvMatrixImageFilterTest_3.tif
  otbMatrixImageFilterTest
    ${INPUTDATA}/poupees.tif
    ${TEMP}/bfTvMatrixImageFilterTest_3.tif
    3
)



add_test(bfTuConcatenateScalarValueImageFilterNew ${BASICFILTERS_TESTS13}
  otbConcatenateScalarValueImageFilterNew )

add_test(bfTvConcatenateScalarValueImageFilterTest ${BASICFILTERS_TESTS13}
  otbConcatenateScalarValueImageFilterTest )

# ----------------------------     otbMaskMuParserFilter   ----------------------------

add_test(bfTuMaskMuParserFilterNew ${BASICFILTERS_TESTS13}
  otbMaskMuParserFilterNew
)

add_test(bfTvMaskMuParserFilterTest ${BASICFILTERS_TESTS13}
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvMaskMuParserFilterTest.tif
  ${TEMP}/bfTvMaskMuParserFilterTest.tif
        otbMaskMuParserFilterTest
    ${INPUTDATA}/ROI_QB_MUL_4.tif
    ${TEMP}/bfTvMaskMuParserFilterTest.tif
    "(b1>100)*(b2>120)"
)

add_test(bfTuHorizontalSobelVectorImageFilterNew
         ${BASICFILTERS_TESTS13}
         otbHorizontalSobelVectorImageFilterNewTest)

add_test(bfTvHorizontalSobelVectorImageFilter
         ${BASICFILTERS_TESTS13}
           --compare-image ${EPSILON_7}
             ${BASELINE}/bfTvHorizontalSobelVectorImageFilter.tif
             ${TEMP}/bfTvHorizontalSobelVectorImageFilter.tif
         otbHorizontalSobelVectorImageFilterTest
		 -in ${INPUTDATA}/cupriteSubHsi.tif
		 -out ${TEMP}/bfTvHorizontalSobelVectorImageFilter.tif)

add_test(bfTuVerticalSobelVectorImageFilterNew
         ${BASICFILTERS_TESTS13}
         otbVerticalSobelVectorImageFilterNewTest)

add_test(bfTvVerticalSobelVectorImageFilter
         ${BASICFILTERS_TESTS13}
           --compare-image ${EPSILON_7}
             ${BASELINE}/bfTvVerticalSobelVectorImageFilter.tif
             ${TEMP}/bfTvVerticalSobelVectorImageFilter.tif
         otbVerticalSobelVectorImageFilterTest
		 -in ${INPUTDATA}/cupriteSubHsi.tif
		 -out ${TEMP}/bfTvVerticalSobelVectorImageFilter.tif)

add_test(bfTuSobelVectorImageFilterNew
         ${BASICFILTERS_TESTS13}
         otbSobelVectorImageFilterNewTest)

add_test(bfTvSobelVectorImageFilter
         ${BASICFILTERS_TESTS13}
           --compare-image ${EPSILON_7}
             ${BASELINE}/bfTvSobelVectorImageFilter.tif
             ${TEMP}/bfTvSobelVectorImageFilter.tif
         otbSobelVectorImageFilterTest
		 -in ${INPUTDATA}/cupriteSubHsi.tif
		 -out ${TEMP}/bfTvSobelVectorImageFilter.tif)

add_test(bfTuLocalGradientVectorImageFilterNew
         ${BASICFILTERS_TESTS13}
         otbLocalGradientVectorImageFilterNewTest)

add_test(bfTvLocalGradientVectorImageFilter
         ${BASICFILTERS_TESTS13}
           --compare-image ${EPSILON_7}
             ${BASELINE}/bfTvLocalGradientVectorImageFilter.tif
             ${TEMP}/bfTvLocalGradientVectorImageFilter.tif
         otbLocalGradientVectorImageFilterTest
		 -in ${INPUTDATA}/cupriteSubHsi.tif
		 -out ${TEMP}/bfTvLocalGradientVectorImageFilter.tif)

add_test(bfTuLocalActivityVectorImageFilterNew
         ${BASICFILTERS_TESTS13}
         otbLocalActivityVectorImageFilterNewTest)

add_test(bfTvLocalActivityVectorImageFilter
         ${BASICFILTERS_TESTS13}
           --compare-image ${EPSILON_7}
             ${BASELINE}/bfTvLocalActivityVectorImageFilter.tif
             ${TEMP}/bfTvLocalActivityVectorImageFilter.tif
         otbLocalActivityVectorImageFilterTest
		 -in ${INPUTDATA}/cupriteSubHsi.tif
		 -out ${TEMP}/bfTvLocalActivityVectorImageFilter.tif)

add_test(bfTuNormalizeVectorImageFilter
         ${BASICFILTERS_TESTS13}
		 otbNormalizeVectorImageFilterNewTest)

add_test(bfTvNormalizeVectorImageFilter
         ${BASICFILTERS_TESTS13}
           --compare-image ${EPSILON_7}
             ${BASELINE}/bfTvNormalizeVectorImageFilter.tif
             ${TEMP}/bfTvNormalizeVectorImageFilter.tif
		 otbNormalizeVectorImageFilterTest
		 -in ${INPUTDATA}/cupriteSubHsi.tif
		 -out ${TEMP}/bfTvNormalizeVectorImageFilter.tif)


add_test(bfTuNormalizeVectorImageFilter
         ${BASICFILTERS_TESTS13}
     otbNormalizeVectorImageFilterNewTest)

add_test(bfTvNormalizeVectorImageFilter
         ${BASICFILTERS_TESTS13}
           --compare-image ${EPSILON_7}
             ${BASELINE}/bfTvNormalizeVectorImageFilter.tif
             ${TEMP}/bfTvNormalizeVectorImageFilter.tif
     otbNormalizeVectorImageFilterTest
     -in ${INPUTDATA}/cupriteSubHsi.tif
     -out ${TEMP}/bfTvNormalizeVectorImageFilter.tif)



add_test(bfTuProjectiveProjectionNew
         ${BASICFILTERS_TESTS13}
         otbProjectiveProjectionNew)

add_test(bfTvProjectiveProjectionTestHighSNR
         ${BASICFILTERS_TESTS13}
           --compare-image ${EPSILON_7}
             ${BASELINE}/bfTvProjectiveProjectionTestHighSNR.tif
             ${TEMP}/bfTvProjectiveProjectionTestHighSNR.tif
     otbProjectiveProjectionTestHighSNR
     ${INPUTDATA}/cupriteSubHsi.tif
     3
     ${TEMP}/bfTvProjectiveProjectionTestHighSNR.tif)


add_test(bfTuVectorImageToMatrixNew
         ${BASICFILTERS_TESTS13}
         otbVectorImageToMatrixNewTest)

add_test(bfTvVectorImageToMatrix
         ${BASICFILTERS_TESTS13}
         otbVectorImageToMatrixTest)

### PCA ###

add_test(bfTuPCAImageFilterNew
         ${BASICFILTERS_TESTS13}
         otbPCAImageFilterNewTest)

# Test input->PCA->inverse PCA
add_test(bfTvPCAImageFilter1
         ${BASICFILTERS_TESTS13}
         --compare-image ${EPSILON_7}
             ${INPUTDATA}/cupriteSubHsi.tif
             ${TEMP}/bfTvPCAImageFilter1Inv.tif
         otbPCAImageFilterTest
         -in ${INPUTDATA}/cupriteSubHsi.tif
         -out ${TEMP}/bfTvPCAImageFilter1.tif
         -inv ${TEMP}/bfTvPCAImageFilter1Inv.tif)

add_test(bfTvPCAImageFilter2
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
add_test(bfTvPCAImageFilter3
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
add_test(bfTvPCAImageFilter3Norm
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
add_test(bfTvPCAImageFilter4Norm
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
add_test(bfTvPCAImageFilter4
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

add_test(bfTuNAPCAImageFilterNew
         ${BASICFILTERS_TESTS13}
         otbNAPCAImageFilterNewTest)

# Test input->NAPCA->inverse NAPCA
add_test(bfTvNAPCAImageFilter1
         ${BASICFILTERS_TESTS13}
         --compare-image ${EPSILON_7}
             ${INPUTDATA}/cupriteSubHsi.tif
             ${TEMP}/bfTvNAPCAImageFilter1Inv.tif
         otbNAPCAImageFilterTest
         -in ${INPUTDATA}/cupriteSubHsi.tif
         -out ${TEMP}/bfTvNAPCAImageFilter1.tif
         -inv ${TEMP}/bfTvNAPCAImageFilter1Inv.tif)

add_test(bfTvNAPCAImageFilter2
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
add_test(bfTvNAPCAImageFilter3
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
add_test(bfTvNAPCAImageFilter4
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
# Test input->MNF->inverse MNF
add_test(bfTvMNFImageFilter1
         ${BASICFILTERS_TESTS13}
         --compare-image ${EPSILON_7}
             ${INPUTDATA}/cupriteSubHsi.tif
             ${TEMP}/bfTvMNFImageFilter1Inv.tif
         otbMNFImageFilterTest
         -in ${INPUTDATA}/cupriteSubHsi.tif
         -out ${TEMP}/bfTvMNFImageFilter1.tif
         -inv ${TEMP}/bfTvMNFImageFilter1Inv.tif)

add_test(bfTvMNFImageFilter2
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
add_test(bfTvMNFImageFilter3
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
add_test(bfTvMNFImageFilter4
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
add_test(bfTuFastICAInternalOptimizerVectorImageFilterNew
     ${BASICFILTERS_TESTS13}
     otbFastICAInternalOptimizerVectorImageFilterNewTest)

add_test(bfTuFastICAImageFilterNew
    ${BASICFILTERS_TESTS13}
    otbFastICAImageFilterNewTest)

add_test(bfTvFastICAImageFilter
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

add_test(bfTuAngularProjectionImageFilterNew
	${BASICFILTERS_TESTS13}
	otbAngularProjectionImageFilterNew)

#add_test(bfTvAngularProjectionImageFilter
#	${BASICFILTERS_TESTS13}
#		--compare-image ${EPSILON_7}
#		${INPUTDATA}/xx
#		${TEMP}/yy
#		otbAngularProjectionImageFilterTest
#		-in ${INPUTDATA}/xx ${INPUTDATA}/yy
#		-out ${TEMP}/zz)

add_test(bfTuAngularProjectionSetImageFilterNew
	${BASICFILTERS_TESTS13}
	otbAngularProjectionSetImageFilterNew)

#add_test(bfTvAngularProjectionSetImageFilter
#	${BASICFILTERS_TESTS13}
#		--compare-image ${EPSILON_7}
#		${INPUTDATA}/xx
#		${TEMP}/yy
#		otbAngularProjectionSetImageFilterTest
#		-in ${INPUTDATA}/xx ${INPUTDATA}/yy ${INPUTDATA}/zz ${INPUTDATA}/zzz
#		-out ${TEMP}/zz)

add_test(bfTuSparseWvltToAngleMapperListFilterNew
	${BASICFILTERS_TESTS13}
	otbSparseWvltToAngleMapperListFilterNew)

#add_test(bfTvSparseWvltToAngleMapperListFilter
#	${BASICFILTERS_TESTS13}
#	otbSparseWvltToAngleMapperListFilterTest
#	-in ${INPUTDATA}/xx ${INPUTDATA}/yy
#	-th 10)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbBasicFiltersTests14 ~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# ----------------------------     otbConnectedComponentMuParserFunctor   ----------------------------

add_test(bfTvConnectedComponentMuParserFunctorTest ${BASICFILTERS_TESTS14}
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvConnectedComponentMuParserFunctorTest_1.tif
  ${TEMP}/bfTvConnectedComponentMuParserFunctorTest_1.tif
        otbConnectedComponentMuParserFunctorTest
    ${INPUTDATA}/ROI_QB_MUL_4.tif
    ${TEMP}/bfTvConnectedComponentMuParserFunctorTest_1.tif
    "distance<40"
)



add_test(bfTvConnectedComponentMuParserFunctorTestMask ${BASICFILTERS_TESTS14}
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvConnectedComponentMuParserFunctorTest_2.tif
  ${TEMP}/bfTvConnectedComponentMuParserFunctorTest_2.tif
        otbConnectedComponentMuParserFunctorTest
    ${INPUTDATA}/ROI_QB_MUL_4.tif
    ${TEMP}/bfTvConnectedComponentMuParserFunctorTest_2.tif
    "distance<40"
    ${INPUTDATA}/ROI_QB_MUL_4_Mask.tif
)

# ----------------------------     otbLabelObjectOpeningMuParserFilter   ----------------------------


add_test(bfTuLabelObjectOpeningMuParserFilterNew ${BASICFILTERS_TESTS14}
  otbLabelObjectOpeningMuParserFilterNew
)


add_test(bfTvLabelObjectOpeningMuParserFilterTest ${BASICFILTERS_TESTS14}
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvLabelObjectOpeningMuParserFilterTest.tif
  ${TEMP}/bfTvLabelObjectOpeningMuParserFilterTest.tif
        otbLabelObjectOpeningMuParserFilterTest
    ${INPUTDATA}/ROI_QB_MUL_4.tif
    ${INPUTDATA}/ROI_QB_MUL_4_Label.tif
    ${TEMP}/bfTvLabelObjectOpeningMuParserFilterTest.tif
    "SHAPE_Elongation>8"
)

# -----------------------  otbVectorDataToRandomLineGenerator  -----------------------

add_test(bfTuVectorDataToRandomLineGeneratorNew ${BASICFILTERS_TESTS14}
  otbVectorDataToRandomLineGeneratorNew
)

add_test(bfTuVectorDataToRandomLineGenerator ${BASICFILTERS_TESTS14}
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/bfTvVectorDataToRandomLineGeneratorOutput.shp
  ${TEMP}/bfTvVectorDataToRandomLineGeneratorOutput.shp
  otbVectorDataToRandomLineGenerator
  ${INPUTDATA}/ObjectReco/Boats/maur_B010202_01Polygons.shp
  ${TEMP}/bfTvVectorDataToRandomLineGeneratorOutput.shp
)


# -----------------------  otbConcatenateVectorDataFilter  -----------------------

add_test(bfTuConcatenateVectorDataFilterNew ${BASICFILTERS_TESTS14}
  otbConcatenateVectorDataFilterNew
)

add_test(bfTvConcatenateVectorDataFilter ${BASICFILTERS_TESTS14}
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

add_test(bfTuNDVIDataNodeFeatureFunctionNew ${BASICFILTERS_TESTS15}
  otbNDVIDataNodeFeatureFunctionNew
)

add_test(bfTvNDVIDataNodeFeatureFunction_Line ${BASICFILTERS_TESTS15}
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

add_test(bfTvNDVIDataNodeFeatureFunction_Polygon ${BASICFILTERS_TESTS15}
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


add_test(bfTvParserConditionDataNodeFeatureFunction_Line_NDVI ${BASICFILTERS_TESTS15}
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

add_test(bfTvParserConditionDataNodeFeatureFunction_Polygon_NDVI ${BASICFILTERS_TESTS15}
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
add_test(bfTuSpectralAngleDataNodeFeatureFunctionNew ${BASICFILTERS_TESTS15}
  otbSpectralAngleDataNodeFeatureFunctionNew
)

add_test(bfTvSpectralAngleDataNodeFeatureFunction_Line ${BASICFILTERS_TESTS15}
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

add_test(bfTvSpectralAngleDataNodeFeatureFunction_Polygon ${BASICFILTERS_TESTS15}
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
add_test(bfTuRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunctionNew ${BASICFILTERS_TESTS15}
  otbRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunctionNew
)

add_test(bfTvRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction_Line ${BASICFILTERS_TESTS15}
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/bfTvRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunctionOutput_Line.shp
  ${TEMP}/bfTvRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunctionOutput_Line.shp
  otbRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction
    ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE_ROADS.shp
    ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE.TIF
    ${INPUTDATA}/DEM/srtm_directory
    ${TEMP}/bfTvRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunctionOutput_Line.shp
    0 )

add_test(bfTvRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction_Polygon ${BASICFILTERS_TESTS15}
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
add_test(bfTuDBOverlapDataNodeFeatureFunctionNew ${BASICFILTERS_TESTS15}
  otbDBOverlapDataNodeFeatureFunctionNew
)

add_test(bfTvDBOverlapDataNodeFeatureFunction ${BASICFILTERS_TESTS15}
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
add_test(bfTuMaximumAutocorrelationFactorImageFilterNew ${BASICFILTERS_TESTS15}
  otbMaximumAutocorrelationFactorImageFilterNew
)

add_test(bfTvMaximumAutocorrelationFactorImageFilter ${BASICFILTERS_TESTS15}
   --compare-image 0.0001
   ${BASELINE}/bfTvMaximumAutocorrelationFactorImageFilterOutput.tif
   ${TEMP}/bfTvMaximumAutocorrelationFactorImageFilterOutput.tif
   otbMaximumAutocorrelationFactorImageFilter
   ${INPUTDATA}/cupriteSubHsi.tif
   ${TEMP}/bfTvMaximumAutocorrelationFactorImageFilterOutput.tif
)

# ---------------------    otbThresholdVectorImageFilter---------------------
add_test(bfTuThresholdVectorImageFilterNew ${BASICFILTERS_TESTS15}
  otbThresholdVectorImageFilterNew
)

add_test(bfTvThresholdVectorImageFilterTest ${BASICFILTERS_TESTS15}
   --compare-image 0.0001
   ${BASELINE}/bfTvThresholdVectorImageFilterTest_Output.tif
   ${TEMP}/bfTvThresholdVectorImageFilterTest_Output.tif
   otbThresholdVectorImageFilterTest
   ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500.tif
   ${TEMP}/bfTvThresholdVectorImageFilterTest_Output.tif
)

# ---------------------    otbClampVectorImageFilter---------------------
add_test(bfTuClampVectorImageFilterNew ${BASICFILTERS_TESTS15}
  otbClampVectorImageFilterNew
)

add_test(bfTvClampVectorImageFilterTest ${BASICFILTERS_TESTS15}
   --compare-image 0.0001
   ${BASELINE}/bfTvClampVectorImageFilterTest_Output.tif
   ${TEMP}/bfTvClampVectorImageFilterTest_Output.tif
   otbClampVectorImageFilterTest
   ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500.tif
   ${TEMP}/bfTvClampVectorImageFilterTest_Output.tif
)

# ---------------------    otbClampImageFilter---------------------
add_test(bfTuClampImageFilterNew ${BASICFILTERS_TESTS15}
  otbClampImageFilterNew
)

add_test(bfTvClampImageFilterTest ${BASICFILTERS_TESTS15}
   --compare-image 0.0001
   ${BASELINE}/bfTvClampImageFilterTest_Output.tif
   ${TEMP}/bfTvClampImageFilterTest_Output.tif
   otbClampImageFilterTest
   ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
   ${TEMP}/bfTvClampImageFilterTest_Output.tif
)


#-------- TileImageFilter --------------------------
add_test(bfTuTileImageFilterNew ${BASICFILTERS_TESTS15}
  otbTileImageFilterNew
)


add_test(bfTvTileImageFilter ${BASICFILTERS_TESTS15}
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


#-------- StreamingHistogramVectorImageFilter.cxx ----
add_test(bfTuStreamingHistogramVIFilterNew ${BASICFILTERS_TESTS15}
  otbStreamingHistogramVectorImageFilterNew
)

# A enrichir
set(BasicFilters_SRCS1
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
set(BasicFilters_SRCS2
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
set(BasicFilters_SRCS3
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
set(BasicFilters_SRCS4
otbBasicFiltersTests4.cxx
otbImageListToImageListApplyFilterNew.cxx
otbImageListToImageListApplyFilter.cxx
otbPerBandVectorImageFilterNew.cxx
otbPerBandVectorImageFilterWithMeanFilter.cxx
otbPerBandVectorImageFilterWithSobelFilter.cxx
otbStreamingStatisticsImageFilterNew.cxx
otbStreamingStatisticsImageFilter.cxx
otbStreamingStatisticsMapFromLabelImageFilterTest.cxx
otbStreamingCompareImageFilter.cxx
otbVectorImageTo3DScalarImageFilterNew.cxx
otbVectorImageTo3DScalarImageFilter.cxx
)

# A enrichir
set(BasicFilters_SRCS5
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
set(BasicFilters_SRCS6
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
set(BasicFilters_SRCS7
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
otbAmplitudeFunctorTest.cxx
otbPhaseFunctorTest.cxx
)

# A enrichir
set(BasicFilters_SRCS8
otbBasicFiltersTests8.cxx
otbLabelizeConnectedThresholdImageFilterNew.cxx
otbLabelizeConnectedThresholdImageFilter.cxx
otbLabelizeNeighborhoodConnectedImageFilterNew.cxx
otbLabelizeNeighborhoodConnectedImageFilter.cxx
otbLabelizeConfidenceConnectedImageFilterNew.cxx
otbLabelizeConfidenceConnectedImageFilter.cxx
otbEuclideanDistanceMetricWithMissingValueNew.cxx
otbEuclideanDistanceMetricWithMissingValue.cxx
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
set(BasicFilters_SRCS9
otbBasicFiltersTests9.cxx
otbContinuousMinimumMaximumImageCalculatorNew.cxx
otbContinuousMinimumMaximumImageCalculatorTest.cxx
otbMeanShiftImageFilterNew.cxx
otbMeanShiftImageFilter.cxx
otbMeanShiftSmoothingImageFilterNew.cxx
otbMeanShiftSmoothingImageFilter.cxx
otbMeanShiftSmoothingImageFilterSpatialStability.cxx
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
set(BasicFilters_SRCS10
otbBasicFiltersTests10.cxx
otbGaborFilterGeneratorNew.cxx
otbGaborFilterGenerator.cxx
otbPolygonCompacityFunctor.cxx
otbPathLengthFunctor.cxx
otbClosePathFunctor.cxx
otbPersistentVectorizationFilterNew.cxx
otbPersistentVectorizationFilter.cxx
)

if(ITK_USE_FFTWD)
set(BasicFilters_SRCS10
${BasicFilters_SRCS10}
otbOverlapSaveConvolutionImageFilterNew.cxx
otbOverlapSaveConvolutionImageFilter.cxx
otbCompareOverlapSaveAndClassicalConvolutionWithGaborFilter.cxx
)
endif()

set(BasicFilters_SRCS11
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

set(BasicFilters_SRCS12
otbBasicFiltersTests12.cxx
otbEdgeDetectorImageFilter.cxx
otbEdgeDetectorImageFilterNew.cxx
otbChangeLabelImageFilterNew.cxx
otbChangeLabelImageFilter.cxx
otbImageAndVectorImageOperationFilterNew.cxx
otbImageAndVectorImageOperationFilterTest.cxx
)

set(BasicFilters_SRCS13
otbBasicFiltersTests13.cxx
otbBandMathImageFilter.cxx
otbBandMathImageFilterX.cxx
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

set(BasicFilters_SRCS14
otbBasicFiltersTests14.cxx
otbConnectedComponentMuParserFunctorTest.cxx
otbLabelObjectOpeningMuParserFilterNew.cxx
otbLabelObjectOpeningMuParserFilterTest.cxx
otbVectorDataToRandomLineGenerator.cxx
otbConcatenateVectorDataFilter.cxx
)
set(BasicFilters_SRCS15
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
otbStreamingHistogramVectorImageFilter.cxx
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
OTB_ADD_EXECUTABLE(otbBasicFiltersTests11 "${BasicFilters_SRCS11}" "OTBBasicFilters;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbBasicFiltersTests12 "${BasicFilters_SRCS12}" "OTBBasicFilters;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbBasicFiltersTests13 "${BasicFilters_SRCS13}" "OTBBasicFilters;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbBasicFiltersTests14 "${BasicFilters_SRCS14}" "OTBBasicFilters;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbBasicFiltersTests15 "${BasicFilters_SRCS15}" "OTBBasicFilters;OTBIO;OTBTesting;OTBOGRAdapters")

endif()
