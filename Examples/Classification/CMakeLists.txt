PROJECT(StatisticsExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(KdTreeBasedKMeansClustering KdTreeBasedKMeansClustering.cxx )
TARGET_LINK_LIBRARIES(KdTreeBasedKMeansClustering OTBCommon ITKCommon ITKStatistics)

ADD_EXECUTABLE(ScalarImageKmeansClassifier ScalarImageKmeansClassifier.cxx )
TARGET_LINK_LIBRARIES(ScalarImageKmeansClassifier OTBIO OTBCommon ITKIO ITKCommon )

ADD_EXECUTABLE(ScalarImageKmeansModelEstimator ScalarImageKmeansModelEstimator.cxx )
TARGET_LINK_LIBRARIES(ScalarImageKmeansModelEstimator  OTBIO OTBCommon ITKIO ITKCommon )

ADD_EXECUTABLE(BayesianPluginClassifier BayesianPluginClassifier.cxx )
TARGET_LINK_LIBRARIES(BayesianPluginClassifier OTBIO OTBCommon ITKCommon ITKStatistics)

ADD_EXECUTABLE(ExpectationMaximizationMixtureModelEstimator ExpectationMaximizationMixtureModelEstimator.cxx )
TARGET_LINK_LIBRARIES(ExpectationMaximizationMixtureModelEstimator OTBIO OTBCommon ITKCommon ITKStatistics)

ADD_EXECUTABLE(ScalarImageMarkovRandomField1 ScalarImageMarkovRandomField1.cxx )
TARGET_LINK_LIBRARIES(ScalarImageMarkovRandomField1 OTBIO OTBCommon ITKIO ITKCommon )


#ADD_EXECUTABLE( BayesianClassifierInitializer BayesianClassifierInitializer.cxx )
#TARGET_LINK_LIBRARIES(BayesianClassifierInitializer ITKCommon ITKStatistics ITKIO)

#ADD_EXECUTABLE( BayesianClassifier BayesianClassifier.cxx )
#TARGET_LINK_LIBRARIES(BayesianClassifier ITKCommon ITKStatistics ITKIO)

#ADD_EXECUTABLE(EuclideanDistance EuclideanDistance.cxx )
#TARGET_LINK_LIBRARIES(EuclideanDistance ITKCommon)


#ADD_EXECUTABLE(GaussianDensityFunction GaussianDensityFunction.cxx )
#TARGET_LINK_LIBRARIES(GaussianDensityFunction ITKCommon)

#ADD_EXECUTABLE(Histogram Histogram.cxx )
#TARGET_LINK_LIBRARIES(Histogram ITKCommon ITKStatistics)

#ADD_EXECUTABLE(ImageToListAdaptor ImageToListAdaptor.cxx )
#TARGET_LINK_LIBRARIES(ImageToListAdaptor ITKCommon)

#ADD_EXECUTABLE(KdTree KdTree.cxx )
#TARGET_LINK_LIBRARIES(KdTree ITKCommon)


#ADD_EXECUTABLE(ListSample ListSample.cxx )
#TARGET_LINK_LIBRARIES(ListSample ITKCommon)

#ADD_EXECUTABLE(ListSampleToHistogramFilter ListSampleToHistogramFilter.cxx )
#TARGET_LINK_LIBRARIES(ListSampleToHistogramFilter ITKCommon ITKStatistics)

#ADD_EXECUTABLE(ListSampleToHistogramGenerator ListSampleToHistogramGenerator.cxx )
#TARGET_LINK_LIBRARIES(ListSampleToHistogramGenerator ITKCommon ITKStatistics)

#ADD_EXECUTABLE(MaximumDecisionRule MaximumDecisionRule.cxx )
#TARGET_LINK_LIBRARIES(MaximumDecisionRule ITKCommon)

#ADD_EXECUTABLE(MaximumRatioDecisionRule MaximumRatioDecisionRule.cxx )
#TARGET_LINK_LIBRARIES(MaximumRatioDecisionRule ITKCommon)

#ADD_EXECUTABLE(MembershipSample MembershipSample.cxx )
#TARGET_LINK_LIBRARIES(MembershipSample ITKCommon)

#ADD_EXECUTABLE(MinimumDecisionRule MinimumDecisionRule.cxx )
#TARGET_LINK_LIBRARIES(MinimumDecisionRule ITKCommon)

#ADD_EXECUTABLE(NeighborhoodSampler NeighborhoodSampler.cxx )
#TARGET_LINK_LIBRARIES(NeighborhoodSampler ITKCommon)

#ADD_EXECUTABLE(NormalVariateGenerator NormalVariateGenerator.cxx )
#TARGET_LINK_LIBRARIES(NormalVariateGenerator ITKCommon ITKStatistics)

#ADD_EXECUTABLE(PointSetToListAdaptor PointSetToListAdaptor.cxx )
#TARGET_LINK_LIBRARIES(PointSetToListAdaptor ITKCommon)

#ADD_EXECUTABLE(NormalVariateGenerator NormalVariateGenerator.cxx )
#TARGET_LINK_LIBRARIES(NormalVariateGenerator ITKCommon)

#ADD_EXECUTABLE(SampleStatistics SampleStatistics.cxx )
#TARGET_LINK_LIBRARIES(SampleStatistics ITKCommon)

#ADD_EXECUTABLE(SampleToHistogramProjectionFilter SampleToHistogramProjectionFilter.cxx )
#TARGET_LINK_LIBRARIES(SampleToHistogramProjectionFilter ITKCommon
#                                                        ITKStatistics)

#ADD_EXECUTABLE(SampleSorting SampleSorting.cxx )
#TARGET_LINK_LIBRARIES(SampleSorting ITKCommon)

#ADD_EXECUTABLE(WeightedSampleStatistics WeightedSampleStatistics.cxx )
#TARGET_LINK_LIBRARIES(WeightedSampleStatistics ITKCommon)

#ADD_EXECUTABLE(ImageHistogram1 ImageHistogram1.cxx )
#TARGET_LINK_LIBRARIES(ImageHistogram1 ITKIO ITKCommon ITKStatistics )

#ADD_EXECUTABLE(ImageHistogram2 ImageHistogram2.cxx )
#TARGET_LINK_LIBRARIES(ImageHistogram2 ITKIO ITKCommon ITKStatistics )

#ADD_EXECUTABLE(ImageHistogram3 ImageHistogram3.cxx )
#TARGET_LINK_LIBRARIES(ImageHistogram3 ITKIO ITKCommon ITKStatistics )

#ADD_EXECUTABLE(ImageHistogram4 ImageHistogram4.cxx )
#TARGET_LINK_LIBRARIES(ImageHistogram4 ITKIO ITKCommon ITKStatistics )

#ADD_EXECUTABLE(ImageEntropy1 ImageEntropy1.cxx )
#TARGET_LINK_LIBRARIES(ImageEntropy1 ITKIO ITKCommon ITKStatistics)



