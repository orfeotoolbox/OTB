
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

SET(PROJECTIONS_TESTS ${CXX_TEST_PATH}/otbProjectionsTests)


ADD_TEST(prTuProjectionBaseNew ${PROJECTIONS_TESTS}  otbProjectionBaseNew )

ADD_TEST(prTuMapProjectionsNew ${PROJECTIONS_TESTS}  otbMapProjectionsNew )

ADD_TEST(prTuSensorModelsNew ${PROJECTIONS_TESTS}  otbSensorModelsNew )



ADD_TEST(prTvTestCreateProjectionWithOSSIM_Toulouse ${PROJECTIONS_TESTS}  
        otbCreateProjectionWithOSSIM
        ${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        )

ADD_TEST(prTvTestCreateProjectionWithOSSIM_Cevennes ${PROJECTIONS_TESTS}  
        otbCreateProjectionWithOSSIM
        ${IMAGEDATA}/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
        )


ADD_TEST(prTvTestCreateProjectionWithOTB_Toulouse ${PROJECTIONS_TESTS}  
        otbCreateProjectionWithOTB
        ${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        )

ADD_TEST(prTvTestCreateProjectionWithOTB_Cevennes ${PROJECTIONS_TESTS}  
        otbCreateProjectionWithOTB
        ${IMAGEDATA}/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
        )

ADD_TEST(prTvRegionProjectionToulouse ${PROJECTIONS_TESTS}  
        otbRegionProjection
        ${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        ${TEMP}/RegionProjectionToulouse.png
        44.08
        3.7
        2000
        2000
        200
        )

ADD_TEST(prTvRegionProjectionCevennes ${PROJECTIONS_TESTS}  
        otbRegionProjection
        ${IMAGEDATA}/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
        ${TEMP}/RegionProjectionCevennes.png
        44.08
        3.7
        2000
        2000
        200
        )

#   ADD_TEST(prTvRegionProjectionStGilles ${PROJECTIONS_TESTS}  
#           otbRegionProjection
#           ${IMAGEDATA}/STGILLES/06FEB04105916-P1BS-005533998010_01_P002.TIF
#           ${TEMP}/RegionProjectionStGilles.png
#           43.60
#           4.33
#           2000
#           2000
#           500
#           )

ADD_TEST(prTvSensorImageToCartoCevennes ${PROJECTIONS_TESTS}  
        otbSensorImageToCarto
         ${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
#        ${IMAGEDATA}/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
        ${TEMP}/SensorImageToCartoCevennes_UTM.png
        556046 
        4.881e+06 
        2000 
        2000 
        220
        )

#   ADD_TEST(prTvSensorImageToCartoStGilles ${PROJECTIONS_TESTS}  
#           otbSensorImageToCarto
#           ${IMAGEDATA}/STGILLES/06FEB04105916-P1BS-005533998010_01_P002.TIF
#           ${TEMP}/SensorImageToCartoStGilles_UTM.png
#           607346 
#           4.82831e+06 
#           2000 
#           2000 
#           500
#           )

ADD_TEST(prTvSensorImageDEMToCartoCevennes ${PROJECTIONS_TESTS}  
        otbSensorImageDEMToCarto
        ${IMAGEDATA}/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
        ${TEMP}/SensorImageDEMToCartoCevennes_UTM_DEM.png
        556046 
        4.881e+06 
        2000 
        2000 
        200 
        ${INPUTDATA}/DEM/srtm_directory/ 
        ${TEMP}/SensorImageDEMToCartoCevennes.hdr
        )


#   ADD_TEST(prTvSensorImageDEMToCartoStGilles ${PROJECTIONS_TESTS}  
#           otbSensorImageDEMToCarto
#           ${IMAGEDATA}/STGILLES/06FEB04105916-P1BS-005533998010_01_P002.TIF
#           ${TEMP}/prSensorImageDEMToCartoStGilles_UTM_DEM.png
#           607346 
#           4.82831e+06 
#           2000 
#           2000 
#           500 
#           ${INPUTDATA}/DEM/srtm_directory/ 
#          ${TEMP}/prSensorImageDEMToCartoStGilles.hdr
#          )


SET(Projections_SRCS
otbProjectionBaseNew.cxx
otbMapProjectionsNew.cxx
otbSensorModelsNew.cxx
otbCreateProjectionWithOSSIM.cxx
otbCreateProjectionWithOTB.cxx
otbRegionProjection.cxx
otbSensorImageToCarto.cxx
otbSensorImageDEMToCarto.cxx
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbProjectionsTests otbProjectionsTests.cxx ${Projections_SRCS})
TARGET_LINK_LIBRARIES(otbProjectionsTests  OTBProjections OTBCommon OTBIO gdal ITKIO ITKCommon ITKBasicFilters otbossim)


ENDIF( NOT OTB_DISABLE_CXX_TESTING )




