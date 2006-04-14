PROJECT(FeatureExtraction)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(AlignmentsExample AlignmentsExample.cxx )
TARGET_LINK_LIBRARIES(AlignmentsExample OTBCommon OTBIO ITKCommon
ITKIO)

ADD_EXECUTABLE(LineRatioDetectorExample LineRatioDetectorExample.cxx )
TARGET_LINK_LIBRARIES(LineRatioDetectorExample OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(TouziEdgeDetectorExample TouziEdgeDetectorExample.cxx )
TARGET_LINK_LIBRARIES(TouziEdgeDetectorExample OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(HarrisExample HarrisExample.cxx )
TARGET_LINK_LIBRARIES(HarrisExample OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(ComplexMomentImageExample ComplexMomentImageExample.cxx )
TARGET_LINK_LIBRARIES(ComplexMomentImageExample OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(ComplexMomentPathExample ComplexMomentPathExample.cxx )
TARGET_LINK_LIBRARIES(ComplexMomentPathExample OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(HuMomentImageExample HuMomentImageExample.cxx )
TARGET_LINK_LIBRARIES(HuMomentImageExample OTBCommon OTBIO ITKCommon ITKIO)

