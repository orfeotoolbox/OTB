PROJECT(IOExamples)

ADD_EXECUTABLE(ImageReadWrite ImageReadWrite.cxx )
TARGET_LINK_LIBRARIES(ImageReadWrite OTBCommon OTBIO)

ADD_EXECUTABLE(StreamingImageReadWrite StreamingImageReadWrite.cxx )
TARGET_LINK_LIBRARIES(StreamingImageReadWrite OTBCommon OTBIO)

ADD_EXECUTABLE(ExplicitStreamingExample ExplicitStreamingExample.cxx )
TARGET_LINK_LIBRARIES(ExplicitStreamingExample OTBCommon OTBIO)


ADD_EXECUTABLE(ImageReadCastWrite ImageReadCastWrite.cxx )
TARGET_LINK_LIBRARIES(ImageReadCastWrite OTBCommon OTBIO)

#ADD_EXECUTABLE(ImageReadExtractWrite ImageReadExtractWrite.cxx )
#TARGET_LINK_LIBRARIES(ImageReadExtractWrite OTBCommon OTBIO)

ADD_EXECUTABLE(ImageReadRegionOfInterestWrite ImageReadRegionOfInterestWrite.cxx )
TARGET_LINK_LIBRARIES(ImageReadRegionOfInterestWrite OTBCommon OTBIO)
ADD_EXECUTABLE(RGBImageReadWrite RGBImageReadWrite.cxx )
TARGET_LINK_LIBRARIES(RGBImageReadWrite OTBCommon OTBIO)

#ADD_EXECUTABLE(CovariantVectorImageWrite CovariantVectorImageWrite.cxx )
#TARGET_LINK_LIBRARIES(CovariantVectorImageWrite)

#ADD_EXECUTABLE(CovariantVectorImageRead CovariantVectorImageRead.cxx )
#TARGET_LINK_LIBRARIES(CovariantVectorImageRead)

#ADD_EXECUTABLE(CovariantVectorImageExtractComponent CovariantVectorImageExtractComponent.cxx )
#TARGET_LINK_LIBRARIES(CovariantVectorImageExtractComponent)

#ADD_EXECUTABLE(VectorImageReadWrite VectorImageReadWrite.cxx )
#TARGET_LINK_LIBRARIES(VectorImageReadWrite)

ADD_EXECUTABLE(ComplexImageReadWrite ComplexImageReadWrite.cxx )
TARGET_LINK_LIBRARIES(ComplexImageReadWrite OTBCommon OTBIO)

ADD_EXECUTABLE(MultibandImageReadWrite MultibandImageReadWrite.cxx )
TARGET_LINK_LIBRARIES(MultibandImageReadWrite OTBCommon OTBIO)

ADD_EXECUTABLE(ExtractROI ExtractROI.cxx )
TARGET_LINK_LIBRARIES(ExtractROI OTBCommon OTBIO)

ADD_EXECUTABLE(ImageSeriesIOExample ImageSeriesIOExample.cxx )
TARGET_LINK_LIBRARIES(ImageSeriesIOExample OTBCommon OTBIO)


ADD_EXECUTABLE(MetadataExample MetadataExample.cxx )
TARGET_LINK_LIBRARIES(MetadataExample OTBCommon OTBIO)

ADD_EXECUTABLE(DXFReaderExample DXFReaderExample.cxx )
TARGET_LINK_LIBRARIES(DXFReaderExample OTBCommon OTBIO  ${OTB_IO_UTILITIES_DEPENDENT_LIBRARIES})

ADD_EXECUTABLE(VectorDataIOExample VectorDataIOExample.cxx )
TARGET_LINK_LIBRARIES(VectorDataIOExample OTBCommon OTBIO)

ADD_EXECUTABLE(DEMToImageGenerator DEMToImageGenerator.cxx )
TARGET_LINK_LIBRARIES(DEMToImageGenerator OTBProjections OTBIO OTBCommon )

ADD_EXECUTABLE(DEMToOrthoImageGenerator DEMToOrthoImageGenerator.cxx )
TARGET_LINK_LIBRARIES(DEMToOrthoImageGenerator OTBProjections OTBIO OTBCommon )

ADD_EXECUTABLE(LidarReaderExample LidarReaderExample.cxx )
TARGET_LINK_LIBRARIES(LidarReaderExample OTBIO OTBCommon )

IF(ITK_USE_REVIEW)
  ADD_EXECUTABLE(LidarToImageExample LidarToImageExample.cxx )
  TARGET_LINK_LIBRARIES(LidarToImageExample OTBIO OTBCommon )
ENDIF(ITK_USE_REVIEW)

IF( OTB_USE_CURL )
  ADD_EXECUTABLE(TileMapImageIOExample TileMapImageIOExample.cxx)
  TARGET_LINK_LIBRARIES(TileMapImageIOExample OTBIO OTBCommon)
ENDIF( OTB_USE_CURL )


#ADD_EXECUTABLE(ImageReadExportVTK ImageReadExportVTK.cxx )
#TARGET_LINK_LIBRARIES(ImageReadExportVTK)

#ADD_EXECUTABLE(RGBImageSeriesReadWrite RGBImageSeriesReadWrite.cxx )
#TARGET_LINK_LIBRARIES(RGBImageSeriesReadWrite)

#ADD_EXECUTABLE(ImageSeriesReadWrite ImageSeriesReadWrite.cxx )
#TARGET_LINK_LIBRARIES(ImageSeriesReadWrite)

#ADD_EXECUTABLE(ImageReadImageSeriesWrite ImageReadImageSeriesWrite.cxx )
#TARGET_LINK_LIBRARIES(ImageReadImageSeriesWrite)

#ADD_EXECUTABLE(DicomSeriesReadImageWrite DicomSeriesReadImageWrite.cxx )
#TARGET_LINK_LIBRARIES(DicomSeriesReadImageWrite)

#ADD_EXECUTABLE(DicomSeriesReadImageWrite2 DicomSeriesReadImageWrite2.cxx )
#TARGET_LINK_LIBRARIES(DicomSeriesReadImageWrite2)

#ADD_EXECUTABLE(DicomSeriesReadPrintTags DicomSeriesReadPrintTags.cxx )
#TARGET_LINK_LIBRARIES(DicomSeriesReadPrintTags)

#ADD_EXECUTABLE(DicomImageReadPrintTags DicomImageReadPrintTags.cxx )
#TARGET_LINK_LIBRARIES(DicomImageReadPrintTags)

#ADD_EXECUTABLE(DicomImageReadWrite DicomImageReadWrite.cxx )
#TARGET_LINK_LIBRARIES(DicomImageReadWrite)

#ADD_EXECUTABLE(DicomImageReadChangeHeaderWrite DicomImageReadChangeHeaderWrite.cxx )
#TARGET_LINK_LIBRARIES(DicomImageReadChangeHeaderWrite)

#ADD_EXECUTABLE(DicomSeriesReadSeriesWrite DicomSeriesReadSeriesWrite.cxx )
#TARGET_LINK_LIBRARIES(DicomSeriesReadSeriesWrite)

#ADD_EXECUTABLE(DicomPrintPatientInformation DicomPrintPatientInformation.cxx )
#TARGET_LINK_LIBRARIES(DicomPrintPatientInformation)

#ADD_EXECUTABLE(TransformReadWrite TransformReadWrite.cxx )
#TARGET_LINK_LIBRARIES(TransformReadWrite)

IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/IO)
SET(DATA ${OTB_DATA_ROOT}/Input)

SET(INPUTDATA ${OTB_DATA_ROOT}/Examples)
SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)


SET(EXE_TESTS1  ${CXX_TEST_PATH}/otbIOExamplesTests1)
SET(EXE_TESTS2  ${CXX_TEST_PATH}/otbIOExamplesTests2)

SET(TOL 0.0)
SET(HIGHTOL 5000.0)

# ------- ExtractROITest----------

ADD_TEST(ExtractROITest ${EXE_TESTS1}
	--compare-n-images ${TOL} 2
	${BASELINE}/ROI_IMAGERY_RGB.png
	${TEMP}/ROI_IMAGERY_RGB.png

	${BASELINE}/ROI_IMAGERY_MIR.png
	${TEMP}/ROI_IMAGERY_MIR.png
	ExtractROITest
	${INPUTDATA}/IMAGERY_SSECH.tif
	${TEMP}/ROI_IMAGERY_RGB.png
	${TEMP}/ROI_IMAGERY_MIR.png
    0 0 100 100
)

ADD_TEST(DEMToImageGeneratorTest ${EXE_TESTS1}
--compare-image ${TOL}  ${BASELINE}/DEMToImageGenerator.tif
                        ${TEMP}/DEMToImageGenerator.tif
        DEMToImageGeneratorTest
         ${TEMP}/DEMToImageGenerator.tif
	 ${TEMP}/pretty_DEMToImageGenerator.png
         6.5
         45.5
         500
         500
	 0.002
	 -0.002
	  ${INPUTDATA}/DEM_srtm
         )

IF(ITK_USE_REVIEW)
ADD_TEST(LidarToImageExampleTest ${EXE_TESTS1}
--compare-image ${TOL}  ${BASELINE}/lidar-image-4.hdr
                        ${TEMP}/lidar-image-4.hdr
         LidarToImageExampleTest
         ${DATA}/TO_core_last_zoom.las
         ${TEMP}/lidar-image-4.hdr
	 ${TEMP}/lidar-image-4.png
         1.0
         5
         4
         )
ADD_TEST(LidarToImageExample2Test ${EXE_TESTS1}
--compare-image ${TOL}  ${BASELINE}/lidar-image-8.hdr
                        ${TEMP}/lidar-image-8.hdr
         LidarToImageExampleTest
         ${DATA}/TO_core_last_zoom.las
         ${TEMP}/lidar-image-8.hdr
	 ${TEMP}/lidar-image-8.png
         1.0
         5
         8
         )
ENDIF(ITK_USE_REVIEW)

IF( OTB_USE_CURL )
ADD_TEST(TileMapImageIOExampleTest ${EXE_TESTS2}
--compare-image ${HIGHTOL}  ${BASELINE}/openStreetMap.png
                        ${TEMP}/openStreetMap.png
         TileMapImageIOExampleTest
         ${INPUTDATA}/osmfile.otb
         ${TEMP}/openStreetMap.png
         ${TEMP}
         1.4835345
         43.55968261
         12
         )
ENDIF( OTB_USE_CURL )


ADD_EXECUTABLE(otbIOExamplesTests1 otbIOExamplesTests1.cxx)
# TARGET_LINK_LIBRARIES(otbIOExamplesTests1 OTBIO OTBBasicFilters OTBCommon OTBDisparityMap  OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBProjections ITKAlgorithms ITKStatistics  ${OTB_IO_UTILITIES_DEPENDENT_LIBRARIES})
TARGET_LINK_LIBRARIES(otbIOExamplesTests1 OTBIO OTBCommon)

IF( OTB_USE_CURL )
  ADD_EXECUTABLE(otbIOExamplesTests2 otbIOExamplesTests2.cxx)
#   TARGET_LINK_LIBRARIES(otbIOExamplesTests2 OTBCommon OTBIO ${OTB_IO_UTILITIES_DEPENDENT_LIBRARIES} ${CURL_LIBRARY} )
  TARGET_LINK_LIBRARIES(otbIOExamplesTests2 OTBCommon OTBIO ${CURL_LIBRARY} )
ENDIF( OTB_USE_CURL )

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
