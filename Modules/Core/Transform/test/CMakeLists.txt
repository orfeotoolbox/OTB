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
