IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Images de teledetection (grosses images )
SET(IMAGEDATA ${OTB_DATA_ROOT}/LargeInput )
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)


#Tolerance sur diff pixel image
SET(TOL 0.0)
SET(EPSILON 0.0001)

SET(PROJECTIONS_TESTS ${CXX_TEST_PATH}/otbProjectionsTests)

# TEST 1
ADD_TEST(prTuProjectionBaseNew ${PROJECTIONS_TESTS}  otbProjectionBaseNew )

# TEST 2
ADD_TEST(prTuMapProjectionsNew ${PROJECTIONS_TESTS}  otbMapProjectionsNew )

# TEST 3
ADD_TEST(prTuSensorModelsNew ${PROJECTIONS_TESTS}  otbSensorModelsNew )

# TEST 4
ADD_TEST(prTvTestCreateProjectionWithOSSIM_Toulouse ${PROJECTIONS_TESTS}  
        otbCreateProjectionWithOSSIM
        ${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        )

# TEST 5
ADD_TEST(prTvTestCreateProjectionWithOSSIM_Cevennes ${PROJECTIONS_TESTS}  
        otbCreateProjectionWithOSSIM
        ${IMAGEDATA}/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
        )

# TEST 6
ADD_TEST(prTvTestCreateProjectionWithOTB_Toulouse ${PROJECTIONS_TESTS}  
        otbCreateProjectionWithOTB
        ${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        )

# TEST 7
ADD_TEST(prTvTestCreateProjectionWithOTB_Cevennes ${PROJECTIONS_TESTS}  
        otbCreateProjectionWithOTB
        ${IMAGEDATA}/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
        )

# TEST 8
ADD_TEST(prTvTestCreateInverseForwardSensorModel_Toulouse ${PROJECTIONS_TESTS}  
        otbCreateInverseForwardSensorModel
        ${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        )

# TEST 9
ADD_TEST(prTvTestCreateInverseForwardSensorModel_Cevennes ${PROJECTIONS_TESTS}  
        otbCreateInverseForwardSensorModel
        ${IMAGEDATA}/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
        )
#=======================================================================================
# TEST 10
ADD_TEST(prTvRegionProjectionToulouse ${PROJECTIONS_TESTS} 
  --compare-image ${TOL}  ${BASELINE}/prTvRegionProjectionToulouse.tif
                          ${TEMP}/prTvRegionProjectionToulouse.tif
        otbRegionProjection
        ${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        ${TEMP}/prTvRegionProjectionToulouse.tif
	1.44 43.6050 # Long/Lat of Toulouse, Capitole
        500
        500
        10
	0.00001
	-0.00001
        )
# TEST 11
ADD_TEST(prTvRegionProjectionCevennes ${PROJECTIONS_TESTS}  
  --compare-image ${TOL}  ${BASELINE}/prTvRegionProjectionCevennes.tif
                          ${TEMP}/prTvRegionProjectionCevennes.tif
        otbRegionProjection
        ${IMAGEDATA}/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
        ${TEMP}/prTvRegionProjectionCevennes.tif
        3.7 44.08 
        500
	500
	10
	0.00001
	-0.00001
        )
#=======================================================================================
# TEST 12
ADD_TEST(prTvRegionProjectionResamplerToulouse ${PROJECTIONS_TESTS}  
  --compare-image ${EPSILON}  ${BASELINE}/prTvRegionProjectionResamplerToulouse.tif
                          ${TEMP}/prTvRegionProjectionResamplerToulouse.tif
        otbRegionProjectionResampler
        ${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        ${TEMP}/prTvRegionProjectionResamplerToulouse.tif
	1.44 43.6050 # Long/Lat of Toulouse, Capitole
        500
        500
        100
	0.00001
	-0.00001
        )				
# TEST 13				
ADD_TEST(prTvRegionProjectionResamplerCevennes ${PROJECTIONS_TESTS}  
  --compare-image ${EPSILON}  ${BASELINE}/prTvRegionProjectionResamplerCevennes.tif
                          ${TEMP}/prTvRegionProjectionResamplerCevennes.tif
				otbRegionProjectionResampler
        ${IMAGEDATA}/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
        ${TEMP}/prTvRegionProjectionResamplerCevennes.tif
        3.7 44.08
        500
	500
	10
	0.00001
	-0.00001
        )
#=======================================================================================
# TEST 14
ADD_TEST(prTvSensorImageToCartoToulouse ${PROJECTIONS_TESTS}  
  --compare-image ${TOL}  ${BASELINE}/prTvSensorImageToCartoToulouse_UTM.tif
                          ${TEMP}/prTvSensorImageToCartoToulouse_UTM.tif
				   otbSensorImageToCarto
	   ${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF		
           ${TEMP}/prTvSensorImageToCartoToulouse_UTM.tif
	   374100.8
	   4829184.8 
	   500
	   500 
	   220
	   0.5
	   -0.5
           )

# TEST 15
ADD_TEST(prTvSensorImageToCartoCevennes ${PROJECTIONS_TESTS}  
  --compare-image ${TOL}  ${BASELINE}/prTvSensorImageToCartoCevennes_UTM.tif
                          ${TEMP}/prTvSensorImageToCartoCevennes_UTM.tif
				   otbSensorImageToCarto
           ${IMAGEDATA}/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
           ${TEMP}/prTvSensorImageToCartoCevennes_UTM.tif
	   556046 
           4.881e+06 
           500 
           500 
           220
	   0.5
	   -0.5
           )
#=======================================================================================
# TEST 15				
ADD_TEST(prTvOrthoRectificationToulouse ${PROJECTIONS_TESTS} 
  --compare-image ${EPSILON}  ${BASELINE}/prTvOrthoRectificationToulouse_UTM.tif
                          ${TEMP}/prTvOrthoRectificationToulouse_UTM.tif
        otbOrthoRectificationFilter
        ${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        ${TEMP}/prTvOrthoRectificationToulouse_UTM.tif
        374100.8
        4829184.8 
        500
	500 
	220
	0.5
        -0.5
        )

# TEST 15
ADD_TEST(prTvOrthoRectificationCevennes ${PROJECTIONS_TESTS} 
  --compare-image ${EPSILON}  ${BASELINE}/prTvOrthoRectificationCevennes_UTM.tif
                          ${TEMP}/prTvOrthoRectificationCevennes_UTM.tif
        otbOrthoRectificationFilter
        ${IMAGEDATA}/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
        ${TEMP}/prTvOrthoRectificationCevennes_UTM.tif
        556046 
        4.881e+06 
        500
	500 
	220
	0.5
        -0.5
        )
#=======================================================================================				
# TEST 16
ADD_TEST(prTvSensorImageDEMToCartoToulouse ${PROJECTIONS_TESTS}  
  --compare-image ${TOL}  ${BASELINE}/prTvSensorImageDEMToCartoToulouse_UTM.tif
                          ${TEMP}/prTvSensorImageDEMToCartoToulouse_UTM.tif
        otbSensorImageDEMToCarto
        ${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        ${TEMP}/prTvSensorImageDEMToCartoToulouse_UTM.tif
        374100.8
        4829184.8 
        500
	500 
	220
        ${INPUTDATA}/DEM/srtm_directory/ 
        ${TEMP}/SensorImageDEMToCartoToulouse.hdr
	0.5
	-0.5
        )
# TEST 17
ADD_TEST(prTvSensorImageDEMToCartoCevennes ${PROJECTIONS_TESTS}  
  --compare-image ${TOL}  ${BASELINE}/prTvSensorImageDEMToCartoCevennes_UTM.tif
                          ${TEMP}/prTvSensorImageDEMToCartoCevennes_UTM.tif
        otbSensorImageDEMToCarto
        ${IMAGEDATA}/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
        ${TEMP}/prTvSensorImageDEMToCartoCevennes_UTM.tif
        556046 
        4.881e+06 
        500 
        500 
        200 
        ${INPUTDATA}/DEM/srtm_directory/ 
        ${TEMP}/SensorImageDEMToCartoCevennes.hdr
	0.5
	-0.5
        )
#=======================================================================================
# TEST 18
ADD_TEST(prTvOrthoRectificationToulouseWithDEM ${PROJECTIONS_TESTS}  
  --compare-image ${EPSILON}  ${BASELINE}/prTvOrthoRectificationToulouseWithDEM_UTM.tif
                          ${TEMP}/prTvOrthoRectificationToulouseWithDEM_UTM.tif
        otbOrthoRectificationFilterWithDEM	
	${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        ${TEMP}/prTvOrthoRectificationToulouseWithDEM_UTM.tif
        374100.8
        4829184.8 
        500
	500 
	220
	0.5
	-0.5
        ${INPUTDATA}/DEM/srtm_directory/ 
        ${TEMP}/prTvOrthoRectificationToulouseWithDEM_UTM.tif
        )

# TEST 19
ADD_TEST(prTvOrthoRectificationCevennesWithDEM ${PROJECTIONS_TESTS}  
  --compare-image ${EPSILON}  ${BASELINE}/prTvOrthoRectificationCevennesWithDEM_UTM.tif
                          ${TEMP}/prTvOrthoRectificationCevennesWithDEM_UTM.tif
        otbOrthoRectificationFilterWithDEM
        ${IMAGEDATA}/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
        ${TEMP}/prTvOrthoRectificationCevennesWithDEM_UTM.tif
        556046 
        4.881e+06 
        500 
        500 
        200 
	0.5
	-0.5
        ${INPUTDATA}/DEM/srtm_directory/ 
        ${TEMP}/prTvOrthoRectificationCevennesWithDEM_UTM.tif
        )
#=======================================================================================
SET(Projections_SRCS
otbProjectionBaseNew.cxx
otbMapProjectionsNew.cxx
otbSensorModelsNew.cxx
otbCreateProjectionWithOSSIM.cxx
otbCreateProjectionWithOTB.cxx
otbCreateInverseForwardSensorModel.cxx
otbRegionProjection.cxx
otbRegionProjectionResampler.cxx
otbOrthoRectificationFilter.cxx
otbOrthoRectificationFilterWithDEM.cxx
otbSensorImageToCarto.cxx
otbSensorImageDEMToCarto.cxx
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbProjectionsTests otbProjectionsTests.cxx ${Projections_SRCS})
TARGET_LINK_LIBRARIES(otbProjectionsTests  OTBProjections OTBCommon OTBIO gdal ITKIO ITKCommon ITKBasicFilters otbossim)


ENDIF( NOT OTB_DISABLE_CXX_TESTING )




