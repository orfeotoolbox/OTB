
IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Images de teledetection (grosses images )
IF(OTB_DATA_USE_LARGEINPUT)
  SET(IMAGEDATA ${OTB_DATA_LARGEINPUT_ROOT} )
ENDIF(OTB_DATA_USE_LARGEINPUT)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)


#Tolerance sur diff pixel image
SET(NOTOL 0.0)
SET(EPSILON_7 0.0000001)
SET(EPSILON_12 0.000000000001)

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

ADD_TEST(bfTvFiltreFrost ${BASICFILTERS_TESTS1}
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
	${IMAGEDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
	${TEMP}/bfTvStreamingShrinkImageFilterQBPANOutput.hdr
	50
)

ADD_TEST(bfTvStreamingShrinkImageFilterQBMUL ${BASICFILTERS_TESTS2}
--compare-image ${NOTOL}
		${BASELINE}/bfTvStreamingShrinkImageFilterQBMULOutput.hdr
		${TEMP}/bfTvStreamingShrinkImageFilterQBMULOutput.hdr
		otbStreamingShrinkImageFilter
	${IMAGEDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
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
         ${IMAGEDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
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


ADD_TEST(bfTVectorRescaleIntensityImageFilter ${BASICFILTERS_TESTS3}
		--compare-image ${EPSILON_7}
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


# -------            otb::ImageListToVectorImageFilter   ----------------------------

ADD_TEST(bfTuImageListToVectorImageFilterNew ${BASICFILTERS_TESTS3}
         otbImageListToVectorImageFilterNew)


ADD_TEST(bfTvImageListToVectorImageFilter ${BASICFILTERS_TESTS3}
		--compare-image ${EPSILON_7}
		    ${BASELINE}/bfTvImageListToVectorImageFilter.png
		    ${TEMP}/bfTvImageListToVectorImageFilter.png
		    otbImageListToVectorImageFilter
          ${INPUTDATA}/poupees_c1.hdr
          ${INPUTDATA}/poupees_c2.hdr
          ${INPUTDATA}/poupees_c3.hdr
            #${INPUTDATA}/poupees.c1.hdr
		    #${INPUTDATA}/poupees.c2.hdr
		    #${INPUTDATA}/poupees.c3.hdr
		    ${TEMP}/bfTvImageListToVectorImageFilter.png
)

ADD_TEST(bfTvImageListToVectorImageFilter2 ${BASICFILTERS_TESTS3}
		    otbImageListToVectorImageFilter2
          ${INPUTDATA}/poupees_c1.hdr
          ${INPUTDATA}/poupees_c2.hdr
          ${INPUTDATA}/poupees_c3.hdr
)
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
#		${IMAGEDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
                ${INPUTDATA}/small_poupees_1canal.hd
		${TEMP}/bfStreamingStatisticsImageFilterResults.txt)

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
#		--compare-image ${NOTOL}
#					${TEMP}/bfTvStreamingResampleSPOT5TestITK.tif
#					${TEMP}/bfTvStreamingResampleSPOT5TestOTB.tif
#				otbStreamingResampleImageFilterCompareWithITK
#					${INPUTDATA}/spot5SubWithGcps.tif
#					1100 1100
#					${TEMP}/bfTvStreamingResampleSPOT5TestITK.tif
#					${TEMP}/bfTvStreamingResampleSPOT5TestOTB.tif
#)


ADD_TEST(bfTvResampleSLCImage ${BASICFILTERS_TESTS5}
            --compare-image ${NOTOL}
                ${BASELINE}/bfTvResampleSLCImage.tif
                ${TEMP}/bfTvResampleSLCImage.tif
            otbResampleSLCImage
                ${INPUTDATA}/RSAT_imagery_HH.tif
                ${TEMP}/bfTvResampleSLCImage.tif
)


# -------            otb::StreamingStatisticsVectorImageFilter   ----------------------------

ADD_TEST(bfTuStreamingStatisticsVectorImageFilterNew ${BASICFILTERS_TESTS5}
         otbStreamingStatisticsVectorImageFilterNew)

ADD_TEST(bfTvStreamingStatisticsVectorImageFilter ${BASICFILTERS_TESTS5}
	--compare-ascii ${NOTOL}
 		   ${BASELINE_FILES}/bfTvStreamingStatisticsVectorImageFilterResults.txt
 		   ${TEMP}/bfTvStreamingStatisticsVectorImageFilterResults.txt
         otbStreamingStatisticsVectorImageFilter
		#${IMAGEDATA}/SPOT5_SCENE01/IMAGERY.TIF
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

ADD_TEST(bfTuBoxAndWhiskerImageFilter ${BASICFILTERS_TESTS8}
         otbBoxAndWhiskerImageFilterNew )

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
	16 16 10 1.0 1 1
	)

ADD_TEST(bfTvMeanShiftImageFilterNotStreamedNotThreaded ${BASICFILTERS_TESTS9}
--compare-n-images ${EPSILON_7} 4
		${BASELINE}/bfMeanShiftImageFilterOutput.tif
		${TEMP}/bfMeanShiftImageFilterNotStreamedNotThreadedOutput.tif
		${BASELINE}/bfMeanShiftImageFilterClusteredOutput.tif
		${TEMP}/bfMeanShiftImageFilterNotStreamedNotThreadedClusteredOutput.tif
		${BASELINE}/bfMeanShiftImageFilterLabeledClusteredOutput.tif
		${TEMP}/bfMeanShiftImageFilterNotStreamedNotThreadedLabeledClusteredOutput.tif
		${BASELINE}/bfMeanShiftImageFilterClusterBoundariesOutput.tif
		${TEMP}/bfMeanShiftImageFilterNotStreamedNotThreadedClusterBoundariesOutput.tif
        otbMeanShiftImageFilter
	${INPUTDATA}/QB_Suburb.png
	${TEMP}/bfMeanShiftImageFilterNotStreamedNotThreadedOutput.tif
	${TEMP}/bfMeanShiftImageFilterNotStreamedNotThreadedClusteredOutput.tif
	${TEMP}/bfMeanShiftImageFilterNotStreamedNotThreadedLabeledClusteredOutput.tif
	${TEMP}/bfMeanShiftImageFilterNotStreamedNotThreadedClusterBoundariesOutput.tif
	16 16 10 1.0 0 0
	)

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
	16 16 10 1.0 1 1
	)


ADD_TEST(bfTvMeanShiftVectorImageFilterNotStreamedNotThreaded ${BASICFILTERS_TESTS9}
--compare-n-images ${EPSILON_7} 4
		${BASELINE}/bfMeanShiftVectorImageFilterOutput.tif
		${TEMP}/bfMeanShiftVectorImageFilterNotStreamedNotThreadedOutput.tif
		${BASELINE}/bfMeanShiftVectorImageFilterClusteredOutput.tif
		${TEMP}/bfMeanShiftVectorImageFilterNotStreamedNotThreadedClusteredOutput.tif
		${BASELINE}/bfMeanShiftVectorImageFilterLabeledClusteredOutput.tif
		${TEMP}/bfMeanShiftVectorImageFilterNotStreamedNotThreadedLabeledClusteredOutput.tif
		${BASELINE}/bfMeanShiftVectorImageFilterClusterBoundariesOutput.tif
		${TEMP}/bfMeanShiftVectorImageFilterNotStreamedNotThreadedClusterBoundariesOutput.tif
        otbMeanShiftVectorImageFilter
	${INPUTDATA}/qb_RoadExtract2sub200x200.tif
	${TEMP}/bfMeanShiftVectorImageFilterNotStreamedNotThreadedOutput.tif
	${TEMP}/bfMeanShiftVectorImageFilterNotStreamedNotThreadedClusteredOutput.tif
	${TEMP}/bfMeanShiftVectorImageFilterNotStreamedNotThreadedLabeledClusteredOutput.tif
	${TEMP}/bfMeanShiftVectorImageFilterNotStreamedNotThreadedClusterBoundariesOutput.tif
	16 16 10 1.0 0 0
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
ADD_TEST(bfTuScalarImageTextureFunctorNew ${BASICFILTERS_TESTS10}
        otbScalarImageTextureFunctorNew )
ADD_TEST(bfTvScalarImageTextureFunctor ${BASICFILTERS_TESTS10}
        --compare-image ${NOTOL}
            ${BASELINE}/bfScalarImageTextureFunctorTest.tif
            ${TEMP}/bfScalarImageTextureFunctorTest.tif
        otbScalarImageTextureFunctor
	    ${INPUTDATA}/small_poupees_1canal.hd
	    ${TEMP}/bfScalarImageTextureFunctorTest.tif
	    2 # radius
            0 # index feature
)
ADD_TEST(bfTuScalarVectorImageTextureFunctorNew ${BASICFILTERS_TESTS10}
        otbScalarVectorImageTextureFunctorNew )

ADD_TEST(bfTvScalarVectorImageTextureFunctor ${BASICFILTERS_TESTS10}
        --compare-image ${NOTOL}
            ${BASELINE}/bfScalarImageTextureFunctorTest.tif # It the same of  bfTvScalarImageTextureFunctor test
            ${TEMP}/bfScalarVectorImageTextureFunctorTest.tif
        otbScalarVectorImageTextureFunctor
	    ${INPUTDATA}/small_poupees_1canal.hd
	    ${TEMP}/bfScalarVectorImageTextureFunctorTest.tif
	    2 # radius
)


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
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbBasicFiltersTests10 ~~~~~~~~~~~~~~~~~~~~~~~~~
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
#	 otbPointSetFunctionNew
#	 )



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
      --compare-image 0
         ${INPUTDATA}/poupees_sub_c1.png
         ${TEMP}/bfFunctionWithNeighborhoodToImageFilter.tif
      otbFunctionWithNeighborhoodToImageFilter
	 ${INPUTDATA}/poupees_sub_c1.png
         ${TEMP}/bfFunctionWithNeighborhoodToImageFilter.tif
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
    ${TEMP}/bfInnerProductPCAImageFilterAllsOutputs_pc1.tif
    ${TEMP}/bfInnerProductPCAImageFilterAllsOutputs_pc2.tif
    ${TEMP}/bfInnerProductPCAImageFilterAllsOutputs_pc3.tif
    ${TEMP}/bfInnerProductPCAImageFilterAllsOutputs_MEAN.tif
)

# -------    Check PCA Implementation with ITK ImagePCAShapeModelEstimator class ----------------------------
ADD_TEST(bfTvItkImagePCAShapeModelEstimatorTest ${BASICFILTERS_TESTS11}
    --compare-n-images ${EPSILON_7} 4
    ${TEMP}/bfITKimagePCAShapeModelEstimatorTest_eigen_vectors1.tif
    ${TEMP}/bfInnerProductPCAImageFilterAllsOutputs_pc1.tif
    ${TEMP}/bfITKimagePCAShapeModelEstimatorTest_eigen_vectors2.tif
    ${TEMP}/bfInnerProductPCAImageFilterAllsOutputs_pc2.tif
    ${TEMP}/bfITKimagePCAShapeModelEstimatorTest_eigen_vectors3.tif
    ${TEMP}/bfInnerProductPCAImageFilterAllsOutputs_pc3.tif
    ${TEMP}/bfInnerProductPCAImageFilterAllsOutputs_MEAN.tif
    ${TEMP}/bfITKimagePCAShapeModelEstimatorTest_mean.tif
    otbImagePCAShapeModelEstimatorTest
    ${INPUTDATA}/poupees_sub_c1.png
    ${INPUTDATA}/poupees_sub_c2.png
    ${INPUTDATA}/poupees_sub_c3.png
    ${TEMP}/bfITKimagePCAShapeModelEstimatorTest_mean.tif
    ${TEMP}/bfITKimagePCAShapeModelEstimatorTest_eigen_vectors1.tif
    ${TEMP}/bfITKimagePCAShapeModelEstimatorTest_eigen_vectors2.tif
    ${TEMP}/bfITKimagePCAShapeModelEstimatorTest_eigen_vectors3.tif
)
SET_TESTS_PROPERTIES(bfTvItkImagePCAShapeModelEstimatorTest PROPERTIES DEPENDS bfTvInnerProductPCAImageFilterAllsOutputs)

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
otbImageListToVectorImageFilterNew.cxx
otbImageListToVectorImageFilter.cxx
otbImageListToVectorImageFilter2.cxx
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
otbVectorImageTo3DScalarImageFilterNew.cxx
otbVectorImageTo3DScalarImageFilter.cxx
)

# A enrichir
SET(BasicFilters_SRCS5
otbBasicFiltersTests5.cxx
otbStreamingResampleImageFilterNew.cxx
otbStreamingResampleImageFilter.cxx
otbStreamingResampleImageFilterCompareWithITK.cxx
otbResampleSLCImage.cxx
otbStreamingStatisticsVectorImageFilterNew.cxx
otbStreamingStatisticsVectorImageFilter.cxx
otbMatrixTransposeMatrixImageFilterNew.cxx
otbMatrixTransposeMatrixImageFilter.cxx
otbUnaryImageFunctorWithVectorImageFilterNew.cxx
otbUnaryImageFunctorWithVectorImageFilter.cxx
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
otbMeanShiftVectorImageFilterNew.cxx
otbMeanShiftVectorImageFilter.cxx
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
otbScalarImageTextureFunctorNew.cxx
otbScalarImageTextureFunctor.cxx
otbScalarVectorImageTextureFunctorNew.cxx
otbScalarVectorImageTextureFunctor.cxx
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


ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
