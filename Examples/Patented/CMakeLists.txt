PROJECT(PatentedExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")


ADD_EXECUTABLE(FuzzyConnectednessImageFilter FuzzyConnectednessImageFilter.cxx )
TARGET_LINK_LIBRARIES(FuzzyConnectednessImageFilter ITKNumerics ITKIO)

ADD_EXECUTABLE(HybridSegmentationFuzzyVoronoi HybridSegmentationFuzzyVoronoi.cxx )
TARGET_LINK_LIBRARIES(HybridSegmentationFuzzyVoronoi ITKNumerics ITKIO)


