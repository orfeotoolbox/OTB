otb_module_test()

set(OTBStatisticsTests
otbStatisticsTestDriver.cxx
otbListSampleToHistogramListGeneratorNew.cxx
otbStreamingMinMaxImageFilter.cxx
otbStreamingMinMaxVectorImageFilterNew.cxx
otbVarianceImageFilterNew.cxx
otbStreamingStatisticsImageFilterNew.cxx
otbListSampleToVariableDimensionHistogramGeneratorNew.cxx
otbStreamingHistogramVectorImageFilter.cxx
otbStreamingStatisticsVectorImageFilterNew.cxx
otbRealImageToComplexImageFilterTest.cxx
otbHistogramStatisticsFunction.cxx
otbContinuousMinimumMaximumImageCalculatorTest.cxx
otbGaussianAdditiveNoiseSampleListFilter.cxx
otbNormalizeVectorImageFilter.cxx
otbVectorImageToMatrixImageFilter.cxx
otbListSampleToVariableDimensionHistogramGenerator.cxx
otbStreamingMinMaxImageFilterNew.cxx
otbShiftScaleSampleListFilter.cxx
otbVectorImageToIntensityImageFilter.cxx
otbVarianceImageFilter.cxx
otbConcatenateSampleListFilter.cxx
otbLocalHistogramImageFunctionTest.cxx
otbVectorImageToIntensityImageFilterNew.cxx
otbProjectiveProjection.cxx
otbShiftScaleVectorImageFilterTest.cxx
otbContinuousMinimumMaximumImageCalculatorNew.cxx
otbStreamingCompareImageFilter.cxx
otbStreamingStatisticsMapFromLabelImageFilterTest.cxx
otbLocalHistogramImageFunctionNew.cxx
otbRealAndImaginaryImageToComplexImageFilterTest.cxx
otbStreamingStatisticsImageFilter.cxx
otbListSampleToBalancedListSampleFilter.cxx
otbStreamingStatisticsVectorImageFilter.cxx
otbStreamingMinMaxVectorImageFilter.cxx
otbListSampleGeneratorTest.cxx
otbImaginaryImageToComplexImageFilterTest.cxx
otbListSampleToHistogramListGenerator.cxx
)

add_executable(otbStatisticsTestDriver ${OTBStatisticsTests})
target_link_libraries(otbStatisticsTestDriver ${OTBStatistics-Test_LIBRARIES})
otb_module_target_label(otbStatisticsTestDriver)

# Tests Declaration

otb_add_test(NAME bfTuListSampleToHistogramListGeneratorNew COMMAND otbStatisticsTestDriver
  otbListSampleToHistogramListGeneratorNew)

otb_add_test(NAME bfTvStreamingMinMaxImageFilter COMMAND otbStatisticsTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfTvStreamingMinMaxImageFilterResults.txt
  ${TEMP}/bfTvStreamingMinMaxImageFilterResults.txt
  otbStreamingMinMaxImageFilter
  ${INPUTDATA}/couleurs_extrait.png
  ${TEMP}/bfTvStreamingMinMaxImageFilterResults.txt
  )

otb_add_test(NAME bfTuStreamingMinMaxVectorImageFilterNew COMMAND otbStatisticsTestDriver
  otbStreamingMinMaxVectorImageFilterNew)

otb_add_test(NAME bfTuVarianceImageFilterNew COMMAND otbStatisticsTestDriver
  otbVarianceImageFilterNew)

otb_add_test(NAME bfTuStreamingStatisticsImageFilterNew COMMAND otbStatisticsTestDriver
  otbStreamingStatisticsImageFilterNew)

otb_add_test(NAME bfTuListSampleToVariableDimensionHistogramGeneratorNew COMMAND otbStatisticsTestDriver
  otbListSampleToVariableDimensionHistogramGeneratorNew)

otb_add_test(NAME bfTuStreamingHistogramVIFilterNew COMMAND otbStatisticsTestDriver
  otbStreamingHistogramVectorImageFilterNew
  )

otb_add_test(NAME bfTvStreamingHistogramVIFilterTest COMMAND otbStatisticsTestDriver
  otbStreamingHistogramVectorImageFilterTest
  )

otb_add_test(NAME bfTuStreamingStatisticsVectorImageFilterNew COMMAND otbStatisticsTestDriver
  otbStreamingStatisticsVectorImageFilterNew)



otb_add_test(NAME bfTvRealImageToComplexImageFilterTest COMMAND otbStatisticsTestDriver
  otbRealImageToComplexImageFilterTest
  ${INPUTDATA}/GomaAvant.png
  )

otb_add_test(NAME coTuHistogramStatisticsFunction COMMAND otbStatisticsTestDriver
  otbHistogramStatisticsFunction 100)


otb_add_test(NAME bfTvContinuousMinimumMaximumImageCalculatorTest COMMAND otbStatisticsTestDriver
  otbContinuousMinimumMaximumImageCalculatorTest
  ${INPUTDATA}/QB_Suburb.png )

otb_add_test(NAME leTuGaussianAdditiveNoiseSampleListFilterNew COMMAND otbStatisticsTestDriver
  otbGaussianAdditiveNoiseSampleListFilterNew)

otb_add_test(NAME leTvGaussianAdditiveNoiseSampleListFilter COMMAND otbStatisticsTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/leTvGaussianAdditiveNoiseSampleListFilterOutput.txt
  ${TEMP}/leTvGaussianAdditiveNoiseSampleListFilterOutput.txt
  otbGaussianAdditiveNoiseSampleListFilter
  ${TEMP}/leTvGaussianAdditiveNoiseSampleListFilterOutput.txt
  2
  -1 -3
  1  2
  -2 -5
  -1 -3
  0 -1
  )

otb_add_test(NAME bfTuNormalizeVectorImageFilter COMMAND otbStatisticsTestDriver
  otbNormalizeVectorImageFilterNewTest)

otb_add_test(NAME bfTvNormalizeVectorImageFilter COMMAND otbStatisticsTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvNormalizeVectorImageFilter.tif
  ${TEMP}/bfTvNormalizeVectorImageFilter.tif
  otbNormalizeVectorImageFilterTest
  -in ${INPUTDATA}/cupriteSubHsi.tif
  -out ${TEMP}/bfTvNormalizeVectorImageFilter.tif)

otb_add_test(NAME bfTuVectorImageToMatrixNew COMMAND otbStatisticsTestDriver
  otbVectorImageToMatrixNewTest)

otb_add_test(NAME bfTvVectorImageToMatrix COMMAND otbStatisticsTestDriver
  otbVectorImageToMatrixTest)

otb_add_test(NAME bfTvListSampleToVariableDimensionHistogramGenerator COMMAND otbStatisticsTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfTvListSampleToVariableDimensionHistogramGeneratorOutput.txt
  ${TEMP}/bfTvListSampleToVariableDimensionHistogramGeneratorOutput.txt
  otbListSampleToVariableDimensionHistogramGenerator
  ${INPUTDATA}/couleurs.tif
  ${TEMP}/bfTvListSampleToVariableDimensionHistogramGeneratorOutput.txt
  10 1
  )

otb_add_test(NAME bfTuStreamingMinMaxImageFilterNew COMMAND otbStatisticsTestDriver
  otbStreamingMinMaxImageFilterNew)

otb_add_test(NAME leTuShiftScaleSampleListFilterNew COMMAND otbStatisticsTestDriver
  otbShiftScaleSampleListFilterNew)

otb_add_test(NAME leTvShiftScaleSampleListFilter COMMAND otbStatisticsTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/leTvShiftScaleSampleListFilterOutput.txt
  ${TEMP}/leTvShiftScaleSampleListFilterOutput.txt
  otbShiftScaleSampleListFilter
  ${TEMP}/leTvShiftScaleSampleListFilterOutput.txt
  2
  -1 -3
  1  2
  -2 -5
  -1 -3
  0 -1
  )

otb_add_test(NAME bfTvVectorImageToIntensityImageFilter COMMAND otbStatisticsTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvVectorImageToIntensityImageOutput.tif
  ${TEMP}/bfTvVectorImageToIntensityImageOutput.tif
  otbVectorImageToIntensityImageFilter
  ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
  ${TEMP}/bfTvVectorImageToIntensityImageOutput.tif
  )

otb_add_test(NAME bfTvVarianceImageFilter COMMAND otbStatisticsTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfVarianceImageFilter.tif
  ${TEMP}/bfVarianceImageFilter.tif
  otbVarianceImageFilter
  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  ${TEMP}/bfVarianceImageFilter.tif
  )

otb_add_test(NAME leTuConcatenateSampleListFilterNew COMMAND otbStatisticsTestDriver
  otbConcatenateSampleListFilterNew)

otb_add_test(NAME leTvConcatenateSampleListFilter COMMAND otbStatisticsTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/leTvConcatenateSampleListFilterOutput.txt
  ${TEMP}/leTvConcatenateSampleListFilterOutput.txt
  otbConcatenateSampleListFilter
  ${TEMP}/leTvConcatenateSampleListFilterOutput.txt
  2 3 2
  -1 -3
  1  2
  -2 -5
  -1 -3
  0 -1
  )

otb_add_test(NAME feTvLocalHistogramImageFunctionTest COMMAND otbStatisticsTestDriver
  --compare-ascii ${EPSILON_8}
  ${BASELINE_FILES}/feLocalHistogramImage.txt
  ${TEMP}/feLocalHistogramImage.txt
  otbLocalHistogramImageFunctionTest
  ${INPUTDATA}/pyramide.tif
  ${TEMP}/feLocalHistogramImage.txt
  127 127 127 0 128
  )

otb_add_test(NAME bfTuVectorImageToIntensityImageFilterNew COMMAND otbStatisticsTestDriver
  otbVectorImageToIntensityImageFilterNew)

otb_add_test(NAME bfTuProjectiveProjectionNew COMMAND otbStatisticsTestDriver
  otbProjectiveProjectionNew)

otb_add_test(NAME bfTvProjectiveProjectionTestHighSNR COMMAND otbStatisticsTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvProjectiveProjectionTestHighSNR.tif
  ${TEMP}/bfTvProjectiveProjectionTestHighSNR.tif
  otbProjectiveProjectionTestHighSNR
  ${INPUTDATA}/cupriteSubHsi.tif
  3
  ${TEMP}/bfTvProjectiveProjectionTestHighSNR.tif)

otb_add_test(NAME bfTvShiftScaleVectorImageFilter COMMAND otbStatisticsTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvShiftScaleVImageOutput.tif
  ${TEMP}/bfTvShiftScaleVImageOutput.tif
  otbShiftScaleVectorImageFilterTest
  ${INPUTDATA}/qb_RoadExtract.img.hdr
  ${TEMP}/bfTvShiftScaleVImageOutput.tif
  )

otb_add_test(NAME bfTuContinuousMinimumMaximumImageCalculatorNew COMMAND otbStatisticsTestDriver
  otbContinuousMinimumMaximumImageCalculatorNew
  )

otb_add_test(NAME bfTuStreamingCompareImageFilterNew COMMAND otbStatisticsTestDriver
  otbStreamingCompareImageFilterNew)

otb_add_test(NAME bfTvStreamingCompareImageFilter COMMAND otbStatisticsTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfStreamingCompareImageFilterResults.txt
  ${TEMP}/bfStreamingCompareImageFilterResults.txt
  otbStreamingCompareImageFilter
  ${INPUTDATA}/small_poupees_1canal.hd
  ${TEMP}/bfStreamingCompareImageFilterResults.txt)

otb_add_test(NAME feTuLocalHistogramImageFunctionNew COMMAND otbStatisticsTestDriver
  otbLocalHistogramImageFunctionNew
  )

otb_add_test(NAME bfTvRealAndImaginaryImageToComplexImageFilterTest COMMAND otbStatisticsTestDriver
  otbRealAndImaginaryImageToComplexImageFilterTest
  ${INPUTDATA}/GomaAvant.png
  ${INPUTDATA}/GomaApres.png
  )

otb_add_test(NAME bfTvStreamingStatisticsImageFilterWithNaNs COMMAND otbStatisticsTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfStreamingStatisticsImageFilterWithNaNsResults.txt
  ${TEMP}/bfStreamingStatisticsImageFilterWithNaNsResults.txt
  otbStreamingStatisticsImageFilter
  ${INPUTDATA}/small_poupees_1canalWithNaNs.TIF
  ${TEMP}/bfStreamingStatisticsImageFilterWithNaNsResults.txt)

otb_add_test(NAME bfTvStreamingStatisticsImageFilterWithBckGrdVal COMMAND otbStatisticsTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfStreamingStatisticsImageFilterWithBckGrdValResults.txt
  ${TEMP}/bfStreamingStatisticsImageFilterWithBckGrdValResults.txt
  otbStreamingStatisticsImageFilter
  ${INPUTDATA}/small_poupees_1canalWithNaNs.TIF
  ${TEMP}/bfStreamingStatisticsImageFilterWithBckGrdValResults.txt
  0 )

otb_add_test(NAME bfTvStreamingStatisticsImageFilter COMMAND otbStatisticsTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfStreamingStatisticsImageFilterResults.txt
  ${TEMP}/bfStreamingStatisticsImageFilterResults.txt
  otbStreamingStatisticsImageFilter
  ${INPUTDATA}/small_poupees_1canal.hd
  ${TEMP}/bfStreamingStatisticsImageFilterResults.txt)
  
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

    otb_add_test(NAME bfTvStreamingStatisticsMapFromLabelImageFilterTest${inputFileType}${lsizeType} COMMAND otbStatisticsTestDriver
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

otb_add_test(NAME leTuListSampleToBalancedListSampleFilterNew COMMAND otbStatisticsTestDriver
  otbListSampleToBalancedListSampleFilterNew)

otb_add_test(NAME leTvListSampleToBalancedListSampleFilter COMMAND otbStatisticsTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/leTvListSampleToBalancedListSampleFilterOutput.txt
  ${TEMP}/leTvListSampleToBalancedListSampleFilterOutput.txt
  otbListSampleToBalancedListSampleFilter
  ${TEMP}/leTvListSampleToBalancedListSampleFilterOutput.txt
  2
  -1 -3  0 # The third element is the label of the SampleList
  1  2  1
  -2 -5  0
  -1 -3  1
  0 -1  1
  -3  1  1
  -5  2  1
  2  1  1
  2  8  1
  1 -4  0
  -1  5  4
  2  5  1
  0 -5  0
  1 -1  2
  )


otb_add_test(NAME bfTvStreamingStatisticsVectorImageFilterWithNaNs COMMAND otbStatisticsTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfTvStreamingStatisticsVectorImageFilterWithNaNsResults.txt
  ${TEMP}/bfTvStreamingStatisticsVectorImageFilterWithNaNsResults.txt
  otbStreamingStatisticsVectorImageFilter
  ${INPUTDATA}/small_poupees_WithNaNs.TIF
  ${TEMP}/bfTvStreamingStatisticsVectorImageFilterWithNaNsResults.txt
  )

otb_add_test(NAME bfTvStreamingStatisticsVectorImageFilter COMMAND otbStatisticsTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfTvStreamingStatisticsVectorImageFilterResults.txt
  ${TEMP}/bfTvStreamingStatisticsVectorImageFilterResults.txt
  otbStreamingStatisticsVectorImageFilter
  ${INPUTDATA}/couleurs_extrait.png
  ${TEMP}/bfTvStreamingStatisticsVectorImageFilterResults.txt
  )

otb_add_test(NAME bfTvStreamingStatisticsVectorImageFilterWithBckGrdVal COMMAND otbStatisticsTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfTvStreamingStatisticsVectorImageFilterWithBckGrdValResults.txt
  ${TEMP}/bfTvStreamingStatisticsVectorImageFilterWithBckGrdValResults.txt
  otbStreamingStatisticsVectorImageFilter
  ${INPUTDATA}/small_poupees_WithNaNs.TIF
  ${TEMP}/bfTvStreamingStatisticsVectorImageFilterWithBckGrdValResults.txt
  0
  )

otb_add_test(NAME bfTvStreamingMinMaxVectorImageFilter COMMAND otbStatisticsTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfTvStreamingMinMaxVectorImageFilterResults.txt
  ${TEMP}/bfTvStreamingMinMaxVectorImageFilterResults.txt
  otbStreamingMinMaxVectorImageFilter
  ${INPUTDATA}/couleurs_extrait.png
  ${TEMP}/bfTvStreamingMinMaxVectorImageFilterResults.txt
  )

otb_add_test(NAME leTuListSampleGeneratorNew COMMAND otbStatisticsTestDriver
  otbListSampleGeneratorNew)

otb_add_test(NAME leTvListSampleGenerator4 COMMAND otbStatisticsTestDriver
  --compare-n-ascii ${NOTOL} 2
  ${BASELINE_FILES}/leTvListSampleGenerator4.txt
  ${TEMP}/leTvListSampleGenerator4.txt
  ${BASELINE_FILES}/leTvListSampleGeneratorValidation4.txt
  ${TEMP}/leTvListSampleGeneratorValidation4.txt
  otbListSampleGenerator
  ${EXAMPLEDATA}/qb_RoadExtract.tif
  ${EXAMPLEDATA}/qb_RoadExtract_classification.shp
  ${TEMP}/leTvListSampleGenerator4.txt
  ${TEMP}/leTvListSampleGeneratorValidation4.txt
  -1
  -1
  0.5
  0
  )

otb_add_test(NAME leTvListSampleGenerator5 COMMAND otbStatisticsTestDriver
  --compare-n-ascii ${NOTOL} 2
  ${BASELINE_FILES}/leTvListSampleGenerator5.txt
  ${TEMP}/leTvListSampleGenerator5.txt
  ${BASELINE_FILES}/leTvListSampleGeneratorValidation5.txt
  ${TEMP}/leTvListSampleGeneratorValidation5.txt
  otbListSampleGenerator
  ${EXAMPLEDATA}/qb_RoadExtract.tif
  ${EXAMPLEDATA}/qb_RoadExtract_classification.shp
  ${TEMP}/leTvListSampleGenerator5.txt
  ${TEMP}/leTvListSampleGeneratorValidation5.txt
  8000
  8000
  0.5
  0
  )

otb_add_test(NAME leTvListSampleGenerator6 COMMAND otbStatisticsTestDriver
  --compare-n-ascii ${NOTOL} 2
  ${BASELINE_FILES}/leTvListSampleGenerator6.txt
  ${TEMP}/leTvListSampleGenerator6.txt
  ${BASELINE_FILES}/leTvListSampleGeneratorValidation6.txt
  ${TEMP}/leTvListSampleGeneratorValidation6.txt
  otbListSampleGenerator
  ${EXAMPLEDATA}/qb_RoadExtract.tif
  ${EXAMPLEDATA}/qb_RoadExtract_classification.shp
  ${TEMP}/leTvListSampleGenerator6.txt
  ${TEMP}/leTvListSampleGeneratorValidation6.txt
  15000
  15000
  0.5
  0
  )

otb_add_test(NAME leTvListSampleGenerator1 COMMAND otbStatisticsTestDriver
  --compare-n-ascii ${NOTOL} 2
  ${BASELINE_FILES}/leTvListSampleGenerator1.txt
  ${TEMP}/leTvListSampleGenerator1.txt
  ${BASELINE_FILES}/leTvListSampleGeneratorValidation1.txt
  ${TEMP}/leTvListSampleGeneratorValidation1.txt
  otbListSampleGenerator
  ${EXAMPLEDATA}/qb_RoadExtract.tif
  ${EXAMPLEDATA}/qb_RoadExtract_classification.shp
  ${TEMP}/leTvListSampleGenerator1.txt
  ${TEMP}/leTvListSampleGeneratorValidation1.txt
  -1
  -1
  0
  1
  )

otb_add_test(NAME leTvListSampleGenerator2 COMMAND otbStatisticsTestDriver
  --compare-n-ascii ${NOTOL} 2
  ${BASELINE_FILES}/leTvListSampleGenerator2.txt
  ${TEMP}/leTvListSampleGenerator2.txt
  ${BASELINE_FILES}/leTvListSampleGeneratorValidation2.txt
  ${TEMP}/leTvListSampleGeneratorValidation2.txt
  otbListSampleGenerator
  ${EXAMPLEDATA}/qb_RoadExtract.tif
  ${EXAMPLEDATA}/qb_RoadExtract_classification.shp
  ${TEMP}/leTvListSampleGenerator2.txt
  ${TEMP}/leTvListSampleGeneratorValidation2.txt
  -1
  -1
  0.5
  1
  )

otb_add_test(NAME leTvListSampleGenerator3 COMMAND otbStatisticsTestDriver
  --compare-n-ascii ${NOTOL} 2
  ${BASELINE_FILES}/leTvListSampleGenerator3.txt
  ${TEMP}/leTvListSampleGenerator3.txt
  ${BASELINE_FILES}/leTvListSampleGeneratorValidation3.txt
  ${TEMP}/leTvListSampleGeneratorValidation3.txt
  otbListSampleGenerator
  ${EXAMPLEDATA}/qb_RoadExtract.tif
  ${EXAMPLEDATA}/qb_RoadExtract_classification.shp
  ${TEMP}/leTvListSampleGenerator3.txt
  ${TEMP}/leTvListSampleGeneratorValidation3.txt
  100
  200
  0.5
  1
  )

otb_add_test(NAME bfTvImaginaryImageToComplexImageFilterTest COMMAND otbStatisticsTestDriver
  otbImaginaryImageToComplexImageFilterTest
  ${INPUTDATA}/GomaAvant.png
  )



otb_add_test(NAME bfTvListSampleToHistogramListGenerator COMMAND otbStatisticsTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfTvListSampleToVariableDimensionHistogramGeneratorOutput.txt
  ${TEMP}/bfTvListSampleToHistogramListGeneratorOutput.txt
  otbListSampleToHistogramListGenerator
  ${INPUTDATA}/couleurs.tif
  ${TEMP}/bfTvListSampleToHistogramListGeneratorOutput.txt
  10 1
  )

