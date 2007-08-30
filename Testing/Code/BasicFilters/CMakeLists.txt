
IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Images de teledetection (grosses images )
SET(IMAGEDATA ${OTB_DATA_ROOT}/LargeInput )
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)


#Tolerance sur diff pixel image
SET(TOL 0.0)
SET(EPSILON 0.0000000001)

SET(BASICFILTERS_TESTS ${CXX_TEST_PATH}/otbBasicFiltersTests)


ADD_TEST(bfTvFiltreLee1CanalPoupees ${BASICFILTERS_TESTS}  
  --compare-image ${TOL}  ${BASELINE}/bfFiltreLee_05_05_04.png
                          ${TEMP}/bfFiltreLee_05_05_04.png  
        otbLeeFilter
	${INPUTDATA}/poupees_1canal.c1.hdr
	${TEMP}/bfFiltreLee_05_05_04.png
	05 05 4.0)

ADD_TEST(bfTvFiltreLee ${BASICFILTERS_TESTS}  
  --compare-image ${TOL}  ${BASELINE}/bfFiltreLee_05_05_12.png
                           ${TEMP}/bfFiltreLee_05_05_12.png  
        otbLeeFilter
	${INPUTDATA}/poupees.hdr
	${TEMP}/bfFiltreLee_05_05_12.png
	05 05 12.0)

#
# Filtre de FROST:
#
	
ADD_TEST(bfTvFiltreFrost ${BASICFILTERS_TESTS}  
  --compare-image ${TOL}  ${BASELINE}/bfFiltreFrost_poupees_05_05_01.png
                          ${TEMP}/bfFiltreFrost_poupees_05_05_01.png 
        otbFrostFilter
	${INPUTDATA}/poupees.hdr
	${TEMP}/bfFiltreFrost_poupees_05_05_01.png
	05 05 0.1)

# -------            otb::PointSetSource   ------------------------------

ADD_TEST(bfTuImageToPointSetFilterTest ${BASICFILTERS_TESTS} 
         otbImageToPointSetFilterTest)  

# -------            otb::OpeningClosingMorphologicalFilter   ------------------------------

ADD_TEST(bfTuOpeningClosingFilterNew ${BASICFILTERS_TESTS}  
        otbOpeningClosingMorphologicalFilterNew )

ADD_TEST(bfTvOpeningClosingFilter ${BASICFILTERS_TESTS}
   --compare-image ${TOL}  ${BASELINE}/bfFiltreOpeningClosing_1.tif
                           ${TEMP}/bfFiltreOpeningClosing_1.tif
        otbOpeningClosingMorphologicalFilter
        ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
	${TEMP}/bfFiltreOpeningClosing_1.tif
	1)

# -------            otb::ClosingOpeningMorphologicalFilter   ------------------------------

ADD_TEST(bfTuClosingOpeningFilterNew ${BASICFILTERS_TESTS}  
        otbClosingOpeningMorphologicalFilterNew )

ADD_TEST(bfTvClosingOpeningFilter ${BASICFILTERS_TESTS}
   --compare-image ${TOL}  ${BASELINE}/bfFiltreClosingOpening_1.tif
                           ${TEMP}/bfFiltreClosingOpening_1.tif
        otbClosingOpeningMorphologicalFilter
        ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
	${TEMP}/bfFiltreClosingOpening_1.tif
	1)


# -------            otb::BinaryImageMinimalBoundingRegionCalculator   ----------------------------

ADD_TEST(bfTuBoundingRegionCalculatorNew ${BASICFILTERS_TESTS}  
         otbBinaryImageMinimalBoundingRegionCalculatorNew)

ADD_TEST(bfTvBoundingRegionCalculator ${BASICFILTERS_TESTS}
   --compare-ascii ${TOL}
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

# -------            otb::LogPolarTransform   ----------------------------

ADD_TEST(bfTuLogPolarTransformNew ${BASICFILTERS_TESTS}  
         otbLogPolarTransformNew)

ADD_TEST(bfTvLogPolarTransform ${BASICFILTERS_TESTS}
   --compare-ascii ${TOL}
 		   ${BASELINE_FILES}/bfLogPolarTransformResults.txt
 		   ${TEMP}/bfLogPolarTransformResults.txt
        otbLogPolarTransform
	1 1 
	${TEMP}/bfLogPolarTransformResults.txt
	5 0 0 0 10 45 0 3.14 3.14 18 1
	)

ADD_TEST(bfTvLogPolarTransformResample ${BASICFILTERS_TESTS}  
  --compare-image ${EPSILON}  ${BASELINE}/bfLogPolarTransformResampleOutput.hdr
                          ${TEMP}/bfLogPolarTransformResampleOutput.hdr
        otbLogPolarTransformResample
        ${INPUTDATA}/circle.hdr
	${TEMP}/bfLogPolarTransformResampleOutput.hdr
)


# -------            otb::InverseLogPolarTransform   ----------------------------

ADD_TEST(bfTuInverseLogPolarTransformNew ${BASICFILTERS_TESTS}  
         otbInverseLogPolarTransformNew)

ADD_TEST(bfTvInverseLogPolarTransform ${BASICFILTERS_TESTS}
     --compare-ascii ${TOL}
		     ${BASELINE_FILES}/bfInverseLogPolarTransformResults.txt
		     ${TEMP}/bfInverseLogPolarTransformResults.txt
        otbInverseLogPolarTransform
	1 1 
	${TEMP}/bfInverseLogPolarTransformResults.txt	
	6 0 0 10 20 45 60 3.14 3.14 1 0 0 1
	)

ADD_TEST(bfTvInverseLogPolarTransformResample ${BASICFILTERS_TESTS}  
--compare-image ${TOL}  ${BASELINE}/bfInverseLogPolarTransformResampleOutput.hdr
                          ${TEMP}/bfInverseLogPolarTransformResampleOutput.hdr
        otbInverseLogPolarTransformResample
	${BASELINE}/bfLogPolarTransformResampleOutput.hdr
	${TEMP}/bfInverseLogPolarTransformResampleOutput.hdr
)

# -------            otb::StreamingShrinkImageFilter   ----------------------------

  ADD_TEST(bfTuStreamingShrinkImageFilterNew ${BASICFILTERS_TESTS}  
         otbStreamingShrinkImageFilterNew)


 ADD_TEST(bfTvStreamingShrinkImageFilterQBPAN ${BASICFILTERS_TESTS}  
--compare-image ${TOL}
		${BASELINE}/bfTvStreamingShrinkImageFilterQBPANOutput.hdr
		${TEMP}/bfTvStreamingShrinkImageFilterQBPANOutput.hdr
	otbStreamingShrinkImageFilter
	${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
	${TEMP}/bfTvStreamingShrinkImageFilterQBPANOutput.hdr
	50
)
	
ADD_TEST(bfTvStreamingShrinkImageFilterQBMUL ${BASICFILTERS_TESTS}  
--compare-image ${TOL}
		${BASELINE}/bfTvStreamingShrinkImageFilterQBMULOutput.hdr
		${TEMP}/bfTvStreamingShrinkImageFilterQBMULOutput.hdr
		otbStreamingShrinkImageFilter
	${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
	${TEMP}/bfTvStreamingShrinkImageFilterQBMULOutput.hdr
	20
)

# -------            otb::SpatialObjectToImageDrawingFilter   ----------------------------

  ADD_TEST(bfTuSpatialObjectToImageDrawingFilterNew ${BASICFILTERS_TESTS}  
         otbSpatialObjectToImageDrawingFilterNew)


 ADD_TEST(bfTvSpatialObjectToImageDrawingFilterWithoutInputImage ${BASICFILTERS_TESTS}  
 --compare-image ${TOL}
		${BASELINE}/bfTvSpatialObjectDrawingNoInput.png
		${TEMP}/bfTvSpatialObjectDrawingNoInput.png
	otbSpatialObjectToImageDrawingFilter
	${TEMP}/bfTvSpatialObjectDrawingNoInput.png
	100
	50
)

# -------            otb::ImportGeoInformationImageFilter   ----------------------------

  ADD_TEST(bfTuImportGeoInformationImageFilterNew ${BASICFILTERS_TESTS}  
         otbImportGeoInformationImageFilterNew)


 ADD_TEST(bfTvImportGeoInformationImageFilter ${BASICFILTERS_TESTS}  
 --compare-metadata ${TOL}
		    ${INPUTDATA}/HFAGeoreferenced.img
		    ${TEMP}/bfTvImportGeoInformationOutput.img
	otbImportGeoInformationImageFilter
		    ${INPUTDATA}/HFAGeoreferenced.img
	            ${TEMP}/bfTvImportGeoInformationOutput.img
)

# -------            otb::SpectralAngleDistanceImageFilter   ----------------------------

  ADD_TEST(bfTuSpectralAngleDistanceImageFilterNew ${BASICFILTERS_TESTS}  
         otbSpectralAngleDistanceImageFilterNew)


 ADD_TEST(bfTvSpectralAngleDistanceImageFilter ${BASICFILTERS_TESTS} 
		--compare-image ${EPSILON}
		    ${BASELINE}/bfTvSpectralAngleDistanceOutput.hdr
		    ${TEMP}/bfTvSpectralAngleDistanceOutput.hdr
		    otbSpectralAngleDistanceImageFilter
		    ${INPUTDATA}/qb_RoadExtract.img.hdr
	            ${TEMP}/bfTvSpectralAngleDistanceOutput.hdr
		    337 557 432 859
)


# -------            otb::VectorRescaleIntensityImageFilter   ----------------------------

ADD_TEST(bfTuVectorRescaleIntensityImageFilterNew ${BASICFILTERS_TESTS}  
         otbVectorRescaleIntensityImageFilterNew)


ADD_TEST(bfTVectorRescaleIntensityImageFilter ${BASICFILTERS_TESTS} 
		--compare-image ${EPSILON}
		    ${BASELINE}/bfTvVectorRescaleIntensityOutput.png
		    ${TEMP}/bfTvVectorRescaleIntensityOutput.png
		    otbVectorRescaleIntensityImageFilter
		    ${INPUTDATA}/qb_RoadExtract.img.hdr
	            ${TEMP}/bfTvVectorRescaleIntensityOutput.png
		    0 255
)

# -------            otb::VectorImageToImageListFilter   ----------------------------

ADD_TEST(bfTuVectorImageToImageListFilterNew ${BASICFILTERS_TESTS}  
         otbVectorImageToImageListFilterNew)


ADD_TEST(bfTvVectorImageToImageListFilter ${BASICFILTERS_TESTS} 
		--compare-n-images ${EPSILON} 3
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


# -------            otb::ImageListToVectorImageFilter   ----------------------------

ADD_TEST(bfTuImageListToVectorImageFilterNew ${BASICFILTERS_TESTS}  
         otbImageListToVectorImageFilterNew)


ADD_TEST(bfTvImageListToVectorImageFilter ${BASICFILTERS_TESTS} 
		--compare-image ${EPSILON} 
		    ${BASELINE}/bfTvImageListToVectorImageFilter.png
		    ${TEMP}/bfTvImageListToVectorImageFilter.png
		    otbImageListToVectorImageFilter
	            ${INPUTDATA}/poupees.c1
		    ${INPUTDATA}/poupees.c2
		    ${INPUTDATA}/poupees.c3
		    ${TEMP}/bfTvImageListToVectorImageFilter.png
)


# -------            otb::ImageListToImageListApplyFilter   ----------------------------

ADD_TEST(bfTuImageListToImageListApplyFilterNew ${BASICFILTERS_TESTS}  
         otbImageListToImageListApplyFilterNew)


ADD_TEST(bfTvImageListToImageListApplyFilter ${BASICFILTERS_TESTS} 
		--compare-n-images ${EPSILON} 3
		    ${BASELINE}/bfTvImageListToImageListApplyFilterBand1.png
		    ${TEMP}/bfTvImageListToImageListApplyFilterBand1.png
		    ${BASELINE}/bfTvImageListToImageListApplyFilterBand2.png
		    ${TEMP}/bfTvImageListToImageListApplyFilterBand2.png
		    ${BASELINE}/bfTvImageListToImageListApplyFilterBand3.png
		    ${TEMP}/bfTvImageListToImageListApplyFilterBand3.png
		    otbImageListToImageListApplyFilter
	            ${INPUTDATA}/poupees.c1
		    ${INPUTDATA}/poupees.c2
		    ${INPUTDATA}/poupees.c3
		    ${TEMP}/bfTvImageListToImageListApplyFilterBand1.png
		    ${TEMP}/bfTvImageListToImageListApplyFilterBand2.png
		    ${TEMP}/bfTvImageListToImageListApplyFilterBand3.png
)

# -------            otb::PerBandVectorImageFilter   ----------------------------

ADD_TEST(bfTuPerBandVectorImageFilterNew ${BASICFILTERS_TESTS}  
         otbPerBandVectorImageFilterNew)

ADD_TEST(bfTvPerBandVectorImageFilterWithMeanFilter ${BASICFILTERS_TESTS}
	--compare-image ${EPSILON}
			${BASELINE}/bfTvPerBandVectorImageFilterWithMeanFilterOutput.hdr
			${TEMP}/bfTvPerBandVectorImageFilterWithMeanFilterOutput.hdr			
	otbPerBandVectorImageFilterWithMeanFilter
			${INPUTDATA}/poupees.png
			${TEMP}/bfTvPerBandVectorImageFilterWithMeanFilterOutput.hdr
)

ADD_TEST(bfTvPerBandVectorImageFilterWithSobelFilter ${BASICFILTERS_TESTS}
	--compare-image ${EPSILON}
			${BASELINE}/bfTvPerBandVectorImageFilterWithSobelFilterOutput.hdr
			${TEMP}/bfTvPerBandVectorImageFilterWithSobelFilterOutput.hdr			
	otbPerBandVectorImageFilterWithSobelFilter
			${INPUTDATA}/poupees.png
			${TEMP}/bfTvPerBandVectorImageFilterWithSobelFilterOutput.hdr
)


# -------            otb::StreamingStatisticsImageFilter   ----------------------------

ADD_TEST(bfTuStreamingStatisticsImageFilterNew ${BASICFILTERS_TESTS}  
         otbStreamingStatisticsImageFilterNew)

ADD_TEST(bfTvStreamingStatisticsImageFilter ${BASICFILTERS_TESTS}  
	--compare-ascii ${TOL}
 		   ${BASELINE_FILES}/bfStreamingStatisticsImageFilterResults.txt
 		   ${TEMP}/bfStreamingStatisticsImageFilterResults.txt
         otbStreamingStatisticsImageFilter
		${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
		${TEMP}/bfStreamingStatisticsImageFilterResults.txt)

# -------            otb::VectorImageTo3DScalarImageFilter   ----------------------------

ADD_TEST(bfTuVectorImageTo3DScalarImageFilterNew ${BASICFILTERS_TESTS}  
         otbVectorImageTo3DScalarImageFilterNew)

ADD_TEST(bfTvVectorImageTo3DScalarImageFilter ${BASICFILTERS_TESTS}  
	--compare-image ${TOL}
 		   ${BASELINE}/bfTvVectorImageTo3DScalarImageFilterOutput.png
 		   ${TEMP}/bfTvVectorImageTo3DScalarImageFilterOutput.png
         otbVectorImageTo3DScalarImageFilter
		${INPUTDATA}/poupees.png
		${TEMP}/bfTvVectorImageTo3DScalarImageFilterOutput.png
)

# -------            otb::StreamingResampleImageFilter   ----------------------------

ADD_TEST(bfTuStreamingResampleImageFilterNew ${BASICFILTERS_TESTS}  
         otbStreamingResampleImageFilterNew)
				 
ADD_TEST(bfTvStreamingResampleImageFilter ${BASICFILTERS_TESTS}  
				otbStreamingResampleImageFilter
				${INPUTDATA}/poupees.tif
				${TEMP}/bfTvStreamingResamplePoupeesTest.tif
)

ADD_TEST(StreamingResampleImageFilterCompareWithITK ${BASICFILTERS_TESTS}  
		--compare-image ${TOL}
					${TEMP}/bfTvStreamingResamplePoupeesTestITK.tif
					${TEMP}/bfTvStreamingResamplePoupeesTestOTB.tif		
				otbStreamingResampleImageFilterCompareWithITK
					${INPUTDATA}/poupees.tif
					1100 1100
					${TEMP}/bfTvStreamingResamplePoupeesTestITK.tif
					${TEMP}/bfTvStreamingResamplePoupeesTestOTB.tif
)

#ADD_TEST(StreamingResampleImageFilterCompareWithITK ${BASICFILTERS_TESTS}  
#		--compare-image ${TOL}
#					${TEMP}/bfTvStreamingResampleSPOT5TestITK.tif
#					${TEMP}/bfTvStreamingResampleSPOT5TestOTB.tif		
#				otbStreamingResampleImageFilterCompareWithITK
#					${INPUTDATA}/spot5SubWithGcps.tif
#					1100 1100
#					${TEMP}/bfTvStreamingResampleSPOT5TestITK.tif
#					${TEMP}/bfTvStreamingResampleSPOT5TestOTB.tif
#)


# A enrichir
SET(BasicFilters_SRCS
otbLeeFilter.cxx
otbFrostFilterTest.cxx
otbFrostFilterNew.cxx
otbFrostFilter.cxx
otbImageToPointSetFilterTest.cxx
otbOpeningClosingMorphologicalFilterNew.cxx
otbOpeningClosingMorphologicalFilter.cxx
otbClosingOpeningMorphologicalFilterNew.cxx
otbClosingOpeningMorphologicalFilter.cxx
otbBinaryImageMinimalBoundingRegionCalculatorNew.cxx
otbBinaryImageMinimalBoundingRegionCalculator.cxx
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
otbImportGeoInformationImageFilterNew.cxx
otbImportGeoInformationImageFilter.cxx
otbSpectralAngleDistanceImageFilterNew.cxx
otbSpectralAngleDistanceImageFilter.cxx
otbVectorRescaleIntensityImageFilterNew.cxx
otbVectorRescaleIntensityImageFilter.cxx
otbVectorImageToImageListFilterNew.cxx
otbVectorImageToImageListFilter.cxx
otbImageListToVectorImageFilterNew.cxx
otbImageListToVectorImageFilter.cxx
otbImageListToImageListApplyFilterNew.cxx
otbImageListToImageListApplyFilter.cxx
otbPerBandVectorImageFilterNew.cxx
otbPerBandVectorImageFilterWithMeanFilter.cxx
otbPerBandVectorImageFilterWithSobelFilter.cxx
otbStreamingStatisticsImageFilterNew.cxx
otbStreamingStatisticsImageFilter.cxx
otbVectorImageTo3DScalarImageFilterNew.cxx
otbVectorImageTo3DScalarImageFilter.cxx
otbStreamingResampleImageFilterNew.cxx
otbStreamingResampleImageFilter.cxx
otbStreamingResampleImageFilterCompareWithITK.cxx
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbBasicFiltersTests otbBasicFiltersTests.cxx ${BasicFilters_SRCS})
TARGET_LINK_LIBRARIES(otbBasicFiltersTests  OTBBasicFilters OTBCommon OTBIO gdal ITKIO ITKCommon ITKBasicFilters)


ENDIF( NOT OTB_DISABLE_CXX_TESTING )




