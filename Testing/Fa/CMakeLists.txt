
ADD_EXECUTABLE(0000307-ExtractROICompareRegion  0000307-ExtractROICompareRegionsImplementations.cxx)
TARGET_LINK_LIBRARIES(0000307-ExtractROICompareRegion OTBIO OTBCommon)

ADD_EXECUTABLE(0000479-WriteInt8Image 0000479-WriteInt8Image.cxx)
TARGET_LINK_LIBRARIES(0000479-WriteInt8Image OTBIO OTBCommon)

IF(BUILD_APPLICATIONS)
ADD_EXECUTABLE(0000478-UncaughtException 0000478-UncaughtException.cxx)
TARGET_LINK_LIBRARIES(0000478-UncaughtException OTBIO OTBCommon OTBApplicationEngine)
ENDIF(BUILD_APPLICATIONS)

ADD_EXECUTABLE(AlignementsQB  AlignementsQB.cxx)
TARGET_LINK_LIBRARIES(AlignementsQB OTBFeatureExtraction OTBIO OTBCommon)

ADD_EXECUTABLE(MapActivation  MapActivation.cxx)
TARGET_LINK_LIBRARIES(MapActivation OTBFeatureExtraction OTBIO OTBCommon)

ADD_EXECUTABLE(StreamingStat  StreamingStat.cxx)
TARGET_LINK_LIBRARIES(StreamingStat OTBFeatureExtraction OTBIO OTBCommon)

ADD_EXECUTABLE(0000041-mean_shift  0000041-mean_shift.cxx)
TARGET_LINK_LIBRARIES(0000041-mean_shift OTBIO OTBCommon OTBBasicFilters)

ADD_EXECUTABLE(00134-feature_extraction  00134-feature_extraction.cxx)
TARGET_LINK_LIBRARIES(00134-feature_extraction OTBIO OTBCommon OTBBasicFilters OTBFeatureExtraction)

IF(OTB_USE_VISU_GUI)
ADD_EXECUTABLE(0000132-jpg 0000132-jpg.cxx )
TARGET_LINK_LIBRARIES(0000132-jpg OTBIO OTBVisualization)
ENDIF(OTB_USE_VISU_GUI)

ADD_EXECUTABLE(0000094-PolygonsVectorization 0000094-PolygonsVectorization.cxx)
TARGET_LINK_LIBRARIES(0000094-PolygonsVectorization OTBIO OTBCommon)

ADD_EXECUTABLE(0000169-fftcomplextocomplex  0000169-fftcomplextocomplex.cxx)
TARGET_LINK_LIBRARIES(0000169-fftcomplextocomplex OTBIO OTBCommon)

ADD_EXECUTABLE(000295-MeanShiftVectorImageFilterWithStreamingShrink 000295-MeanShiftVectorImageFilterWithStreamingShrink.cxx)
TARGET_LINK_LIBRARIES(000295-MeanShiftVectorImageFilterWithStreamingShrink OTBIO OTBCommon)

ADD_EXECUTABLE(SensorModelBorder SensorModelBorder.cxx)
TARGET_LINK_LIBRARIES(SensorModelBorder OTBIO OTBCommon OTBBasicFilters OTBProjections)

ADD_EXECUTABLE(WriteUnsignedLong WriteUnsignedLong.cxx)
TARGET_LINK_LIBRARIES(WriteUnsignedLong OTBIO OTBCommon)

ADD_EXECUTABLE(0000209-SVMValidationLinearlySeparableProbEstimation 0000209-SVMValidationLinearlySeparableProbEstimation.cxx)
TARGET_LINK_LIBRARIES(0000209-SVMValidationLinearlySeparableProbEstimation OTBIO OTBLearning)

ADD_EXECUTABLE(0000428-CastImageFilterStreaming  0000428-CastImageFilterStreaming.cxx)
TARGET_LINK_LIBRARIES(0000428-CastImageFilterStreaming OTBIO OTBCommon OTBBasicFilters)

ADD_EXECUTABLE(0000433-LineSegmentDetector_8b_16b_compare 0000433-LineSegmentDetector_8b_16b_compare.cxx)
TARGET_LINK_LIBRARIES(0000433-LineSegmentDetector_8b_16b_compare OTBIO OTBFeatureExtraction)

ADD_EXECUTABLE(otbVectorDataToLabelImageFilterWithoutReader  otbVectorDataToLabelImageFilterWithoutReader.cxx)
TARGET_LINK_LIBRARIES(otbVectorDataToLabelImageFilterWithoutReader OTBBasicFilters OTBIO OTBCommon)

IF(BUILD_APPLICATIONS)
  ADD_EXECUTABLE(0000436-WrapperInputImage_GetImage 0000436-WrapperInputImage_GetImage.cxx)
  TARGET_LINK_LIBRARIES(0000436-WrapperInputImage_GetImage OTBApplicationEngine)
ENDIF(BUILD_APPLICATIONS)

ADD_EXECUTABLE(0000437-WriteImageCentOS 0000437-WriteImageCentOS.cxx)
TARGET_LINK_LIBRARIES(0000437-WriteImageCentOS OTBIO OTBCommon)

ADD_EXECUTABLE(0000495-StreamingImageFileWriterProgressReporting
               0000495-StreamingImageFileWriterProgressReporting.cxx)
TARGET_LINK_LIBRARIES(0000495-StreamingImageFileWriterProgressReporting OTBIO OTBCommon)

SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

# Always add the Resolved Fa to be compiled and tested
ADD_SUBDIRECTORY(Resolved)

# Add the failing bugs
IF(BUILD_BUG_TRACKER_TESTS)
  ADD_SUBDIRECTORY(Failing)
ENDIF(BUILD_BUG_TRACKER_TESTS)
