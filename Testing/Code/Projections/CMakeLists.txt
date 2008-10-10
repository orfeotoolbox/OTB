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
SET(TOL 0.0)
SET(EPSILON 0.0001)

SET(PROJECTIONS_TESTS1 ${CXX_TEST_PATH}/otbProjectionsTests1)
SET(PROJECTIONS_TESTS2 ${CXX_TEST_PATH}/otbProjectionsTests2)
SET(PROJECTIONS_TESTS3 ${CXX_TEST_PATH}/otbProjectionsTests3)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbPROJECTIONS_TESTS1 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# TEST 1
ADD_TEST(prTuProjectionBaseNew ${PROJECTIONS_TESTS1}  otbProjectionBaseNew )

# TEST 2
ADD_TEST(prTuMapProjectionsNew ${PROJECTIONS_TESTS1}  otbMapProjectionsNew )

# TEST 3
ADD_TEST(prTuSensorModelsNew ${PROJECTIONS_TESTS1}  otbSensorModelsNew )

# TEST 4
IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(prTvTestCreateProjectionWithOSSIM_Toulouse ${PROJECTIONS_TESTS1}  
        otbCreateProjectionWithOSSIM
        ${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        )

# TEST 5
ADD_TEST(prTvTestCreateProjectionWithOSSIM_Cevennes ${PROJECTIONS_TESTS1}  
        otbCreateProjectionWithOSSIM
        ${IMAGEDATA}/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
        )

# TEST 6
ADD_TEST(prTvTestCreateProjectionWithOTB_Toulouse ${PROJECTIONS_TESTS1}  
        otbCreateProjectionWithOTB
        ${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        )

# TEST 7
ADD_TEST(prTvTestCreateProjectionWithOTB_Cevennes ${PROJECTIONS_TESTS1}  
        otbCreateProjectionWithOTB
        ${IMAGEDATA}/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
        )

# TEST 8
ADD_TEST(prTvTestCreateInverseForwardSensorModel_Toulouse ${PROJECTIONS_TESTS1}  
        otbCreateInverseForwardSensorModel
        ${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        )

# TEST 9
ADD_TEST(prTvTestCreateInverseForwardSensorModel_Cevennes ${PROJECTIONS_TESTS1}  
        otbCreateInverseForwardSensorModel
        ${IMAGEDATA}/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
        )
#=======================================================================================
# TEST 10
#   ADD_TEST(prTvRegionProjectionToulouse ${PROJECTIONS_TESTS1} 
#     --compare-image ${TOL}  ${BASELINE}/prTvRegionProjectionToulouse.tif
#                             ${TEMP}/prTvRegionProjectionToulouse.tif
#           otbRegionProjection
#           ${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
#           ${TEMP}/prTvRegionProjectionToulouse.tif
#   				1.44 43.6050 # Long/Lat of Toulouse, Capitole
#           500
#           500
#           10
#   				0.00001
#   				-0.00001
#           )
# TEST 11
#   ADD_TEST(prTvRegionProjectionCevennes ${PROJECTIONS_TESTS1}  
#     --compare-image ${TOL}  ${BASELINE}/prTvRegionProjectionCevennes.tif
#                             ${TEMP}/prTvRegionProjectionCevennes.tif
#           otbRegionProjection
#           ${IMAGEDATA}/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
#           ${TEMP}/prTvRegionProjectionCevennes.tif
#           3.7 44.08 
#           500
#   				500
#   				10
#   				0.00001
#   				-0.00001
#           )
#=======================================================================================
# TEST 12
ADD_TEST(prTvRegionProjectionResamplerToulouse ${PROJECTIONS_TESTS1}  
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
ADD_TEST(prTvRegionProjectionResamplerCevennes ${PROJECTIONS_TESTS1}  
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

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbPROJECTIONS_TESTS2 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#=======================================================================================
# TEST 14
#ADD_TEST(prTvSensorImageToCartoToulouse ${PROJECTIONS_TESTS2}  
#  --compare-image ${TOL}  ${BASELINE}/prTvSensorImageToCartoToulouse_UTM.tif
#                          ${TEMP}/prTvSensorImageToCartoToulouse_UTM.tif
#				   otbSensorImageToCarto
#	   ${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF		
#           ${TEMP}/prTvSensorImageToCartoToulouse_UTM.tif
#	   374100.8
#	   4829184.8 
#	   500
#	   500 
#	   220
#	   0.5
#	   -0.5
#           )

# TEST 15
#ADD_TEST(prTvSensorImageToCartoCevennes ${PROJECTIONS_TESTS2}  
#  --compare-image ${TOL}  ${BASELINE}/prTvSensorImageToCartoCevennes_UTM.tif
#                          ${TEMP}/prTvSensorImageToCartoCevennes_UTM.tif
#				   otbSensorImageToCarto
#           ${IMAGEDATA}/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
#           ${TEMP}/prTvSensorImageToCartoCevennes_UTM.tif
#	   			 556046 
#           4.881e+06 
#           500 
#           500 
#           220
#	   			 0.5
#	   			 -0.5
#           )
#=======================================================================================

ENDIF(OTB_DATA_USE_LARGEINPUT)

# TU pour otb::OrthoRectificationFilterBase otb::OrthoRectificationFilter 
ADD_TEST(prTuOrthoRectificationFilterNew ${PROJECTIONS_TESTS2} 
        otbOrthoRectificationFilterNew )


# TEST 16.2
IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(prTvOrthoRectificationToulouse ${PROJECTIONS_TESTS2} 
  --compare-image ${EPSILON}  ${BASELINE}/prTvOrthoRectificationToulouse_UTM.tif
                          ${TEMP}/prTvOrthoRectificationToulouse_UTM.tif
        otbOrthoRectificationFilter
        ${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        ${TEMP}/prTvOrthoRectificationToulouse_UTM.tif
        374100.8
        4829184.8 
        500
        500 
#         220
        0.5
        -0.5
        31
        N
        )

# TEST 17
ADD_TEST(prTvOrthoRectificationCevennes ${PROJECTIONS_TESTS2} 
  --compare-image ${EPSILON}  ${BASELINE}/prTvOrthoRectificationCevennes_UTM.tif
                          ${TEMP}/prTvOrthoRectificationCevennes_UTM.tif
        otbOrthoRectificationFilter
        ${IMAGEDATA}/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
        ${TEMP}/prTvOrthoRectificationCevennes_UTM.tif
        556046 
        4.881e+06 
        500
        500 
#         220
        0.5
        -0.5
        31
        N
        )
#=======================================================================================				
# TEST 18
#ADD_TEST(prTvSensorImageDEMToCartoToulouse ${PROJECTIONS_TESTS2}  
#  --compare-image ${TOL}  ${BASELINE}/prTvSensorImageDEMToCartoToulouse_UTM.tif
#                          ${TEMP}/prTvSensorImageDEMToCartoToulouse_UTM.tif
#        otbSensorImageDEMToCarto
#        ${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
#        ${TEMP}/prTvSensorImageDEMToCartoToulouse_UTM.tif
#        374100.8
#        4829184.8 
#        500
#				500 
#				220
#        ${INPUTDATA}/DEM/srtm_directory/ 
#				0.5
#				-0.5
#        )
				
# TEST 19
#ADD_TEST(prTvSensorImageDEMToCartoCevennes ${PROJECTIONS_TESTS2}  
#  --compare-image ${TOL}  ${BASELINE}/prTvSensorImageDEMToCartoCevennes_UTM.tif
#                          ${TEMP}/prTvSensorImageDEMToCartoCevennes_UTM.tif
#        otbSensorImageDEMToCarto
#        ${IMAGEDATA}/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
#        ${TEMP}/prTvSensorImageDEMToCartoCevennes_UTM.tif
#        556046 
#        4.881e+06 
#        500 
#        500 
#        200 
#        ${INPUTDATA}/DEM/srtm_directory/ 
#				0.5
#				-0.5
#        )
#=======================================================================================

# TEST 20.2
ADD_TEST(prTvOrthoRectificationToulouseWithDEM ${PROJECTIONS_TESTS2}  
  --compare-image ${EPSILON}  ${BASELINE}/prTvOrthoRectificationToulouseWithDEM_UTM.tif
                          ${TEMP}/prTvOrthoRectificationToulouseWithDEM_UTM.tif
        otbOrthoRectificationFilterWithDEM	
	${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        ${TEMP}/prTvOrthoRectificationToulouseWithDEM_UTM.tif
        374100.8
        4829184.8 
        500
        500 
# 				220
        0.5
        -0.5
        31
        N
        ${INPUTDATA}/DEM/srtm_directory/ 
#         ${TEMP}/prTvOrthoRectificationToulouseWithDEM_UTM.tif
        )

# TEST 21
ADD_TEST(prTvOrthoRectificationCevennesWithDEM ${PROJECTIONS_TESTS2}  
  --compare-image ${EPSILON}  ${BASELINE}/prTvOrthoRectificationCevennesWithDEM_UTM.tif
                          ${TEMP}/prTvOrthoRectificationCevennesWithDEM_UTM.tif
        otbOrthoRectificationFilterWithDEM
        ${IMAGEDATA}/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
        ${TEMP}/prTvOrthoRectificationCevennesWithDEM_UTM.tif
        556046 
        4.881e+06 
        500 
        500 
#         200 
        0.5
        -0.5
        31
        N
        ${INPUTDATA}/DEM/srtm_directory/ 
#         ${TEMP}/prTvOrthoRectificationCevennesWithDEM_UTM.tif
        )

#========================= Ortho rectif SPOT5 ===============================        

ADD_TEST(prTvOrthoRectificationSPOT5 ${PROJECTIONS_TESTS2} 
  --compare-image ${EPSILON}  ${BASELINE}/prTvOrthoRectificationSPOT5_UTM.tif
                          ${TEMP}/prTvOrthoRectificationSPOT5_UTM.tif
        otbOrthoRectificationFilter
        ${IMAGEDATA}/SPOT5_SCENE01/IMAGERY.TIF
        ${TEMP}/prTvOrthoRectificationSPOT5_UTM.tif
        536187
        3950748
        500
        500 
        5
        -5
        39
        N
        )        
        
 ADD_TEST(prTvOrthoRectificationMonoThreadSPOT5 ${PROJECTIONS_TESTS2} 
  --compare-image ${EPSILON}  ${BASELINE}/prTvOrthoRectificationSPOT5_UTM.tif
                          ${TEMP}/prTvOrthoRectificationMonoThreadSPOT5_UTM.tif
        otbOrthoRectificationMonoThreadFilter
        ${IMAGEDATA}/SPOT5_SCENE01/IMAGERY.TIF
        ${TEMP}/prTvOrthoRectificationMonoThreadSPOT5_UTM.tif
        536187
        3950748
        500
        500 
        5
        -5
        39
        N
        )
ENDIF(OTB_DATA_USE_LARGEINPUT)
#========================= TESTS SINGAPOUR ===============================

#SET(EMMANUELDATA ${OTB_DATA_ROOT}/Emmanuel/Singapour/000000173549_01_P001_MUL)

# TEST 22
#ADD_TEST(prTvRegionProjectionResamplerSingapour ${PROJECTIONS_TESTS2}  
#				otbRegionProjectionResampler
#        ${EMMANUELDATA}/04MAR21031747-M1BS-000000173549_01_P001.TIF
#        ${TEMP}/prTvRegionProjectionResamplerSingapour.tif
#        103.8464 1.31
#        500
#				 500
#				 10
#				 0.00001
#				 -0.00001
#        )

# TEST 23
#ADD_TEST(prTvOrthoRectificationSingapour ${PROJECTIONS_TESTS2} 
#        otbOrthoRectificationFilter
#        ${EMMANUELDATA}/04MAR21031747-M1BS-000000173549_01_P001.TIF
#        ${TEMP}/prTvOrthoRectificationSingapour_UTM.tif
#        371997.12 144885.4
#        500
#				 500 
#				 220
#				 1
#        -1
#        )


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbPROJECTIONS_TESTS3 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#=======================================================================================
SET(Projections_SRCS1
otbProjectionBaseNew.cxx
otbMapProjectionsNew.cxx
otbSensorModelsNew.cxx
otbCreateProjectionWithOSSIM.cxx
otbCreateProjectionWithOTB.cxx
otbCreateInverseForwardSensorModel.cxx
#otbRegionProjection.cxx
otbRegionProjectionResampler.cxx
)
SET(Projections_SRCS2
otbOrthoRectificationFilterNew.cxx
otbOrthoRectificationFilter.cxx
otbOrthoRectificationFilterWithDEM.cxx
otbOrthoRectificationMonoThreadFilter.cxx
#otbSensorImageToCarto.cxx
#otbSensorImageDEMToCarto.cxx
)
SET(Projections_SRCS3
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbProjectionsTests1 otbProjectionsTests1.cxx ${Projections_SRCS1})
TARGET_LINK_LIBRARIES(otbProjectionsTests1  OTBProjections OTBIO)
ADD_EXECUTABLE(otbProjectionsTests2 otbProjectionsTests2.cxx ${Projections_SRCS2})
TARGET_LINK_LIBRARIES(otbProjectionsTests2  OTBProjections OTBIO)
ADD_EXECUTABLE(otbProjectionsTests3 otbProjectionsTests3.cxx ${Projections_SRCS3})
TARGET_LINK_LIBRARIES(otbProjectionsTests3  OTBProjections OTBIO)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
