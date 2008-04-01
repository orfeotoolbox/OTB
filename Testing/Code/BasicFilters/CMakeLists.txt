
IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Images de teledetection (grosses images )
SET(IMAGEDATA ${OTB_DATA_ROOT}/LargeInput )
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)


#Tolerance sur diff pixel image
SET(TOL 0.0)
SET(EPSILON 0.000000001)

SET(BASICFILTERS_TESTS1 ${CXX_TEST_PATH}/otbBasicFiltersTests1)
SET(BASICFILTERS_TESTS2 ${CXX_TEST_PATH}/otbBasicFiltersTests2)
SET(BASICFILTERS_TESTS3 ${CXX_TEST_PATH}/otbBasicFiltersTests3)
SET(BASICFILTERS_TESTS4 ${CXX_TEST_PATH}/otbBasicFiltersTests4)
SET(BASICFILTERS_TESTS5 ${CXX_TEST_PATH}/otbBasicFiltersTests5)
SET(BASICFILTERS_TESTS6 ${CXX_TEST_PATH}/otbBasicFiltersTests6)
SET(BASICFILTERS_TESTS7 ${CXX_TEST_PATH}/otbBasicFiltersTests7)
SET(BASICFILTERS_TESTS8 ${CXX_TEST_PATH}/otbBasicFiltersTests8)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbBasicFiltersTests1 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 

ADD_TEST(bfTvFiltreLee1CanalPoupees ${BASICFILTERS_TESTS1}  
  --compare-image ${EPSILON}  ${BASELINE}/bfFiltreLee_05_05_04.tif
                          ${TEMP}/bfFiltreLee_05_05_04.tif
        otbLeeFilter
	${INPUTDATA}/poupees_1canal.c1.hdr
	${TEMP}/bfFiltreLee_05_05_04.tif
	05 05 4.0)

ADD_TEST(bfTvFiltreLee ${BASICFILTERS_TESTS1}  
  --compare-image ${EPSILON}  ${BASELINE}/bfFiltreLee_05_05_12.tif
                           ${TEMP}/bfFiltreLee_05_05_12.tif  
        otbLeeFilter
	${INPUTDATA}/poupees.hdr
	${TEMP}/bfFiltreLee_05_05_12.tif
	05 05 12.0)

#
# Filtre de FROST:
#
	
ADD_TEST(bfTvFiltreFrost ${BASICFILTERS_TESTS1}  
  --compare-image ${EPSILON}  ${BASELINE}/bfFiltreFrost_poupees_05_05_01.tif
                          ${TEMP}/bfFiltreFrost_poupees_05_05_01.tif
        otbFrostFilter
	${INPUTDATA}/poupees.hdr
	${TEMP}/bfFiltreFrost_poupees_05_05_01.tif
	05 05 0.1)

# -------            otb::PointSetSource   ------------------------------

ADD_TEST(bfTuImageToPointSetFilterTest ${BASICFILTERS_TESTS1} 
         otbImageToPointSetFilterTest)  

# -------            otb::OpeningClosingMorphologicalFilter   ------------------------------

ADD_TEST(bfTuOpeningClosingFilterNew ${BASICFILTERS_TESTS1}  
        otbOpeningClosingMorphologicalFilterNew )

ADD_TEST(bfTvOpeningClosingFilter ${BASICFILTERS_TESTS1}
   --compare-image ${TOL}  ${BASELINE}/bfFiltreOpeningClosing_1.tif
                           ${TEMP}/bfFiltreOpeningClosing_1.tif
        otbOpeningClosingMorphologicalFilter
        ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
	${TEMP}/bfFiltreOpeningClosing_1.tif
	1)

# -------            otb::ClosingOpeningMorphologicalFilter   ------------------------------

ADD_TEST(bfTuClosingOpeningFilterNew ${BASICFILTERS_TESTS1}  
        otbClosingOpeningMorphologicalFilterNew )

ADD_TEST(bfTvClosingOpeningFilter ${BASICFILTERS_TESTS1}
   --compare-image ${TOL}  ${BASELINE}/bfFiltreClosingOpening_1.tif
                           ${TEMP}/bfFiltreClosingOpening_1.tif
        otbClosingOpeningMorphologicalFilter
        ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
	${TEMP}/bfFiltreClosingOpening_1.tif
	1)


# -------            otb::BinaryImageMinimalBoundingRegionCalculator   ----------------------------

ADD_TEST(bfTuBoundingRegionCalculatorNew ${BASICFILTERS_TESTS1}  
         otbBinaryImageMinimalBoundingRegionCalculatorNew)



ADD_TEST(bfTvBoundingRegionCalculator ${BASICFILTERS_TESTS1}
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

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbBasicFiltersTests2 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 

# -------            otb::LogPolarTransform   ----------------------------

ADD_TEST(bfTuLogPolarTransformNew ${BASICFILTERS_TESTS2}  
         otbLogPolarTransformNew)

ADD_TEST(bfTvLogPolarTransform ${BASICFILTERS_TESTS2}
   --compare-ascii ${TOL}
 		   ${BASELINE_FILES}/bfLogPolarTransformResults.txt
 		   ${TEMP}/bfLogPolarTransformResults.txt
        otbLogPolarTransform
	1 1 
	${TEMP}/bfLogPolarTransformResults.txt
	5 0 0 0 10 45 0 3.14 3.14 18 1
	)

ADD_TEST(bfTvLogPolarTransformResample ${BASICFILTERS_TESTS2}  
  --compare-image ${EPSILON}  ${BASELINE}/bfLogPolarTransformResampleOutput.hdr
                          ${TEMP}/bfLogPolarTransformResampleOutput.hdr
        otbLogPolarTransformResample
        ${INPUTDATA}/circle.hdr
	${TEMP}/bfLogPolarTransformResampleOutput.hdr
)


# -------            otb::InverseLogPolarTransform   ----------------------------

ADD_TEST(bfTuInverseLogPolarTransformNew ${BASICFILTERS_TESTS2}  
         otbInverseLogPolarTransformNew)

ADD_TEST(bfTvInverseLogPolarTransform ${BASICFILTERS_TESTS2}
     --compare-ascii ${TOL}
		     ${BASELINE_FILES}/bfInverseLogPolarTransformResults.txt
		     ${TEMP}/bfInverseLogPolarTransformResults.txt
        otbInverseLogPolarTransform
	1. 1. 
	${TEMP}/bfInverseLogPolarTransformResults.txt	
	6 0 0 10 20 45 60 3.14 3.14 1 0 0 1
	)

ADD_TEST(bfTvInverseLogPolarTransformResample ${BASICFILTERS_TESTS2}  
--compare-image ${TOL}  ${BASELINE}/bfInverseLogPolarTransformResampleOutput.hdr
                          ${TEMP}/bfInverseLogPolarTransformResampleOutput.hdr
        otbInverseLogPolarTransformResample
	${BASELINE}/bfLogPolarTransformResampleOutput.hdr
	${TEMP}/bfInverseLogPolarTransformResampleOutput.hdr
)

# -------            otb::StreamingShrinkImageFilter   ----------------------------

  ADD_TEST(bfTuStreamingShrinkImageFilterNew ${BASICFILTERS_TESTS2}  
         otbStreamingShrinkImageFilterNew)


 ADD_TEST(bfTvStreamingShrinkImageFilterQBPAN ${BASICFILTERS_TESTS2}  
--compare-image ${TOL}
		${BASELINE}/bfTvStreamingShrinkImageFilterQBPANOutput.hdr
		${TEMP}/bfTvStreamingShrinkImageFilterQBPANOutput.hdr
	otbStreamingShrinkImageFilter
	${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
	${TEMP}/bfTvStreamingShrinkImageFilterQBPANOutput.hdr
	50
)
	
ADD_TEST(bfTvStreamingShrinkImageFilterQBMUL ${BASICFILTERS_TESTS2}  
--compare-image ${TOL}
		${BASELINE}/bfTvStreamingShrinkImageFilterQBMULOutput.hdr
		${TEMP}/bfTvStreamingShrinkImageFilterQBMULOutput.hdr
		otbStreamingShrinkImageFilter
	${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
	${TEMP}/bfTvStreamingShrinkImageFilterQBMULOutput.hdr
	20
)

# -------            otb::SpatialObjectToImageDrawingFilter   ----------------------------

  ADD_TEST(bfTuSpatialObjectToImageDrawingFilterNew ${BASICFILTERS_TESTS2}  
         otbSpatialObjectToImageDrawingFilterNew)


 ADD_TEST(bfTvSpatialObjectToImageDrawingFilterWithoutInputImage ${BASICFILTERS_TESTS2}  
 --compare-image ${TOL}
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
 --compare-metadata ${TOL}
		    ${INPUTDATA}/HFAGeoreferenced.img
		    ${TEMP}/bfTvImportGeoInformationOutput.img
	otbImportGeoInformationImageFilter
		    ${INPUTDATA}/HFAGeoreferenced.img
	            ${TEMP}/bfTvImportGeoInformationOutput.img
)

# -------            otb::SpectralAngleDistanceImageFilter   ----------------------------

  ADD_TEST(bfTuSpectralAngleDistanceImageFilterNew ${BASICFILTERS_TESTS3}  
         otbSpectralAngleDistanceImageFilterNew)


 ADD_TEST(bfTvSpectralAngleDistanceImageFilter ${BASICFILTERS_TESTS3} 
		--compare-image ${EPSILON}
		    ${BASELINE}/bfTvSpectralAngleDistanceOutput.hdr
		    ${TEMP}/bfTvSpectralAngleDistanceOutput.hdr
		    otbSpectralAngleDistanceImageFilter
		    ${INPUTDATA}/qb_RoadExtract.img.hdr
	            ${TEMP}/bfTvSpectralAngleDistanceOutput.hdr
		    337 557 432 859
)


# -------            otb::VectorRescaleIntensityImageFilter   ----------------------------

ADD_TEST(bfTuVectorRescaleIntensityImageFilterNew ${BASICFILTERS_TESTS3}  
         otbVectorRescaleIntensityImageFilterNew)


ADD_TEST(bfTVectorRescaleIntensityImageFilter ${BASICFILTERS_TESTS3} 
		--compare-image ${EPSILON}
		    ${BASELINE}/bfTvVectorRescaleIntensityOutput.png
		    ${TEMP}/bfTvVectorRescaleIntensityOutput.png
		    otbVectorRescaleIntensityImageFilter
		    ${INPUTDATA}/qb_RoadExtract.img.hdr
	            ${TEMP}/bfTvVectorRescaleIntensityOutput.png
		    0 255
)

# -------            otb::VectorImageToImageListFilter   ----------------------------

ADD_TEST(bfTuVectorImageToImageListFilterNew ${BASICFILTERS_TESTS3}  
         otbVectorImageToImageListFilterNew)


ADD_TEST(bfTvVectorImageToImageListFilter ${BASICFILTERS_TESTS3} 
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

ADD_TEST(bfTuImageListToVectorImageFilterNew ${BASICFILTERS_TESTS3}  
         otbImageListToVectorImageFilterNew)


ADD_TEST(bfTvImageListToVectorImageFilter ${BASICFILTERS_TESTS3} 
		--compare-image ${EPSILON} 
		    ${BASELINE}/bfTvImageListToVectorImageFilter.png
		    ${TEMP}/bfTvImageListToVectorImageFilter.png
		    otbImageListToVectorImageFilter
	            ${INPUTDATA}/poupees.c1
		    ${INPUTDATA}/poupees.c2
		    ${INPUTDATA}/poupees.c3
		    ${TEMP}/bfTvImageListToVectorImageFilter.png
)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbBasicFiltersTests4 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# -------            otb::ImageListToImageListApplyFilter   ----------------------------

ADD_TEST(bfTuImageListToImageListApplyFilterNew ${BASICFILTERS_TESTS4}  
         otbImageListToImageListApplyFilterNew)


ADD_TEST(bfTvImageListToImageListApplyFilter ${BASICFILTERS_TESTS4} 
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

ADD_TEST(bfTuPerBandVectorImageFilterNew ${BASICFILTERS_TESTS4}  
         otbPerBandVectorImageFilterNew)

ADD_TEST(bfTvPerBandVectorImageFilterWithMeanFilter ${BASICFILTERS_TESTS4}
	--compare-image ${EPSILON}
			${BASELINE}/bfTvPerBandVectorImageFilterWithMeanFilterOutput.hdr
			${TEMP}/bfTvPerBandVectorImageFilterWithMeanFilterOutput.hdr			
	otbPerBandVectorImageFilterWithMeanFilter
			${INPUTDATA}/poupees.png
			${TEMP}/bfTvPerBandVectorImageFilterWithMeanFilterOutput.hdr
)

ADD_TEST(bfTvPerBandVectorImageFilterWithSobelFilter ${BASICFILTERS_TESTS4}
	--compare-image ${EPSILON}
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
	--compare-ascii ${TOL}
 		   ${BASELINE_FILES}/bfStreamingStatisticsImageFilterResults.txt
 		   ${TEMP}/bfStreamingStatisticsImageFilterResults.txt
         otbStreamingStatisticsImageFilter
		${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
		${TEMP}/bfStreamingStatisticsImageFilterResults.txt)

# -------            otb::VectorImageTo3DScalarImageFilter   ----------------------------

ADD_TEST(bfTuVectorImageTo3DScalarImageFilterNew ${BASICFILTERS_TESTS4}  
         otbVectorImageTo3DScalarImageFilterNew)

ADD_TEST(bfTvVectorImageTo3DScalarImageFilter ${BASICFILTERS_TESTS4}  
	--compare-image ${TOL}
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

ADD_TEST(bfTuStreamingResampleImageFilterNew ${BASICFILTERS_TESTS5}  
         otbStreamingResampleImageFilterNew)
				 
ADD_TEST(bfTvStreamingResampleImageFilter ${BASICFILTERS_TESTS5}  
				otbStreamingResampleImageFilter
				${INPUTDATA}/poupees.tif
				${TEMP}/bfTvStreamingResamplePoupeesTest.tif
)

ADD_TEST(StreamingResampleImageFilterCompareWithITK ${BASICFILTERS_TESTS5}  
		--compare-image ${TOL}
					${TEMP}/bfTvStreamingResamplePoupeesTestITK.tif
					${TEMP}/bfTvStreamingResamplePoupeesTestOTB.tif		
				otbStreamingResampleImageFilterCompareWithITK
					${INPUTDATA}/poupees.tif
					3000 3000
					${TEMP}/bfTvStreamingResamplePoupeesTestITK.tif
					${TEMP}/bfTvStreamingResamplePoupeesTestOTB.tif
)

#ADD_TEST(StreamingResampleImageFilterCompareWithITK ${BASICFILTERS_TESTS5}  
#		--compare-image ${TOL}
#					${TEMP}/bfTvStreamingResampleSPOT5TestITK.tif
#					${TEMP}/bfTvStreamingResampleSPOT5TestOTB.tif		
#				otbStreamingResampleImageFilterCompareWithITK
#					${INPUTDATA}/spot5SubWithGcps.tif
#					1100 1100
#					${TEMP}/bfTvStreamingResampleSPOT5TestITK.tif
#					${TEMP}/bfTvStreamingResampleSPOT5TestOTB.tif
#)



# -------            otb::StreamingStatisticsVectorImageFilter   ----------------------------

ADD_TEST(bfTuStreamingStatisticsVectorImageFilterNew ${BASICFILTERS_TESTS5}  
         otbStreamingStatisticsVectorImageFilterNew)

ADD_TEST(bfTvStreamingStatisticsVectorImageFilter ${BASICFILTERS_TESTS5}  
	--compare-ascii ${TOL}
 		   ${BASELINE_FILES}/bfTvStreamingStatisticsVectorImageFilterResults.txt
 		   ${TEMP}/bfTvStreamingStatisticsVectorImageFilterResults.txt
         otbStreamingStatisticsVectorImageFilter
		#${IMAGEDATA}/SPOT5_SCENE01/IMAGERY.TIF
		${INPUTDATA}/sbuv.png
		${TEMP}/bfTvStreamingStatisticsVectorImageFilterResults.txt
		)
		

# -------            otb::MatrixTransposeMatrixImageFilter   ----------------------------

ADD_TEST(bfTuMatrixTransposeMatrixImageFilterNew ${BASICFILTERS_TESTS5}  
         otbMatrixTransposeMatrixImageFilterNew)

ADD_TEST(bfTvMatrixTransposeMatrixImageFilter ${BASICFILTERS_TESTS5}  
	--compare-ascii ${TOL}
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
     --compare-image ${TOL}
     		     ${BASELINE}/bfTvUnaryImageFunctorWithVectorImageFilter.tif
		     ${TEMP}/bfTvUnaryImageFunctorWithVectorImageFilter.tif
         otbUnaryImageFunctorWithVectorImageFilter
		${INPUTDATA}/poupees_sub.png
		${TEMP}/bfTvUnaryImageFunctorWithVectorImageFilter.tif
		)

# -------            otb::PrintableImageFilter   ------------------------------

ADD_TEST(bfTuPrintableImageFilterNew ${BASICFILTERS_TESTS5}  
         otbPrintableImageFilterNew)
         
ADD_TEST(bfTvPrintableImageFilter ${BASICFILTERS_TESTS5}  
     --compare-image ${TOL}
     		     ${BASELINE}/bfTvPrintableImageFilter.png
		     ${TEMP}/bfTvPrintableImageFilter.png
         otbPrintableImageFilter
		${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
		${TEMP}/bfTvPrintableImageFilter.png
		)



# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbBasicFiltersTests6 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::BSplineDecompositionImageFilter  ------------------------------

ADD_TEST(bfTuBSplineDecompositionImageFilterNew ${BASICFILTERS_TESTS6}  
         otbBSplineDecompositionImageFilterNew)
         
ADD_TEST(bfTvBSplineDecompositionImageFilter ${BASICFILTERS_TESTS6}  
     --compare-image ${EPSILON}
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
     --compare-ascii ${EPSILON}
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
     --compare-ascii ${TOL}
     		     ${BASELINE_FILES}/bfProlateInterpolateImageFunctionOutput.txt
		     ${TEMP}/bfProlateInterpolateImageFunctionOutput.txt
        otbProlateInterpolateImageFunction
		${INPUTDATA}/poupees.tif
		${TEMP}/bfProlateInterpolateImageFunctionOutput.txt
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
     --compare-ascii ${TOL}
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
     --compare-ascii ${TOL}
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

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbBasicFiltersTests7 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::WindowedSincInterpolateImageCosineFunction  ------------------------------
 ADD_TEST(bfTuWindowedSincInterpolateImageCosineFunctionNew ${BASICFILTERS_TESTS7}  
         otbWindowedSincInterpolateImageCosineFunctionNew)

ADD_TEST(bfTvWindowedSincInterpolateImageCosineFunction ${BASICFILTERS_TESTS7}  
     --compare-ascii ${TOL}
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
     --compare-ascii ${TOL}
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
     --compare-ascii ${TOL}
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
     --compare-ascii ${TOL}
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

ADD_TEST(ProlateValidationTest ${BASICFILTERS_TESTS7}  
  --compare-image ${EPSILON}
     		  ${BASELINE}/bfTvProlateValidationTest.tif
		  ${TEMP}/bfTvProlateValidationTest.tif
        otbProlateValidationTest
		${INPUTDATA}/poupees.tif
		${TEMP}/bfTvProlateValidationTest.tif
		3 # radius
		0.5# factor
		512 # size	
		${TEMP}/defaultprolatevalidationtest.tif # nearest neighborhood interpolator : NOT GENERATE IN THE TEST
		)


# -------            otb::VectorImageToAmplitudeImageFilter  ------------------------------

ADD_TEST(bfTuVectorImageToAmplitudeImageFilterNew ${BASICFILTERS_TESTS7}
otbVectorImageToAmplitudeImageFilterNew)

ADD_TEST(bfTvVectorImageToAmplitudeImageFilter ${BASICFILTERS_TESTS7}
 --compare-image ${EPSILON}
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
 --compare-image ${EPSILON}
     		  ${BASELINE}/bfTvVectorImageToAmplitudeImageFilterOutput.tif
		  ${TEMP}/bfTvVectorImageToAmplitudeImageFilterOutput.tif
otbVectorImageToAmplitudeImageFilter
${INPUTDATA}/poupees.tif
${TEMP}/bfTvVectorImageToAmplitudeImageFilterOutput.tif
)


# -------    otb::RationalQuotientResampleImageFilter  --------------------------

ADD_TEST(bfTuRationalQuotientResampleImageFilterNew ${BASICFILTERS_TESTS7}
         otbRationalQuotientResampleImageFilterNew)

ADD_TEST(bfTvRationalQuotientResampleImageFilter ${BASICFILTERS_TESTS7}
	--compare-image ${EPSILON}
			${BASELINE}/bfTvRationalQuotientResampleImageFilterOutput.tif
			${TEMP}/bfTvRationalQuotientResampleImageFilterOutput.tif
        otbRationalQuotientResampleImageFilter
		${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
		${TEMP}/bfTvRationalQuotientResampleImageFilterOutput.tif
		2 3
)

# -------            otb::VectorImageToIntensityImageFilter   ----------------------------

  ADD_TEST(bfTuVectorImageToIntensityImageFilterNew ${BASICFILTERS_TESTS7}  
         otbVectorImageToIntensityImageFilterNew)


 ADD_TEST(bfTvVectorImageToIntensityImageFilter ${BASICFILTERS_TESTS7} 
		--compare-image ${EPSILON}
		    ${BASELINE}/bfTvVectorImageToIntensityImageOutput.tif
		    ${TEMP}/bfTvVectorImageToIntensityImageOutput.tif
		    otbVectorImageToIntensityImageFilter
		    ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
	            ${TEMP}/bfTvVectorImageToIntensityImageOutput.tif
)

# -------            otb::LabelizeImageFilter   ----------------------------

  ADD_TEST(bfTuLabelizeImageFilterNew ${BASICFILTERS_TESTS8}
         otbLabelizeImageFilterNew)

  ADD_TEST(bfTvLabelizeImageFilter ${BASICFILTERS_TESTS8} 
		--compare-image ${EPSILON}
		    ${BASELINE}/bfTvLabelizeImageFilterOutput.tif
		    ${TEMP}/bfTvLabelizeImageFilterOutput.tif
		    otbLabelizeImageFilter
		    ${INPUTDATA}/cthead1.png
	            ${TEMP}/bfTvLabelizeImageFilterOutput.png
		    127 128
		    10 10
)

# A enrichir
SET(BasicFilters_SRCS1
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
)

# A enrichir
SET(BasicFilters_SRCS2
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
)

# A enrichir
SET(BasicFilters_SRCS4
otbImageListToImageListApplyFilterNew.cxx
otbImageListToImageListApplyFilter.cxx
otbPerBandVectorImageFilterNew.cxx
otbPerBandVectorImageFilterWithMeanFilter.cxx
otbPerBandVectorImageFilterWithSobelFilter.cxx
otbStreamingStatisticsImageFilterNew.cxx
otbStreamingStatisticsImageFilter.cxx
otbVectorImageTo3DScalarImageFilterNew.cxx
otbVectorImageTo3DScalarImageFilter.cxx
)

# A enrichir
SET(BasicFilters_SRCS5
otbStreamingResampleImageFilterNew.cxx
otbStreamingResampleImageFilter.cxx
otbStreamingResampleImageFilterCompareWithITK.cxx
otbStreamingStatisticsVectorImageFilterNew.cxx
otbStreamingStatisticsVectorImageFilter.cxx
otbMatrixTransposeMatrixImageFilterNew.cxx
otbMatrixTransposeMatrixImageFilter.cxx
otbUnaryImageFunctorWithVectorImageFilterNew.cxx
otbUnaryImageFunctorWithVectorImageFilter.cxx
otbPrintableImageFilterNew.cxx
otbPrintableImageFilter.cxx
)

# A enrichir
SET(BasicFilters_SRCS6
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
)

# A enrichir
SET(BasicFilters_SRCS7
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
otbRationalQuotientResampleImageFilterNew.cxx
otbRationalQuotientResampleImageFilter.cxx
otbVectorImageToIntensityImageFilterNew.cxx
otbVectorImageToIntensityImageFilter.cxx
)

# A enrichir
SET(BasicFilters_SRCS8
otbLabelizeImageFilterNew.cxx
otbLabelizeImageFilter.cxx
)

INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbBasicFiltersTests1 otbBasicFiltersTests1.cxx ${BasicFilters_SRCS1})
TARGET_LINK_LIBRARIES(otbBasicFiltersTests1  OTBBasicFilters OTBCommon OTBIO gdal ITKIO ITKCommon ITKBasicFilters)

ADD_EXECUTABLE(otbBasicFiltersTests2 otbBasicFiltersTests2.cxx ${BasicFilters_SRCS2})
TARGET_LINK_LIBRARIES(otbBasicFiltersTests2  OTBBasicFilters OTBCommon OTBIO gdal ITKIO ITKCommon ITKBasicFilters)

ADD_EXECUTABLE(otbBasicFiltersTests3 otbBasicFiltersTests3.cxx ${BasicFilters_SRCS3})
TARGET_LINK_LIBRARIES(otbBasicFiltersTests3  OTBBasicFilters OTBCommon OTBIO gdal ITKIO ITKCommon ITKBasicFilters)

ADD_EXECUTABLE(otbBasicFiltersTests4 otbBasicFiltersTests4.cxx ${BasicFilters_SRCS4})
TARGET_LINK_LIBRARIES(otbBasicFiltersTests4  OTBBasicFilters OTBCommon OTBIO gdal ITKIO ITKCommon ITKBasicFilters)

ADD_EXECUTABLE(otbBasicFiltersTests5 otbBasicFiltersTests5.cxx ${BasicFilters_SRCS5})
TARGET_LINK_LIBRARIES(otbBasicFiltersTests5  OTBBasicFilters OTBCommon OTBIO gdal ITKIO ITKCommon ITKBasicFilters)

ADD_EXECUTABLE(otbBasicFiltersTests6 otbBasicFiltersTests6.cxx ${BasicFilters_SRCS6})
TARGET_LINK_LIBRARIES(otbBasicFiltersTests6  OTBBasicFilters OTBCommon OTBIO gdal ITKIO ITKCommon ITKBasicFilters)

ADD_EXECUTABLE(otbBasicFiltersTests7 otbBasicFiltersTests7.cxx ${BasicFilters_SRCS7})
TARGET_LINK_LIBRARIES(otbBasicFiltersTests7  OTBBasicFilters OTBCommon OTBIO gdal ITKIO ITKCommon ITKBasicFilters)

ADD_EXECUTABLE(otbBasicFiltersTests8 otbBasicFiltersTests8.cxx ${BasicFilters_SRCS8})
TARGET_LINK_LIBRARIES(otbBasicFiltersTests8  OTBBasicFilters OTBCommon OTBIO gdal ITKIO ITKCommon ITKBasicFilters)

ENDIF( NOT OTB_DISABLE_CXX_TESTING )
