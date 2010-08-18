IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Images de teledetection (grosses images )
IF(OTB_DATA_USE_LARGEINPUT)
  SET(LARGEINPUT ${OTB_DATA_LARGEINPUT_ROOT} )
ENDIF(OTB_DATA_USE_LARGEINPUT)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)


#Tolerance sur diff pixel image
SET(NOTOL 0.0)
SET(EPSILON_4 0.0001)
SET(EPSILON_6 0.000001)

SET(PROJECTIONS_TESTS1 ${CXX_TEST_PATH}/otbProjectionsTests1)
SET(PROJECTIONS_TESTS2 ${CXX_TEST_PATH}/otbProjectionsTests2)
SET(PROJECTIONS_TESTS3 ${CXX_TEST_PATH}/otbProjectionsTests3)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbPROJECTIONS_TESTS1 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ADD_TEST(prTuProjectionBaseNew ${PROJECTIONS_TESTS1}  otbProjectionBaseNew )

ADD_TEST(prTuMapProjectionsNew ${PROJECTIONS_TESTS1}  otbMapProjectionsNew )

ADD_TEST(prTvMapProjection ${PROJECTIONS_TESTS1}
        --compare-ascii ${EPSILON_4} ${BASELINE_FILES}/prTvMapProjection.txt
        ${TEMP}/prTvMapProjection.txt
        otbMapProjection
        ${TEMP}/prTvMapProjection.txt
)

ADD_TEST(prTvUtmMapProjection ${PROJECTIONS_TESTS1}
        --compare-ascii ${EPSILON_4} ${BASELINE_FILES}/prTvUtmMapProjection.txt
        ${TEMP}/prTvUtmMapProjection.txt
        otbUtmMapProjection
        ${TEMP}/prTvUtmMapProjection.txt
)

ADD_TEST(prTuSensorModelsNew ${PROJECTIONS_TESTS1}  otbSensorModelsNew )

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(prTvTestCreateProjectionWithOSSIM_Toulouse ${PROJECTIONS_TESTS1}
        otbCreateProjectionWithOSSIM
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        )

ADD_TEST(prTvTestCreateProjectionWithOSSIM_Cevennes ${PROJECTIONS_TESTS1}
        otbCreateProjectionWithOSSIM
        ${LARGEINPUT}/QUICKBIRD/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
        )

ADD_TEST(prTvTestCreateProjectionWithOTB_Toulouse ${PROJECTIONS_TESTS1}
        otbCreateProjectionWithOTB
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        )

ADD_TEST(prTvTestCreateProjectionWithOTB_Cevennes ${PROJECTIONS_TESTS1}
        otbCreateProjectionWithOTB
        ${LARGEINPUT}/QUICKBIRD/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
        )

ADD_TEST(prTvTestCreateInverseForwardSensorModel_Toulouse ${PROJECTIONS_TESTS1}
        otbCreateInverseForwardSensorModel
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        )

ADD_TEST(prTvTestCreateInverseForwardSensorModel_Cevennes ${PROJECTIONS_TESTS1}
        otbCreateInverseForwardSensorModel
        ${LARGEINPUT}/QUICKBIRD/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
        )

ADD_TEST(prTvRegionProjectionResamplerToulouse ${PROJECTIONS_TESTS1}
  --compare-image ${EPSILON_4}  ${BASELINE}/prTvRegionProjectionResamplerToulouse.tif
                          ${TEMP}/prTvRegionProjectionResamplerToulouse.tif
        otbRegionProjectionResampler
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        ${TEMP}/prTvRegionProjectionResamplerToulouse.tif
				1.44 43.6050 # Long/Lat of Toulouse, Capitole
        500
        500
        100
				0.00001
				-0.00001
        )

ADD_TEST(prTvRegionProjectionResamplerCevennes ${PROJECTIONS_TESTS1}
  --compare-image ${EPSILON_4}  ${BASELINE}/prTvRegionProjectionResamplerCevennes.tif
                          ${TEMP}/prTvRegionProjectionResamplerCevennes.tif
				otbRegionProjectionResampler
        ${LARGEINPUT}/QUICKBIRD/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
        ${TEMP}/prTvRegionProjectionResamplerCevennes.tif
        3.7 44.08
        500
				500
				10
				0.00001
				-0.00001
        )

ADD_TEST(prTvSensorModelSPOT5 ${PROJECTIONS_TESTS1}
  --ignore-order --compare-ascii ${NOTOL}
         ${BASELINE_FILES}/prTvSensorModelSpot5.txt
         ${TEMP}/prTvSensorModelSpot5.txt
--ignore-lines-with 3 support_data.metadata_file meters_per_pixel_x meters_per_pixel_y
    otbSensorModel
         ${LARGEINPUT}/SPOT5/TEHERAN/IMAGERY.TIF
         ${TEMP}/prTvSensorModelSpot5.txt
)

ADD_TEST(prTvSensorModelFORMOSAT2 ${PROJECTIONS_TESTS1}
  --ignore-order --compare-ascii ${NOTOL}
         ${BASELINE_FILES}/prTvSensorModelFORMOSAT.txt
         ${TEMP}/prTvSensorModelFORMOSAT2.txt
--ignore-lines-with 3 support_data.metadata_file meters_per_pixel_x meters_per_pixel_y
    otbSensorModel
         ${LARGEINPUT}/FORMOSAT/Sudouest_20071013_MS_fmsat/IMAGERY.TIF
         ${TEMP}/prTvSensorModelFORMOSAT2.txt
)


ADD_TEST(prTvSensorModelQuickbird ${PROJECTIONS_TESTS1}
  --ignore-order --compare-ascii ${NOTOL}
         ${BASELINE_FILES}/prTvSensorModelQuickbird.txt
         ${TEMP}/prTvSensorModelQuickbird.txt
         --ignore-lines-with 2 meters_per_pixel_x meters_per_pixel_y
    otbSensorModel
         ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
         ${TEMP}/prTvSensorModelQuickbird.txt
)

ADD_TEST(prTvSensorModelIkonos ${PROJECTIONS_TESTS1}
  --ignore-order --compare-ascii ${NOTOL}
         ${BASELINE_FILES}/prTvSensorModelIkonos.txt
         ${TEMP}/prTvSensorModelIkonos.txt
         --ignore-lines-with 2 meters_per_pixel_x meters_per_pixel_y
    otbSensorModel
         ${LARGEINPUT}/IKONOS/BLOSSEVILLE/po_2619900_pan_0000000.tif
         ${TEMP}/prTvSensorModelIkonos.txt
)

ADD_TEST(prTvSensorModelRapidEye ${PROJECTIONS_TESTS1}
  --ignore-order --compare-ascii ${NOTOL}
         ${BASELINE_FILES}/prTvSensorModelRapidEye.txt
         ${TEMP}/prTvSensorModelRapidEye.txt
         --ignore-lines-with 2 meters_per_pixel_x meters_per_pixel_y
    otbSensorModel
         ${LARGEINPUT}/RAPIDEYE/level1B/2008-12-25T005918_RE3_1B-NAC_397971_12345_band3.ntf
         ${TEMP}/prTvSensorModelRapidEye.txt
)

ADD_TEST(prTvSensorModelEnvisatAsar ${PROJECTIONS_TESTS1}
  --ignore-order --compare-ascii ${NOTOL}
         ${BASELINE_FILES}/prTvSensorModelEnvisatAsar.txt
         ${TEMP}/prTvSensorModelEnvisatAsar.txt
    otbSensorModel
         ${LARGEINPUT}/ENVISAT_ASAR_SCENE01/ASA_APS_1PNDPA20030821_7713.N1
         ${TEMP}/prTvSensorModelEnvisatAsar.txt
)

ADD_TEST(prTvSensorModelAlosPalsar ${PROJECTIONS_TESTS1}
  --ignore-order --compare-ascii ${EPSILON_6}
         ${BASELINE_FILES}/prTvSensorModelAlosPalsar.txt
         ${TEMP}/prTvSensorModelAlosPalsar.txt
    otbSensorModel
         ${LARGEINPUT}/PALSAR/200801280007/l1data/VOL-ALPSRP037120700-H1.1__A
         ${TEMP}/prTvSensorModelAlosPalsar.txt
)

ADD_TEST(prTvSensorModelRadarSat1 ${PROJECTIONS_TESTS1}
  --ignore-order --compare-ascii ${NOTOL}
         ${BASELINE_FILES}/prTvSensorModelRadarSat1.txt
         ${TEMP}/prTvSensorModelRadarSat1.txt
    otbSensorModel
         ${LARGEINPUT}/RADARSAT1/GOMA/SCENE01/DAT_01.001
         ${TEMP}/prTvSensorModelRadarSat1.txt
)

ADD_TEST(prTvSensorModelRadarSat2 ${PROJECTIONS_TESTS1}
  --ignore-order --compare-ascii ${NOTOL}
         ${BASELINE_FILES}/prTvSensorModelRadarSat2.txt
         ${TEMP}/prTvSensorModelRadarSat2.txt
        --ignore-lines-with 3 product_xml_filename meters_per_pixel_x meters_per_pixel_y
    otbSensorModel
         ${LARGEINPUT}/RADARSAT2/ALTONA/Fine_Quad-Pol_Dataset/PK6621_DK406_FQ9_20080405_124900_HH_VV_HV_VH_SLC_Altona/product.xml
         ${TEMP}/prTvSensorModelRadarSat2.txt
)

ADD_TEST(prTvSensorModelTerraSarX ${PROJECTIONS_TESTS1}
  --ignore-order --compare-ascii ${NOTOL}
         ${BASELINE_FILES}/prTvSensorModelTerraSarX.txt
         ${TEMP}/prTvSensorModelTerraSarX.txt
        --ignore-lines-with 3 product_xml_filename meters_per_pixel_x meters_per_pixel_y
    otbSensorModel
         ${LARGEINPUT}/TERRASARX/dims/TSX-1.SAR.L1B/TSX1_SAR__SSC/TSX1_SAR__SSC.xml
         ${TEMP}/prTvSensorModelTerraSarX.txt
)

ADD_TEST(prTvSensorModelTerraSarX_Upsala ${PROJECTIONS_TESTS1}
  --ignore-order --compare-ascii ${NOTOL}
        ${BASELINE_FILES}/prTvSensorModelTerraSarX_Upsala.txt
        ${TEMP}/prTvSensorModelTerraSarX_Upsala.txt
        --ignore-lines-with 3 product_xml_filename meters_per_pixel_x meters_per_pixel_y
    otbSensorModel
         ${LARGEINPUT}/TERRASARX/UPSALA_GLACIER/TSX1_SAR__MGD/TSX1_SAR__MGD.xml
         ${TEMP}/prTvSensorModelTerraSarX_Upsala.txt
)
ADD_TEST(prTvSensorModelTerraSarX_Toronto ${PROJECTIONS_TESTS1}
  --ignore-order --compare-ascii ${NOTOL}
        ${BASELINE_FILES}/prTvSensorModelTerraSarX_Toronto.txt
        ${TEMP}/prTvSensorModelTerraSarX_Toronto.txt
        --ignore-lines-with 3 product_xml_filename meters_per_pixel_x meters_per_pixel_y
    otbSensorModel
         ${LARGEINPUT}/TERRASARX/TORONTO/TSX1_SAR__SSC/TSX1_SAR__SSC.xml
         ${TEMP}/prTvSensorModelTerraSarX_Toronto.txt
)

ADD_TEST(prTvSensorModelErs ${PROJECTIONS_TESTS1}
  --ignore-order --compare-ascii ${NOTOL}
         ${BASELINE_FILES}/prTvSensorModelErs.txt
         ${TEMP}/prTvSensorModelErs.txt
    otbSensorModel
         ${LARGEINPUT}/SAR_ERS2_SLCI_SCENE1/DAT_01.001
         ${TEMP}/prTvSensorModelErs.txt
)

ENDIF(OTB_DATA_USE_LARGEINPUT)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbPROJECTIONS_TESTS2 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



# TU pour otb::OrthoRectificationFilterBase otb::OrthoRectificationFilter
ADD_TEST(prTuOrthoRectificationFilterNew ${PROJECTIONS_TESTS2}
        otbOrthoRectificationFilterNew )


IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(prTvOrthoRectificationToulouse ${PROJECTIONS_TESTS2}
  --compare-image ${EPSILON_4}  ${BASELINE}/prTvOrthoRectificationToulouse_UTM.tif
                          ${TEMP}/prTvOrthoRectificationToulouse_UTM.tif
        otbOrthoRectificationFilter
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
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

ADD_TEST(prTvOrthoRectificationToulouseXS ${PROJECTIONS_TESTS2}
  --compare-image ${EPSILON_4}  ${BASELINE}/prTvOrthoRectificationToulouseXS_UTM.tif
                          ${TEMP}/prTvOrthoRectificationToulouseXS_UTM.tif
        otbOrthoRectificationFilter
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
        ${TEMP}/prTvOrthoRectificationToulouseXS_UTM.tif
	367319 4835740 300 300 2.5608 -2.8452 31 N
        )

ADD_TEST(prTvOrthoRectificationCevennes ${PROJECTIONS_TESTS2}
  --compare-image ${EPSILON_4}  ${BASELINE}/prTvOrthoRectificationCevennes_UTM.tif
                          ${TEMP}/prTvOrthoRectificationCevennes_UTM.tif
        otbOrthoRectificationFilter
        ${LARGEINPUT}/QUICKBIRD/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
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

ADD_TEST(prTvOrthoRectificationToulouseWithDEM ${PROJECTIONS_TESTS2}
  --compare-image ${EPSILON_4}  ${BASELINE}/prTvOrthoRectificationToulouseWithDEM_UTM.tif
                          ${TEMP}/prTvOrthoRectificationToulouseWithDEM_UTM.tif
        otbOrthoRectificationFilterWithDEM
	${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
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

ADD_TEST(prTvOrthoRectificationCevennesWithDEM ${PROJECTIONS_TESTS2}
  --compare-image ${EPSILON_4}  ${BASELINE}/prTvOrthoRectificationCevennesWithDEM_UTM.tif
                          ${TEMP}/prTvOrthoRectificationCevennesWithDEM_UTM.tif
        otbOrthoRectificationFilterWithDEM
        ${LARGEINPUT}/QUICKBIRD/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
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

ADD_TEST(prTlOrthoRectificationSPOT5 ${PROJECTIONS_TESTS2}
  --compare-image ${EPSILON_4}  ${BASELINE}/prTvOrthoRectificationSPOT5_UTM.tif
                          ${TEMP}/prTvOrthoRectificationSPOT5_UTM.tif
        otbOrthoRectificationFilter
        ${LARGEINPUT}/SPOT5/TEHERAN/IMAGERY.TIF
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

 ADD_TEST(prTlOrthoRectificationMonoThreadSPOT5 ${PROJECTIONS_TESTS2}
  --compare-image ${EPSILON_4}  ${BASELINE}/prTvOrthoRectificationSPOT5_UTM.tif
                          ${TEMP}/prTvOrthoRectificationMonoThreadSPOT5_UTM.tif
        otbOrthoRectificationMonoThreadFilter
        ${LARGEINPUT}/SPOT5/TEHERAN/IMAGERY.TIF
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

ADD_TEST(prTuGCPsToRPCSensorModelImageFilterNew ${PROJECTIONS_TESTS2}
         otbGCPsToRPCSensorModelImageFilterNew)
   
IF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(prTvGCPsToRPCSensorModelImageFilterWithoutDEM ${PROJECTIONS_TESTS2}
  --compare-ascii ${EPSILON_4} 
         ${BASELINE_FILES}/otbGCPsToRPCSensorModelImageFilterWithoutDEMOutput.txt
         ${TEMP}/otbGCPsToRPCSensorModelImageFilterWithoutDEMOutput.txt
         otbGCPsToRPCSensorModelImageFilterWithoutDEM
	 ${LARGEINPUT}/SPOT4/RIO_DE_JANEIRO/IMAG_01.DAT
	 ${TEMP}/otbGCPsToRPCSensorModelImageFilterWithoutDEMOutput.txt
	 1199 1259 -22.76847 -43.168043 0.
	 1497 727  -22.688931 -43.066686 0.
	 699 1146 -22.7254 -43.288651 0.
	 1379 2457 -22.986073 -43.193502 0.
	 2592 1846 -22.932684 -42.854215 0.
	 1430 1797 -22.872988 -43.136813 0.
	 587 2654 -22.983847  -43.393297 0.
	 2398 2422 -23.025664 -42.928778 0.
	 1436 2946 -23.074483 -43.192431 0.
	 963 2915  -23.047382 -43.310011 0.
	 1081 1024 -22.721919 -43.18523  0.
	 1395 1213 -22.769191 -43.116752 0.
	 1781 2202 -22.959556 -43.069911 0.
	 2803 1942 -22.958868 -42.808235 0.
	 980 2734 -22.99534 -43.309672 0.
	 713 2754 -23.007862 -43.365307 0.
)

ENDIF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(prTuLeastSquareAffineTransformEstimatorNew ${PROJECTIONS_TESTS2}
         otbLeastSquareAffineTransformEstimatorNew)

ADD_TEST(prTvLeastSquareAffineTransformEstimator ${PROJECTIONS_TESTS2}
   --compare-ascii ${NOTOL} 
         ${BASELINE_FILES}/otbLeastSquareAffineTransformEstimatorOutput.txt
         ${TEMP}/otbLeastSquareAffineTransformEstimatorOutput.txt
         otbLeastSquareAffineTransformEstimator
	 ${TEMP}/otbLeastSquareAffineTransformEstimatorOutput.txt
	 840 394 783 7907 9925 11943
	 798 911 197 5831 7738 9645
	 335 768 277 5356 6737 8118
	 553 477 628 6867 8526 10185
	 364 513 952 9090 10920 12750
	 916 635 717 8485 10754 13023
	 141 606 16 2687 3451 4215
	 242 137 804 6428 7612 8796
	 156 400 129 2669 3355 4041
	 108 998 218 5636 6961 8286
	 512 839 612 8162 10126 12090
	 296 637 524 6522 7980 9438
	 493 972 292 6435 8193 9951
	 771 526 769 8268 10335 12402
	 400 891 283 5955 7530 9105
	 352 807 919 10023 12102 14181
)

#========================= TESTS SINGAPOUR ===============================

#SET(EMMANUELDATA ${OTB_DATA_ROOT}/Emmanuel/Singapour/000000173549_01_P001_MUL)

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

ADD_TEST(prTuCompositeTransformNew ${PROJECTIONS_TESTS3}  otbCompositeTransformNew )

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(prTvCompositeTransform ${PROJECTIONS_TESTS3}
        --compare-ascii ${EPSILON_4} ${BASELINE_FILES}/prTvCompositeTransform.txt
        ${TEMP}/prTvCompositeTransform.txt
        otbCompositeTransform
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        ${TEMP}/prTvCompositeTransform.txt
)
ENDIF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(prTuGenericMapProjectionNew ${PROJECTIONS_TESTS3}  otbGenericMapProjectionNew )

ADD_TEST(prTvGenericMapProjection ${PROJECTIONS_TESTS3}
        --compare-ascii ${EPSILON_4} 
            ${BASELINE_FILES}/prTvGenericMapProjection.txt
            ${TEMP}/prTvGenericMapProjection.txt
        otbGenericMapProjection
            ${TEMP}/prTvGenericMapProjection.txt
)

ADD_TEST(prTuGenericRSTransformNew ${PROJECTIONS_TESTS3}  otbGenericRSTransformNew )

ADD_TEST(prTvGenericRSTransform ${PROJECTIONS_TESTS3} 
         --compare-ascii ${EPSILON_4}
	     ${BASELINE_FILES}/prTvGenericRSTransform.txt
	     ${TEMP}/prTvGenericRSTransform.txt
	 otbGenericRSTransform
	     1.35617289802566
	     43.4876035537
	     ${TEMP}/prTvGenericRSTransform.txt
)

ADD_TEST(prTuVectorDataProjectionFilterNew ${PROJECTIONS_TESTS3}  otbVectorDataProjectionFilterNew )

#test points
ADD_TEST(prTvVectorDataProjectionFilterPoints ${PROJECTIONS_TESTS3}
        --compare-ogr ${NOTOL}
            ${BASELINE_FILES}/prTvVectorDataProjectionFilterPoints.shp
            ${TEMP}/prTvVectorDataProjectionFilterPoints.shp
        otbVectorDataProjectionFilter
        ${INPUTDATA}/ToulousePoints-examples.shp
        ${TEMP}/prTvVectorDataProjectionFilterPoints.shp
)

#test lines
ADD_TEST(prTvVectorDataProjectionFilterLines ${PROJECTIONS_TESTS3}
        --compare-ogr ${NOTOL}
            ${BASELINE_FILES}/prTvVectorDataProjectionFilterLines.shp
            ${TEMP}/prTvVectorDataProjectionFilterLines.shp
        otbVectorDataProjectionFilter
        ${INPUTDATA}/ToulouseRoad-examples.shp
        ${TEMP}/prTvVectorDataProjectionFilterLines.shp
)

# test polygons
ADD_TEST(prTvVectorDataProjectionFilterPolygons ${PROJECTIONS_TESTS3}
        --compare-ogr  ${NOTOL}
            ${BASELINE_FILES}/prTvVectorDataProjectionFilterPolygons.shp
            ${TEMP}/prTvVectorDataProjectionFilterPolygons.shp
        otbVectorDataProjectionFilter
        ${INPUTDATA}/Capitole-Shadows.shp
        ${TEMP}/prTvVectorDataProjectionFilterPolygons.shp
)


# test to sensor
# NB: no prj file here as it is in sensor geometry
IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(prTvVectorDataProjectionFilterFromMapToSensor ${PROJECTIONS_TESTS3}
        --compare-ogr  ${NOTOL}
            ${BASELINE_FILES}/prTvVectorDataProjectionFilterFromMapToSensor.shp
            ${TEMP}/prTvVectorDataProjectionFilterFromMapToSensor.shp
        otbVectorDataProjectionFilterFromMapToSensor
        ${INPUTDATA}/ToulousePoints-examples.shp
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        ${TEMP}/prTvVectorDataProjectionFilterFromMapToSensor.shp
)
ENDIF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(prTvVectorDataProjectionFilterFromMapToGeo ${PROJECTIONS_TESTS3}
        --compare-ascii ${NOTOL}
        ${BASELINE_FILES}/prTvVectorDataProjectionFilterFromMapToGeo.kml
        ${TEMP}/prTvVectorDataProjectionFilterFromMapToGeo.kml
        otbVectorDataProjectionFilterFromMapToGeo
        ${INPUTDATA}/ToulousePoints-examples.shp
        ${TEMP}/prTvVectorDataProjectionFilterFromMapToGeo.kml
)

ADD_TEST(prTvVectorDataProjectionFilterFromMapToImage ${PROJECTIONS_TESTS3}
        --compare-ascii ${NOTOL}
        ${BASELINE_FILES}/prTvVectorDataProjectionFilterFromMapToImage.kml
        ${TEMP}/prTvVectorDataProjectionFilterFromMapToImage.kml
        otbVectorDataProjectionFilterFromMapToImage
        ${INPUTDATA}/ToulousePoints-examples.shp
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif 
        ${TEMP}/prTvVectorDataProjectionFilterFromMapToImage.kml
)


ADD_TEST(prTuGeocentricTransformNew ${PROJECTIONS_TESTS3}  otbGeocentricTransformNew )


ADD_TEST(prTvGeocentricTransform ${PROJECTIONS_TESTS3}
         --compare-ascii ${EPSILON_4} ${BASELINE_FILES}/prTvGeocentricTransform.txt
        ${TEMP}/prTvGeocentricTransform.txt
        otbGeocentricTransform
        ${TEMP}/prTvGeocentricTransform.txt
)

ADD_TEST(prTvTileMapTransform ${PROJECTIONS_TESTS3}
         --compare-ascii ${EPSILON_4} ${BASELINE_FILES}/prTvTileMapTransform.txt
        ${TEMP}/prTvTileMapTransform.txt
        otbTileMapTransform
        ${TEMP}/prTvTileMapTransform.txt
)

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(prTvVectorDataExtractROIandProjection ${PROJECTIONS_TESTS3}
   otbVectorDataExtractROIandProjection
        ${LARGEINPUT}/VECTOR/MidiPyrenees/roads.shp
)
ENDIF(OTB_DATA_USE_LARGEINPUT)




#=======================================================================================
SET(Projections_SRCS1
otbProjectionsTests1.cxx
otbProjectionBaseNew.cxx
otbMapProjectionsNew.cxx
otbMapProjection.cxx
otbUtmMapProjection.cxx
otbSensorModelsNew.cxx
otbCreateProjectionWithOSSIM.cxx
otbCreateProjectionWithOTB.cxx
otbCreateInverseForwardSensorModel.cxx
otbRegionProjectionResampler.cxx
otbSensorModel.cxx
)
SET(Projections_SRCS2
otbProjectionsTests2.cxx
otbOrthoRectificationFilterNew.cxx
otbOrthoRectificationFilter.cxx
otbOrthoRectificationFilterWithDEM.cxx
otbOrthoRectificationMonoThreadFilter.cxx
otbGCPsToRPCSensorModelImageFilterNew.cxx
otbGCPsToRPCSensorModelImageFilterWithoutDEM.cxx
otbLeastSquareAffineTransformEstimatorNew.cxx
otbLeastSquareAffineTransformEstimator.cxx
)

SET(Projections_SRCS3
otbProjectionsTests3.cxx
otbCompositeTransformNew.cxx
otbCompositeTransform.cxx
otbGenericMapProjectionNew.cxx
otbGenericMapProjection.cxx
otbGenericRSTransformNew.cxx
otbGenericRSTransform.cxx
otbVectorDataProjectionFilterNew.cxx
otbVectorDataProjectionFilter.cxx
otbVectorDataProjectionFilterFromMapToSensor.cxx
otbVectorDataProjectionFilterFromMapToGeo.cxx
otbVectorDataProjectionFilterFromMapToImage.cxx
otbGeocentricTransformNew.cxx
otbGeocentricTransform.cxx
otbTileMapTransform.cxx
otbVectorDataExtractROIandProjection.cxx
)


OTB_ADD_EXECUTABLE(otbProjectionsTests1 "${Projections_SRCS1}" "OTBProjections;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbProjectionsTests2 "${Projections_SRCS2}" "OTBProjections;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbProjectionsTests3 "${Projections_SRCS3}" "OTBProjections;OTBIO;OTBTesting")

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
