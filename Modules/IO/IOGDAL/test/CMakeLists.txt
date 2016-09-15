otb_module_test()

set(OTBIOGDALTests
otbIOGDALTestDriver.cxx
otbGDALImageIOTest.cxx
otbGDALImageIOTestWriteMetadata.cxx
otbGDALOverviewsBuilder.cxx
otbOGRVectorDataIONew.cxx
otbGDALImageIOTestCanWrite.cxx
otbOGRVectorDataIOCanWrite.cxx
otbGDALReadPxlComplex.cxx
otbGDALImageIOTestCanRead.cxx
otbMultiDatasetReadingInfo.cxx
otbOGRVectorDataIOCanRead.cxx
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

otb_add_test(NAME ioTuGDALOverviewsBuilder COMMAND otbIOGDALTestDriver
  otbGDALOverviewsBuilderNew)

otb_add_test(NAME ioTvGDALOverviewsBuilder_TIFF COMMAND otbIOGDALTestDriver
  otbGDALOverviewsBuilder
  ${TEMP}/ioTvGDALImageIO_Tiff_NoOption.tif
  4
  )
set_property(TEST ioTvGDALOverviewsBuilder_TIFF PROPERTY DEPENDS ioTvGDALImageIO_Tiff_NoOption)

otb_add_test(NAME ioTuOGRVectorDataIO COMMAND otbIOGDALTestDriver
  otbOGRVectorDataIONew )

otb_add_test(NAME ioTuGDALImageIOCanWrite_HFA COMMAND otbIOGDALTestDriver otbGDALImageIOTestCanWrite
  ${INPUTDATA}/HFAGeoreferenced.img)

otb_add_test(NAME ioTuGDALImageIOCanWrite_TIFF COMMAND otbIOGDALTestDriver otbGDALImageIOTestCanWrite
  ${INPUTDATA}/testGDALWriter.tif)

otb_add_test(NAME ioTuOGRVectorDataIOCanWriteMapInfo COMMAND otbIOGDALTestDriver
  otbOGRVectorDataIOCanWrite
  ${INPUTDATA}/LOCALITY_POLYGON.tab)

otb_add_test(NAME ioTuOGRVectorDataIOCanWriteGML COMMAND otbIOGDALTestDriver
  otbOGRVectorDataIOCanWrite
  ${INPUTDATA}/adressPoint.gml)

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
  ${INPUTDATA}/adressPoint.gml)

otb_add_test(NAME ioTuOGRVectorDataIOTestCanReadMapInfo COMMAND otbIOGDALTestDriver
  otbOGRVectorDataIOTestCanRead
  ${INPUTDATA}/LOCALITY_POLYGON.tab)


# Tests with GDAL only to read complex data
set(INPUTFILE_PIXELTYPES_LIST "Float" "Double")

foreach(INPUTFILE_PIXELTYPE ${INPUTFILE_PIXELTYPES_LIST})

  otb_add_test(NAME ioTvGDALReadPxlComplex${INPUTFILE_PIXELTYPE} COMMAND otbIOGDALTestDriver
    otbGDALReadPxlComplex${INPUTFILE_PIXELTYPE}
    ${INPUTDATA}/multibandComplex${INPUTFILE_PIXELTYPE}_3bands.tif
    1 5 10 2) #old file hdr sans extensions

endforeach()
