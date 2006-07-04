#
#  Examples on the use of Iterators.
#
PROJECT(IteratorExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(ImageRandomConstIteratorWithIndex ImageRandomConstIteratorWithIndex.cxx)
TARGET_LINK_LIBRARIES(ImageRandomConstIteratorWithIndex OTBIO OTBCommon ITKIO   )

ADD_EXECUTABLE(NeighborhoodIterators4 NeighborhoodIterators4.cxx)
TARGET_LINK_LIBRARIES(NeighborhoodIterators4 OTBIO OTBCommon ITKIO   )

ADD_EXECUTABLE(NeighborhoodIterators5 NeighborhoodIterators5.cxx)
TARGET_LINK_LIBRARIES(NeighborhoodIterators5 OTBIO OTBCommon ITKIO   )

IF(NOT BORLAND)
ADD_EXECUTABLE(NeighborhoodIterators1 NeighborhoodIterators1.cxx)
TARGET_LINK_LIBRARIES(NeighborhoodIterators1 OTBIO OTBCommon ITKIO   )

ADD_EXECUTABLE(NeighborhoodIterators2 NeighborhoodIterators2.cxx)
TARGET_LINK_LIBRARIES(NeighborhoodIterators2 OTBIO OTBCommon ITKIO   )

ADD_EXECUTABLE(NeighborhoodIterators3 NeighborhoodIterators3.cxx)
TARGET_LINK_LIBRARIES(NeighborhoodIterators3 OTBIO OTBCommon ITKIO   )

ADD_EXECUTABLE(NeighborhoodIterators6 NeighborhoodIterators6.cxx)
TARGET_LINK_LIBRARIES(NeighborhoodIterators6 ITKAlgorithms ITKBasicFilters OTBIO OTBCommon ITKIO )

ADD_EXECUTABLE(ImageRegionIteratorWithIndex ImageRegionIteratorWithIndex.cxx)
TARGET_LINK_LIBRARIES(ImageRegionIteratorWithIndex OTBIO OTBCommon ITKIO   )

ADD_EXECUTABLE(ImageRegionIterator ImageRegionIterator.cxx)
TARGET_LINK_LIBRARIES(ImageRegionIterator OTBIO OTBCommon ITKIO   )

ADD_EXECUTABLE(ImageLinearIteratorWithIndex2 ImageLinearIteratorWithIndex2.cxx)
TARGET_LINK_LIBRARIES(ImageLinearIteratorWithIndex2 OTBIO OTBCommon ITKIO   )

ADD_EXECUTABLE(ImageLinearIteratorWithIndex ImageLinearIteratorWithIndex.cxx)
TARGET_LINK_LIBRARIES(ImageLinearIteratorWithIndex OTBIO OTBCommon ITKIO   )

ADD_EXECUTABLE(ImageSliceIteratorWithIndex ImageSliceIteratorWithIndex.cxx)
TARGET_LINK_LIBRARIES(ImageSliceIteratorWithIndex OTBIO OTBCommon ITKIO   )

ADD_EXECUTABLE(ShapedNeighborhoodIterators1 ShapedNeighborhoodIterators1.cxx)
TARGET_LINK_LIBRARIES(ShapedNeighborhoodIterators1 ITKAlgorithms ITKBasicFilters OTBIO OTBCommon ITKIO )

ADD_EXECUTABLE(ShapedNeighborhoodIterators2 ShapedNeighborhoodIterators2.cxx)
TARGET_LINK_LIBRARIES(ShapedNeighborhoodIterators2 ITKAlgorithms ITKBasicFilters OTBIO OTBCommon ITKIO )

ADD_EXECUTABLE(IteratorsExamples IteratorsExamples.cxx )
TARGET_LINK_LIBRARIES(IteratorsExamples OTBIO OTBCommon ITKIO ITKBasicFilters )

ENDIF(NOT BORLAND)

