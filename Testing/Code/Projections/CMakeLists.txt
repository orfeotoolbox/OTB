IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


SET(PROJECTIONS_TESTS1 ${CXX_TEST_PATH}/otbProjectionsTests1)
SET(PROJECTIONS_TESTS2 ${CXX_TEST_PATH}/otbProjectionsTests2)
SET(PROJECTIONS_TESTS3 ${CXX_TEST_PATH}/otbProjectionsTests3)
SET(PROJECTIONS_TESTS4 ${CXX_TEST_PATH}/otbProjectionsTests4)
SET(PROJECTIONS_TESTS5 ${CXX_TEST_PATH}/otbProjectionsTests5)

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

ADD_TEST(prTvSensorModelPleiades-PRE-TIFF ${PROJECTIONS_TESTS1}
  --ignore-order --compare-ascii ${NOTOL}
         ${BASELINE_FILES}/prTvSensorModelPleiades-PRE-TIFF.txt
         ${TEMP}/prTvSensorModelPleiades-PRE-TIFF_OUT.txt
         --ignore-lines-with 2 meters_per_pixel_x meters_per_pixel_y
    otbSensorModel
         ${LARGEINPUT}/PLEIADES-PRE/TLSE_TIFF_PRIMARY_DIMAPv2_MS_lossly_12bits/IMG_PHR1A_MS_002/IMG_PHR1A_MS_201006181052297_SEN_IPU_20111025_3077-004_R1C1.TIF
         ${TEMP}/prTvSensorModelPleiades-PRE-TIFF_OUT.txt
)

ADD_TEST(prTvSensorModelPleiades-JP2 ${PROJECTIONS_TESTS1}
  --ignore-order --compare-ascii ${NOTOL}
         ${BASELINE_FILES}/prTvSensorModelPleiades.txt
         ${TEMP}/prTvSensorModelPleiades_OUT.txt
         --ignore-lines-with 2 meters_per_pixel_x meters_per_pixel_y
    otbSensorModel
         ${LARGEINPUT}/PLEIADES/TLSE_JP2_DIMAPv2_PRIMARY_PMS_lossless_12bits/IMGPHR_201222215194743808/IMG_PHR1A_PMS_201201151100183_SEN_IPU_20120222_0901-001_R2C1.JP2
         ${TEMP}/prTvSensorModelPleiades_OUT.txt
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

ADD_TEST(prTvSensorModelRadarSat1_GOMA ${PROJECTIONS_TESTS1}
  --ignore-order --compare-ascii ${NOTOL}
         ${BASELINE_FILES}/prTvSensorModelRadarSat1_GOMA.txt
         ${TEMP}/prTvSensorModelRadarSat1_GOMA.txt
    otbSensorModel
         ${LARGEINPUT}/RADARSAT1/GOMA/SCENE01/DAT_01.001
         ${TEMP}/prTvSensorModelRadarSat1_GOMA.txt
)

ADD_TEST(prTvSensorModelRadarSat1_SARDEGNA ${PROJECTIONS_TESTS1}
  --ignore-order --compare-ascii ${NOTOL}
         ${BASELINE_FILES}/prTvSensorModelRadarSat1_SARDEGNA.txt
         ${TEMP}/prTvSensorModelRadarSat1_SARDEGNA.txt
    otbSensorModel
         ${LARGEINPUT}/RADARSAT1/SARDEGNA/scene01/dat_01.001
         ${TEMP}/prTvSensorModelRadarSat1_SARDEGNA.txt
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
          --compare-n-images ${EPSILON_6} 2
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

        ADD_TEST(prTvForwardSensorModelGrid_${sensor_name} ${PROJECTIONS_TESTS1}
          --compare-n-images ${EPSILON_6} 2
                 ${BASELINE}/prTvForwardSensorModelGridX_${sensor_name}.tif
                 ${TEMP}/prTvForwardSensorModelGridX_${sensor_name}.tif
                 ${BASELINE}/prTvForwardSensorModelGridY_${sensor_name}.tif
                 ${TEMP}/prTvForwardSensorModelGridY_${sensor_name}.tif
            otbForwardSensorModelGrid
                    ${LARGEINPUT}/${file}
                 ${TEMP}/prTvForwardSensorModelGridX_${sensor_name}.tif
                 ${TEMP}/prTvForwardSensorModelGridY_${sensor_name}.tif
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

ADD_TEST(prTvOrthoRectificationSPOT5 ${PROJECTIONS_TESTS2}
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

 ADD_TEST(prTvOrthoRectificationMonoThreadSPOT5 ${PROJECTIONS_TESTS2}
  --compare-image ${EPSILON_4}  ${BASELINE}/prTvOrthoRectificationSPOT5_UTM_band1.tif
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

#============= Test for bug 253 ===============================================
ADD_TEST(prTvOrthoRectificationCoastalZoneWithSRTMNoData ${PROJECTIONS_TESTS2}
 --compare-image ${EPSILON_4}  ${BASELINE}/prTvOrthoRectificationCoastalZoneWithSRTMNoData.tif
                               ${TEMP}/prTvOrthoRectificationCoastalZoneWithSRTMNoData.tif
        otbOrthoRectificationFilterWithDEM
        ${INPUTDATA}/qb_coastal.tif
        ${TEMP}/prTvOrthoRectificationCoastalZoneWithSRTMNoData.tif
        536724
        4.07959e+06
        741
        781
        24
        -24
        31
        N
        ${INPUTDATA}/DEM/srtm_directory/
        24
        )


#========================= Ortho rectif SAR data  ===============================


IF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(prTvOrthoRectificationUTM_RADARDSAT1 ${PROJECTIONS_TESTS2}
          --compare-image ${EPSILON_4}  ${BASELINE}/prTvOrthoRectificationUTM_RADARDSAT1.tif
                          ${TEMP}/prTvOrthoRectificationUTM_RADARDSAT1.tif
        otbOrthoRectificationFilter
                    ${LARGEINPUT}/RADARSAT1/GOMA/SCENE01/
                ${TEMP}/prTvOrthoRectificationUTM_RADARDSAT1.tif
                755929.75 9821313.25
                200        200 10 -10 35 S 4
        )

ADD_TEST(prTvOrthoRectificationUTM_RADARSAT2_XML ${PROJECTIONS_TESTS2}
          --compare-image ${EPSILON_4}  ${BASELINE}/prTvOrthoRectificationUTM_RADARSAT2.tif
                          ${TEMP}/prTvOrthoRectificationUTM_RADARSAT2.tif
        otbOrthoRectificationFilter
                    ${LARGEINPUT}/RADARSAT2/ALTONA/Fine_Quad-Pol_Dataset/PK6621_DK406_FQ9_20080405_124900_HH_VV_HV_VH_SLC_Altona/product.xml
                ${TEMP}/prTvOrthoRectificationUTM_RADARSAT2.tif
                611000.953075 5439991.842531
                200        200 10 -10 14 N 4
        )
ADD_TEST(prTvOrthoRectificationUTM_RADARSAT2_HH ${PROJECTIONS_TESTS2}
          --compare-image ${EPSILON_4}  ${BASELINE}/prTvOrthoRectificationUTM_RADARSAT2_XML.tif
                          ${TEMP}/prTvOrthoRectificationUTM_RADARSAT2_XML.tif
        otbOrthoRectificationFilter
                    ${LARGEINPUT}/RADARSAT2/ALTONA/Fine_Quad-Pol_Dataset/PK6621_DK406_FQ9_20080405_124900_HH_VV_HV_VH_SLC_Altona/imagery_HH.tif
                ${TEMP}/prTvOrthoRectificationUTM_RADARSAT2_XML.tif
                611000.953075 5439991.842531
                200        200 10 -10 14 N 4
        )
ADD_TEST(prTvOrthoRectificationUTM_RADARSAT2_HV ${PROJECTIONS_TESTS2}
          --compare-image ${EPSILON_4}  ${BASELINE}/prTvOrthoRectificationUTM_RADARSAT2_HV.tif
                          ${TEMP}/prTvOrthoRectificationUTM_RADARSAT2_HV.tif
        otbOrthoRectificationFilter
                    ${LARGEINPUT}/RADARSAT2/ALTONA/Fine_Quad-Pol_Dataset/PK6621_DK406_FQ9_20080405_124900_HH_VV_HV_VH_SLC_Altona/imagery_HV.tif
                ${TEMP}/prTvOrthoRectificationUTM_RADARSAT2_HV.tif
                611000.953075 5439991.842531
                200        200 10 -10 14 N 4
        )
ADD_TEST(prTvOrthoRectificationUTM_RADARSAT2_VH ${PROJECTIONS_TESTS2}
          --compare-image ${EPSILON_4}  ${BASELINE}/prTvOrthoRectificationUTM_RADARSAT2_VH.tif
                          ${TEMP}/prTvOrthoRectificationUTM_RADARSAT2_VH.tif
        otbOrthoRectificationFilter
                    ${LARGEINPUT}/RADARSAT2/ALTONA/Fine_Quad-Pol_Dataset/PK6621_DK406_FQ9_20080405_124900_HH_VV_HV_VH_SLC_Altona/imagery_VH.tif
                ${TEMP}/prTvOrthoRectificationUTM_RADARSAT2_VH.tif
                611000.953075 5439991.842531
                200        200 10 -10 14 N 4
        )
ADD_TEST(prTvOrthoRectificationUTM_RADARSAT2_VV ${PROJECTIONS_TESTS2}
          --compare-image ${EPSILON_4}  ${BASELINE}/prTvOrthoRectificationUTM_RADARSAT2_VV.tif
                          ${TEMP}/prTvOrthoRectificationUTM_RADARSAT2_VV.tif
        otbOrthoRectificationFilter
                    ${LARGEINPUT}/RADARSAT2/ALTONA/Fine_Quad-Pol_Dataset/PK6621_DK406_FQ9_20080405_124900_HH_VV_HV_VH_SLC_Altona/imagery_VV.tif
                ${TEMP}/prTvOrthoRectificationUTM_RADARSAT2_VV.tif
                611000.953075 5439991.842531
                200        200 10 -10 14 N 4
        )

ADD_TEST(prTvOrthoRectificationUTM_ENVISAT ${PROJECTIONS_TESTS2}
          --compare-image ${EPSILON_4}  ${BASELINE}/prTvOrthoRectificationUTM_ENVISAT.tif
                          ${TEMP}/prTvOrthoRectificationUTM_ENVISAT.tif
        otbOrthoRectificationComplexFilter
                    ${LARGEINPUT}/ENVISAT_ASAR_SCENE01/ASA_APS_1PNDPA20030821_7713.N1
                ${TEMP}/prTvOrthoRectificationUTM_ENVISAT.tif
                655696.679130 4906584.046495
                200        200 10 -10 30 N 4
        )

ADD_TEST(prTvOrthoRectificationUTM_PALSAR ${PROJECTIONS_TESTS2}
          --compare-image ${EPSILON_4}  ${BASELINE}/prTvOrthoRectificationUTM_PALSAR.tif
                          ${TEMP}/prTvOrthoRectificationUTM_PALSAR.tif
        otbOrthoRectificationComplexFilter
                    ${LARGEINPUT}/PALSAR/200801280007/l1data/VOL-ALPSRP037120700-H1.1__A
                ${TEMP}/prTvOrthoRectificationUTM_PALSAR.tif
                309298.731439 3909055.130746
                200        200 10 -10 54 N 4
        )

ADD_TEST(prTvOrthoRectificationUTM_TERRASARX_HH ${PROJECTIONS_TESTS2}
          --compare-image ${EPSILON_4}  ${BASELINE}/prTvOrthoRectificationUTM_TERRASARX_HH.tif
                          ${TEMP}/prTvOrthoRectificationUTM_TERRASARX_HH.tif
        otbOrthoRectificationComplexFilter
                    ${LARGEINPUT}/TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_HH_SRA_stripFar_008.cos
                ${TEMP}/prTvOrthoRectificationUTM_TERRASARX_HH.tif
                591992.001073 9699689.825866
                200        200 10 -10 49 S 4
        )

ADD_TEST(prTvOrthoRectificationUTM_TERRASARX_VV ${PROJECTIONS_TESTS2}
          --compare-image ${EPSILON_4}  ${BASELINE}/prTvOrthoRectificationUTM_TERRASARX_VV.tif
                          ${TEMP}/prTvOrthoRectificationUTM_TERRASARX_VV.tif
        otbOrthoRectificationComplexFilter
                    ${LARGEINPUT}/TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_VV_SRA_stripFar_008.cos
                ${TEMP}/prTvOrthoRectificationUTM_TERRASARX_VV.tif
                591992.001073 9699689.825866
                200        200 10 -10 49 S 4
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



ADD_TEST(prTvGCPsToRPCSensorModelImageCheckInputGcpPointsProjection_WithDEM ${PROJECTIONS_TESTS2}
  otbGCPsToRPCSensorModelImageFilterCheckRpcModel
  -in ${INPUTDATA}/Haiti1.jpeg
  -dem ${INPUTDATA}/DEM/srtm_directory
  -gcp  1980.000000 1986.000000 -72.351257 18.555075 0.000000
  1963.000000 1944.000000 -72.352051 18.556946 0.000000
  2078.000000 1566.000000 -72.346130 18.574358 0.000000
  2154.000000 1430.000000 -72.342377 18.580441 0.000000
  2432.000000 1408.000000 -72.328964 18.581335 0.000000
  2558.000000 934.000000 -72.322571 18.602974 0.000000
  3492.000000 1397.000000 -72.278343 18.580887 0.000000
  2890.000000 1439.000000 -72.306908 18.579464 0.000000
  2098.000000 2469.000000 -72.345741 18.532839 0.000000
  1941.000000 2473.000000 -72.353317 18.532595 0.00000
  1644.000000 2409.000000 -72.367691 18.535810 0.000000
  1440.000000 2448.000000 -72.377457 18.534243 0.000000
  687.000000 2005.000000 -72.413315 18.554974 0.000000
  741.000000 2366.000000 -72.410934 18.538496 0.000000
  855.000000 2802.000000 -72.405739 18.518456 0.000000
  1936.000000 2855.000000 -72.353920 18.515240 0.000000
  3338.000000 2824.000000 -72.286240 18.515160 0.000000
  3715.000000 1772.000000 -72.267570 18.563681 0.000000
  3676.000000 1580.000000 -72.269203 18.572529 0.000000
  3395.000000 931.000000 -72.282402 18.602425 0.000000
  3426.000000 856.000000 -72.280792 18.605862 0.000000
  2822.000000 707.000000 -72.309692 18.613203 0.000000
  1891.000000 360.000000 -72.354301 18.629776 0.000000
  3989.000000 2639.000000 -72.255020 18.523664 0.000000
  417.000000 2317.000000 -72.426552 18.540876 0.000000
  -err 100 # Tolerance in meters, the first goal of this test
           # is highlighting nan in projected point using the rpc model.
           # and not to test the precision of the projection. 100 is then
           # to detect false reprojections results.
  )

ADD_TEST(prTvGCPsToRPCSensorModelImageCheckInputGcpPointsProjection_WithoutDEM ${PROJECTIONS_TESTS2}
  otbGCPsToRPCSensorModelImageFilterCheckRpcModel
  -in ${INPUTDATA}/Haiti1.jpeg
  -gcp  1980.000000 1986.000000 -72.351257 18.555075 0.000000
  1963.000000 1944.000000 -72.352051 18.556946 0.000000
  2078.000000 1566.000000 -72.346130 18.574358 0.000000
  2154.000000 1430.000000 -72.342377 18.580441 0.000000
  2432.000000 1408.000000 -72.328964 18.581335 0.000000
  2558.000000 934.000000 -72.322571 18.602974 0.000000
  3492.000000 1397.000000 -72.278343 18.580887 0.000000
  2890.000000 1439.000000 -72.306908 18.579464 0.000000
  2098.000000 2469.000000 -72.345741 18.532839 0.000000
  1941.000000 2473.000000 -72.353317 18.532595 0.00000
  1644.000000 2409.000000 -72.367691 18.535810 0.000000
  1440.000000 2448.000000 -72.377457 18.534243 0.000000
  687.000000 2005.000000 -72.413315 18.554974 0.000000
  741.000000 2366.000000 -72.410934 18.538496 0.000000
  855.000000 2802.000000 -72.405739 18.518456 0.000000
  1936.000000 2855.000000 -72.353920 18.515240 0.000000
  3338.000000 2824.000000 -72.286240 18.515160 0.000000
  3715.000000 1772.000000 -72.267570 18.563681 0.000000
  3676.000000 1580.000000 -72.269203 18.572529 0.000000
  3395.000000 931.000000 -72.282402 18.602425 0.000000
  3426.000000 856.000000 -72.280792 18.605862 0.000000
  2822.000000 707.000000 -72.309692 18.613203 0.000000
  1891.000000 360.000000 -72.354301 18.629776 0.000000
  3989.000000 2639.000000 -72.255020 18.523664 0.000000
  417.000000 2317.000000 -72.426552 18.540876 0.000000
  -err 100 # Tolerance in meters, the first goal of this test
           # is highlighting nan in projected point using the rpc model.
           # and not to test the precision of the projection. 100 is then
           # to detect false reprojections results.
  )

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
#                                otbRegionProjectionResampler
#        ${EMMANUELDATA}/04MAR21031747-M1BS-000000173549_01_P001.TIF
#        ${TEMP}/prTvRegionProjectionResamplerSingapour.tif
#        103.8464 1.31
#        500
#                                 500
#                                 10
#                                 0.00001
#                                 -0.00001
#        )

#ADD_TEST(prTvOrthoRectificationSingapour ${PROJECTIONS_TESTS2}
#        otbOrthoRectificationFilter
#        ${EMMANUELDATA}/04MAR21031747-M1BS-000000173549_01_P001.TIF
#        ${TEMP}/prTvOrthoRectificationSingapour_UTM.tif
#        371997.12 144885.4
#        500
#                                 500
#                                 220
#                                 1
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


ADD_TEST(prTvGenericRSTransformFromImage ${PROJECTIONS_TESTS3}
               otbGenericRSTransformFromImage
               ${INPUTDATA}/WithoutProjRefWithKeywordlist.tif)

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(prTvGenericRSTransformQuickbirdToulouseGeodesicPointChecking ${PROJECTIONS_TESTS3}
otbGenericRSTransformImageAndMNTToWGS84ConversionChecking
${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
${INPUTDATA}/DEM/srtm_directory/
${INPUTDATA}/DEM/egm96.grd
16271 15647 # IGN reference point based inside CNES (site 31555A point c)
1.48353 43.55968 # IGN reference point lon/lat
192.205 # IGN reference point elevation (prec < 10 cm)
24 # Tolerance in pixels according to DigitalGlobe specification
23 # Tolerance in meters according to DigitalGlobe specification
)



# TODO MSD need to update the tolerance values and used a real pleiades file
ADD_TEST(prTvGenericRSTransformPleiadesToulouseGeodesicPointChecking_PRE ${PROJECTIONS_TESTS3}
otbGenericRSTransformImageAndMNTToWGS84ConversionChecking
${LARGEINPUT}/PLEIADES-PRE/TLSE_TIFF_PRIMARY_DIMAPv2_MS_lossly_12bits/IMG_PHR1A_MS_002/IMG_PHR1A_MS_201006181052297_SEN_IPU_20111025_3077-004_R1C1.TIF
${INPUTDATA}/DEM/srtm_directory/
${INPUTDATA}/DEM/egm96.grd
27760 25888 # IGN reference point based inside CNES (site 31555A point c)
1.48353 43.55968 # IGN reference point lon/lat
192.205 # IGN reference point elevation (prec < 10 cm)
1 # Tolerance in pixels according to values found in RPC-DIMAPv2 file (ERR_BIAS_ROW and ERR_BIAS_COL)
1 # Tolerance in meters according to values found in RPC-DIMAPv2 file (ERR_BIAS_X and ERR_BIAS_Y)
)

# TODO MSD need to update the tolerance values and used a real pleiades file
ADD_TEST(prTvGenericRSTransformPleiadesToulouseGeodesicPointChecking ${PROJECTIONS_TESTS3}
otbGenericRSTransformImageAndMNTToWGS84ConversionChecking
${LARGEINPUT}/PLEIADES/TLSE_JP2_DIMAPv2_PRIMARY_PMS_lossless_12bits/IMGPHR_201222215194743808/IMG_PHR1A_PMS_201201151100183_SEN_IPU_20120222_0901-001_R2C1.JP2
${INPUTDATA}/DEM/srtm_directory/
${INPUTDATA}/DEM/egm96.grd
20000 28010 # IGN reference point based inside CNES (site 31555A point c)
1.48353 43.55968 # IGN reference point lon/lat
192.205 # IGN reference point elevation (prec < 10 cm)
1 # Tolerance in pixels according to values found in RPC-DIMAPv2 file (ERR_BIAS_ROW and ERR_BIAS_COL)
1 # Tolerance in meters according to values found in RPC-DIMAPv2 file (ERR_BIAS_X and ERR_BIAS_Y)
)


# Generic Testing
SET(GenericConversionChecking_INPUTS
    ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
    #${LARGEINPUT}/SPOT4/RIO_DE_JANEIRO/IMAG_01.DAT
    ${LARGEINPUT}/SPOT5/TEHERAN/IMAGERY.TIF
    ${LARGEINPUT}/RADARSAT1/GOMA2/SCENE01/DAT_01.001
    ${LARGEINPUT}/RADARSAT2/ALTONA/Fine_Quad-Pol_Dataset/PK6621_DK406_FQ9_20080405_124900_HH_VV_HV_VH_SLC_Altona/imagery_HH.tif
    ${LARGEINPUT}/FORMOSAT/Sudouest_20071013_MS_fmsat/IMAGERY.TIF
    #${LARGEINPUT}/GEOEYE/LES_ROCHES/po_350134_bgrn_0000000.tif
    #${LARGEINPUT}/IKONOS/BLOSSEVILLE/po_2619900_pan_0000000.tif
    #${LARGEINPUT}/PALSAR/ALOS-H1_1__A-ORBIT__ALPSRP037120700.tif
    #${LARGEINPUT}/WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_PAN/09DEC10103019-P2AS-052298844010_01_P001.TIF
    ${LARGEINPUT}/TERRASARX/PANGKALANBUUN/TSX1_SAR__SSC______SM_D_SRA_20080313T221955_20080313T222003.xml
    ${LARGEINPUT}/ENVISAT_ASAR_SCENE01/ASA_APS_1PNDPA20030821_7713.N1
    #${LARGEINPUT}/COSMOSKYMED/Toulouse_spotlight/CSKS3_GTC_B_S2_08_HH_RD_SF_20110418180325_20110418180332.h5
    #${LARGEINPUT}/MODIS/MOD13Q1/MOD13Q1.A2010001.h17v05.005.2010028003734.hdf
    #${LARGEINPUT}/ONERA/spa3_0215_rad.dat
    #${LARGEINPUT}/RAPIDEYE/level3A/2008-12-25T005918_RE3_3A-NAC_752656_40091.tif
    )

SET(GenericConversionChecking_TYPES
	QUICKBIRD
	#SPOT4
	SPOT5
	RADARSAT1
	RADARSAT2
	FORMOSAT
	#GEOEYE
	#IKONOS
	#PALSAR
	#WORLDVIEW2
	TERRASARX
	ENVISAT
	#COSMOSKYMED
	#MODIS
	#ONERA
	#RAPIDEYE
	)

SET(GenericConversionChecking_TOL_PIX
	24 # according to DigitalGlobe specification
	#30
	30
	30
	30
	30
	#30
	#30
	#30
	#30
	30
	30
	#30
	#30
	#30
	#30
	)

SET(GenericConversionChecking_TOL_M
	23 # according to DigitalGlobe specification
	#30
	30
	30
	30
	30
	#30
	#30
	#30
	#30
	#30
	30
	30
	#30
	#30
	#30
	)

SET( GenericConversionChecking_TESTNB 0)

FOREACH( file ${GenericConversionChecking_INPUTS} )
  LIST(GET GenericConversionChecking_TYPES   ${GenericConversionChecking_TESTNB} current_type   )
  LIST(GET GenericConversionChecking_TOL_PIX ${GenericConversionChecking_TESTNB} current_tol_pix)
  LIST(GET GenericConversionChecking_TOL_M   ${GenericConversionChecking_TESTNB} current_tol_m  )
  MATH(EXPR GenericConversionChecking_TESTNB "${GenericConversionChecking_TESTNB} + 1")
  ADD_TEST(prTvGenericRSTransformGenericConversionCheckingFromGCP_${current_type} ${PROJECTIONS_TESTS3}
	otbGenericRSTransformGenericConversionCheckingFromGCP
	${file}
	${INPUTDATA}/DEM/srtm_directory/
	${INPUTDATA}/DEM/egm96.grd #Not used
	${current_tol_pix} # Tolerance in pixels
	${current_tol_m}   # Tolerance in meters
	1                  # Verbose
)
ENDFOREACH()

# Forward / Backward projection consistency checking

SET(FWDBWDChecking_INPUTS
    ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
    #${LARGEINPUT}/SPOT4/RIO_DE_JANEIRO/IMAG_01.DAT
    ${LARGEINPUT}/SPOT5/TEHERAN/IMAGERY.TIF
    ${LARGEINPUT}/RADARSAT1/GOMA2/SCENE01/DAT_01.001
    ${LARGEINPUT}/RADARSAT2/ALTONA/Fine_Quad-Pol_Dataset/PK6621_DK406_FQ9_20080405_124900_HH_VV_HV_VH_SLC_Altona/imagery_HH.tif
    ${LARGEINPUT}/FORMOSAT/Sudouest_20071013_MS_fmsat/IMAGERY.TIF
    #${LARGEINPUT}/GEOEYE/LES_ROCHES/po_350134_bgrn_0000000.tif
    #${LARGEINPUT}/IKONOS/BLOSSEVILLE/po_2619900_pan_0000000.tif
    #${LARGEINPUT}/PALSAR/ALOS-H1_1__A-ORBIT__ALPSRP037120700.tif
    #${LARGEINPUT}/WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_PAN/09DEC10103019-P2AS-052298844010_01_P001.TIF
    ${LARGEINPUT}/TERRASARX/2008-03-10_GrandCanyon_SSC/TSX1_SAR__SSC______SM_S_SRA_20080310T133220_20080310T133228/IMAGEDATA/IMAGE_HH_SRA_strip_011.cos
    ${LARGEINPUT}/ENVISAT_ASAR_SCENE01/ASA_APS_1PNDPA20030821_7713.N1
    #${LARGEINPUT}/COSMOSKYMED/Toulouse_spotlight/CSKS3_GTC_B_S2_08_HH_RD_SF_20110418180325_20110418180332.h5
    #${LARGEINPUT}/MODIS/MOD13Q1/MOD13Q1.A2010001.h17v05.005.2010028003734.hdf
    #${LARGEINPUT}/ONERA/spa3_0215_rad.dat
    #${LARGEINPUT}/RAPIDEYE/level3A/2008-12-25T005918_RE3_3A-NAC_752656_40091.tif
    )
    
SET(FWDBWDChecking_TYPES
	QUICKBIRD
	#SPOT4
	SPOT5
	RADARSAT1
	RADARSAT2
	FORMOSAT
	#GEOEYE
	#IKONOS
	#PALSAR
	#WORLDVIEW2
	TERRASARX
	ENVISAT
	#COSMOSKYMED
	#MODIS
	#ONERA
	#RAPIDEYE
	)
SET( FWDBWDChecking_TESTNB 0)

FOREACH( file ${FWDBWDChecking_INPUTS} )
  LIST(GET FWDBWDChecking_TYPES   ${FWDBWDChecking_TESTNB} current_type   )
  MATH(EXPR FWDBWDChecking_TESTNB "${FWDBWDChecking_TESTNB} + 1")
  ADD_TEST(prTvForwardBackwardProjectionConsistency_${current_type} ${PROJECTIONS_TESTS3}
	otbForwardBackwardProjectionConsistency
	${file}
	${INPUTDATA}/DEM/srtm_directory/
	${INPUTDATA}/DEM/egm96.grd #Not used
	20 # nb Tested Points
	1  # Tolerance in pixels
	1  # Verbose
)
ENDFOREACH()

ENDIF(OTB_DATA_USE_LARGEINPUT)

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


#test points
ADD_TEST(prTvGeometriesProjectionFilterPoints ${PROJECTIONS_TESTS3}
        --compare-ogr ${NOTOL}
            ${BASELINE_FILES}/prTvVectorDataProjectionFilterPoints.shp
            ${TEMP}/prTvGeometriesProjectionFilterPoints.shp
        otbGeometriesProjectionFilter
        ${INPUTDATA}/ToulousePoints-examples.shp
        ${TEMP}/prTvGeometriesProjectionFilterPoints.shp
)

#test lines
ADD_TEST(prTvGeometriesProjectionFilterLines ${PROJECTIONS_TESTS3}
        --compare-ogr ${NOTOL}
            ${BASELINE_FILES}/prTvVectorDataProjectionFilterLines.shp
            ${TEMP}/prTvGeometriesProjectionFilterLines.shp
        otbGeometriesProjectionFilter
        ${INPUTDATA}/ToulouseRoad-examples.shp
        ${TEMP}/prTvGeometriesProjectionFilterLines.shp
)

# test polygons
ADD_TEST(prTvGeometriesProjectionFilterPolygons ${PROJECTIONS_TESTS3}
        --compare-ogr  ${NOTOL}
            ${BASELINE_FILES}/prTvVectorDataProjectionFilterPolygons.shp
            ${TEMP}/prTvGeometriesProjectionFilterPolygons.shp
        otbGeometriesProjectionFilter
        ${INPUTDATA}/Capitole-Shadows.shp
        ${TEMP}/prTvGeometriesProjectionFilterPolygons.shp
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

ADD_TEST(prTvGeometriesProjectionFilterFromMapToSensor ${PROJECTIONS_TESTS3}
        --compare-ogr  ${NOTOL}
            ${BASELINE_FILES}/prTvVectorDataProjectionFilterFromMapToSensor.shp
            ${TEMP}/prTvGeometriesProjectionFilterFromMapToSensor.shp
        otbGeometriesProjectionFilterFromMapToSensor
        ${INPUTDATA}/ToulousePoints-examples.shp
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        ${TEMP}/prTvGeometriesProjectionFilterFromMapToSensor.shp
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

ADD_TEST(prTvGeometriesProjectionFilterFromMapToGeo ${PROJECTIONS_TESTS3}
        --compare-ascii ${NOTOL}
        ${BASELINE_FILES}/prTvVectorDataProjectionFilterFromMapToGeo.kml
        ${TEMP}/prTvGeometriesProjectionFilterFromMapToGeo.kml
        otbGeometriesProjectionFilterFromMapToGeo
        ${INPUTDATA}/ToulousePoints-examples.shp
        ${TEMP}/prTvGeometriesProjectionFilterFromMapToGeo.kml
)

ADD_TEST(prTvVectorDataProjectionFilterFromMapToImage ${PROJECTIONS_TESTS3}
        --compare-ascii ${NOTOL}
        ${BASELINE_FILES}/prTvVectorDataProjectionFilterFromMapToImage.shp
        ${TEMP}/prTvVectorDataProjectionFilterFromMapToImage.shp
        otbVectorDataProjectionFilterFromMapToImage
        ${INPUTDATA}/ToulousePoints-examples.shp
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
        ${TEMP}/prTvVectorDataProjectionFilterFromMapToImage.shp
)

ADD_TEST(prTvGeometriesProjectionFilterFromMapToImage ${PROJECTIONS_TESTS3}
        --compare-ascii ${NOTOL}
        ${BASELINE_FILES}/prTvVectorDataProjectionFilterFromMapToImage.shp
        ${TEMP}/prTvGeometriesProjectionFilterFromMapToImage.shp
        otbGeometriesProjectionFilterFromMapToImage
        ${INPUTDATA}/ToulousePoints-examples.shp
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
        ${TEMP}/prTvGeometriesProjectionFilterFromMapToImage.shp
)

# With QGIS the two vectordata are equal
ADD_TEST(prTvVectorDataProjectionFilterFromGeoToMap ${PROJECTIONS_TESTS3}
        #--compare-ascii ${NOTOL}
        #${INPUTDATA}/ToulousePoints-examples.shp
        #${TEMP}/prTvVectorDataProjectionFilterFromGeoToMap.shp
        otbVectorDataProjectionFilterFromGeoToMap
        ${TEMP}/prTvVectorDataProjectionFilterFromMapToGeo.kml
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
        ${TEMP}/prTvVectorDataProjectionFilterFromGeoToMap.shp
)
SET_TESTS_PROPERTIES(prTvVectorDataProjectionFilterFromGeoToMap PROPERTIES DEPENDS prTvVectorDataProjectionFilterFromMapToGeo)
ADD_TEST(prTvGeometriesProjectionFilterFromGeoToMap ${PROJECTIONS_TESTS3}
        #--compare-ascii ${NOTOL}
        #${INPUTDATA}/ToulousePoints-examples.shp
        #${TEMP}/prTvGeometriesProjectionFilterFromGeoToMap.shp
        otbGeometriesProjectionFilterFromGeoToMap
        ${TEMP}/prTvGeometriesProjectionFilterFromMapToGeo.kml
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
        ${TEMP}/prTvGeometriesProjectionFilterFromGeoToMap.shp
)
SET_TESTS_PROPERTIES(prTvGeometriesProjectionFilterFromGeoToMap PROPERTIES DEPENDS prTvGeometriesProjectionFilterFromMapToGeo)



ADD_TEST(prTvGeometriesProjectionFilterFromMapToEPSG_UTM ${PROJECTIONS_TESTS3}
         --compare-ogr ${NOTOL}
           ${BASELINE_FILES}/prTvVectorDataProjectionFilterLines.shp
           ${TEMP}/prTvGeometriesProjectionFilterFromMapToEPSG_UTM.shp
         otbGeometriesProjectionFilterFromMapToEPSG
           ${INPUTDATA}/ToulouseRoad-examples.shp
           ${TEMP}/prTvGeometriesProjectionFilterFromMapToEPSG_UTM.shp
           32631 )

ADD_TEST(prTvGeometriesProjectionFilterFromMapToEPSG_WGS84 ${PROJECTIONS_TESTS3}
         --compare-ogr ${NOTOL}
           ${BASELINE_FILES}/prTvVectorDataProjectionFilterFromMapToGeo.kml
           ${TEMP}/prTvGeometriesProjectionFilterFromMapToEPSG_WGS84.kml
         otbGeometriesProjectionFilterFromMapToEPSG
           ${INPUTDATA}/ToulousePoints-examples.shp
           ${TEMP}/prTvGeometriesProjectionFilterFromMapToEPSG_WGS84.kml
           4326 )


###################
# TESTS ABOUT REPROJECTION INTO AN IMAGE OF A VECTORDATA
IF(OTB_DATA_USE_LARGEINPUT)
  SET(VectorDataIntoImage_FILES
    ${LARGEINPUT}/QB_EXTRACT/qb_extract_00_withgeom.tif
    ${LARGEINPUT}/QB_EXTRACT/qb_extract_00_withgeom_ENVI.hdr
    ${LARGEINPUT}/QB_EXTRACT/qb_extract_11_withgeom.tif
    ${LARGEINPUT}/QB_EXTRACT/qb_extract_11_withgeom_ENVI.hdr
    ${LARGEINPUT}/QB_EXTRACT/qb_utm.tif
    ${LARGEINPUT}/QB_EXTRACT/qb_utm_withgeom.tif
    ${LARGEINPUT}/QB_EXTRACT/qb_wgs84.tif
    ${LARGEINPUT}/QB_EXTRACT/qb_wgs84_withgeom.tif
    ${LARGEINPUT}/QB_EXTRACT/qb_wgs84_worldfile.tif
    )

  SET( VectorDataIntoImage_TESTNB 0)

  FOREACH( file ${VectorDataIntoImage_FILES} )
    MATH(EXPR VectorDataIntoImage_TESTNB "${VectorDataIntoImage_TESTNB} + 1")
    ADD_TEST(prTvVectorDataIntoImageProjectionFilter_${VectorDataIntoImage_TESTNB} ${PROJECTIONS_TESTS3}
             otbVectorDataIntoImageProjectionFilterTest
              ${file}
              ${LARGEINPUT}/VECTOR/MidiPyrenees/natural.shp
              ${INPUTDATA}/DEM/srtm_directory
              3 )
  ENDFOREACH()

#  These tests will fail since data does not have geographic/cartographic information
#  The call SET_TESTS_PROPERTIES(test PROPERTIES WILL_FAIL true) does not do what it is supposed to
#
#  SET(VectorDataIntoImage_FILES_NOGEOM
#    ${LARGEINPUT}/QB_EXTRACT/qb_extract_00_nogeom.tif
#    ${LARGEINPUT}/QB_EXTRACT/qb_extract_00_nogeom_ENVI.hdr
#    ${LARGEINPUT}/QB_EXTRACT/qb_extract_11_nogeom.tif
#    ${LARGEINPUT}/QB_EXTRACT/qb_extract_11_nogeom_ENVI.hdr)
#
#  FOREACH( file ${VectorDataIntoImage_FILES_NOGEOM} )
#    MATH(EXPR VectorDataIntoImage_TESTNB "${VectorDataIntoImage_TESTNB} + 1")
#    ADD_TEST(prTvVectorDataIntoImageProjectionFilter_${VectorDataIntoImage_TESTNB} ${PROJECTIONS_TESTS3}
#             otbVectorDataIntoImageProjectionFilterTest
#              ${file}
#              ${LARGEINPUT}/VECTOR/MidiPyrenees/natural.shp
#              ${INPUTDATA}/DEM/srtm_directory
#              3 )
#    SET_TESTS_PROPERTIES(prTvVectorDataIntoImageProjectionFilter_${VectorDataIntoImage_TESTNB}
#                         PROPERTIES WILL_FAIL true)
#  ENDFOREACH()

ENDIF(OTB_DATA_USE_LARGEINPUT)

# QB ortho and extract and extract of MidiPyrenees roads file
# ADD_TEST(prTvVectorData_ProjRef_ProjIntoImage_WithProjRef ${PROJECTIONS_TESTS3}
#         otbVectorDataIntoImageProjectionFilterTest
#           ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE.TIF
#           ${LARGEINPUT}/VECTOR/MidiPyrenees/roads.shp
#           ${INPUTDATA}/DEM/srtm_directory
#           ${TEMP}/prTvVectorDataProjRefProjIntoImageWithProjRef.shp
#           ${TEMP}/prTvVectorDataProjRefProjIntoImageWithProjRef.txt
#           0
# )

# ADD_TEST(prTvVectorData_ProjRef_ProjIntoImage_WithProjRef1 ${PROJECTIONS_TESTS3}
#         otbVectorDataIntoImageProjectionFilterTest
#           ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE.TIF
#           ${LARGEINPUT}/VECTOR/MidiPyrenees/roads.shp
#           ${INPUTDATA}/DEM/srtm_directory
#           ${TEMP}/prTvVectorDataProjRefProjIntoImageWithProjRef1.shp
#           ${TEMP}/prTvVectorDataProjRefProjIntoImageWithProjRef1.txt
#           1
# )

#ADD_TEST(prTvVectorData_ProjRef_ProjIntoImage_WithSensorModel ${PROJECTIONS_TESTS3}
#        otbVectorDataIntoImageProjectionFilterTest
#          ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE_EXTRACT.HDR
#          ${LARGEINPUT}/VECTOR/MidiPyrenees/roads.shp
#          ${INPUTDATA}/DEM/srtm_directory
#          ${TEMP}/prTvVectorDataProjRefProjIntoImageWithWithSensorModel.shp
#          ${TEMP}/prTvVectorDataProjRefProjIntoImageWithWithSensorModel.txt
#          0
#)

#ADD_TEST(prTvVectorData_ProjRef_ProjIntoImage_WithSensorModel1 ${PROJECTIONS_TESTS3}
#        otbVectorDataIntoImageProjectionFilterTest
#          ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE_EXTRACT.HDR
#          ${LARGEINPUT}/VECTOR/MidiPyrenees/roads.shp
#          ${INPUTDATA}/DEM/srtm_directory
#          ${TEMP}/prTvVectorDataProjRefProjIntoImageWithWithSensorModel1.shp
#          ${TEMP}/prTvVectorDataProjRefProjIntoImageWithWithSensorModel1.txt
#          1
#)

#ADD_TEST(prTvVectorData_ProjRef_ProjIntoImage_Index ${PROJECTIONS_TESTS3}
#        otbVectorDataIntoImageProjectionFilterTest
#          ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE_EXTRACT_INDEX.HDR
#          ${LARGEINPUT}/VECTOR/MidiPyrenees/roads.shp
#          ${INPUTDATA}/DEM/srtm_directory
#          ${TEMP}/prTvVectorDataProjRefProjIntoImageIndex.shp
#          ${TEMP}/prTvVectorDataProjRefProjIntoImageindex.txt
#          0
#)

#ADD_TEST(prTvVectorData_ProjRef_ProjIntoImage_Index1 ${PROJECTIONS_TESTS3}
#        otbVectorDataIntoImageProjectionFilterTest
#           ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE_EXTRACT_INDEX.HDR
#          ${LARGEINPUT}/VECTOR/MidiPyrenees/roads.shp
#          ${INPUTDATA}/DEM/srtm_directory
#          ${TEMP}/prTvVectorDataProjRefProjIntoImageIndex1.shp
#          ${TEMP}/prTvVectorDataProjRefProjIntoImageindex1.txt
#          1
#)

#ADD_TEST(prTvVectorData_NoProjRef_ProjIntoImage_Index ${PROJECTIONS_TESTS3}
#        otbVectorDataIntoImageProjectionFilterTest
#          ${INPUTDATA}/QB_Toulouse_extract_leftup_400x128_index.hdr
#          ${INPUTDATA}/QB_Toulouse_extract_leftup_400x128_index.shp
#          ${INPUTDATA}/DEM/srtm_directory
#          ${TEMP}/prTvVectorDataNoProjRefProjIntoImageIndex.shp
#          ${TEMP}/prTvVectorDataNoProjRefProjIntoImageindex.txt
#          0
#)
#ADD_TEST(prTvVectorData_NoProjRef_ProjIntoImage_Index1 ${PROJECTIONS_TESTS3}
#        otbVectorDataIntoImageProjectionFilterTest
#          ${INPUTDATA}/QB_Toulouse_extract_leftup_400x128_index.hdr
#          ${INPUTDATA}/QB_Toulouse_extract_leftup_400x128_index.shp
#          ${INPUTDATA}/DEM/srtm_directory
#          ${TEMP}/prTvVectorDataNoProjRefProjIntoImageIndex1.shp
#          ${TEMP}/prTvVectorDataNoProjRefProjIntoImageindex1.txt
#          1
#)



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
  --compare-image ${NOTOL}
    ${BASELINE}/prTvotbGenericRSResampleImageFilterOutput.tif
    ${TEMP}/prTvotbGenericRSResampleImageFilterOutput.tif
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
    --compare-image ${NOTOL}
    ${BASELINE}/prTvotbGenericRSResampleImageFilterOutputFromMap_${sensor_name}.tif
    ${TEMP}/prTvotbGenericRSResampleImageFilterOutputFromMap_${sensor_name}.tif
    otbGenericRSResampleImageFilterFromMap
    ${LARGEINPUT}/${file}
    15
    0
    ${TEMP}/prTvotbGenericRSResampleImageFilterOutputFromMap_${sensor_name}.tif
    )
ENDFOREACH( file ${SENSOR_TYPES})

ENDIF(OTB_DATA_USE_LARGEINPUT)


#------ otb::ImageToEnvelopeVectorDataFilter ---------------------
ADD_TEST(prTuImageToEnvelopeVectorDataFilterNew ${PROJECTIONS_TESTS4}
otbImageToEnvelopeVectorDataFilterNew)

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(prTvImageToEnvelopeVectorDataFilter ${PROJECTIONS_TESTS4}
--compare-ascii ${EPSILON_6}
  ${BASELINE_FILES}/prTvImageToEnvelopeVectorDataFilterOutput.kml
  ${TEMP}/prTvImageToEnvelopeVectorDataFilterOutput.kml
  otbImageToEnvelopeVectorDataFilter
  ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
  ${TEMP}/prTvImageToEnvelopeVectorDataFilterOutput.kml
)
ENDIF(OTB_DATA_USE_LARGEINPUT)

#------ otb::ImageToGenericRSOutputParameters ---------------------
ADD_TEST(prTuImageToGenericRSOutputParametersNew ${PROJECTIONS_TESTS4}
otbImageToGenericRSOutputParametersNew)


IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(prTvImageToGenericRSOutputParameters ${PROJECTIONS_TESTS4}
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/prTvImageToGenericRSOutputParametersOutput.txt
  ${TEMP}/prTvImageToGenericRSOutputParametersOutput.txt
  otbImageToGenericRSOutputParameters
  ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
  ${TEMP}/prTvImageToGenericRSOutputParametersOutput.txt
)
ENDIF(OTB_DATA_USE_LARGEINPUT)

#----- otb::RationalTransform ------------------------
ADD_TEST(prTuRationalTransformNew ${PROJECTIONS_TESTS4}
otbRationalTransformNew)

ADD_TEST(prTvRationalTransform ${PROJECTIONS_TESTS4}
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/otbRationalTransformOutput.txt
  ${TEMP}/otbRationalTransformOutput.txt
otbRationalTransform
${TEMP}/otbRationalTransformOutput.txt
0 0
1 1
10 10
-10 -10
)

ADD_TEST(prTvRationalTransformToDeformationFiledSourceTest ${PROJECTIONS_TESTS4}
--compare-image ${EPSILON_9}  ${BASELINE}/prTvRationalTransformToDeformationFiledSourceTest.hdr
                        ${TEMP}/prTvRationalTransformToDeformationFiledSourceTest.hdr
otbRationalTransformToDeformationFiledSourceTest
${TEMP}/prTvRationalTransformToDeformationFiledSourceTest.hdr
)

#----- otb::GeographicalDistance ---------------------
ADD_TEST(prTuGeographicalDistanceNew ${PROJECTIONS_TESTS4}
  otbGeographicalDistanceNew)

ADD_TEST(prTvGeographicalDistance ${PROJECTIONS_TESTS4}
  otbGeographicalDistance
  -1.84944 53.14722
   0.14056 52.20444
   170254.85084
  ${EPSILON_4}
)


#----- otb::VectorDataTransformFilter ---------------------
ADD_TEST(prTuVectorDataTransformFilterNew ${PROJECTIONS_TESTS4}
  otbVectorDataTransformFilterNew)

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(prTvVectorDataTransformFilter ${PROJECTIONS_TESTS4}
  --compare-ogr ${EPSILON_4}
  ${BASELINE_FILES}/prTvVectorDataTransformFilter_Translation.shp
   ${TEMP}/prTvVectorDataTransformFilter_Translation.shp
 otbVectorDataTransformFilter
  ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
  ${INPUTDATA}/ToulouseRoad-examples.shp
  ${TEMP}/prTvVectorDataTransformFilter_Translation.shp
)
ENDIF(OTB_DATA_USE_LARGEINPUT)

#---- Generic tests on GenericRSTransform -----
ADD_TEST(prTvGenericRSTransformGenericTestWGS84toEPSG3857 ${PROJECTIONS_TESTS4}
  otbGenericRSTransformGenericTest
  1.48353 43.55968 165145.804154 5397552.58726 EPSG 4326 EPSG 3857 GEO 1e-3 PHYSICAL 1e-3 NOELEV)

ADD_TEST(prTvGenericRSTransformGenericTestEPSG4326toEPSG4326 ${PROJECTIONS_TESTS4}
  otbGenericRSTransformGenericTest
  1.35617289802566 43.4876035537 1.35617289802566 43.4876035537 EPSG 4326 EPSG 4326 GEO 1e-6 GEO 1e-6 NOELEV)

ADD_TEST(prTvGenericRSTransformGenericTestEPSG4326toEPSG32631 ${PROJECTIONS_TESTS4}
  otbGenericRSTransformGenericTest
  1.3561728980 43.4876035537 367077.72312001337 4816277.086791252 EPSG 4326 EPSG 32631 GEO 1e-3 PHYSICAL 1e-3 NOELEV)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbPROJECTIONS_TESTS5 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
IF(NOT OTB_TESTING_STANDALONE)

ADD_TEST(prTvossimElevManagerTest ${PROJECTIONS_TESTS5}
--compare-image ${EPSILON_9}  ${BASELINE}/prTvossimElevManagerTest.tif
                        ${TEMP}/prTvossimElevManagerTest.tif
        otbOssimElevManagerTest
         ${INPUTDATA}/DEM/srtm_directory
         ${TEMP}/prTvossimElevManagerTest.tif
        6.5
        45.5
        0.002
        -0.002
        500
        500
        )

ADD_TEST(prTvossimElevManagerTest3 ${PROJECTIONS_TESTS5}
--compare-image ${EPSILON_9}  ${BASELINE}/prTvossimElevManagerTest3.tif
                        ${TEMP}/prTvossimElevManagerTest3.tif
        otbOssimElevManagerTest
         ${INPUTDATA}/DEM/srtm_directory
         ${TEMP}/prTvossimElevManagerTest3.tif
         -1.8 52   0.02 -0.018 232 422
#         -1.8870830 52.0135758  0.024363378967 -0.018457085039 232 422
        )

#This test investigate the problem with the ossimElevManager
# - differences between plaforms
# - file descriptor leak
ADD_TEST(prTvossimElevManagerTest4 ${PROJECTIONS_TESTS5}
--compare-image ${EPSILON_9}  ${BASELINE}/prTvossimElevManagerTest3.tif
                        ${TEMP}/prTvossimElevManagerTest4.hdr
        otbOssimElevManagerTest4
         ${INPUTDATA}/DEM/srtm_directory
         ${TEMP}/prTvossimElevManagerTest4
         -1.8 52   0.02 -0.018 232 422
        )


ADD_TEST(prTvossimElevManagerTest2 ${PROJECTIONS_TESTS5}
--compare-ascii ${EPSILON_9}  ${BASELINE_FILES}/prTvossimElevManagerTest2.txt
                        ${TEMP}/prTvossimElevManagerTest2.txt
        otbOssimElevManagerTest2
         ${INPUTDATA}/DEM/srtm_directory
         ${TEMP}/prTvossimElevManagerTest2.txt
        6.5
        44.5
        0.002
        500
         )

IF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(prTvTestCreateProjectionWithOSSIM_Toulouse ${PROJECTIONS_TESTS5}
        otbCreateProjectionWithOSSIM
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        )

ADD_TEST(prTvTestCreateProjectionWithOSSIM_Cevennes ${PROJECTIONS_TESTS5}
        otbCreateProjectionWithOSSIM
        ${LARGEINPUT}/QUICKBIRD/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
        )

ADD_TEST(prTvTestCreateProjectionWithOTB_Toulouse ${PROJECTIONS_TESTS5}
        otbCreateProjectionWithOTB
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        )

ADD_TEST(prTvTestCreateProjectionWithOTB_Cevennes ${PROJECTIONS_TESTS5}
        otbCreateProjectionWithOTB
        ${LARGEINPUT}/QUICKBIRD/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
        )

ADD_TEST(prTvTileImageFilterGenericRSTransformTest_TLSE_DIMAPv2 ${PROJECTIONS_TESTS5}
         otbTileImageFilterRSTransformTest
         1 3
         ${LARGEINPUT}/PLEIADES/TLSE_JP2_DIMAPv2_PRIMARY_PMS_lossless_12bits/IMGPHR_201222215194743808/IMG_PHR1A_PMS_201201151100183_SEN_IPU_20120222_0901-001_R1C1.JP2
         ${LARGEINPUT}/PLEIADES/TLSE_JP2_DIMAPv2_PRIMARY_PMS_lossless_12bits/IMGPHR_201222215194743808/IMG_PHR1A_PMS_201201151100183_SEN_IPU_20120222_0901-001_R2C1.JP2
         ${LARGEINPUT}/PLEIADES/TLSE_JP2_DIMAPv2_PRIMARY_PMS_lossless_12bits/IMGPHR_201222215194743808/IMG_PHR1A_PMS_201201151100183_SEN_IPU_20120222_0901-001_R3C1.JP2
)

ADD_TEST(prTvTileImageFilterGenericRSTransformTest_LASVEGAS_DIMAPv1 ${PROJECTIONS_TESTS5}
         otbTileImageFilterRSTransformTest
         1 3
         ${LARGEINPUT}/PLEIADES/LASVEGAS_JP2_DIMAPv1_PRIMARY_PMS_lossless_12bits/IMGPHR_PHR1A_SENSOR_20120217_IPU_20120217_5374-001_V1/IMG_R1C1.JP2
         ${LARGEINPUT}/PLEIADES/LASVEGAS_JP2_DIMAPv1_PRIMARY_PMS_lossless_12bits/IMGPHR_PHR1A_SENSOR_20120217_IPU_20120217_5374-001_V1/IMG_R2C1.JP2
         ${LARGEINPUT}/PLEIADES/LASVEGAS_JP2_DIMAPv1_PRIMARY_PMS_lossless_12bits/IMGPHR_PHR1A_SENSOR_20120217_IPU_20120217_5374-001_V1/IMG_R3C1.JP2
)

ENDIF(OTB_DATA_USE_LARGEINPUT)

ENDIF(NOT OTB_TESTING_STANDALONE)


#=======================================================================================
SET(Projections_SRCS1
otbProjectionsTests1.cxx
otbProjectionBaseNew.cxx
otbMapProjectionsNew.cxx
otbMapProjection.cxx
otbUtmMapProjection.cxx
otbSensorModelsNew.cxx
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
otbGCPsToRPCSensorModelImageFilterCheckRpcModel.cxx
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
otbGenericRSTransformFromImage.cxx
otbVectorDataProjectionFilterNew.cxx
otbVectorDataProjectionFilter.cxx
otbVectorDataProjectionFilterFromMapToSensor.cxx
otbVectorDataProjectionFilterFromMapToGeo.cxx
otbVectorDataProjectionFilterFromMapToImage.cxx
otbVectorDataProjectionFilterFromGeoToMap.cxx
otbVectorDataIntoImageProjectionFilterTest.cxx
otbGeocentricTransformNew.cxx
otbGeocentricTransform.cxx
otbTileMapTransform.cxx
otbVectorDataExtractROIandProjection.cxx
otbGenericRSTransformGenericConversionCheckingFromGCP.cxx
otbForwardBackwardProjectionConsistency.cxx
otbGeometriesProjectionFilter.cxx
otbGeometriesProjectionFilterFromMapToSensor.cxx
otbGeometriesProjectionFilterFromMapToGeo.cxx
otbGeometriesProjectionFilterFromMapToImage.cxx
otbGeometriesProjectionFilterFromGeoToMap.cxx
otbGeometriesProjectionFilterFromMapToEPSG.cxx
)

SET(Projections_SRCS4
otbProjectionsTests4.cxx
otbPhysicalToRPCSensorModelImageFilter.cxx
otbGenericRSResampleImageFilter.cxx
otbImageToEnvelopeVectorDataFilter.cxx
otbImageToGenericRSOutputParameters.cxx
otbRationalTransform.cxx
otbRationalTransformToDeformationFieldSource.cxx
otbGeographicalDistance.cxx
otbVectorDataTransformFilter.cxx
otbGenericRSTransformGenericTest.cxx
)

#Tests requiring access to ossim header
#should not be launched when using an installed version of OTB
IF(NOT OTB_TESTING_STANDALONE)
SET(Projections_SRCS5
otbProjectionsTests5.cxx
otbOssimElevManagerTest.cxx
otbOssimElevManagerTest2.cxx
otbOssimElevManagerTest4.cxx
otbCreateProjectionWithOSSIM.cxx
otbCreateProjectionWithOTB.cxx
otbTileImageFilterRSTransformTest.cxx
)
ENDIF(NOT OTB_TESTING_STANDALONE)

IF(OTB_USE_EXTERNAL_OSSIM)
  INCLUDE_DIRECTORIES( ${OSSIM_INCLUDE_DIR} )
ELSE(OTB_USE_EXTERNAL_OSSIM)
  INCLUDE_DIRECTORIES( ${OTB_SOURCE_DIR}/Utilities/otbossim/include
                       ${OTB_BINARY_DIR}/Utilities/otbossim/include )
ENDIF(OTB_USE_EXTERNAL_OSSIM)

OTB_ADD_EXECUTABLE(otbProjectionsTests1 "${Projections_SRCS1}" "OTBProjections;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbProjectionsTests2 "${Projections_SRCS2}" "OTBProjections;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbProjectionsTests3 "${Projections_SRCS3}" "OTBProjections;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbProjectionsTests4 "${Projections_SRCS4}" "OTBProjections;OTBIO;OTBTesting")
IF(NOT OTB_TESTING_STANDALONE)
OTB_ADD_EXECUTABLE(otbProjectionsTests5 "${Projections_SRCS5}" "OTBProjections;OTBIO;OTBTesting")
ENDIF(NOT OTB_TESTING_STANDALONE)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
