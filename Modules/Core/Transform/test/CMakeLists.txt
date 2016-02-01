otb_module_test()

set(OTBTransformTests
otbTransformTestDriver.cxx
otbGenericRSTransformWithSRID.cxx
otbCreateInverseForwardSensorModel.cxx
##>otbForwardBackwardProjectionConsistency.cxx
otbGenericRSTransform.cxx
otbCreateProjectionWithOSSIM.cxx
otbLogPolarTransformResample.cxx
otbStreamingWarpImageFilterNew.cxx
otbLogPolarTransform.cxx
otbGenericRSTransformNew.cxx
otbLogPolarTransformNew.cxx
##>otbForwardSensorModelGrid.cxx
##>otbSensorModel.cxx
otbGeocentricTransform.cxx
otbCreateProjectionWithOTB.cxx
##>otbSensorModelGrid.cxx
otbGeocentricTransformNew.cxx
otbGenericMapProjection.cxx
otbStreamingWarpImageFilter.cxx
otbSensorModelsNew.cxx
otbGenericMapProjectionNew.cxx
otbInverseLogPolarTransform.cxx
otbInverseLogPolarTransformNew.cxx
otbInverseLogPolarTransformResample.cxx
otbStreamingResampleImageFilterWithAffineTransform.cxx
)

add_executable(otbTransformTestDriver ${OTBTransformTests})
target_link_libraries(otbTransformTestDriver ${OTBTransform-Test_LIBRARIES})
otb_module_target_label(otbTransformTestDriver)

# Tests Declaration

otb_add_test(NAME prTvGenericMapProjection COMMAND otbTransformTestDriver
  --compare-ascii ${EPSILON_4}
  ${BASELINE_FILES}/prTvGenericMapProjection.txt
  ${TEMP}/prTvGenericMapProjection.txt
  otbGenericMapProjection
  ${TEMP}/prTvGenericMapProjection.txt
  )

otb_add_test(NAME prTvGenericRSTransformWithSRID COMMAND otbTransformTestDriver
  --compare-ascii ${EPSILON_4}
  ${BASELINE_FILES}/prTvGenericRSTransform.txt
  ${TEMP}/prTvGenericRSTransform_WithSRID.txt
  otbGenericRSTransformWithSRID
  1.35617289802566
  43.4876035537
  ${TEMP}/prTvGenericRSTransform_WithSRID.txt
  )

otb_add_test(NAME prTvTestCreateInverseForwardSensorModel_Cevennes COMMAND otbTransformTestDriver
  otbCreateInverseForwardSensorModel
  LARGEINPUT{QUICKBIRD/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF}
  )

otb_add_test(NAME prTvTestCreateInverseForwardSensorModel_Toulouse COMMAND otbTransformTestDriver
  otbCreateInverseForwardSensorModel
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
  )

otb_add_test(NAME prTvGenericRSTransform COMMAND otbTransformTestDriver
  --compare-ascii ${EPSILON_4}
  ${BASELINE_FILES}/prTvGenericRSTransform.txt
  ${TEMP}/prTvGenericRSTransform.txt
  otbGenericRSTransform
  1.35617289802566
  43.4876035537
  ${TEMP}/prTvGenericRSTransform.txt
  )

otb_add_test(NAME prTvTestCreateProjectionWithOSSIM_Cevennes COMMAND otbTransformTestDriver
  otbCreateProjectionWithOSSIM
  LARGEINPUT{QUICKBIRD/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF}
  )

otb_add_test(NAME prTvTestCreateProjectionWithOSSIM_Toulouse COMMAND otbTransformTestDriver
  otbCreateProjectionWithOSSIM
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
  )

otb_add_test(NAME bfTvLogPolarTransformResample COMMAND otbTransformTestDriver
  --compare-image ${EPSILON_7}  ${BASELINE}/bfLogPolarTransformResampleOutput.hdr
  ${TEMP}/bfLogPolarTransformResampleOutput.hdr
  otbLogPolarTransformResample
  ${INPUTDATA}/circle.hdr
  ${TEMP}/bfLogPolarTransformResampleOutput.hdr
  )

otb_add_test(NAME dmTuStreamingWarpImageFilterNew COMMAND otbTransformTestDriver
  otbStreamingWarpImageFilterNew)

otb_add_test(NAME bfTvLogPolarTransform COMMAND otbTransformTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfLogPolarTransformResults.txt
  ${TEMP}/bfLogPolarTransformResults.txt
  otbLogPolarTransform
  1 1
  ${TEMP}/bfLogPolarTransformResults.txt
  5 0 0 0 10 45 0 3.14 3.14 18 1
  )

otb_add_test(NAME prTuGenericRSTransformNew COMMAND otbTransformTestDriver  otbGenericRSTransformNew )

otb_add_test(NAME bfTuLogPolarTransformNew COMMAND otbTransformTestDriver
  otbLogPolarTransformNew)

##>otb_add_test(NAME prTvSensorModelPleiades-JP2 COMMAND otbTransformTestDriver
##>  --ignore-order --compare-ascii ${EPSILON_9}
##>  ${BASELINE_FILES}/prTvSensorModelPleiades.txt  RM
##>  ${TEMP}/prTvSensorModelPleiades_OUT.txt
##>  --ignore-lines-with 2 meters_per_pixel_x meters_per_pixel_y
##>  otbSensorModel
##>  LARGEINPUT{PLEIADES/TLSE_JP2_DIMAPv2_PRIMARY_PMS_lossless_12bits/IMGPHR_201222215194743808/IMG_PHR1A_PMS_201201151100183_SEN_IPU_20120222_0901-001_R2C1.JP2}
##>  ${TEMP}/prTvSensorModelPleiades_OUT.txt
##>  )

##>otb_add_test(NAME prTvSensorModelIkonos COMMAND otbTransformTestDriver
##>  --ignore-order --compare-ascii ${NOTOL}
##>  ${BASELINE_FILES}/prTvSensorModelIkonos.txt    RM
##>  ${TEMP}/prTvSensorModelIkonos.txt
##>  --ignore-lines-with 2 meters_per_pixel_x meters_per_pixel_y
##>  otbSensorModel
##>  LARGEINPUT{IKONOS/BLOSSEVILLE/po_2619900_pan_0000000.tif}
##>  ${TEMP}/prTvSensorModelIkonos.txt
##>  )

##>otb_add_test(NAME prTvSensorModelSPOT5 COMMAND otbTransformTestDriver
##>  --ignore-order --compare-ascii ${NOTOL}
##>  ${BASELINE_FILES}/prTvSensorModelSpot5.txt    RM
##>  ${TEMP}/prTvSensorModelSpot5.txt
##>  --ignore-lines-with 3 support_data.metadata_file meters_per_pixel_x meters_per_pixel_y
##>  otbSensorModel
##>  LARGEINPUT{SPOT5/TEHERAN/IMAGERY.TIF}
##>  ${TEMP}/prTvSensorModelSpot5.txt
##>  )

##>otb_add_test(NAME prTvSensorModelRadarSat1_SARDEGNA COMMAND otbTransformTestDriver
##>  --ignore-order --compare-ascii ${NOTOL}
##>  ${BASELINE_FILES}/prTvSensorModelRadarSat1_SARDEGNA.txt  DISPARU
##>  ${TEMP}/prTvSensorModelRadarSat1_SARDEGNA.txt
##>  otbSensorModel
##>  LARGEINPUT{RADARSAT1/SARDEGNA/scene01/dat_01.001}
##>  ${TEMP}/prTvSensorModelRadarSat1_SARDEGNA.txt
##>  )

##>otb_add_test(NAME prTvSensorModelPleiades-PRE-TIFF COMMAND otbTransformTestDriver
##>  --ignore-order --compare-ascii ${NOTOL}
##>  ${BASELINE_FILES}/prTvSensorModelPleiades-PRE-TIFF.txt  RM
##>  ${TEMP}/prTvSensorModelPleiades-PRE-TIFF_OUT.txt
##>  --ignore-lines-with 2 meters_per_pixel_x meters_per_pixel_y
##>  otbSensorModel
##>  LARGEINPUT{PLEIADES-PRE/TLSE_TIFF_PRIMARY_DIMAPv2_MS_lossly_12bits/IMG_PHR1A_MS_002/IMG_PHR1A_MS_201006181052297_SEN_IPU_20111025_3077-004_R1C1.TIF}
##>  ${TEMP}/prTvSensorModelPleiades-PRE-TIFF_OUT.txt
##>  )

##>otb_add_test(NAME prTvSensorModelTerraSarX_Toronto COMMAND otbTransformTestDriver
##>  --ignore-order --compare-ascii ${NOTOL}
##>  ${BASELINE_FILES}/prTvSensorModelTerraSarX_Toronto.txt  RM
##>  ${TEMP}/prTvSensorModelTerraSarX_Toronto.txt
##>  --ignore-lines-with 3 product_xml_filename meters_per_pixel_x meters_per_pixel_y
##>  otbSensorModel
##>  LARGEINPUT{TERRASARX/TORONTO/TSX1_SAR__SSC/TSX1_SAR__SSC.xml}
##>  ${TEMP}/prTvSensorModelTerraSarX_Toronto.txt
##>  )

##>otb_add_test(NAME prTvSensorModelFORMOSAT2 COMMAND otbTransformTestDriver
##>  --ignore-order --compare-ascii ${NOTOL}
##>  ${BASELINE_FILES}/prTvSensorModelFORMOSAT.txt RM
##>  ${TEMP}/prTvSensorModelFORMOSAT2.txt
##>  --ignore-lines-with 3 support_data.metadata_file meters_per_pixel_x meters_per_pixel_y
##>  otbSensorModel
##>  LARGEINPUT{FORMOSAT/Sudouest_20071013_MS_fmsat/IMAGERY.TIF}
##>  ${TEMP}/prTvSensorModelFORMOSAT2.txt
##>  )

##>otb_add_test(NAME prTvSensorModelAlosPalsar COMMAND otbTransformTestDriver
##>  --ignore-order --compare-ascii ${EPSILON_6}
##>  ${BASELINE_FILES}/prTvSensorModelAlosPalsar.txt RM
##>  ${TEMP}/prTvSensorModelAlosPalsar.txt
##>  otbSensorModel
##>  LARGEINPUT{PALSAR/200801280007/l1data/VOL-ALPSRP037120700-H1.1__A}
##>  ${TEMP}/prTvSensorModelAlosPalsar.txt
##>  )

##>otb_add_test(NAME prTvSensorModelEnvisatAsar COMMAND otbTransformTestDriver
##>  --ignore-order --compare-ascii ${NOTOL}
##>  ${BASELINE_FILES}/prTvSensorModelEnvisatAsar.txt RM
##>  ${TEMP}/prTvSensorModelEnvisatAsar.txt
##>  otbSensorModel
##>  LARGEINPUT{ENVISAT_ASAR_SCENE01/ASA_APS_1PNDPA20030821_7713.N1}
##>  ${TEMP}/prTvSensorModelEnvisatAsar.txt
##>  )

##>otb_add_test(NAME prTvSensorModelRapidEye COMMAND otbTransformTestDriver
##>  --ignore-order --compare-ascii ${NOTOL}
##>  ${BASELINE_FILES}/prTvSensorModelRapidEye.txt  RM
##>  ${TEMP}/prTvSensorModelRapidEye.txt
##>  --ignore-lines-with 2 meters_per_pixel_x meters_per_pixel_y
##>  otbSensorModel
##>  LARGEINPUT{RAPIDEYE/level1B/2008-12-25T005918_RE3_1B-NAC_397971_12345_band3.ntf}
##>  ${TEMP}/prTvSensorModelRapidEye.txt
##>  )

##>otb_add_test(NAME prTvSensorModelQuickbird COMMAND otbTransformTestDriver
##>  --ignore-order --compare-ascii ${NOTOL}
##>  ${BASELINE_FILES}/prTvSensorModelQuickbird.txt RM
##>  ${TEMP}/prTvSensorModelQuickbird.txt
##>  --ignore-lines-with 2 meters_per_pixel_x meters_per_pixel_y
##>  otbSensorModel
##>  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
##>  ${TEMP}/prTvSensorModelQuickbird.txt
##>  )

##>otb_add_test(NAME prTvSensorModelRadarSat1_GOMA COMMAND otbTransformTestDriver
##>  --ignore-order --compare-ascii ${NOTOL}
##>  ${BASELINE_FILES}/prTvSensorModelRadarSat1_GOMA.txt RM
##>  ${TEMP}/prTvSensorModelRadarSat1_GOMA.txt
##>  otbSensorModel
##>  LARGEINPUT{RADARSAT1/GOMA/SCENE01/DAT_01.001}
##>  ${TEMP}/prTvSensorModelRadarSat1_GOMA.txt
##>  )

##>otb_add_test(NAME prTvSensorModelRadarSat2 COMMAND otbTransformTestDriver
##>  --ignore-order --compare-ascii ${NOTOL}
##>  ${BASELINE_FILES}/prTvSensorModelRadarSat2.txt RM
##>  ${TEMP}/prTvSensorModelRadarSat2.txt
##>  --ignore-lines-with 3 product_xml_filename meters_per_pixel_x meters_per_pixel_y
##>  otbSensorModel
##>  LARGEINPUT{RADARSAT2/ALTONA/Fine_Quad-Pol_Dataset/PK6621_DK406_FQ9_20080405_124900_HH_VV_HV_VH_SLC_Altona/product.xml}
##>  ${TEMP}/prTvSensorModelRadarSat2.txt
##>  )

##>otb_add_test(NAME prTvSensorModelTerraSarX COMMAND otbTransformTestDriver
##>  --ignore-order --compare-ascii ${NOTOL}
##>  ${BASELINE_FILES}/prTvSensorModelTerraSarX.txt  RM
##>  ${TEMP}/prTvSensorModelTerraSarX.txt
##>  --ignore-lines-with 3 product_xml_filename meters_per_pixel_x meters_per_pixel_y
##>  otbSensorModel
##>  LARGEINPUT{TERRASARX/dims/TSX-1.SAR.L1B/TSX1_SAR__SSC/TSX1_SAR__SSC.xml}
##>  ${TEMP}/prTvSensorModelTerraSarX.txt
##>  )

##>otb_add_test(NAME prTvSensorModelTerraSarX_Upsala COMMAND otbTransformTestDriver
##>  --ignore-order --compare-ascii ${NOTOL}
##>  ${BASELINE_FILES}/prTvSensorModelTerraSarX_Upsala.txt  RM
##>  ${TEMP}/prTvSensorModelTerraSarX_Upsala.txt
##>  --ignore-lines-with 3 product_xml_filename meters_per_pixel_x meters_per_pixel_y
##>  otbSensorModel
##>  LARGEINPUT{TERRASARX/UPSALA_GLACIER/TSX1_SAR__MGD/TSX1_SAR__MGD.xml}
##>  ${TEMP}/prTvSensorModelTerraSarX_Upsala.txt
##>  )

##>otb_add_test(NAME prTvSensorModelErs COMMAND otbTransformTestDriver
##>  --ignore-order --compare-ascii ${NOTOL}
##>  ${BASELINE_FILES}/prTvSensorModelErs.txt RM
##>  ${TEMP}/prTvSensorModelErs.txt
##>  otbSensorModel
##>  LARGEINPUT{SAR_ERS2_SLCI_SCENE1/DAT_01.001}
##>  ${TEMP}/prTvSensorModelErs.txt
##>  )

otb_add_test(NAME prTvGeocentricTransform COMMAND otbTransformTestDriver
  --compare-ascii ${EPSILON_4} ${BASELINE_FILES}/prTvGeocentricTransform.txt
  ${TEMP}/prTvGeocentricTransform.txt
  otbGeocentricTransform
  ${TEMP}/prTvGeocentricTransform.txt
  )

otb_add_test(NAME prTvTestCreateProjectionWithOTB_Toulouse COMMAND otbTransformTestDriver
  otbCreateProjectionWithOTB
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
  )

otb_add_test(NAME prTvTestCreateProjectionWithOTB_Cevennes COMMAND otbTransformTestDriver
  otbCreateProjectionWithOTB
  LARGEINPUT{QUICKBIRD/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF}
  )

otb_add_test(NAME prTuGeocentricTransformNew COMMAND otbTransformTestDriver  otbGeocentricTransformNew )

otb_add_test(NAME dmTvStreamingWarpImageFilter COMMAND otbTransformTestDriver
  --compare-image ${EPSILON_10}
  ${BASELINE}/dmStreamingWarpImageFilterOutput.tif
  ${TEMP}/dmStreamingWarpImageFilterOutput.tif
  otbStreamingWarpImageFilter
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub.tif
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub_deformation_field.tif
  ${TEMP}/dmStreamingWarpImageFilterOutput.tif
  5
  )

otb_add_test(NAME prTuSensorModelsNew COMMAND otbTransformTestDriver  otbSensorModelsNew )

otb_add_test(NAME prTuGenericMapProjectionNew COMMAND otbTransformTestDriver  otbGenericMapProjectionNew )

# Forward / Backward projection consistency checking
set(FWDBWDChecking_INPUTS
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
  #LARGEINPUT{SPOT4/RIO_DE_JANEIRO/IMAG_01.DAT}
  LARGEINPUT{SPOT5/TEHERAN/IMAGERY.TIF}
  LARGEINPUT{RADARSAT1/GOMA2/SCENE01/DAT_01.001}
  LARGEINPUT{RADARSAT2/ALTONA/Fine_Quad-Pol_Dataset/PK6621_DK406_FQ9_20080405_124900_HH_VV_HV_VH_SLC_Altona/imagery_HH.tif}
  LARGEINPUT{FORMOSAT/Sudouest_20071013_MS_fmsat/IMAGERY.TIF}
  #LARGEINPUT{GEOEYE/LES_ROCHES/po_350134_bgrn_0000000.tif}
  #LARGEINPUT{IKONOS/BLOSSEVILLE/po_2619900_pan_0000000.tif}
  #LARGEINPUT{PALSAR/ALOS-H1_1__A-ORBIT__ALPSRP037120700.tif}
  #LARGEINPUT{WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_PAN/09DEC10103019-P2AS-052298844010_01_P001.TIF}
  LARGEINPUT{TERRASARX/2008-03-10_GrandCanyon_SSC/TSX1_SAR__SSC______SM_S_SRA_20080310T133220_20080310T133228/IMAGEDATA/IMAGE_HH_SRA_strip_011.cos}
  LARGEINPUT{ENVISAT_ASAR_SCENE01/ASA_APS_1PNDPA20030821_7713.N1}
  #LARGEINPUT{COSMOSKYMED/Toulouse_spotlight/CSKS3_GTC_B_S2_08_HH_RD_SF_20110418180325_20110418180332.h5}
  #LARGEINPUT{MODIS/MOD13Q1/MOD13Q1.A2010001.h17v05.005.2010028003734.hdf}
  #LARGEINPUT{ONERA/spa3_0215_rad.dat}
  #LARGEINPUT{RAPIDEYE/level3A/2008-12-25T005918_RE3_3A-NAC_752656_40091.tif}
  )

##>set(FWDBWDChecking_TYPES
  ##>QUICKBIRD
  ###SPOT4
  ##>SPOT5
  ##>RADARSAT1
  ##>RADARSAT2
  ##>FORMOSAT
  ###GEOEYE
  ###IKONOS
  ###PALSAR
  ###WORLDVIEW2
  ##>TERRASARX
  ##>ENVISAT
  ###COSMOSKYMED
  ###MODIS
  ###ONERA
  ###RAPIDEYE
  ##>)
##>set( FWDBWDChecking_TESTNB 0)

##>foreach( file ${FWDBWDChecking_INPUTS} )
  ##>list(GET FWDBWDChecking_TYPES   ${FWDBWDChecking_TESTNB} current_type   )
  ##>math(EXPR FWDBWDChecking_TESTNB "${FWDBWDChecking_TESTNB} + 1")
  
  ##>otb_add_test(NAME prTvForwardBackwardProjectionConsistency_${current_type} COMMAND otbTransformTestDriver
    ##>otbForwardBackwardProjectionConsistency
      ##>${file}
      ##>${INPUTDATA}/DEM/srtm_directory/
      ##>${INPUTDATA}/DEM/egm96.grd #Not used
      ##>20 # nb Tested Points
      ##>1  # Tolerance in pixels
      ##>1  # Verbose
  ##>)
##>endforeach()




##>set(SARSENSORMODEL_TYPES
  ##>"RADARSAT1/GOMA/SCENE01/"
  ##>"RADARSAT2/ALTONA/Fine_Quad-Pol_Dataset/PK6621_DK406_FQ9_20080405_124900_HH_VV_HV_VH_SLC_Altona/product.xml"
  ##>"ENVISAT_ASAR_SCENE01/ASA_APS_1PNDPA20030821_7713.N1"
  ##>"PALSAR/200801280007/l1data/VOL-ALPSRP037120700-H1.1__A"
  ##>"TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_HH_SRA_stripFar_008.cos"
##>)

##>foreach( file ${SARSENSORMODEL_TYPES})
  ##># Get the sensor name
  ##>set(sharp_regexp "([0-9A-Za-z_]*)[ ]*/[ ]*(.*)")
  ##>string(REGEX REPLACE "${sharp_regexp}" "\\1" sensor_name "${file}")
  ##># Tests
  ##>otb_add_test(NAME prTvSensorModelGrid_${sensor_name} COMMAND otbTransformTestDriver
    ##>--compare-n-images ${EPSILON_6} 2
           ##>${BASELINE}/prTvSensorModelGridDeltaX_${sensor_name}.tif
           ##>${TEMP}/prTvSensorModelGridDeltaX_${sensor_name}.tif
           ##>${BASELINE}/prTvSensorModelGridDeltaY_${sensor_name}.tif
           ##>${TEMP}/prTvSensorModelGridDeltaY_${sensor_name}.tif
      ##>otbSensorModelGrid
           ##>LARGEINPUT{${file}}
           ##>${TEMP}/prTvSensorModelGridDeltaX_${sensor_name}.tif
           ##>${TEMP}/prTvSensorModelGridDeltaY_${sensor_name}.tif
           ##>10 10
          ##>)

  ##>otb_add_test(NAME prTvForwardSensorModelGrid_${sensor_name} COMMAND otbTransformTestDriver
    ##>--compare-n-images ${EPSILON_6} 2
           ##>${BASELINE}/prTvForwardSensorModelGridX_${sensor_name}.tif
           ##>${TEMP}/prTvForwardSensorModelGridX_${sensor_name}.tif
           ##>${BASELINE}/prTvForwardSensorModelGridY_${sensor_name}.tif
           ##>${TEMP}/prTvForwardSensorModelGridY_${sensor_name}.tif
      ##>otbForwardSensorModelGrid
           ##>LARGEINPUT{${file}}
           ##>${TEMP}/prTvForwardSensorModelGridX_${sensor_name}.tif
           ##>${TEMP}/prTvForwardSensorModelGridY_${sensor_name}.tif
           ##>10 10
          ##>)
##>endforeach()


otb_add_test(NAME bfTvInverseLogPolarTransform COMMAND otbTransformTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfInverseLogPolarTransformResults.txt
  ${TEMP}/bfInverseLogPolarTransformResults.txt
  otbInverseLogPolarTransform
  1. 1.
  ${TEMP}/bfInverseLogPolarTransformResults.txt
  6 0 0 10 20 45 60 3.14 3.14 1 0 0 1
  )
otb_add_test(NAME bfTuInverseLogPolarTransformNew COMMAND otbTransformTestDriver
  otbInverseLogPolarTransformNew)
otb_add_test(NAME bfTvInverseLogPolarTransformResample COMMAND otbTransformTestDriver
  --compare-image ${NOTOL}  ${BASELINE}/bfInverseLogPolarTransformResampleOutput.hdr
  ${TEMP}/bfInverseLogPolarTransformResampleOutput.hdr
  otbInverseLogPolarTransformResample
  ${BASELINE}/bfLogPolarTransformResampleOutput.hdr
  ${TEMP}/bfInverseLogPolarTransformResampleOutput.hdr
  )
otb_add_test(NAME bfTvStreamingResampleImageFilterWithAffineTransform COMMAND otbTransformTestDriver
  otbStreamingResampleImageFilterWithAffineTransform
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
  500
  ${TEMP}/bfTvotbStreamingResampledImageWithAffineTransform.tif
  )
otb_add_test(NAME bfTuStreamingResampleImageFilterWithAffineTransformNew COMMAND otbTransformTestDriver
  otbStreamingResampleImageFilterWithAffineTransform
  )
