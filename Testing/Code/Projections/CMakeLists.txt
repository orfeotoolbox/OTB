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
SET(PROJECTIONS_TESTS4 ${CXX_TEST_PATH}/otbProjectionsTests4)

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

ADD_TEST(prTvMapProjectionsTest ${PROJECTIONS_TESTS1}
        --compare-ascii ${EPSILON_4} ${BASELINE_FILES}/prTvMapProjectionsTest.txt
        ${TEMP}/prTvMapProjectionsTest.txt
        otbMapProjectionsTest
        ${TEMP}/prTvMapProjectionsTest.txt
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

SET(SARSENSORMODEL_TYPES
  "RADARSAT1/GOMA/SCENE01/"
  "RADARSAT2/ALTONA/Fine_Quad-Pol_Dataset/PK6621_DK406_FQ9_20080405_124900_HH_VV_HV_VH_SLC_Altona/product.xml"
  "ENVISAT_ASAR_SCENE01/ASA_APS_1PNDPA20030821_7713.N1"
  "PALSAR/200801280007/l1data/VOL-ALPSRP037120700-H1.1__A"
  "TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_HH_SRA_stripFar_008.cos"
)

FOREACH( file ${SARSENSORMODEL_TYPES})
  # Get the sensor name 
  SET(sharp_regexp "([0-9A-Za-z_]*)[ ]*/[ ]*(.*)")
  STRING(REGEX REPLACE "${sharp_regexp}" "\\1" sensor_name "${file}")
  # Tests
	ADD_TEST(prTvSensorModelGrid_${sensor_name} ${PROJECTIONS_TESTS1}
	  --compare-n-images ${NOTOL} 2
	         ${BASELINE}/prTvSensorModelGridDeltaX_${sensor_name}.tif
	         ${TEMP}/prTvSensorModelGridDeltaX_${sensor_name}.tif
	         ${BASELINE}/prTvSensorModelGridDeltaY_${sensor_name}.tif
	         ${TEMP}/prTvSensorModelGridDeltaY_${sensor_name}.tif
    	otbSensorModelGrid
    		${LARGEINPUT}/${file}
         	${TEMP}/prTvSensorModelGridDeltaX_${sensor_name}.tif
         	${TEMP}/prTvSensorModelGridDeltaY_${sensor_name}.tif
         	10 10
		)
ENDFOREACH( file ${SARSENSORMODEL_TYPES})


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
        4 # Spacing of the deformation field
        )

ADD_TEST(prTvOrthoRectificationToulouseXS ${PROJECTIONS_TESTS2}
  --compare-image ${EPSILON_4}  ${BASELINE}/prTvOrthoRectificationToulouseXS_UTM.tif
                          ${TEMP}/prTvOrthoRectificationToulouseXS_UTM.tif
        otbOrthoRectificationFilter
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
        ${TEMP}/prTvOrthoRectificationToulouseXS_UTM.tif
	367319 4835740 300 300 2.5608 -2.8452 31 N 4
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
        0.5
        -0.5
        31
        N
        4
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
        0.5
        -0.5
        31
        N
        ${INPUTDATA}/DEM/srtm_directory/
        0.5
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
        0.5
        -0.5
        31
        N
        ${INPUTDATA}/DEM/srtm_directory/
        0.5
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
        5
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
        5
        )
ENDIF(OTB_DATA_USE_LARGEINPUT)


#========================= Ortho rectif SAR data  ===============================


IF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(prTvOrthoRectificationUTM_RADARDSAT1 ${PROJECTIONS_TESTS2}
  	--compare-image ${EPSILON_4}  ${BASELINE}/prTvOrthoRectificationUTM_RADARDSAT1.tif
                          ${TEMP}/prTvOrthoRectificationUTM_RADARDSAT1.tif
        otbOrthoRectificationFilter
    		${LARGEINPUT}/RADARSAT1/GOMA/SCENE01/
        	${TEMP}/prTvOrthoRectificationUTM_RADARDSAT1.tif
	        755929.75 9821313.25
        	200	200 10 -10 35 S 4 
        )

ADD_TEST(prTvOrthoRectificationUTM_RADARSAT2_XML ${PROJECTIONS_TESTS2}
  	--compare-image ${EPSILON_4}  ${BASELINE}/prTvOrthoRectificationUTM_RADARSAT2.tif
                          ${TEMP}/prTvOrthoRectificationUTM_RADARSAT2.tif
        otbOrthoRectificationFilter
    		${LARGEINPUT}/RADARSAT2/ALTONA/Fine_Quad-Pol_Dataset/PK6621_DK406_FQ9_20080405_124900_HH_VV_HV_VH_SLC_Altona/product.xml
        	${TEMP}/prTvOrthoRectificationUTM_RADARSAT2.tif
	        611000.953075 5439991.842531
        	200	200 10 -10 14 N 4 
        )
ADD_TEST(prTvOrthoRectificationUTM_RADARSAT2_HH ${PROJECTIONS_TESTS2}
  	--compare-image ${EPSILON_4}  ${BASELINE}/prTvOrthoRectificationUTM_RADARSAT2_XML.tif
                          ${TEMP}/prTvOrthoRectificationUTM_RADARSAT2_XML.tif
        otbOrthoRectificationFilter
    		${LARGEINPUT}/RADARSAT2/ALTONA/Fine_Quad-Pol_Dataset/PK6621_DK406_FQ9_20080405_124900_HH_VV_HV_VH_SLC_Altona/imagery_HH.tif
        	${TEMP}/prTvOrthoRectificationUTM_RADARSAT2_XML.tif
	        611000.953075 5439991.842531
        	200	200 10 -10 14 N 4 
        )
ADD_TEST(prTvOrthoRectificationUTM_RADARSAT2_HV ${PROJECTIONS_TESTS2}
  	--compare-image ${EPSILON_4}  ${BASELINE}/prTvOrthoRectificationUTM_RADARSAT2_HV.tif
                          ${TEMP}/prTvOrthoRectificationUTM_RADARSAT2_HV.tif
        otbOrthoRectificationFilter
    		${LARGEINPUT}/RADARSAT2/ALTONA/Fine_Quad-Pol_Dataset/PK6621_DK406_FQ9_20080405_124900_HH_VV_HV_VH_SLC_Altona/imagery_HV.tif
        	${TEMP}/prTvOrthoRectificationUTM_RADARSAT2_HV.tif
	        611000.953075 5439991.842531
        	200	200 10 -10 14 N 4 
        )
ADD_TEST(prTvOrthoRectificationUTM_RADARSAT2_VH ${PROJECTIONS_TESTS2}
  	--compare-image ${EPSILON_4}  ${BASELINE}/prTvOrthoRectificationUTM_RADARSAT2_VH.tif
                          ${TEMP}/prTvOrthoRectificationUTM_RADARSAT2_VH.tif
        otbOrthoRectificationFilter
    		${LARGEINPUT}/RADARSAT2/ALTONA/Fine_Quad-Pol_Dataset/PK6621_DK406_FQ9_20080405_124900_HH_VV_HV_VH_SLC_Altona/imagery_VH.tif
        	${TEMP}/prTvOrthoRectificationUTM_RADARSAT2_VH.tif
	        611000.953075 5439991.842531
        	200	200 10 -10 14 N 4 
        )
ADD_TEST(prTvOrthoRectificationUTM_RADARSAT2_VV ${PROJECTIONS_TESTS2}
  	--compare-image ${EPSILON_4}  ${BASELINE}/prTvOrthoRectificationUTM_RADARSAT2_VV.tif
                          ${TEMP}/prTvOrthoRectificationUTM_RADARSAT2_VV.tif
        otbOrthoRectificationFilter
    		${LARGEINPUT}/RADARSAT2/ALTONA/Fine_Quad-Pol_Dataset/PK6621_DK406_FQ9_20080405_124900_HH_VV_HV_VH_SLC_Altona/imagery_VV.tif
        	${TEMP}/prTvOrthoRectificationUTM_RADARSAT2_VV.tif
	        611000.953075 5439991.842531
        	200	200 10 -10 14 N 4 
        )

ADD_TEST(prTvOrthoRectificationUTM_ENVISAT ${PROJECTIONS_TESTS2}
  	--compare-image ${EPSILON_4}  ${BASELINE}/prTvOrthoRectificationUTM_ENVISAT.tif
                          ${TEMP}/prTvOrthoRectificationUTM_ENVISAT.tif
        otbOrthoRectificationFilter
    		${LARGEINPUT}/ENVISAT_ASAR_SCENE01/ASA_APS_1PNDPA20030821_7713.N1
        	${TEMP}/prTvOrthoRectificationUTM_ENVISAT.tif
	        655696.679130 4906584.046495
        	200	200 10 -10 30 N 4 
        )

ADD_TEST(prTvOrthoRectificationUTM_PALSAR ${PROJECTIONS_TESTS2}
  	--compare-image ${EPSILON_4}  ${BASELINE}/prTvOrthoRectificationUTM_PALSAR.tif
                          ${TEMP}/prTvOrthoRectificationUTM_PALSAR.tif
        otbOrthoRectificationFilter
    		${LARGEINPUT}/PALSAR/200801280007/l1data/VOL-ALPSRP037120700-H1.1__A
        	${TEMP}/prTvOrthoRectificationUTM_PALSAR.tif
	        309298.731439 3909055.130746
        	200	200 10 -10 54 N 4 
        )

ADD_TEST(prTvOrthoRectificationUTM_TERRASARX_HH ${PROJECTIONS_TESTS2}
  	--compare-image ${EPSILON_4}  ${BASELINE}/prTvOrthoRectificationUTM_TERRASARX_HH.tif
                          ${TEMP}/prTvOrthoRectificationUTM_TERRASARX_HH.tif
        otbOrthoRectificationFilter
    		${LARGEINPUT}/TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_HH_SRA_stripFar_008.cos
        	${TEMP}/prTvOrthoRectificationUTM_TERRASARX_HH.tif
	        591992.001073 9699689.825866
        	200	200 10 -10 49 S 4 
        )

ADD_TEST(prTvOrthoRectificationUTM_TERRASARX_VV ${PROJECTIONS_TESTS2}
  	--compare-image ${EPSILON_4}  ${BASELINE}/prTvOrthoRectificationUTM_TERRASARX_VV.tif
                          ${TEMP}/prTvOrthoRectificationUTM_TERRASARX_VV.tif
        otbOrthoRectificationFilter
    		${LARGEINPUT}/TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_VV_SRA_stripFar_008.cos
        	${TEMP}/prTvOrthoRectificationUTM_TERRASARX_VV.tif
	        591992.001073 9699689.825866
        	200	200 10 -10 49 S 4 
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
	 1199 1259 -43.168043 -22.76847  0.
	 1497 727  -43.066686 -22.688931 0.
	 699 1146  -43.288651 -22.7254   0.
	 1379 2457 -43.193502 -22.986073 0.
	 2592 1846 -42.854215 -22.932684 0.
	 1430 1797 -43.136813 -22.872988 0.
	 587 2654  -43.393297 -22.983847 0.
	 2398 2422 -42.928778 -23.025664 0.
	 1436 2946 -43.192431 -23.074483 0.
	 963 2915  -43.310011 -23.047382 0.
	 1081 1024 -43.18523  -22.721919 0.
	 1395 1213 -43.116752 -22.769191 0.
	 1781 2202 -43.069911 -22.959556 0.
	 2803 1942 -42.808235 -22.958868 0.
	 980 2734  -43.309672 -22.99534  0.
	 713 2754  -43.365307 -23.007862 0.
)
ENDIF(OTB_DATA_USE_LARGEINPUT)


ADD_TEST(prTvGCPsToRPCSensorModelImageFilterAndOrtho ${PROJECTIONS_TESTS2}
  --compare-image ${EPSILON_4} 
         ${BASELINE_FILES}/otbGCPsToRPCSensorModelImageFilterAndOrthoOutput.tif
         ${TEMP}/otbGCPsToRPCSensorModelImageFilterAndOrthoOutput.tif
         otbGCPsToRPCSensorModelImageFilterAndOrtho
     ${INPUTDATA}/Haiti1.jpeg
     ${TEMP}/otbGCPsToRPCSensorModelImageFilterAndOrthoOutput.tif
     
     782166.394827
     2054821.977902  
     500
     500
     5
     -5
     18
     N

        1980.000000
        1986.000000
        -72.351257
        18.555075
        0.000000 

        1963.000000
        1944.000000
        -72.352051
        18.556946
        0.000000
    
        2078.000000
        1566.000000
        -72.346130
        18.574358
        0.000000
    
        2154.000000
        1430.000000
        -72.342377
        18.580441
        0.000000
    
        2432.000000
        1408.000000
        -72.328964
        18.581335
        0.000000
    
        2558.000000
        934.000000
        -72.322571
        18.602974
        0.000000
    
        3492.000000
        1397.000000
        -72.278343
        18.580887
        0.000000
    
        2890.000000
        1439.000000
        -72.306908
        18.579464
        0.000000
    
        2098.000000
        2469.000000
        -72.345741
        18.532839
        0.000000
    
        1941.000000
        2473.000000
        -72.353317
        18.532595
        0.000000
    
        1644.000000
        2409.000000
        -72.367691
        18.535810
        0.000000
    
        1440.000000
        2448.000000
        -72.377457
        18.534243
        0.000000
    
        687.000000
        2005.000000
        -72.413315
        18.554974
        0.000000
    
        741.000000
        2366.000000
        -72.410934
        18.538496
        0.000000
    
        855.000000
        2802.000000
        -72.405739
        18.518456
        0.000000
    
        1936.000000
        2855.000000
        -72.353920
        18.515240
        0.000000
    
        3338.000000
        2824.000000
        -72.286240
        18.515160
        0.000000
    
        3715.000000
        1772.000000
        -72.267570
        18.563681
        0.000000
    
        3676.000000
        1580.000000
        -72.269203
        18.572529
        0.000000
    
        3395.000000
        931.000000
        -72.282402
        18.602425
        0.000000
    
        3426.000000
        856.000000
        -72.280792
        18.605862
        0.000000
    
        2822.000000
        707.000000
        -72.309692
        18.613203
        0.000000
    
        1891.000000
        360.000000
        -72.354301
        18.629776
        0.000000
    
        3989.000000
        2639.000000
        -72.255020
        18.523664
        0.000000
    
        417.000000
        2317.000000
        -72.426552
        18.540876
        0.000000
)
 
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

ADD_TEST(prTvGenericRSTransformWithSRID ${PROJECTIONS_TESTS3} 
         --compare-ascii ${EPSILON_4}
	     ${BASELINE_FILES}/prTvGenericRSTransform.txt
	     ${TEMP}/prTvGenericRSTransform_WithSRID.txt
	 otbGenericRSTransformWithSRID
	     1.35617289802566
	     43.4876035537
	     ${TEMP}/prTvGenericRSTransform_WithSRID.txt
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

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbPROJECTIONS_TESTS4 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


ADD_TEST(prTuPhysicalToRPCSensorModelImageFilterNew ${PROJECTIONS_TESTS4}
  otbPhysicalToRPCSensorModelImageFilter
  )

ADD_TEST(prTvPhysicalToRPCSensorModelImageFilter ${PROJECTIONS_TESTS4}
  otbPhysicalToRPCSensorModelImageFilter
  ${INPUTDATA}/ToulouseExtract_WithGeom.tif 
  15
  ${TEMP}/prTvotbPhysicalToRPCSensorModelImageFilter.tif
  )

ADD_TEST(prTuotbGenericRSResampleImageFilterNew ${PROJECTIONS_TESTS4}
  otbGenericRSResampleImageFilterNew
  )
IF(OTB_DATA_USE_LARGEINPUT)


ADD_TEST(prTvotbGenericRSResampleImageFilter ${PROJECTIONS_TESTS4}
  otbGenericRSResampleImageFilter
  ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
  1000
  15
  0
  1
  ${TEMP}/prTvotbGenericRSResampleImageFilterOutput.tif
  )


# Experimental : Set a variable with the several sensors image 
# paths
# TODO : parse it with a cmake file in OTB-Data-LargeInput directory
# Done here for the moment

SET(SENSOR_TYPES 
  "QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF"
  "GEOEYE/LES_ROCHES/po_350134_bgrn_0000000.tif"
  "WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_MUL/09DEC10103019-M2AS-052298844010_01_P001.TIF"
)

FOREACH( file ${SENSOR_TYPES})
  # Get the sensor name 
  SET(sharp_regexp "([0-9A-Za-z_]*)[ ]*/[ ]*(.*)")
  STRING(REGEX REPLACE "${sharp_regexp}" "\\1" sensor_name "${file}")
  # Tests
  ADD_TEST(prTvotbGenericRSResampleImageFilterFromMap_${sensor_name} ${PROJECTIONS_TESTS4}
    otbGenericRSResampleImageFilterFromMap
    ${LARGEINPUT}/${file}
    15
    0
    ${TEMP}/prTvotbGenericRSResampleImageFilterOutputFromMap_${sensor_name}.tif
    )
ENDFOREACH( file ${SENSOR_TYPES})

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
otbSensorModelGrid.cxx
otbForwardSensorModelGrid.cxx
)

SET(Projections_SRCS2
otbProjectionsTests2.cxx
otbOrthoRectificationFilterNew.cxx
otbOrthoRectificationFilter.cxx
otbOrthoRectificationFilterWithDEM.cxx
otbOrthoRectificationMonoThreadFilter.cxx
otbGCPsToRPCSensorModelImageFilterNew.cxx
otbGCPsToRPCSensorModelImageFilterWithoutDEM.cxx
otbGCPsToRPCSensorModelImageFilterAndOrtho.cxx
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
otbGenericRSTransformWithSRID.cxx
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


SET(Projections_SRCS4
otbProjectionsTests4.cxx
otbPhysicalToRPCSensorModelImageFilter.cxx
otbGenericRSResampleImageFilter.cxx
)

OTB_ADD_EXECUTABLE(otbProjectionsTests1 "${Projections_SRCS1}" "OTBProjections;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbProjectionsTests2 "${Projections_SRCS2}" "OTBProjections;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbProjectionsTests3 "${Projections_SRCS3}" "OTBProjections;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbProjectionsTests4 "${Projections_SRCS4}" "OTBProjections;OTBIO;OTBTesting")

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
