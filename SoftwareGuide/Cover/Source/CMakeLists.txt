
PROJECT( VWSegmentation )

#
#  Find the binary directory where ITK is available
#
FIND_PACKAGE(ITK)
IF(ITK_FOUND)
  INCLUDE(${ITK_USE_FILE})
ELSE(ITK_FOUND)
  MESSAGE(FATAL_ERROR
          "Cannot build InsightApplications without ITK.  Please set ITK_DIR.")
ENDIF(ITK_FOUND)


ADD_EXECUTABLE( VWHistogramRGB VWHistogramRGB.cxx )
ADD_EXECUTABLE( VWHistogramHSV VWHistogramHSV.cxx )
ADD_EXECUTABLE( VWColorSegmentation VWColorSegmentation.cxx )
ADD_EXECUTABLE( VWBlueRemoval VWBlueRemoval.cxx )
ADD_EXECUTABLE( ImageReadExtractWriteRGB ImageReadExtractWriteRGB.cxx )
ADD_EXECUTABLE( BinaryMaskMedianFilter BinaryMaskMedianFilter.cxx )
ADD_EXECUTABLE( AntialiasFilter AntialiasFilter.cxx )
ADD_EXECUTABLE( DilateFilter DilateFilter.cxx )
ADD_EXECUTABLE( NegateFilter NegateFilter.cxx )
ADD_EXECUTABLE( VectorGradientAnisotropicDiffusionFilter VectorGradientAnisotropicDiffusionFilter.cxx )
ADD_EXECUTABLE( BinaryThresholdFilter BinaryThresholdFilter.cxx )
ADD_EXECUTABLE( RescaleIntensityFilter RescaleIntensityFilter.cxx )
ADD_EXECUTABLE( ModelBasedSegmentation ModelBasedSegmentation.cxx )

TARGET_LINK_LIBRARIES( VWHistogramRGB ITKIO ITKBasicFilters ITKCommon ITKStatistics )
TARGET_LINK_LIBRARIES( VWHistogramHSV ITKIO ITKBasicFilters ITKCommon ITKStatistics )
TARGET_LINK_LIBRARIES( VWColorSegmentation  ITKIO ITKBasicFilters ITKCommon ITKStatistics )
TARGET_LINK_LIBRARIES( VWBlueRemoval  ITKIO ITKBasicFilters ITKCommon ITKStatistics )
TARGET_LINK_LIBRARIES( ImageReadExtractWriteRGB  ITKIO ITKBasicFilters ITKCommon ITKStatistics )
TARGET_LINK_LIBRARIES( BinaryMaskMedianFilter  ITKIO ITKBasicFilters ITKCommon ITKStatistics )
TARGET_LINK_LIBRARIES( AntialiasFilter  ITKIO ITKBasicFilters ITKCommon ITKStatistics )
TARGET_LINK_LIBRARIES( DilateFilter  ITKIO ITKBasicFilters ITKCommon ITKStatistics )
TARGET_LINK_LIBRARIES( NegateFilter  ITKIO ITKBasicFilters ITKCommon ITKStatistics )
TARGET_LINK_LIBRARIES( VectorGradientAnisotropicDiffusionFilter  ITKIO ITKBasicFilters ITKCommon ITKStatistics )
TARGET_LINK_LIBRARIES( BinaryThresholdFilter  ITKIO ITKBasicFilters ITKCommon ITKStatistics )
TARGET_LINK_LIBRARIES( RescaleIntensityFilter  ITKIO ITKBasicFilters ITKCommon ITKStatistics )
TARGET_LINK_LIBRARIES( ModelBasedSegmentation  ITKIO ITKNumerics ITKBasicFilters ITKCommon ITKStatistics )
