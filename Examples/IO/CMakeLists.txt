PROJECT(IOExamples)

ADD_EXECUTABLE(ImageReadWrite ImageReadWrite.cxx )
TARGET_LINK_LIBRARIES(ImageReadWrite OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(StreamingImageReadWrite StreamingImageReadWrite.cxx )
TARGET_LINK_LIBRARIES(StreamingImageReadWrite OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(ExplicitStreamingExample ExplicitStreamingExample.cxx )
TARGET_LINK_LIBRARIES(ExplicitStreamingExample OTBCommon OTBIO ITKCommon ITKIO)


ADD_EXECUTABLE(ImageReadCastWrite ImageReadCastWrite.cxx )
TARGET_LINK_LIBRARIES(ImageReadCastWrite OTBCommon OTBIO ITKCommon ITKIO)

#ADD_EXECUTABLE(ImageReadExtractWrite ImageReadExtractWrite.cxx )
#TARGET_LINK_LIBRARIES(ImageReadExtractWrite OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(ImageReadRegionOfInterestWrite ImageReadRegionOfInterestWrite.cxx )
TARGET_LINK_LIBRARIES(ImageReadRegionOfInterestWrite OTBCommon OTBIO ITKCommon ITKIO)
ADD_EXECUTABLE(RGBImageReadWrite RGBImageReadWrite.cxx )
TARGET_LINK_LIBRARIES(RGBImageReadWrite OTBCommon OTBIO ITKCommon ITKIO)

#ADD_EXECUTABLE(CovariantVectorImageWrite CovariantVectorImageWrite.cxx )
#TARGET_LINK_LIBRARIES(CovariantVectorImageWrite ITKCommon ITKIO)

#ADD_EXECUTABLE(CovariantVectorImageRead CovariantVectorImageRead.cxx )
#TARGET_LINK_LIBRARIES(CovariantVectorImageRead ITKCommon ITKIO)

#ADD_EXECUTABLE(CovariantVectorImageExtractComponent CovariantVectorImageExtractComponent.cxx )
#TARGET_LINK_LIBRARIES(CovariantVectorImageExtractComponent ITKCommon ITKIO)

#ADD_EXECUTABLE(VectorImageReadWrite VectorImageReadWrite.cxx )
#TARGET_LINK_LIBRARIES(VectorImageReadWrite ITKCommon ITKIO)

ADD_EXECUTABLE(ComplexImageReadWrite ComplexImageReadWrite.cxx )
TARGET_LINK_LIBRARIES(ComplexImageReadWrite OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(MultibandImageReadWrite MultibandImageReadWrite.cxx )
TARGET_LINK_LIBRARIES(MultibandImageReadWrite OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(ExtractROI ExtractROI.cxx )
TARGET_LINK_LIBRARIES(ExtractROI OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(ImageSeriesIOExample ImageSeriesIOExample.cxx )
TARGET_LINK_LIBRARIES(ImageSeriesIOExample OTBCommon OTBIO ITKCommon ITKIO)


ADD_EXECUTABLE(MetadataExample MetadataExample.cxx )
TARGET_LINK_LIBRARIES(MetadataExample OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(DXFReaderExample DXFReaderExample.cxx )
TARGET_LINK_LIBRARIES(DXFReaderExample OTBCommon OTBIO ITKCommon ITKIO  ${OTB_IO_UTILITIES_DEPENDENT_LIBRARIES})

ADD_EXECUTABLE(VectorDataIOExample VectorDataIOExample.cxx )
TARGET_LINK_LIBRARIES(VectorDataIOExample OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(DEMToImageGenerator DEMToImageGenerator.cxx )
TARGET_LINK_LIBRARIES(DEMToImageGenerator OTBProjections OTBIO OTBCommon ITKCommon ITKIO otbossim )

ADD_EXECUTABLE(DEMToOrthoImageGenerator DEMToOrthoImageGenerator.cxx )
TARGET_LINK_LIBRARIES(DEMToOrthoImageGenerator OTBProjections OTBIO OTBCommon ITKCommon ITKIO otbossim )

IF(ITK_USE_REVIEW)
  ADD_EXECUTABLE(LidarToImageExample LidarToImageExample.cxx )
  TARGET_LINK_LIBRARIES(LidarToImageExample OTBIO OTBCommon ITKCommon ITKIO otbossim )
ENDIF(ITK_USE_REVIEW)

#ADD_EXECUTABLE(ImageReadExportVTK ImageReadExportVTK.cxx )
#TARGET_LINK_LIBRARIES(ImageReadExportVTK ITKCommon ITKIO)

#ADD_EXECUTABLE(RGBImageSeriesReadWrite RGBImageSeriesReadWrite.cxx )
#TARGET_LINK_LIBRARIES(RGBImageSeriesReadWrite ITKCommon ITKIO)

#ADD_EXECUTABLE(ImageSeriesReadWrite ImageSeriesReadWrite.cxx )
#TARGET_LINK_LIBRARIES(ImageSeriesReadWrite ITKCommon ITKIO)

#ADD_EXECUTABLE(ImageReadImageSeriesWrite ImageReadImageSeriesWrite.cxx )
#TARGET_LINK_LIBRARIES(ImageReadImageSeriesWrite ITKCommon ITKIO)

#ADD_EXECUTABLE(DicomSeriesReadImageWrite DicomSeriesReadImageWrite.cxx )
#TARGET_LINK_LIBRARIES(DicomSeriesReadImageWrite ITKCommon ITKIO)

#ADD_EXECUTABLE(DicomSeriesReadImageWrite2 DicomSeriesReadImageWrite2.cxx )
#TARGET_LINK_LIBRARIES(DicomSeriesReadImageWrite2 ITKCommon ITKIO)

#ADD_EXECUTABLE(DicomSeriesReadPrintTags DicomSeriesReadPrintTags.cxx )
#TARGET_LINK_LIBRARIES(DicomSeriesReadPrintTags ITKCommon ITKIO)

#ADD_EXECUTABLE(DicomImageReadPrintTags DicomImageReadPrintTags.cxx )
#TARGET_LINK_LIBRARIES(DicomImageReadPrintTags ITKCommon ITKIO)

#ADD_EXECUTABLE(DicomImageReadWrite DicomImageReadWrite.cxx )
#TARGET_LINK_LIBRARIES(DicomImageReadWrite ITKCommon ITKIO)

#ADD_EXECUTABLE(DicomImageReadChangeHeaderWrite DicomImageReadChangeHeaderWrite.cxx )
#TARGET_LINK_LIBRARIES(DicomImageReadChangeHeaderWrite ITKCommon ITKIO)

#ADD_EXECUTABLE(DicomSeriesReadSeriesWrite DicomSeriesReadSeriesWrite.cxx )
#TARGET_LINK_LIBRARIES(DicomSeriesReadSeriesWrite ITKCommon ITKIO)

#ADD_EXECUTABLE(DicomPrintPatientInformation DicomPrintPatientInformation.cxx )
#TARGET_LINK_LIBRARIES(DicomPrintPatientInformation ITKCommon ITKIO)

#ADD_EXECUTABLE(TransformReadWrite TransformReadWrite.cxx )
#TARGET_LINK_LIBRARIES(TransformReadWrite ITKCommon ITKIO)

IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/IO)
SET(DATA ${OTB_DATA_ROOT}/Input)

SET(INPUTDATA ${OTB_SOURCE_DIR}/Examples/Data)
SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbIOExamplesTests)

SET(TOL 0.0)


# ------- ExtractROITest----------

ADD_TEST(ExtractROITest ${EXE_TESTS}
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

ADD_TEST(DEMToImageGeneratorTest ${EXE_TESTS}
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
ADD_TEST(LidarToImageExampleTest ${EXE_TESTS}
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
ENDIF(ITK_USE_REVIEW)

ADD_EXECUTABLE(otbIOExamplesTests otbIOExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbIOExamplesTests otbossim OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBProjections ITKIO ITKAlgorithms ITKStatistics ITKCommon  ${OTB_IO_UTILITIES_DEPENDENT_LIBRARIES})

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
