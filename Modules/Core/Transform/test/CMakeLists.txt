#
# Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

otb_module_test()

set(OTBTransformTests
otbTransformTestDriver.cxx
otbGenericRSTransformWithSRID.cxx
otbLogPolarTransformResample.cxx
otbLogPolarTransform.cxx
otbGeocentricTransform.cxx
otbCreateProjectionWithOTB.cxx
otbGenericMapProjection.cxx
otbStreamingWarpImageFilter.cxx
otbInverseLogPolarTransform.cxx
otbInverseLogPolarTransformResample.cxx
otbStreamingResampleImageFilterWithAffineTransform.cxx
otbSarSensorModelAdapterTest.cxx
otbRPCTransformTest.cxx
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

otb_add_test(NAME trTvRPCTransformTest_ikonos_geom COMMAND otbTransformTestDriver
  otbRPCTransformTest
  ${INPUTDATA}/ikonos/ikonos-1.geom # Geom
  ${INPUTDATA}/ikonos/ikonos-1.gcp2 # GCP
  0.04 # GeoTol
  0.1 # ImgTol
  )

otb_add_test(NAME trTvRPCTransformTest_ikonos_product COMMAND otbTransformTestDriver
  otbRPCTransformTest
  LARGEINPUT{IKONOS/BLOSSEVILLE/po_2619900_blu_0000000.tif} # Product
  ${INPUTDATA}/ikonos/ikonos-1.gcp2 # GCP
  0.04 # GeoTol
  0.1 # ImgTol
  )

otb_add_test(NAME trTvRPCTransformTest_quickbird COMMAND otbTransformTestDriver
  otbRPCTransformTest
  ${INPUTDATA}/QB/qb-1.geom # Geom
  ${INPUTDATA}/QB/qb-1.gcp2 # GCP
  0.024 # GeoTol
  0.1 # ImgTol
  )

otb_add_test(NAME trTvRPCTransformTest_spot6 COMMAND otbTransformTestDriver
  otbRPCTransformTest
  ${INPUTDATA}/spot6/spot6-1.geom # Geom
  ${INPUTDATA}/spot6/spot6-1.gcp2 # GCP
  0.06 # GeoTol
  0.1 # ImgTol
  )

otb_add_test(NAME trTvRPCTransformTest_worldview2 COMMAND otbTransformTestDriver
  otbRPCTransformTest
  ${INPUTDATA}/wv2/wv2-1.geom # Geom
  ${INPUTDATA}/wv2/wv2-1.gcp2 # GCP
  0.0185 # GeoTol
  0.1 # ImgTol
  )

otb_add_test(NAME trTvRPCTransformTest_pl_hnord_geom COMMAND otbTransformTestDriver
  otbRPCTransformTest
  ${INPUTDATA}/pleiades/pleiades-1.geom # Geom
  ${INPUTDATA}/pleiades/pleiades-1.gcp2 # GCP
  0.02 # GeoTol
  0.1 # ImgTol
  )

otb_add_test(NAME trTvRPCTransformTest_pl_hnord_product COMMAND otbTransformTestDriver
  otbRPCTransformTest
  LARGEINPUT{PLEIADES/TLSE_JP2_DIMAPv2_PRIMARY_PMS_lossless_12bits/IMGPHR_201222215194743808/IMG_PHR1A_PMS_201201151100183_SEN_IPU_20120222_0901-001_R1C1.JP2} # Product
  ${INPUTDATA}/pleiades/pleiades-1.gcp2 # GCP
  0.02 # GeoTol
  0.1 # ImgTol
  )

otb_add_test(NAME bfTvLogPolarTransformResample COMMAND otbTransformTestDriver
  --compare-image ${EPSILON_7}  ${BASELINE}/bfLogPolarTransformResampleOutput.tif
  ${TEMP}/bfLogPolarTransformResampleOutput.tif
  otbLogPolarTransformResample
  ${INPUTDATA}/circle.raw.hdr
  ${TEMP}/bfLogPolarTransformResampleOutput.tif
  )

otb_add_test(NAME bfTvLogPolarTransform COMMAND otbTransformTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfLogPolarTransformResults.txt
  ${TEMP}/bfLogPolarTransformResults.txt
  otbLogPolarTransform
  1 1
  ${TEMP}/bfLogPolarTransformResults.txt
  5 0 0 0 10 45 0 3.14 3.14 18 1
  )

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

otb_add_test(NAME dmTvStreamingWarpImageFilterEmtpyRegion COMMAND otbTransformTestDriver
                  otbStreamingWarpImageFilterEmptyRegion)

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
otb_add_test(NAME bfTvInverseLogPolarTransformResample COMMAND otbTransformTestDriver
  --compare-image ${NOTOL}  ${BASELINE}/bfInverseLogPolarTransformResampleOutput.tif
  ${TEMP}/bfInverseLogPolarTransformResampleOutput.tif
  otbInverseLogPolarTransformResample
  ${BASELINE}/bfLogPolarTransformResampleOutput.tif
  ${TEMP}/bfInverseLogPolarTransformResampleOutput.tif
  )
otb_add_test(NAME bfTvStreamingResampleImageFilterWithAffineTransform COMMAND otbTransformTestDriver
  otbStreamingResampleImageFilterWithAffineTransform
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
  500
  ${TEMP}/bfTvotbStreamingResampledImageWithAffineTransform.tif
  )

otb_add_test(NAME uaTvSarSensorModelAdapter COMMAND otbTransformTestDriver
  otbSarSensorModelAdapterTest
  ${INPUTDATA}/s1a-iw1-slc-vh-amp_xt.geom
  )

otb_add_test(NAME uaTvSarSensorModelAdapter2 COMMAND otbTransformTestDriver
  otbSarSensorModelAdapterTest
  ${INPUTDATA}/s1a-iw1-slc-vv-20170111_Burst01_amp.geom
  )

if (Boost_UNIT_TEST_FRAMEWORK_FOUND)
  add_executable(otbSarSensorModelTest otbSarSensorModelTest.cxx )
  target_link_libraries(
    otbSarSensorModelTest
    ${OTBTransform-Test_LIBRARIES}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} )

  otb_add_test(NAME prTvSarSensorModelSentinel1_SLC
           COMMAND otbSarSensorModelTest --
           LARGEINPUT{SENTINEL1/S1A_S6_SLC__1SSV_20150619T195043/measurement/s1a-s6-slc-vv-20150619t195043-20150619t195101-006447-00887d-001.tiff} 
           -23.9431732671333
           15.7637929806278
           0)
  
  otb_add_test(NAME prTvSarSensorModelSentinel1_geom
           COMMAND otbSarSensorModelTest --
           LARGEINPUT{SENTINEL1/S1A_S6_SLC__1SSV_20150619T195043/measurement/s1a-s6-slc-vv-20150619t195043-20150619t195101-006447-00887d-001.tiff}?&geom=${INPUTDATA}/sentinel1/sentinel1-2.geom
           -23.9431732671333
           15.7637929806278
           0)

  otb_add_test(NAME prTvSarSensorModelCosmoSkyMed
           COMMAND otbSarSensorModelTest --
           LARGEINPUT{COSMOSKYMED/Toulouse_spotlight/CSKS3_SCS_B_S2_08_HH_RD_SF_20110418180325_20110418180332.h5} 
           1.3736840073413
           43.5726757080402
           0)

#  otb_add_test(NAME prTvSarSensorModelTerraSarX
#           COMMAND otbSarSensorModelTest --
#           LARGEINPUT{TERRASARX/2008-03-10_GrandCanyon_SSC/TSX1_SAR__SSC______SM_S_SRA_20080310T133220_20080310T133228}
#           -111.664738184032
#           36.2692959995967
#           0)

  add_executable(otbSentinel1SensorModelTest otbSentinel1SensorModelTest.cxx )
  target_link_libraries(
    otbSentinel1SensorModelTest
    ${OTBTransform-Test_LIBRARIES}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} )

  #S1 tests
  file(GLOB s1_files ${INPUTDATA}/ossimPlugins/s1/*.xml)
  #Loop over all S1 annotation files
  foreach(entry ${s1_files})
    get_filename_component(name ${entry} NAME_WE)
    otb_add_test( NAME prTvSentinel1SensorModel_${name} COMMAND otbSentinel1SensorModelTest ${entry})
  endforeach()

endif()