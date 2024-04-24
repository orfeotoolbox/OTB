#
# Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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

set(OTBMetadataTests
otbMetadataTestDriver.cxx
otbImageMetadataInterfaceTest.cxx
otbNoDataHelperTest.cxx
otbSarCalibrationLookupDataTest.cxx
otbImageMetadataTest.cxx
otbGeomMetadataSupplierTest.cxx
otbXMLMetadataSupplierTest.cxx
otbSentinel1ThermalNoiseLutTest.cxx
)

add_executable(otbMetadataTestDriver ${OTBMetadataTests})
target_link_libraries(otbMetadataTestDriver ${OTBMetadata-Test_LIBRARIES})
otb_module_target_label(otbMetadataTestDriver)

# Tests Declaration

otb_add_test(NAME coreMetaDataNoDataHelperTest COMMAND otbMetadataTestDriver
  otbNoDataHelperTest)

otb_add_test(NAME ioTvSarCalibrationLookupDataTest_SENTINEL1 COMMAND otbMetadataTestDriver
  --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvSarCalibrationLookupDataTest_SENTINEL1.txt
  ${TEMP}/ioTvSarCalibrationLookupDataTest_SENTINEL1.txt
  otbSarCalibrationLookupDataTest
  ${INPUTDATA}/SENTINEL1_SLC_S6_1S_extract_1200_1100_300_300.tiff?&geom=${INPUTDATA}/SENTINEL1_SLC_S6_1S_extract_1200_1100_300_300.geom
  ${TEMP}/ioTvSarCalibrationLookupDataTest_SENTINEL1.txt
  )

  otb_add_test(NAME ioTvSarCalibrationLookupDataTest_SENTINEL1_GRD COMMAND otbMetadataTestDriver
  --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvSarCalibrationLookupDataTest_SENTINEL1_GRD.txt
  ${TEMP}/ioTvSarCalibrationLookupDataTest_SENTINEL1_GRD.txt
  otbSarCalibrationLookupDataTest
  ${INPUTDATA}/S1A_IW_GRDH_1SDV_20210202T060035_20210202T060100_036407_0445F3_B8EA_vh_extract.tif?&geom=${INPUTDATA}/S1A_IW_GRDH_1SDV_20210202T060035_20210202T060100_036407_0445F3_B8EA_vh_extract.geom
  ${TEMP}/ioTvSarCalibrationLookupDataTest_SENTINEL1_GRD.txt
  )

otb_add_test(NAME ioTvSarCalibrationLookupDataTest_RADARSAT2 COMMAND otbMetadataTestDriver
  --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvSarCalibrationLookupDataTest_RADARSAT2.txt
  ${TEMP}/ioTvSarCalibrationLookupDataTest_RADARSAT2.txt
  otbSarCalibrationLookupDataTest
  ${INPUTDATA}/radarsat2/RADARSAT2_ALTONA_300_300_VV.tif?&geom=${INPUTDATA}/radarsat2/RADARSAT2_ALTONA_300_300_VV.geom
  ${TEMP}/ioTvSarCalibrationLookupDataTest_RADARSAT2.txt
  )

otb_add_test(NAME ioTuMetaDataKeyTest COMMAND otbMetadataTestDriver 
  --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTuMetaDataKeyTest.txt
  ${TEMP}/ioTuMetaDataKeyTest.txt
  otbImageMetadataTest
  otbMetadataKeyTest
  ${TEMP}/ioTuMetaDataKeyTest.txt
  )
  
otb_add_test(NAME ioTuImageMetadataSliceTest COMMAND otbMetadataTestDriver
  --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTuImageMetadataSliceTest.txt
  ${TEMP}/ioTuImageMetadataSliceTest.txt
  otbImageMetadataTest
  otbImageMetadataSliceTest
  ${TEMP}/ioTuImageMetadataSliceTest.txt
  )
  
otb_add_test(NAME ioTuImageMetadataCompactTest COMMAND otbMetadataTestDriver
  --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTuImageMetadataCompactTest.txt
  ${TEMP}/ioTuImageMetadataCompactTest.txt
  otbImageMetadataTest
  otbImageMetadataCompactTest
  ${TEMP}/ioTuImageMetadataCompactTest.txt
  )

otb_add_test(NAME ioTuImageMetadataGetSizeTest COMMAND otbMetadataTestDriver
  --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTuImageMetadataGetSizeTest.txt
  ${TEMP}/ioTuImageMetadataGetSizeTest.txt
  otbImageMetadataTest
  otbImageMetadataGetSizeTest
  ${TEMP}/ioTuImageMetadataGetSizeTest.txt
  )
  
otb_add_test(NAME ioTuImageMetadataAppendTest COMMAND otbMetadataTestDriver
  --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTuImageMetadataAppendTest.txt
  ${TEMP}/ioTuImageMetadataAppendTest.txt
  otbImageMetadataTest
  otbImageMetadataAppendTest
  ${TEMP}/ioTuImageMetadataAppendTest.txt
  )

otb_add_test(NAME ioTuImageMetadataMergeTest COMMAND otbMetadataTestDriver
  --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTuImageMetadataMergeTest.txt
  ${TEMP}/ioTuImageMetadataMergeTest.txt
  otbImageMetadataTest
  otbImageMetadataMergeTest
  ${TEMP}/ioTuImageMetadataMergeTest.txt
  )

otb_add_test(NAME ioTuImageMetadataToFromKeywordlistTest COMMAND otbMetadataTestDriver
  --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTuImageMetadataToFromKeywordlistTest.txt
  ${TEMP}/ioTuImageMetadataToFromKeywordlistTest.txt
  otbImageMetadataTest
  otbImageMetadataToFromKeywordlistTest
  ${TEMP}/ioTuImageMetadataToFromKeywordlistTest.txt
  )
  
otb_add_test(NAME ioTuotbGeomMetadataSupplierTest COMMAND otbMetadataTestDriver
  --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTuotbGeomMetadataSupplierTest.txt
  ${TEMP}/ioTuotbGeomMetadataSupplierTest.txt
  otbGeomMetadataSupplierTest
  ${INPUTDATA}/QB/qb-1.geom
  ${TEMP}/ioTuotbGeomMetadataSupplierTest.txt
  )
  
otb_add_test(NAME ioTuotbXMLMetadataSupplierTest COMMAND otbMetadataTestDriver
  --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTuotbXMLMetadataSupplierTest.txt
  ${TEMP}/ioTuotbXMLMetadataSupplierTest.txt
  otbXMLMetadataSupplierTest
  ${INPUTDATA}/ioTuotbXMLMetadataSupplierTest.xml
  ${TEMP}/ioTuotbXMLMetadataSupplierTest.txt
  )

otb_add_test(NAME saTvS1ThermalNoiseLutTest COMMAND otbMetadataTestDriver
  otbSentinel1ThermalNoiseLutTest
  ${INPUTDATA}/S1A_IW_GRDH_1SDV_20210202T060035_20210202T060100_036407_0445F3_B8EA_vh_extract.tif?&geom=${INPUTDATA}/S1A_IW_GRDH_1SDV_20210202T060035_20210202T060100_036407_0445F3_B8EA_vh_extract.geom
  659.993737792968772737
  2088.05796390530122152995318174)


# Image metadata interface tests.
# The image metadata are read from a product and dumped into a txt file, which is then compared 
# to a baseline ${BASELINE_FILES}/ioTvImageMetadataInterfaceTest_${sensor}.txt.
# To create a test, add the name of the sensor in "sensor list" and define the ${sensor}_filename variable.
# Additionally, it is possible to compare the metadata of the product with the metadata of a geom file
# with the --compare-metadata utility by setting ${sensor}_geom_file

set(sensor_list pleiades pleiades_2 spot6 spot6_2 spot5 spot5_2 formosat ikonos worldview2 quickbird Radarsat2 TerraSar CosmoSkyMed Sentinel1)

set(pleiades_filename LARGEINPUT{PLEIADES/MAIDO_JP2_DIMAPv2_PRIMARY_MS_lossy_12bits/IMG_PHR1A_MS_002/IMG_PHR1A_MS_201206050630064_SEN_559102101-002_R1C1.JP2})
set(pleiades_2_filename LARGEINPUT{PLEIADES/MAIDO_JP2_DIMAPv2_PRIMARY_MS_lossy_12bits/IMG_PHR1A_MS_002/DIM_PHR1A_MS_201206050630064_SEN_559102101-002.XML})
set(spot6_filename LARGEINPUT{SPOT6/600143101-Primary-Bundle-JP2-LOSSLESS/PROD_SPOT6_001/VOL_SPOT6_001_A/IMG_SPOT6_MS_001_A/IMG_SPOT6_MS_201212071020271_SEN_600143101_R1C1.JP2})
set(spot6_2_filename LARGEINPUT{SPOT6/600143101-Primary-Bundle-JP2-LOSSLESS/PROD_SPOT6_001/VOL_SPOT6_001_A/IMG_SPOT6_MS_001_A/DIM_SPOT6_MS_201212071020271_SEN_600143101.XML})
set(spot5_filename LARGEINPUT{SPOT5/Arcachon-SCENE01/IMAGERY.TIF})
set(spot5_2_filename LARGEINPUT{SPOT5/TEHERAN/IMAGERY.TIF})
set(formosat_filename LARGEINPUT{FORMOSAT/Sudouest_20071013_MS_fmsat/IMAGERY.TIF})
set(ikonos_filename LARGEINPUT{IKONOS/BLOSSEVILLE/po_2619900_blu_0000000.tif})
set(worldview2_filename LARGEINPUT{WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_MUL/09DEC10103019-M2AS-052298844010_01_P001.TIF})
set(quickbird_filename LARGEINPUT{QUICKBIRD/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF})
set(Radarsat2_filename LARGEINPUT{RADARSAT2/ALTONA/Fine_Quad-Pol_Dataset/PK6621_DK406_FQ9_20080405_124900_HH_VV_HV_VH_SLC_Altona/imagery_HH.tif})
set(TerraSar_filename LARGEINPUT{TERRASARX/2008-03-10_GrandCanyon_SSC/TSX1_SAR__SSC______SM_S_SRA_20080310T133220_20080310T133228/IMAGEDATA/IMAGE_HH_SRA_strip_011.cos})
set(CosmoSkyMed_filename LARGEINPUT{COSMOSKYMED/Toulouse_spotlight/CSKS3_SCS_B_S2_08_HH_RD_SF_20110418180325_20110418180332.h5})
set(Sentinel1_filename LARGEINPUT{SENTINEL1/S1A_S6_SLC__1SSV_20150619T195043/measurement/s1a-s6-slc-vv-20150619t195043-20150619t195101-006447-00887d-001.tiff})

set(spot5_2_geom_file ${INPUTDATA}/spot5/spot5-1.geom)
set(formosat_geom_file ${INPUTDATA}/FORMOSAT_ROI_1000_100.geom)
set(pleiades_geom_file ${INPUTDATA}/pleiades/maido.geom)
set(spot6_geom_file ${INPUTDATA}/spot6/spot6-2.geom)
set(ikonos_geom_file ${INPUTDATA}/ikonos/ikonos-2.geom)
set(worldview2_geom_file ${INPUTDATA}/wv2/wv2-2.geom)
set(quickbird_geom_file ${INPUTDATA}/QB/qb-2.geom)
set(Radarsat2_geom_file ${INPUTDATA}/radarsat2/radarsat2-1.geom)
set(TerraSar_geom_file ${INPUTDATA}/terrasar/terrasar-1.geom)
set(CosmoSkyMed_geom_file ${INPUTDATA}/cosmoskymed/cosmoskymed-1.geom)
set(Sentinel1_geom_file ${INPUTDATA}/sentinel1/sentinel1-1.geom)

foreach(sensor ${sensor_list})

if (${sensor}_geom_file)
otb_add_test(NAME ioTvImageMetadataInterfaceTest_${sensor} COMMAND otbMetadataTestDriver 
  --compare-ascii 0 
  ${BASELINE_FILES}/ioTvImageMetadataInterfaceTest_${sensor}.txt
  ${TEMP}/ioTvImageMetadataInterfaceTest_${sensor}.txt
  --compare-metadata ${EPSILON_9} 
  ${${sensor}_filename}
  ${${sensor}_filename}?&geom=${${sensor}_geom_file}
  otbImageMetadataInterfaceTest
  ${${sensor}_filename}
  ${TEMP}/ioTvImageMetadataInterfaceTest_${sensor}.txt
  )
else()
otb_add_test(NAME ioTvImageMetadataInterfaceTest_${sensor} COMMAND otbMetadataTestDriver 
  --compare-ascii 0 
  ${BASELINE_FILES}/ioTvImageMetadataInterfaceTest_${sensor}.txt
  ${TEMP}/ioTvImageMetadataInterfaceTest_${sensor}.txt
  otbImageMetadataInterfaceTest
  ${${sensor}_filename}
  ${TEMP}/ioTvImageMetadataInterfaceTest_${sensor}.txt
  )
endif()

endforeach()


if (Boost_UNIT_TEST_FRAMEWORK_FOUND)

  add_executable(otbDateTest otbDateTest.cxx )
  target_link_libraries(
    otbDateTest
    ${OTBMetadata-Test_LIBRARIES}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} )

  otb_add_test(NAME ioTvDateTest
           COMMAND otbDateTest --
           )

endif()
