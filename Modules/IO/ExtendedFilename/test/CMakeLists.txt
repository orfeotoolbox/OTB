otb_module_test()

set(OTBExtendedFilenameTests
otbExtendedFilenameTestDriver.cxx
otbExtendedFilenameToReaderOptionsTest.cxx
otbExtendedFilenameToWriterOptionsTest.cxx
otbExtendedFilenameTest.cxx
)

add_executable(otbExtendedFilenameTestDriver ${OTBExtendedFilenameTests})
target_link_libraries(otbExtendedFilenameTestDriver ${OTBExtendedFilename-Test_LIBRARIES})
otb_module_target_label(otbExtendedFilenameTestDriver)

# Tests Declaration
foreach(streaming_type "Tiled" "Stripped")
  foreach(streaming_sizemode "Auto" "Nbsplits" "Height")
    string(TOLOWER ${streaming_type} type)
    string(TOLOWER ${streaming_sizemode} sizemode)

    set(_test_name "ioTvImageFileWriterExtendedFileName_Streaming${streaming_type}${streaming_sizemode}")

    otb_add_test(NAME ${_test_name} COMMAND otbExtendedFilenameTestDriver
      --compare-image ${NOTOL}
               ${INPUTDATA}/maur_rgb_24bpp.tif
               ${TEMP}/${test_name}.tif
      otbImageFileWriterWithExtendedFilename
              ${INPUTDATA}/maur_rgb_24bpp.tif
              ${TEMP}/${test_name}.tif?&streaming:type=${type}&streaming:sizemode=${sizemode}&streaming:sizevalue=${streaming_sizevalue_${sizemode}}
    )
  endforeach()
endforeach()

otb_add_test(NAME ioTvExtendedFilenameToReaderOptions_FullOptions COMMAND otbExtendedFilenameTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE}/ioTvExtendedFilenameToReaderOptions_FullOptions.txt
  ${TEMP}/ioTvExtendedFilenameToReaderOptions_FullOptions.txt
  otbExtendedFilenameToReaderOptions
  /home/data/filename.tif?&geom=/home/dev/custom.geom&sdataidx=2&resol=4&skipcarto=On
  ${TEMP}/ioTvExtendedFilenameToReaderOptions_FullOptions.txt
  )

otb_add_test(NAME ioTvExtendedFilenameToReaderOptions_NoOptions COMMAND otbExtendedFilenameTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE}/ioTvExtendedFilenameToReaderOptions_NoOptions.txt
  ${TEMP}/ioTvExtendedFilenameToReaderOptions_NoOptions.txt
  otbExtendedFilenameToReaderOptions
  /home/data/filename.tif
  ${TEMP}/ioTvExtendedFilenameToReaderOptions_NoOptions.txt
  )

otb_add_test(NAME ioTvExtendedFilenameToWriterOptions_FullOptions COMMAND otbExtendedFilenameTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE}/ioTvExtendedFilenameToWriterOptions_FullOptions.txt
  ${TEMP}/ioTvExtendedFilenameToWriterOptions_FullOptions.txt
  otbExtendedFilenameToWriterOptions
  /home/data/filename.tif?&writegeom=ON&gdal:co:QUALITY=75&gdal:co:TILED=YES&gdal:co:BLOCKYSIZE=1024
  ${TEMP}/ioTvExtendedFilenameToWriterOptions_FullOptions.txt
  )

otb_add_test(NAME ioTvExtendedFilenameToWriterOptions_NoOptions COMMAND otbExtendedFilenameTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE}/ioTvExtendedFilenameToWriterOptions_NoOptions.txt
  ${TEMP}/ioTvExtendedFilenameToWriterOptions_NoOptions.txt
  otbExtendedFilenameToWriterOptions
  /home/data/filename.tif
  ${TEMP}/ioTvExtendedFilenameToWriterOptions_NoOptions.txt
  )

otb_add_test(NAME ioTvImageFileWriterExtendedFileName_StreamingNone COMMAND otbExtendedFilenameTestDriver
  --compare-image ${NOTOL}
  ${INPUTDATA}/maur_rgb_24bpp.tif
  ${TEMP}/ioImageFileWriterExtendedFileName_streamingNone.tif
  otbImageFileWriterWithExtendedFilename
  ${INPUTDATA}/maur_rgb_24bpp.tif
  ${TEMP}/ioImageFileWriterExtendedFileName_streamingNone.tif?&streaming:type=none)

otb_add_test(NAME ioTvImageFileReaderExtendedFileName_GEOM COMMAND otbExtendedFilenameTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE}/ioImageFileReaderWithExternalGEOMFile.txt
  ${TEMP}/ioImageFileReaderWithExternalGEOMFile.txt
  otbImageFileReaderWithExtendedFilename
  ${INPUTDATA}/ToulouseExtract_WithGeom.tif?&geom=${INPUTDATA}/ToulouseExtract_ModifiedGeom.geom
  ${TEMP}/ioImageFileReaderWithExternalGEOMFile.txt
  ${TEMP}/ioImageFileReaderWithExternalGEOMFile_pr.txt
  )

otb_add_test(NAME ioTvImageFileWriterExtendedFileName_GEOM COMMAND otbExtendedFilenameTestDriver
  --ignore-order --compare-ascii ${NOTOL}
  ${INPUTDATA}/ToulouseExtract_WithGeom.geom
  ${TEMP}/ioImageFileWriterExtendedFileName_GEOM.geom
  --ignore-lines-with 2 ce90_absolute ce90_relative
  otbImageFileWriterWithExtendedFilename
  ${INPUTDATA}/ToulouseExtract_WithGeom.tif
  ${TEMP}/ioImageFileWriterExtendedFileName_GEOM.tif?&writegeom=ON
  )

otb_add_test(NAME ioTvImageFileWriterExtendedFileName_StreamingAuto COMMAND otbExtendedFilenameTestDriver
  --compare-image ${NOTOL}
  ${INPUTDATA}/maur_rgb_24bpp.tif
  ${TEMP}/ioImageFileWriterExtendedFileName_streamingAuto.tif
  otbImageFileWriterWithExtendedFilename
  ${INPUTDATA}/maur_rgb_24bpp.tif
  ${TEMP}/ioImageFileWriterExtendedFileName_streamingAuto.tif?&streaming:type=auto&streaming:sizevalue=${streaming_sizevalue_auto})

otb_add_test(NAME ioTvImageFileReaderExtendedFileName_mix1 COMMAND otbExtendedFilenameTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE}/ioImageFileReaderExtendedFileName_mix1pr.txt
  ${TEMP}/ioImageFileReaderExtendedFileName_mix1pr.txt
  otbImageFileReaderWithExtendedFilename
  ${INPUTDATA}/bretagne.j2k?&skipcarto=true&resol=2
  ${TEMP}/ioImageFileReaderExtendedFileName_mix1kwl.txt
  ${TEMP}/ioImageFileReaderExtendedFileName_mix1pr.txt
  )

otb_add_test(NAME ioTvImageFileWriterExtendedFileName_gdalco COMMAND otbExtendedFilenameTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/ioTvGDALImageIO_JPEG_20.jpg
  ${TEMP}/ioImageFileWriterExtendedFileName_gdalco.jpg
  otbImageFileWriterWithExtendedFilename
  ${INPUTDATA}/maur_rgb_24bpp.tif
  ${TEMP}/ioImageFileWriterExtendedFileName_gdalco.jpg?&gdal:co:QUALITY=20
  )

otb_add_test(NAME ioTvImageFileReaderExtendedFileName_SkipGeom COMMAND otbExtendedFilenameTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE}/ioImageFileReaderExtendedFileName_Skipgeom_pr.txt
  ${TEMP}/ioImageFileReaderExtendedFileName_Skipgeom_pr.txt
  otbImageFileReaderWithExtendedFilename
  ${INPUTDATA}/ToulouseExtract_WithGeom.tif?&skipgeom=true
  ${TEMP}/ioImageFileReaderExtendedFileName_Skipgeom_kwl.txt
  ${TEMP}/ioImageFileReaderExtendedFileName_Skipgeom_pr.txt
  )

otb_add_test(NAME ioTvImageFileReaderExtendedFileName_SkipPR COMMAND otbExtendedFilenameTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE}/ioImageFileReaderExtendedFileName_SkipPR.txt
  ${TEMP}/ioImageFileReaderExtendedFileName_SkipPR.txt
  otbImageFileReaderWithExtendedFilename
  ${INPUTDATA}/ToulouseExtract_WithGeom.tif?&skipcarto=true
  ${TEMP}/ioImageFileReaderExtendedFileName_SkipPR_kwl.txt
  ${TEMP}/ioImageFileReaderExtendedFileName_SkipPR.txt
  )

