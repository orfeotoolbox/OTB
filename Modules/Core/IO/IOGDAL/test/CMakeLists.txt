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

set(OTBIOGDALTests
otbIOGDALTestDriver.cxx
otbGDALImageIOTest.cxx
otbGDALImageIOTestWriteMetadata.cxx
otbGDALOverviewsBuilder.cxx
otbGDALImageIOTestCanWrite.cxx
otbOGRVectorDataIOCanWrite.cxx
otbGDALReadPxlComplex.cxx
otbGDALImageIOTestCanRead.cxx
otbMultiDatasetReadingInfo.cxx
otbOGRVectorDataIOCanRead.cxx
otbDEMHandlerTest.cxx
otbGDALRPCTransformerTest.cxx
otbGDALRPCTransformerTest2.cxx
)

add_executable(otbIOGDALTestDriver ${OTBIOGDALTests})
target_link_libraries(otbIOGDALTestDriver ${OTBIOGDAL-Test_LIBRARIES})
otb_module_target_label(otbIOGDALTestDriver)

# Tests Declaration

otb_add_test(NAME ioTvGDALImageIO_Tiff_JPEG_99 COMMAND otbIOGDALTestDriver
  --compare-image ${NOTOL} ${BASELINE}/ioTvGDALImageIO_Tiff_JPEG_99.tif
  ${TEMP}/ioTvGDALImageIO_Tiff_JPEG_99.tif
  otbGDALImageIOTest_uint8
  ${INPUTDATA}/maur_rgb_24bpp.tif
  ${TEMP}/ioTvGDALImageIO_Tiff_JPEG_99.tif
  "COMPRESS=JPEG"
  "JPEG_QUALITY=99"
  "PHOTOMETRIC=YCBCR"
  "TILED=YES"
  "BLOCKXSIZE=256"
  "BLOCKYSIZE=256"
  )

otb_add_test(NAME ioTvGDALImageIO_JPEG_99 COMMAND otbIOGDALTestDriver
  --compare-image ${NOTOL} ${BASELINE}/ioTvGDALImageIO_JPEG_99.jpg
  ${TEMP}/ioTvGDALImageIO_JPEG_99.jpg
  otbGDALImageIOTest_uint8
  ${INPUTDATA}/maur_rgb_24bpp.tif
  ${TEMP}/ioTvGDALImageIO_JPEG_99.jpg
  "QUALITY=99"
  )

otb_add_test(NAME ioTvGDALImageIO_Tiff_NoOption COMMAND otbIOGDALTestDriver
  --compare-image ${NOTOL} ${INPUTDATA}/maur_rgb.tif
  ${TEMP}/ioTvGDALImageIO_Tiff_NoOption.tif
  otbGDALImageIOTest_uint16
  ${INPUTDATA}/maur_rgb.tif
  ${TEMP}/ioTvGDALImageIO_Tiff_NoOption.tif
  )

otb_add_test(NAME ioTvGDALImageIO_Tiff_JPEG_20 COMMAND otbIOGDALTestDriver
  --compare-image ${NOTOL} ${BASELINE}/ioTvGDALImageIO_Tiff_JPEG_20.tif
  ${TEMP}/ioTvGDALImageIO_Tiff_JPEG_20.tif
  otbGDALImageIOTest_uint8
  ${INPUTDATA}/maur_rgb_24bpp.tif
  ${TEMP}/ioTvGDALImageIO_Tiff_JPEG_20.tif
  "COMPRESS=JPEG"
  "JPEG_QUALITY=20"
  "PHOTOMETRIC=YCBCR"
  "TILED=YES"
  "BLOCKXSIZE=256"
  "BLOCKYSIZE=256"
  )

otb_add_test(NAME ioTvGDALImageIO_Tiff_Stripped COMMAND otbIOGDALTestDriver
  --compare-image ${NOTOL} ${INPUTDATA}/maur_rgb.tif
  ${TEMP}/ioTvGDALImageIO_Tiff_stripped.tif
  otbGDALImageIOTest_uint16
  ${INPUTDATA}/maur_rgb.tif
  ${TEMP}/ioTvGDALImageIO_Tiff_stripped.tif
  "BLOCKYSIZE=10"
  )

otb_add_test(NAME ioTvGDALImageIO_Tiff_12bits COMMAND otbIOGDALTestDriver
  --compare-image ${NOTOL} ${INPUTDATA}/maur_rgb.tif
  ${TEMP}/ioTvGDALImageIO_Tiff_12bits.tif
  otbGDALImageIOTest_uint16
  ${INPUTDATA}/maur_rgb.tif
  ${TEMP}/ioTvGDALImageIO_Tiff_12bits.tif
  "NBITS=12"
  )

otb_add_test(NAME ioTvGDALImageIO_Tiff_Tiled_16x16 COMMAND otbIOGDALTestDriver
  --compare-image ${NOTOL} ${INPUTDATA}/maur_rgb.tif
  ${TEMP}/ioTvGDALImageIO_Tiff_tiled_16x16.tif
  otbGDALImageIOTest_uint16
  ${INPUTDATA}/maur_rgb.tif
  ${TEMP}/ioTvGDALImageIO_Tiff_tiled_16x16.tif
  "TILED=YES"
  "BLOCKXSIZE=16"
  "BLOCKYSIZE=16"
  )

otb_add_test(NAME ioTvGDALImageIO_JPEG_20 COMMAND otbIOGDALTestDriver
  --compare-image ${NOTOL} ${BASELINE}/ioTvGDALImageIO_JPEG_20.jpg
  ${TEMP}/ioTvGDALImageIO_JPEG_20.jpg
  otbGDALImageIOTest_uint8
  ${INPUTDATA}/maur_rgb_24bpp.tif
  ${TEMP}/ioTvGDALImageIO_JPEG_20.jpg
  "QUALITY=20"
  )

otb_add_test(NAME ioTvGDALImageIOWriteMetadata COMMAND otbIOGDALTestDriver
  otbGDALImageIOTestWriteMetadata
  ${TEMP}/ioTvGDALImageIOWriteMetadata.tif
  ${TEMP}/ioTvGDALImageIOWriteMetadata.hdr
  ${TEMP}/ioTvGDALImageIOWriteMetadata_Report.txt
  )

otb_add_test(NAME ioTvGDALOverviewsBuilder_TIFF COMMAND otbIOGDALTestDriver
  otbGDALOverviewsBuilder
  ${TEMP}/ioTvGDALImageIO_Tiff_NoOption.tif
  4
  )
set_property(TEST ioTvGDALOverviewsBuilder_TIFF PROPERTY DEPENDS ioTvGDALImageIO_Tiff_NoOption)

otb_add_test(NAME ioTuGDALImageIOCanWrite_HFA COMMAND otbIOGDALTestDriver otbGDALImageIOTestCanWrite
  ${INPUTDATA}/HFAGeoreferenced.img)

otb_add_test(NAME ioTuGDALImageIOCanWrite_TIFF COMMAND otbIOGDALTestDriver otbGDALImageIOTestCanWrite
  ${INPUTDATA}/testGDALWriter.tif)

otb_add_test(NAME ioTuOGRVectorDataIOCanWriteMapInfo COMMAND otbIOGDALTestDriver
  otbOGRVectorDataIOCanWrite
  ${INPUTDATA}/LOCALITY_POLYGON.tab)

otb_add_test(NAME ioTuOGRVectorDataIOCanWriteGML COMMAND otbIOGDALTestDriver
  otbOGRVectorDataIOCanWrite
  ${INPUTDATA}/addressPoint.gml)

otb_add_test(NAME ioTuGDALImageIOCanRead_IKONOS_RED COMMAND otbIOGDALTestDriver otbGDALImageIOTestCanRead
  LARGEINPUT{IKONOS/PARIS/po_79039_red_0000000.tif})

otb_add_test(NAME ioTuGDALImageIOCanRead_SPOT4 COMMAND otbIOGDALTestDriver otbGDALImageIOTestCanRead
  LARGEINPUT{SPOT4/RIO_DE_JANEIRO/IMAG_01.DAT} )

otb_add_test(NAME ioTuGDALImageIOCanRead_WV2_MUL COMMAND otbIOGDALTestDriver otbGDALImageIOTestCanRead
  LARGEINPUT{WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_MUL/09DEC10103019-M2AS-052298844010_01_P001.TIF} )

otb_add_test(NAME ioTuGDALImageIOCanRead_WV2_PAN COMMAND otbIOGDALTestDriver otbGDALImageIOTestCanRead
  LARGEINPUT{WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_PAN/09DEC10103019-P2AS-052298844010_01_P001.TIF} )

otb_add_test(NAME ioTuGDALImageIOCanRead_ENVI COMMAND otbIOGDALTestDriver otbGDALImageIOTestCanRead
  ${INPUTDATA}/poupees_1canal.c1 )

otb_add_test(NAME ioTuGDALImageIOCanRead_IKONOS_GRN COMMAND otbIOGDALTestDriver otbGDALImageIOTestCanRead
  LARGEINPUT{IKONOS/PARIS/po_79039_grn_0000000.tif})

otb_add_test(NAME ioTuGDALImageIOCanRead_IKONOS_BLU COMMAND otbIOGDALTestDriver otbGDALImageIOTestCanRead
  LARGEINPUT{IKONOS/PARIS/po_79039_blu_0000000.tif})

otb_add_test(NAME ioTuGDALImageIOCanRead_QUICK_PAN COMMAND otbIOGDALTestDriver otbGDALImageIOTestCanRead
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF} )

otb_add_test(NAME ioTuGDALImageIOCanRead_HFA COMMAND otbIOGDALTestDriver otbGDALImageIOTestCanRead
  ${INPUTDATA}/HFAGeoreferenced.img)

otb_add_test(NAME ioTuGDALImageIOCanRead_SPOT5TIF COMMAND otbIOGDALTestDriver otbGDALImageIOTestCanRead
  ${INPUTDATA}/poupeesTIF/IMAGERY.TIF )

otb_add_test(NAME ioTuGDALImageIOCanRead_PCI COMMAND otbIOGDALTestDriver otbGDALImageIOTestCanRead
  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.pix)

if (GDAL_HAS_HDF5)
otb_add_test(NAME ioTuGDALImageIOCanRead_COSMOSKYMED2 COMMAND otbIOGDALTestDriver otbGDALImageIOTestCanRead
  LARGEINPUT{COSMOSKYMED/Toulouse_spotlight/CSKS3_SCS_B_S2_08_HH_RD_SF_20110418180325_20110418180332.h5})

otb_add_test(NAME ioTuGDALImageIOCanRead_COSMOSKYMED3 COMMAND otbIOGDALTestDriver otbGDALImageIOTestCanRead
  LARGEINPUT{COSMOSKYMED/Toulouse_spotlight/CSKS2_GTC_B_S2_08_HH_RD_SF_20110417180326_20110417180333.h5})

otb_add_test(NAME ioTuGDALImageIOCanRead_COSMOSKYMED COMMAND otbIOGDALTestDriver otbGDALImageIOTestCanRead
  LARGEINPUT{COSMOSKYMED/Toulouse_spotlight/CSKS3_GTC_B_S2_08_HH_RD_SF_20110418180325_20110418180332.h5})
endif()

otb_add_test(NAME ioTuGDALImageIOCanRead_WV2_PSH COMMAND otbIOGDALTestDriver otbGDALImageIOTestCanRead
  LARGEINPUT{WORLDVIEW2/ROME/WV-2_standard_pansharpened_natural_color_8bit/052298834010_01_P001_PSH/09DEC10103019-S2AS-052298834010_01_P001.TIF} )

otb_add_test(NAME ioTuGDALImageIOCanRead_SAR_ERS2 COMMAND otbIOGDALTestDriver otbGDALImageIOTestCanRead
  LARGEINPUT{SAR_ERS2_SLCI_SCENE1/DAT_01.001} )

otb_add_test(NAME ioTuGDALImageIOCanRead_QUICK_XS COMMAND otbIOGDALTestDriver otbGDALImageIOTestCanRead
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF} )

otb_add_test(NAME ioTuGDALImageIOCanRead_IKONOS_NIR COMMAND otbIOGDALTestDriver otbGDALImageIOTestCanRead
  LARGEINPUT{IKONOS/PARIS/po_79039_nir_0000000.tif})


otb_add_test(NAME ioTuGDALImageIOCanRead_PDS COMMAND otbIOGDALTestDriver otbGDALImageIOTestCanRead
  ${INPUTDATA}/pdsImage.img )

otb_add_test(NAME ioTuGDALImageIOCanRead_RADARSAT COMMAND otbIOGDALTestDriver otbGDALImageIOTestCanRead
  LARGEINPUT{RADARSAT1/GOMA/SCENE01/DAT_01.001} )

otb_add_test(NAME ioTuGDALImageIOCanRead_IKONOS_PAN COMMAND otbIOGDALTestDriver otbGDALImageIOTestCanRead
  LARGEINPUT{IKONOS/PARIS/po_79039_pan_0000000.tif})

otb_add_test(NAME ioTuGDALImageIOCanRead_FORMOSAT2 COMMAND otbIOGDALTestDriver otbGDALImageIOTestCanRead
  LARGEINPUT{FORMOSAT/Sudouest_20071013_MS_fmsat/IMAGERY.TIF})

if(GDAL_HAS_HDF4)
  otb_add_test(NAME ioTvMultiDatasetReadingInfo COMMAND otbIOGDALTestDriver
    --compare-ascii ${NOTOL}  ${BASELINE_FILES}/ioTvMultiDatasetReadingInfo.txt
    ${TEMP}/ioTvMultiDatasetReadingInfoOut.txt
    otbMultiDatasetReadingInfo
    ${INPUTDATA}/MOD09Q1G_EVI.A2006233.h07v03.005.2008338190308.hdf
    ${TEMP}/ioTvMultiDatasetReadingInfoOut.txt
    )
endif()

otb_add_test(NAME ioTuOGRVectorDataIOTestCanReadGML COMMAND otbIOGDALTestDriver
  otbOGRVectorDataIOTestCanRead
  ${INPUTDATA}/addressPoint.gml)

otb_add_test(NAME ioTuOGRVectorDataIOTestCanReadMapInfo COMMAND otbIOGDALTestDriver
  otbOGRVectorDataIOTestCanRead
  ${INPUTDATA}/LOCALITY_POLYGON.tab)


# Tests with GDAL only to read complex data
set(INPUTFILE_PIXELTYPES_LIST "Float" "Double")

foreach(INPUTFILE_PIXELTYPE ${INPUTFILE_PIXELTYPES_LIST})

  otb_add_test(NAME ioTvGDALReadPxlComplex${INPUTFILE_PIXELTYPE} COMMAND otbIOGDALTestDriver
    otbGDALReadPxlComplex${INPUTFILE_PIXELTYPE}
    ${INPUTDATA}/multibandComplex${INPUTFILE_PIXELTYPE}_3bands_50x50.tif
    1 5 10 2) #old file hdr sans extensions

endforeach()

otb_add_test(NAME uaTvDEMHandler_AboveEllipsoid_NoSRTM_NoGeoid_NoData COMMAND otbIOGDALTestDriver
  otbDEMHandlerTest
  no
  no
  40
  8.687917
  44.237917
  0
  40
  0.001
  no
  )

otb_add_test(NAME uaTvDEMHandler_AboveMSL_NoSRTM_Geoid COMMAND otbIOGDALTestDriver
  otbDEMHandlerTest
  no
  ${INPUTDATA}/DEM/egm96.grd
  40
  8.434583
  44.647083
  1
  0
  0.001
  no
  )

otb_add_test(NAME uaTvDEMHandler_AboveEllipsoid_BadSRTM_Geoid COMMAND otbIOGDALTestDriver
  otbDEMHandlerTest
  ${INPUTDATA}/poupeesTIF/
  ${INPUTDATA}/DEM/egm96.grd
  40
  8.434583
  44.647083
  0
  383.580313671
  0.001
  no
  )
set_property(TEST uaTvDEMHandler_AboveEllipsoid_BadSRTM_Geoid PROPERTY WILL_FAIL 1)

otb_add_test(NAME uaTvDEMHandler_AboveMSL_NoSRTM_NoGeoid COMMAND otbIOGDALTestDriver
  otbDEMHandlerTest
  no
  no
  40
  8.434583
  44.647083
  1
  0
  0.001
  no
  )

otb_add_test(NAME uaTvDEMHandler_AboveEllipsoid_NoSRTM_Geoid COMMAND otbIOGDALTestDriver
  otbDEMHandlerTest
  no
  ${INPUTDATA}/DEM/egm96.grd
  40
  8.434583
  44.647083
  0
  44.067113
  0.001
  no
  )

otb_add_test(NAME uaTvDEMHandler_AboveEllipsoid_SRTM_Geoid COMMAND otbIOGDALTestDriver
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  ${INPUTDATA}/DEM/egm96.grd
  40
  8.434583
  44.647083
  0
  383.580313671
  0.001
  no
  )

otb_add_test(NAME uaTvDEMHandler_AboveMSL_SRTM_NoGeoid_NoSRTMCoverage COMMAND otbIOGDALTestDriver
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  no
  20
  10. # Point not covered
  10. # Point not covered
  1
  0
  0.001
  no
  )

otb_add_test(NAME uaTvDEMHandler_AboveEllipsoid_SRTM_NoGeoid_NoSRTMCoverage COMMAND otbIOGDALTestDriver
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  no
  20
  10. # Point not covered
  10. # Point not covered
  0
  20
  0.001
  no
  )

otb_add_test(NAME uaTvDEMHandler_AboveMSL_SRTM_NoGeoid COMMAND otbIOGDALTestDriver
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  no
  40
  8.434583
  44.647083
  1
  339.513
  0.001
  no
  )

otb_add_test(NAME uaTvDEMHandler_AboveEllipsoid_SRTM_NoGeoid COMMAND otbIOGDALTestDriver
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  no
  40
  8.434583
  44.647083
  0
  339.513
  0.001
  no
  )

otb_add_test(NAME uaTvDEMHandler_AboveEllipsoid_SRTM_Geoid_NoData COMMAND otbIOGDALTestDriver
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  ${INPUTDATA}/DEM/egm96.grd
  40
  8.687917
  44.237917
  0
  45.7464
  0.001
  no
  )

otb_add_test(NAME uaTvDEMHandler_AboveEllipsoid_SRTM_BadGeoid COMMAND otbIOGDALTestDriver
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  ${INPUTDATA}/poupees.jpg
  40
  8.434583
  44.647083
  0
  339.513
  0.001
  no
  )

otb_add_test(NAME uaTvDEMHandler_AboveEllipsoid_SRTM_BadGeoid_2 COMMAND otbIOGDALTestDriver
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  ${INPUTDATA}/acqui-params.txt
  40
  8.434583
  44.647083
  0
  339.513
  0.001
  no
  )

otb_add_test(NAME uaTvDEMHandler_AboveMSL_SRTM_NoGeoid_NoData COMMAND otbIOGDALTestDriver
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  no
  40
  8.687917
  44.237917
  1
  0
  0.001
  no
  )

otb_add_test(NAME uaTvDEMHandler_AboveEllipsoid_BDALTI_TIF_NoGeoid COMMAND otbIOGDALTestDriver
  otbDEMHandlerTest
  LARGEINPUT{BD_ALTI/}
  no
  800
  7.283262
  48.598675
  0
  869
  1
  no
  )

otb_add_test(NAME uaTvDEMHandler_AboveMSL_SRTM_Geoid_NoData COMMAND otbIOGDALTestDriver
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  ${INPUTDATA}/DEM/egm96.grd
  40
  8.687917
  44.237917
  1
  0
  0.001
  no
  )

otb_add_test(NAME uaTvDEMHandler_AboveMSL_NoSRTM_NoGeoid_NoData COMMAND otbIOGDALTestDriver
  otbDEMHandlerTest
  no
  no
  40
  8.687917
  44.237917
  1
  0
  0.001
  no
  )

otb_add_test(NAME uaTvDEMHandler_AboveMSL_SRTM_Geoid COMMAND otbIOGDALTestDriver
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  ${INPUTDATA}/DEM/egm96.grd
  40
  8.434583
  44.647083
  1
  339.513
  0.001
  no
  )

otb_add_test(NAME uaTvDEMHandler_AboveEllipsoid_NoSRTM_NoGeoid COMMAND otbIOGDALTestDriver
  otbDEMHandlerTest
  no
  no
  40
  8.434583
  44.647083
  0
  40
  0.001
  no
  )

otb_add_test(NAME uaTvDEMHandler_AboveMSL_SRTM_Geoid_NoSRTMCoverage COMMAND otbIOGDALTestDriver
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  ${INPUTDATA}/DEM/egm96.grd
  20
  10. # Point not covered
  10. # Point not covered
  1
  0
  0.001
  no
  )

otb_add_test(NAME uaTvDEMHandler_AboveEllipsoid_SRTM_Geoid_NoSRTMCoverage COMMAND otbIOGDALTestDriver
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  ${INPUTDATA}/DEM/egm96.grd
  20
  10. # Point not covered
  10. # Point not covered
  0
  21.569 # Geoid offset
  0.001
  no
  )

otb_add_test(NAME uaTvDEMHandler_AboveEllipsoid_SRTM_NoGeoid_NoData COMMAND otbIOGDALTestDriver
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  no
  40
  8.687917
  44.237917
  0
  40
  0.001
  no
  )

otb_add_test(NAME uaTvDEMHandler_ClearElevationParameters COMMAND otbIOGDALTestDriver
  otbDEMHandlerTest
  no
  ${INPUTDATA}/DEM/egm96.grd
  40
  8.434583
  44.647083
  1
  0
  0.001
  yes
  )

otb_add_test(NAME TuGDALRPCTransformerTest
  COMMAND otbIOGDALTestDriver
  otbGDALRPCTransformerTest
  ${INPUTDATA}/TuGDALRPCTransformerTest_DEM.tif
  )

otb_add_test(NAME ioTvGDALRPCTransformerTest_ikonos_geom
  COMMAND otbIOGDALTestDriver
  otbGDALRPCTransformerTest2
  ${INPUTDATA}/ikonos/ikonos-1.geom # Geom
  ${INPUTDATA}/ikonos/ikonos-1.gcp2 # GCP
  0.04 # GeoTol
  0.1 # ImgTol
  )

otb_add_test(NAME ioTvGDALRPCTransformerTest_ikonos_product
  COMMAND otbIOGDALTestDriver
  otbGDALRPCTransformerTest2
  LARGEINPUT{IKONOS/BLOSSEVILLE/po_2619900_blu_0000000.tif} # Product
  ${INPUTDATA}/ikonos/ikonos-1.gcp2 # GCP
  0.04 # GeoTol
  0.1 # ImgTol
  )

otb_add_test(NAME ioTvGDALRPCTransformerTest_quickbird
  COMMAND otbIOGDALTestDriver
  otbGDALRPCTransformerTest2
  ${INPUTDATA}/QB/qb-1.geom # Geom
  ${INPUTDATA}/QB/qb-1.gcp2 # GCP
  0.024 # GeoTol
  0.1 # ImgTol
  )

otb_add_test(NAME ioTvGDALRPCTransformerTest_spot6
  COMMAND otbIOGDALTestDriver
  otbGDALRPCTransformerTest2
  ${INPUTDATA}/spot6/spot6-1.geom # Geom
  ${INPUTDATA}/spot6/spot6-1.gcp2 # GCP
  0.06 # GeoTol
  0.1 # ImgTol
  )

otb_add_test(NAME ioTvGDALRPCTransformerTest_worldview2
  COMMAND otbIOGDALTestDriver
  otbGDALRPCTransformerTest2
  ${INPUTDATA}/wv2/wv2-1.geom # Geom
  ${INPUTDATA}/wv2/wv2-1.gcp2 # GCP
  0.0185 # GeoTol
  0.1 # ImgTol
  )

#For issue 2293, we had to regenerate the baseline pleaides-1.geom because there was a shift
#in the geom generated with OTB 7.4, the baseline was wrong. To keep the compatibility, we added this shift in the tests
otb_add_test(NAME ioTvGDALRPCTransformerTest_pl_hnord_geom
  COMMAND otbIOGDALTestDriver
  otbGDALRPCTransformerTest2
  ${INPUTDATA}/pleiades/pleiades-1.geom # Geom
  ${INPUTDATA}/pleiades/pleiades-1.gcp2 # GCP
  0.02 # GeoTol
  0.1 # ImgTol
  0.5 # lineOffset
  0.5 # sampleOffset
  )

#For issue 2293, we had to regenerate the baseline pleiades-1.gcp2 because there was a shift
#in the gcps that was fixed by the MR 918
#To regenerate the gcps, see the wiki on the gitlab
otb_add_test(NAME ioTvGDALRPCTransformerTest_pl_hnord_product
  COMMAND otbIOGDALTestDriver
  otbGDALRPCTransformerTest2
  LARGEINPUT{PLEIADES/TLSE_JP2_DIMAPv2_PRIMARY_PMS_lossless_12bits/IMGPHR_201222215194743808/IMG_PHR1A_PMS_201201151100183_SEN_IPU_20120222_0901-001_R1C1.JP2} # Product
  ${INPUTDATA}/pleiades/pleiades-1.gcp2 # GCP
  0.02 # GeoTol
  0.1 # ImgTol
  )

