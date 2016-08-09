otb_module_test()

set(OTBOSSIMAdaptersTests
otbOSSIMAdaptersTestDriver.cxx
otbTestImageKeywordlist.cxx
otbOssimJpegFileRessourceLeakTest.cxx
otbMapProjectionAdapterTest.cxx
otbOssimElevManagerTest2.cxx
otbOssimElevManagerTest4.cxx
otbGeometricSarSensorModelAdapter.cxx
otbPlatformPositionAdapter.cxx
otbDEMHandlerTest.cxx
otbRPCSolverAdapterTest.cxx
)

add_executable(otbOSSIMAdaptersTestDriver ${OTBOSSIMAdaptersTests})
target_link_libraries(otbOSSIMAdaptersTestDriver ${OTBOSSIMAdapters-Test_LIBRARIES})
otb_module_target_label(otbOSSIMAdaptersTestDriver)

# Tests Declaration

#KEYWORDLIST : OPTICAL SENSORS
set(NAME_TESTS
"pleiades-1"
"ikonos-1"
"rapideye-1"
"quickbird-1"
"spot6-1"
"worldview2-1"
"spot5-1"
"geoeye1-1"
)

set(IMG
LARGEINPUT{PLEIADES/TLSE_JP2_DIMAPv2_PRIMARY_PMS_lossless_12bits/IMGPHR_201222215194743808/IMG_PHR1A_PMS_201201151100183_SEN_IPU_20120222_0901-001_R1C1.JP2}
LARGEINPUT{IKONOS/BLOSSEVILLE/po_2619900_nir_0000000.tif}
LARGEINPUT{RAPIDEYE/level1B/2008-12-25T005918_RE3_1B-NAC_397971_12345_band3.ntf}
LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
LARGEINPUT{SPOT6/600143101-Primary-Bundle-JP2-LOSSLESS/PROD_SPOT6_001/VOL_SPOT6_001_A/IMG_SPOT6_MS_001_A/IMG_SPOT6_MS_201212071020271_SEN_600143101_R1C1.JP2}
LARGEINPUT{WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_PAN/09DEC10103019-P2AS-052298844010_01_P001.TIF}
LARGEINPUT{SPOT5/TEHERAN/IMAGERY.TIF}
LARGEINPUT{GEOEYE/MARCILLOLES/po_350134_bgrn_0000001.tif}
)

set(NEEDEDKW
"line_den_coeff line_num_coeff samp_den_coeff samp_num_coeff support_data.physical_gain support_data.physical_bias support_data.solar_irradiance support_data.image_date support_data.elevation_angle" #PLEIADES
"line_den_coeff line_num_coeff samp_den_coeff samp_num_coeff support_data.physical_gain support_data.physical_bias support_data.solar_irradiance support_data.image_date support_data.elevation_angle" #IKONOS
"line_den_coeff line_num_coeff samp_den_coeff samp_num_coeff support_data.physical_gain support_data.physical_bias support_data.solar_irradiance support_data.image_date support_data.elevation_angle" #RAPIDEYE
"line_den_coeff line_num_coeff samp_den_coeff samp_num_coeff support_data.physical_gain support_data.physical_bias support_data.solar_irradiance support_data.image_date support_data.elevation_angle" #QB
"line_den_coeff line_num_coeff samp_den_coeff samp_num_coeff support_data.physical_gain support_data.physical_bias support_data.solar_irradiance support_data.image_date support_data.elevation_angle" #SPOT6
"line_den_coeff line_num_coeff samp_den_coeff samp_num_coeff support_data.physical_gain support_data.physical_bias support_data.solar_irradiance support_data.image_date support_data.elevation_angle" #WORLDVIEW2
"line_den_coeff line_num_coeff samp_den_coeff samp_num_coeff support_data.physical_gain support_data.physical_bias support_data.solar_irradiance support_data.image_date support_data.elevation_angle" #SPOT5
"line_den_coeff line_num_coeff samp_den_coeff samp_num_coeff support_data.physical_gain support_data.physical_bias support_data.solar_irradiance support_data.image_date support_data.elevation_angle" #geoeye1
)

set(TOLS
"1e-6 1e-6 1e-6 1e-6 1e-6 1e-6 1e-6 1e-6 1e-6" #PLEIADES
"1e-6 1e-6 1e-6 1e-6 1e-6 1e-6 1e-6 1e-6 1e-6" #IKONOS
"1e-6 1e-6 1e-6 1e-6 1e-6 1e-6 1e-6 1e-6 1e-6" #RAPIDEYE
"1e-6 1e-6 1e-6 1e-6 1e-6 1e-6 1e-6 1e-6 1e-6" #QB
"1e-6 1e-6 1e-6 1e-6 1e-6 1e-6 1e-6 1e-6 1e-6" #SPOT6
"1e-6 1e-6 1e-6 1e-6 1e-6 1e-6 1e-6 1e-6 1e-6" #WORLDVIEW2
"1e-6 1e-6 1e-6 1e-6 1e-6 1e-6 1e-6 1e-6 1e-6" #SPOT5
"1e-6 1e-6 1e-6 1e-6 1e-6 1e-6 1e-6 1e-6 1e-6" #geoeye1
)

set(TESTNB 0 )
foreach( file ${NAME_TESTS} )
  list(GET IMG   ${TESTNB} current_img   )
  list(GET NEEDEDKW   ${TESTNB} current_neededkw   )
  list(GET TOLS   ${TESTNB} current_tols   )
  list(GET NAME_TESTS   ${TESTNB} current_name   )
  math(EXPR TESTNB "${TESTNB} + 1")

  otb_add_test(NAME ioTvTestImageKeywordlist_${current_name} COMMAND otbOSSIMAdaptersTestDriver
             otbTestImageKeywordlist
             ${current_img}
             ${current_neededkw}
             ${current_tols}
             ${current_name}.geom
             0 # 1 : Check all keywords (ignore argv[2] and argv[3]) / 0 : Only check the needed keywords
            )

endforeach()

#KEYWORDLIST : RADAR SENSORS
set(NAME_TESTS
"radarsat2-1"
"terrasarx-1"
"sentinel1-1"
"envisat-asar-1"
"radarsat1-1"
"ers2-1"
)

set(IMG
LARGEINPUT{RADARSAT2/ALTONA/Fine_Quad-Pol_Dataset/PK6621_DK406_FQ9_20080405_124900_HH_VV_HV_VH_SLC_Altona/imagery_VH.tif}
LARGEINPUT{TERRASARX/2008-03-21_GrandCanyon_SSC/TSX1_SAR__SSC______SM_S_SRA_20080321T133220_20080321T133228/IMAGEDATA/IMAGE_HH_SRA_strip_011.cos}
LARGEINPUT{SENTINEL1/S1A_S6_SLC__1SSV_20150619T195043/measurement/s1a-s6-slc-vv-20150619t195043-20150619t195101-006447-00887d-001.tiff}
LARGEINPUT{ENVISAT_ASAR_SCENE01/ASA_APS_1PNDPA20030821_7713.N1}
LARGEINPUT{RADARSAT1/SARDEGNA/scene01/dat_01.001}
LARGEINPUT{SAR_ERS2_SLCI_SCENE1/DAT_01.001}
)

set(TESTNB 0 )
foreach( file ${NAME_TESTS} )
  list(GET IMG   ${TESTNB} current_img   )
  list(GET NAME_TESTS   ${TESTNB} current_name   )
  math(EXPR TESTNB "${TESTNB} + 1")

  otb_add_test(NAME ioTvTestImageKeywordlist_${current_name} COMMAND otbOSSIMAdaptersTestDriver
             otbTestImageKeywordlist
             ${current_img}
             "Foo"
             "Foo"
             ${current_name}.geom
             1 # 1 : Check all keywords (ignore argv[2] and argv[3]) / 0 : Only check the needed keywords
            )

endforeach()


otb_add_test(NAME ioTvOssimJpegFileRessourceLeak COMMAND otbOSSIMAdaptersTestDriver
  otbOssimJpegFileRessourceLeakTest
  ${INPUTDATA}/poupees.jpg
  1200
  )

otb_add_test(NAME ioTvMapProjectionAdapterTest COMMAND otbOSSIMAdaptersTestDriver
  --compare-ascii ${EPSILON_9}
  ${BASELINE_FILES}/ioTvMapProjectionAdapterTest.txt
  ${TEMP}/ioTvMapProjectionAdapterTest.txt
  otbMapProjectionAdapterTest
  ${TEMP}/ioTvMapProjectionAdapterTest.txt
  )

otb_add_test(NAME prTvossimElevManagerTest2 COMMAND otbOSSIMAdaptersTestDriver
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

otb_add_test(NAME prTvossimElevManagerTest4 COMMAND otbOSSIMAdaptersTestDriver
  --compare-image ${EPSILON_9}  ${BASELINE}/prTvossimElevManagerTest4
  ${TEMP}/prTvossimElevManagerTest4.hdr
  otbOssimElevManagerTest4
  ${INPUTDATA}/DEM/srtm_directory
  ${TEMP}/prTvossimElevManagerTest4
  -1.8 52   0.02 -0.018 232 422
  )

otb_add_test(NAME uaTvGeometricSarSensorModelAdapterNew COMMAND otbOSSIMAdaptersTestDriver
  otbGeometricSarSensorModelAdapterNewTest)

if (0)
  # Inhibated for now, these two tests should be changed to use a Sar Product
  # type which is not TerraSar-X
  otb_add_test(NAME uaTvGeometricSarSensorModelAdapterTest COMMAND otbOSSIMAdaptersTestDriver
    --compare-ascii ${EPSILON_9}
    ${BASELINE_FILES}/uaTvGeometricSarSensorModelAdapterTest.txt
    ${TEMP}/uaTvGeometricSarSensorModelAdapterTest.txt
    otbGeometricSarSensorModelAdapterTest
    LARGEINPUT{TERRASARX/2008-03-10_GrandCanyon_SSC/TSX1_SAR__SSC______SM_S_SRA_20080310T133220_20080310T133228/TSX1_SAR__SSC______SM_S_SRA_20080310T133220_20080310T133228.xml}
    ${TEMP}/uaTvGeometricSarSensorModelAdapterTest.txt)

otb_add_test(NAME uaTvPlatformPositionComputeBaselineTest COMMAND otbOSSIMAdaptersTestDriver
  --compare-ascii ${EPSILON_9}
  ${BASELINE_FILES}/uaTvPlatformPositionComputeBaselineTest.txt
  ${TEMP}/uaTvPlatformPositionComputeBaselineTest.txt
  otbPlatformPositionComputeBaselineTest
  LARGEINPUT{TERRASARX/2008-03-10_GrandCanyon_SSC/TSX1_SAR__SSC______SM_S_SRA_20080310T133220_20080310T133228/TSX1_SAR__SSC______SM_S_SRA_20080310T133220_20080310T133228.xml}
  ${TEMP}/uaTvPlatformPositionComputeBaselineTest.txt)
endif()

otb_add_test(NAME uaTvPlatformPositionComputeBaselineNew COMMAND otbOSSIMAdaptersTestDriver
  otbPlatformPositionComputeBaselineNewTest)

otb_add_test(NAME uaTvDEMHandler_AboveEllipsoid_NoSRTM_NoGeoid_NoData COMMAND otbOSSIMAdaptersTestDriver
  otbDEMHandlerTest
  no
  no
  40
  8.687917
  44.237917
  0
  40
  0.001
  )

otb_add_test(NAME uaTvDEMHandler_AboveMSL_NoSRTM_Geoid COMMAND otbOSSIMAdaptersTestDriver
  otbDEMHandlerTest
  no
  ${INPUTDATA}/DEM/egm96.grd
  40
  8.434583
  44.647083
  1
  0
  0.001
  )

otb_add_test(NAME uaTvDEMHandler_AboveEllipsoid_BadSRTM_Geoid COMMAND otbOSSIMAdaptersTestDriver
  otbDEMHandlerTest
  ${INPUTDATA}/poupeesTIF/
  ${INPUTDATA}/DEM/egm96.grd
  40
  8.434583
  44.647083
  0
  383.580313671
  0.001
  )
set_property(TEST uaTvDEMHandler_AboveEllipsoid_BadSRTM_Geoid PROPERTY WILL_FAIL 1)

otb_add_test(NAME uaTvDEMHandler_AboveMSL_NoSRTM_NoGeoid COMMAND otbOSSIMAdaptersTestDriver
  otbDEMHandlerTest
  no
  no
  40
  8.434583
  44.647083
  1
  0
  0.001
  )

otb_add_test(NAME uaTvDEMHandler_AboveEllipsoid_NoSRTM_Geoid COMMAND otbOSSIMAdaptersTestDriver
  otbDEMHandlerTest
  no
  ${INPUTDATA}/DEM/egm96.grd
  40
  8.434583
  44.647083
  0
  44.067113
  0.001
  )

otb_add_test(NAME uaTvDEMHandler_AboveEllipsoid_SRTM_Geoid COMMAND otbOSSIMAdaptersTestDriver
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  ${INPUTDATA}/DEM/egm96.grd
  40
  8.434583
  44.647083
  0
  383.580313671
  0.001
  )

otb_add_test(NAME uaTvDEMHandler_AboveMSL_SRTM_NoGeoid_NoSRTMCoverage COMMAND otbOSSIMAdaptersTestDriver
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  no
  20
  10. # Point not covered
  10. # Point not covered
  1
  0
  0.001
  )

otb_add_test(NAME uaTvDEMHandler_AboveEllipsoid_SRTM_NoGeoid_NoSRTMCoverage COMMAND otbOSSIMAdaptersTestDriver
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  no
  20
  10. # Point not covered
  10. # Point not covered
  0
  20
  0.001
  )

otb_add_test(NAME uaTvDEMHandler_AboveMSL_SRTM_NoGeoid COMMAND otbOSSIMAdaptersTestDriver
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  no
  40
  8.434583
  44.647083
  1
  339.513
  0.001
  )

otb_add_test(NAME uaTvDEMHandler_AboveEllipsoid_SRTM_NoGeoid COMMAND otbOSSIMAdaptersTestDriver
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  no
  40
  8.434583
  44.647083
  0
  339.513
  0.001
  )

otb_add_test(NAME uaTvDEMHandler_AboveEllipsoid_SRTM_Geoid_NoData COMMAND otbOSSIMAdaptersTestDriver
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  ${INPUTDATA}/DEM/egm96.grd
  40
  8.687917
  44.237917
  0
  45.7464
  0.001
  )

otb_add_test(NAME uaTvDEMHandler_AboveEllipsoid_SRTM_BadGeoid COMMAND otbOSSIMAdaptersTestDriver
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  ${INPUTDATA}/poupees.jpg
  40
  8.434583
  44.647083
  0
  339.513
  0.001
  )
set_property(TEST uaTvDEMHandler_AboveEllipsoid_SRTM_BadGeoid PROPERTY WILL_FAIL true)

otb_add_test(NAME uaTvDEMHandler_AboveMSL_SRTM_NoGeoid_NoData COMMAND otbOSSIMAdaptersTestDriver
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  no
  40
  8.687917
  44.237917
  1
  0
  0.001
  )

otb_add_test(NAME uaTvDEMHandler_AboveEllipsoid_BDALTI_TIF_NoGeoid COMMAND otbOSSIMAdaptersTestDriver
  otbDEMHandlerTest
  LARGEINPUT{BD_ALTI/}
  no
  800
  7.283262
  48.598675
  0
  869
  1
  )

otb_add_test(NAME uaTvDEMHandler_AboveMSL_SRTM_Geoid_NoData COMMAND otbOSSIMAdaptersTestDriver
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  ${INPUTDATA}/DEM/egm96.grd
  40
  8.687917
  44.237917
  1
  0
  0.001
  )

otb_add_test(NAME uaTvDEMHandler_AboveMSL_NoSRTM_NoGeoid_NoData COMMAND otbOSSIMAdaptersTestDriver
  otbDEMHandlerTest
  no
  no
  40
  8.687917
  44.237917
  1
  0
  0.001
  )

otb_add_test(NAME uaTvDEMHandler_AboveMSL_SRTM_Geoid COMMAND otbOSSIMAdaptersTestDriver
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  ${INPUTDATA}/DEM/egm96.grd
  40
  8.434583
  44.647083
  1
  339.513
  0.001
  )

otb_add_test(NAME uaTvDEMHandler_AboveEllipsoid_NoSRTM_NoGeoid COMMAND otbOSSIMAdaptersTestDriver
  otbDEMHandlerTest
  no
  no
  40
  8.434583
  44.647083
  0
  40
  0.001
  )

otb_add_test(NAME uaTvDEMHandler_AboveMSL_SRTM_Geoid_NoSRTMCoverage COMMAND otbOSSIMAdaptersTestDriver
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  ${INPUTDATA}/DEM/egm96.grd
  20
  10. # Point not covered
  10. # Point not covered
  1
  0
  0.001
  )

otb_add_test(NAME uaTvDEMHandler_AboveEllipsoid_SRTM_Geoid_NoSRTMCoverage COMMAND otbOSSIMAdaptersTestDriver
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  ${INPUTDATA}/DEM/egm96.grd
  20
  10. # Point not covered
  10. # Point not covered
  0
  21.569 # Geoid offset
  0.001
  )

otb_add_test(NAME uaTvDEMHandler_AboveEllipsoid_SRTM_NoGeoid_NoData COMMAND otbOSSIMAdaptersTestDriver
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  no
  40
  8.687917
  44.237917
  0
  40
  0.001
  )

otb_add_test(NAME uaTvRPCSolverAdapterNoDEMValidationTest COMMAND otbOSSIMAdaptersTestDriver
  otbRPCSolverAdapterTest
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
  10 0.25 0.35
  no
  no
  )

otb_add_test(NAME uaTvRPCSolverAdapterNotEnoughPointsForElevationTest COMMAND otbOSSIMAdaptersTestDriver
  otbRPCSolverAdapterTest
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
  5 0.25 0.35
  no
  no
  )

otb_add_test(NAME uaTvRPCSolverAdapterNotEnoughPointsTest COMMAND otbOSSIMAdaptersTestDriver
  otbRPCSolverAdapterTest
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
  4 0.25 0.35
  ${INPUTDATA}/DEM/srtm_directory/
  ${INPUTDATA}/DEM/egm96.grd
  )
if (OTB_DATA_USE_LARGEINPUT)
  set_property(TEST uaTvRPCSolverAdapterNotEnoughPointsTest PROPERTY WILL_FAIL TRUE)
endif()

#otb_add_test(NAME uaTvRPCSolverAdapterOutGeomTest COMMAND otbOSSIMAdaptersTestDriver
  #--compare-ascii ${EPSILON_9}
  #${BASELINE_FILES}/uaTvRPCSolverAdapterOutGeomTest.geom
  #${TEMP}/uaTvRPCSolverAdapterOutGeomTest.geom
  #otbRPCSolverAdapterTest
  #LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
  #10 0.25 0.35
  #${INPUTDATA}/DEM/srtm_directory/
  #${INPUTDATA}/DEM/egm96.grd
  #${TEMP}/uaTvRPCSolverAdapterOutGeomTest.geom
  #)

otb_add_test(NAME uaTvRPCSolverAdapterValidationTest COMMAND otbOSSIMAdaptersTestDriver
  otbRPCSolverAdapterTest
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
  10 0.25 0.35
  ${INPUTDATA}/DEM/srtm_directory/
  ${INPUTDATA}/DEM/egm96.grd
  )

