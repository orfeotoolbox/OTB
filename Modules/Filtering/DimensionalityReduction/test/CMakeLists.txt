otb_module_test()

set(OTBDimensionalityReductionTests
otbDimensionalityReductionTestDriver.cxx
otbNormalizeInnerProductPCAImageFilterNew.cxx
otbFastICAImageFilter.cxx
otbNormalizeInnerProductPCAImageFilter.cxx
otbMaximumAutocorrelationFactorImageFilter.cxx
otbMNFImageFilter.cxx
otbInnerProductPCAImageFilter.cxx
otbEstimateInnerProductPCAImageFilterNew.cxx
otbEstimateInnerProductPCAImageFilter.cxx
otbPCAImageFilter.cxx
otbAngularProjectionSetImageFilter.cxx
otbAngularProjectionSetImageFilterNew.cxx
otbNAPCAImageFilter.cxx
otbInnerProductPCAImageFilterNew.cxx
otbSparseWvltToAngleMapperListFilterNew.cxx
otbAngularProjectionImageFilterNew.cxx
otbAngularProjectionImageFilter.cxx
otbAngularProjectionBinaryImageFilter.cxx
otbSparseWvltToAngleMapperListFilter.cxx
otbLocalActivityVectorImageFilter.cxx
)

add_executable(otbDimensionalityReductionTestDriver ${OTBDimensionalityReductionTests})
target_link_libraries(otbDimensionalityReductionTestDriver ${OTBDimensionalityReduction-Test_LIBRARIES})
otb_module_target_label(otbDimensionalityReductionTestDriver)

# Tests Declaration

otb_add_test(NAME bfTuNormalizeInnerProductPCAImageFilterNew COMMAND otbDimensionalityReductionTestDriver
  otbNormalizeInnerProductPCAImageFilterNew )

otb_add_test(NAME bfTuFastICAImageFilterNew COMMAND otbDimensionalityReductionTestDriver
  otbFastICAImageFilterNewTest)

otb_add_test(NAME bfTvFastICAImageFilter COMMAND otbDimensionalityReductionTestDriver
  --compare-n-images ${EPSILON_7} 2
  ${BASELINE}/hyTvFastICAImageFilter.tif
  ${TEMP}/hyTvFastICAImageFilter.tif
  ${BASELINE}/hyTvFastICAImageFilterInv.tif
  ${TEMP}/hyTvFastICAImageFilterInv.tif
  otbFastICAImageFilterTest
  -in ${INPUTDATA}/cupriteSubHsi.tif
  -inv ${TEMP}/hyTvFastICAImageFilterInv.tif
  -out ${TEMP}/hyTvFastICAImageFilter.tif)

otb_add_test(NAME bfTuFastICAInternalOptimizerVectorImageFilterNew COMMAND otbDimensionalityReductionTestDriver
  otbFastICAInternalOptimizerVectorImageFilterNewTest)

otb_add_test(NAME bfTvNormalizeInnerProductPCAImageFilter COMMAND otbDimensionalityReductionTestDriver
  --compare-image ${EPSILON_12}
  ${BASELINE}/bfNormalizeInnerProductPCAImageFilter.tif
  ${TEMP}/bfNormalizeInnerProductPCAImageFilter.tif
  otbNormalizeInnerProductPCAImageFilter
  ${INPUTDATA}/poupees_sub_3c.png
  ${TEMP}/bfNormalizeInnerProductPCAImageFilter.tif
  )

otb_add_test(NAME bfTvMaximumAutocorrelationFactorImageFilter COMMAND otbDimensionalityReductionTestDriver
  --compare-image 0.0001
  ${BASELINE}/bfTvMaximumAutocorrelationFactorImageFilterOutput.tif
  ${TEMP}/bfTvMaximumAutocorrelationFactorImageFilterOutput.tif
  otbMaximumAutocorrelationFactorImageFilter
  ${INPUTDATA}/cupriteSubHsi.tif
  ${TEMP}/bfTvMaximumAutocorrelationFactorImageFilterOutput.tif
  )

otb_add_test(NAME bfTuMaximumAutocorrelationFactorImageFilterNew COMMAND otbDimensionalityReductionTestDriver
  otbMaximumAutocorrelationFactorImageFilterNew
  )

otb_add_test(NAME bfTvMNFImageFilter3 COMMAND otbDimensionalityReductionTestDriver
  --compare-n-images ${EPSILON_7} 2
  ${BASELINE}/bfTvMNFImageFilter3.tif
  ${TEMP}/bfTvMNFImageFilter3.tif
  ${BASELINE}/bfTvMNFImageFilter3Inv.tif
  ${TEMP}/bfTvMNFImageFilter3Inv.tif
  otbMNFImageFilterTest
  -in ${INPUTDATA}/cupriteSubHsi.tif
  -out ${TEMP}/bfTvMNFImageFilter3.tif
  -inv ${TEMP}/bfTvMNFImageFilter3Inv.tif
  -norm)

otb_add_test(NAME bfTvMNFImageFilter2 COMMAND otbDimensionalityReductionTestDriver
  --compare-image ${EPSILON_7}
  ${INPUTDATA}/cupriteSubHsi.tif
  ${TEMP}/bfTvMNFImageFilter2Inv.tif
  otbMNFImageFilterTest
  -in ${INPUTDATA}/cupriteSubHsi.tif
  -out ${TEMP}/bfTvMNFImageFilter2.tif
  -inv ${TEMP}/bfTvMNFImageFilter2Inv.tif
  -norm)

otb_add_test(NAME bfTvMNFImageFilter1 COMMAND otbDimensionalityReductionTestDriver
  --compare-image ${EPSILON_7}
  ${INPUTDATA}/cupriteSubHsi.tif
  ${TEMP}/bfTvMNFImageFilter1Inv.tif
  otbMNFImageFilterTest
  -in ${INPUTDATA}/cupriteSubHsi.tif
  -out ${TEMP}/bfTvMNFImageFilter1.tif
  -inv ${TEMP}/bfTvMNFImageFilter1Inv.tif)

otb_add_test(NAME bfTvMNFImageFilter4 COMMAND otbDimensionalityReductionTestDriver
  --compare-n-images ${EPSILON_7} 2
  ${BASELINE}/bfTvMNFImageFilter4.tif
  ${TEMP}/bfTvMNFImageFilter4.tif
  ${BASELINE}/bfTvMNFImageFilter4Inv.tif
  ${TEMP}/bfTvMNFImageFilter4Inv.tif
  otbMNFImageFilterTest
  -in ${INPUTDATA}/cupriteSubHsi.tif
  -out ${TEMP}/bfTvMNFImageFilter4.tif
  -inv ${TEMP}/bfTvMNFImageFilter4Inv.tif
  -norm
  -n 4)

otb_add_test(NAME bfTvInnerProductPCAImageFilter_PC12 COMMAND otbDimensionalityReductionTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfInnerProductPCAImageFilter_PC12.tif
  ${TEMP}/bfInnerProductPCAImageFilter_PC12.tif
  otbInnerProductPCAImageFilter
  ${INPUTDATA}/poupees_sub_3c.png
  ${TEMP}/bfInnerProductPCAImageFilter_PC12.tif
  0 # Flag generate Mean component ?
  1 #center data
  2 # Nb Of Principal Components
  1 # use unbiased estimator
  ${TEMP}/bfInnerProductPCAImageFilter_PC12_1.tif
  ${TEMP}/bfInnerProductPCAImageFilter_PC12_2.tif
  )

otb_add_test(NAME bfTvInnerProductPCAImageFilter_PC1 COMMAND otbDimensionalityReductionTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfInnerProductPCAImageFilter_PC1.tif
  ${TEMP}/bfInnerProductPCAImageFilter_PC1.tif
  otbInnerProductPCAImageFilter
  ${INPUTDATA}/poupees_sub_3c.png
  ${TEMP}/bfInnerProductPCAImageFilter_PC1.tif
  0 # Flag generate Mean component ?
  1 #center data
  1 # Nb Of Principal Components
  1 # use unbiased estimator
  ${TEMP}/bfInnerProductPCAImageFilter_PC1_1.tif
  )

otb_add_test(NAME bfTvInnerProductPCAImageFilterDisableCenterData COMMAND otbDimensionalityReductionTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfInnerProductPCAImageFilterDisableCenterData.tif
  ${TEMP}/bfInnerProductPCAImageFilterDisableCenterData.tif
  otbInnerProductPCAImageFilter
  ${INPUTDATA}/poupees_sub_3c.png
  ${TEMP}/bfInnerProductPCAImageFilterDisableCenterData.tif
  0 # Flag generate Mean component ?
  0 #center data
  3 # Nb Of Principal Components
  1 # use unbiased estimator
  ${TEMP}/bfInnerProductPCAImageFilterDisableCenterData_pc1.tif
  ${TEMP}/bfInnerProductPCAImageFilterDisableCenterData_pc2.tif
  ${TEMP}/bfInnerProductPCAImageFilterDisableCenterData_pc3.tif
  )

otb_add_test(NAME bfTvInnerProductPCAImageFilterAllsOutputsWithoutUnbiasedEstimator COMMAND otbDimensionalityReductionTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfInnerProductPCAImageFilterAllsOutputs.tif
  ${TEMP}/bfInnerProductPCAImageFilterAllsOutputsWithoutUnbiasedEstimator.tif
  otbInnerProductPCAImageFilter
  ${INPUTDATA}/poupees_sub_3c.png
  ${TEMP}/bfInnerProductPCAImageFilterAllsOutputsWithoutUnbiasedEstimator.tif
  1 # Flag generate Mean component ?
  1 #center data
  3 # Nb Of Principal Components
  0 # do not use unbiased estimator (use in bfTvItkImagePCAShapeModelEstimatorTest to compare results with ITK PCA filter)
  ${TEMP}/bfInnerProductPCAImageFilterAllsOutputsWithoutUnbiasedEstimator_pc1.tif
  ${TEMP}/bfInnerProductPCAImageFilterAllsOutputsWithoutUnbiasedEstimator_pc2.tif
  ${TEMP}/bfInnerProductPCAImageFilterAllsOutputsWithoutUnbiasedEstimator_pc3.tif
  ${TEMP}/bfInnerProductPCAImageFilterAllsOutputsWithoutUnbiasedEstimator_MEAN.tif
  )

otb_add_test(NAME bfTvInnerProductPCAImageFilterAllsOutputs COMMAND otbDimensionalityReductionTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfInnerProductPCAImageFilterAllsOutputs.tif
  ${TEMP}/bfInnerProductPCAImageFilterAllsOutputs.tif
  otbInnerProductPCAImageFilter
  ${INPUTDATA}/poupees_sub_3c.png
  ${TEMP}/bfInnerProductPCAImageFilterAllsOutputs.tif
  1 # Flag generate Mean component ?
  1 #center data
  3 # Nb Of Principal Components
  1 # use unbiased estimator (default mode)
  ${TEMP}/bfInnerProductPCAImageFilterAllsOutputs_pc1.tif
  ${TEMP}/bfInnerProductPCAImageFilterAllsOutputs_pc2.tif
  ${TEMP}/bfInnerProductPCAImageFilterAllsOutputs_pc3.tif
  ${TEMP}/bfInnerProductPCAImageFilterAllsOutputs_MEAN.tif
  )

otb_add_test(NAME bfTvInnerProductPCAImageFilter COMMAND otbDimensionalityReductionTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfInnerProductPCAImageFilter.tif
  ${TEMP}/bfInnerProductPCAImageFilter.tif
  otbInnerProductPCAImageFilter
  ${INPUTDATA}/poupees_sub_3c.png
  ${TEMP}/bfInnerProductPCAImageFilter.tif
  0 # Flag generate Mean component ?
  1 #center data
  3 # Nb Of Principal Components
  1 # use unbiased estimator
  ${TEMP}/bfInnerProductPCAImageFilter_pc1.tif
  ${TEMP}/bfInnerProductPCAImageFilter_pc2.tif
  ${TEMP}/bfInnerProductPCAImageFilter_pc3.tif
  )

otb_add_test(NAME bfTuEstimateInnerProductPCAImageFilterNew COMMAND otbDimensionalityReductionTestDriver
  otbEstimateInnerProductPCAImageFilterNew )

otb_add_test(NAME bfTvEstimateInnerProductPCAImageFilterDisableCenterData COMMAND otbDimensionalityReductionTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfEstimateInnerProductPCAImageFilterDisableCenterData.tif
  ${TEMP}/bfEstimateInnerProductPCAImageFilterDisableCenterData.tif
  otbEstimateInnerProductPCAImageFilter
  ${INPUTDATA}/poupees_sub_3c.png
  ${TEMP}/bfEstimateInnerProductPCAImageFilterDisableCenterData.tif
  3
  0 #center data
  )

otb_add_test(NAME bfTvEstimateInnerProductPCAImageFilterCP1 COMMAND otbDimensionalityReductionTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfEstimateInnerProductPCAImageFilter_CP1.tif
  ${TEMP}/bfEstimateInnerProductPCAImageFilter_CP1.tif
  otbEstimateInnerProductPCAImageFilter
  ${INPUTDATA}/poupees_sub_3c.png
  ${TEMP}/bfEstimateInnerProductPCAImageFilter_CP1.tif
  1
  1 #center data
  )

otb_add_test(NAME bfTvEstimateInnerProductPCAImageFilterCP12 COMMAND otbDimensionalityReductionTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfEstimateInnerProductPCAImageFilter_CP12.tif
  ${TEMP}/bfEstimateInnerProductPCAImageFilter_CP12.tif
  otbEstimateInnerProductPCAImageFilter
  ${INPUTDATA}/poupees_sub_3c.png
  ${TEMP}/bfEstimateInnerProductPCAImageFilter_CP12.tif
  2
  1 #center data
  )

otb_add_test(NAME bfTvEstimateInnerProductPCAImageFilter COMMAND otbDimensionalityReductionTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfEstimateInnerProductPCAImageFilter.tif
  ${TEMP}/bfEstimateInnerProductPCAImageFilter.tif
  otbEstimateInnerProductPCAImageFilter
  ${INPUTDATA}/poupees_sub_3c.png
  ${TEMP}/bfEstimateInnerProductPCAImageFilter.tif
  3
  1 #center data
  )

otb_add_test(NAME bfTuPCAImageFilterNew COMMAND otbDimensionalityReductionTestDriver
  otbPCAImageFilterNewTest)

otb_add_test(NAME bfTvPCAImageFilter2 COMMAND otbDimensionalityReductionTestDriver
  --compare-image ${EPSILON_7}
  ${INPUTDATA}/cupriteSubHsi.tif
  ${TEMP}/bfTvPCAImageFilter2Inv.tif
  otbPCAImageFilterTest
  -in ${INPUTDATA}/cupriteSubHsi.tif
  -out ${TEMP}/bfTvPCAImageFilter2.tif
  -inv ${TEMP}/bfTvPCAImageFilter2Inv.tif
  -norm)

otb_add_test(NAME bfTvPCAImageFilter4 COMMAND otbDimensionalityReductionTestDriver
  --compare-n-images ${EPSILON_7} 2
  ${BASELINE}/bfTvPCAImageFilter4.tif
  ${TEMP}/bfTvPCAImageFilter4.tif
  ${BASELINE}/bfTvPCAImageFilter4Inv.tif
  ${TEMP}/bfTvPCAImageFilter4Inv.tif
  otbPCAImageFilterTest
  -in ${INPUTDATA}/cupriteSubHsi.tif
  -out ${TEMP}/bfTvPCAImageFilter4.tif
  -inv ${TEMP}/bfTvPCAImageFilter4Inv.tif
  -n 4)

otb_add_test(NAME bfTvPCAImageFilter4Norm COMMAND otbDimensionalityReductionTestDriver
  --compare-n-images ${EPSILON_7} 2
  ${BASELINE}/bfTvPCAImageFilter4Norm.tif
  ${TEMP}/bfTvPCAImageFilter4Norm.tif
  ${BASELINE}/bfTvPCAImageFilter4InvNorm.tif
  ${TEMP}/bfTvPCAImageFilter4InvNorm.tif
  otbPCAImageFilterTest
  -in ${INPUTDATA}/cupriteSubHsi.tif
  -out ${TEMP}/bfTvPCAImageFilter4Norm.tif
  -inv ${TEMP}/bfTvPCAImageFilter4InvNorm.tif
  -norm
  -n 4)

otb_add_test(NAME bfTvPCAImageFilter3 COMMAND otbDimensionalityReductionTestDriver
  --compare-n-images ${EPSILON_7} 2
  ${BASELINE}/bfTvPCAImageFilter3.tif
  ${TEMP}/bfTvPCAImageFilter3.tif
  ${BASELINE}/bfTvPCAImageFilter3Inv.tif
  ${TEMP}/bfTvPCAImageFilter3Inv.tif
  otbPCAImageFilterTest
  -in ${INPUTDATA}/cupriteSubHsi.tif
  -out ${TEMP}/bfTvPCAImageFilter3.tif
  -inv ${TEMP}/bfTvPCAImageFilter3Inv.tif
  )

otb_add_test(NAME bfTvPCAImageFilter1 COMMAND otbDimensionalityReductionTestDriver
  --compare-image ${EPSILON_7}
  ${INPUTDATA}/cupriteSubHsi.tif
  ${TEMP}/bfTvPCAImageFilter1Inv.tif
  otbPCAImageFilterTest
  -in ${INPUTDATA}/cupriteSubHsi.tif
  -out ${TEMP}/bfTvPCAImageFilter1.tif
  -inv ${TEMP}/bfTvPCAImageFilter1Inv.tif)

otb_add_test(NAME bfTvPCAImageFilter3Norm COMMAND otbDimensionalityReductionTestDriver
  --compare-n-images ${EPSILON_7} 2
  ${BASELINE}/bfTvPCAImageFilter3Norm.tif
  ${TEMP}/bfTvPCAImageFilter3Norm.tif
  ${BASELINE}/bfTvPCAImageFilter3InvNorm.tif
  ${TEMP}/bfTvPCAImageFilter3InvNorm.tif
  otbPCAImageFilterTest
  -in ${INPUTDATA}/cupriteSubHsi.tif
  -out ${TEMP}/bfTvPCAImageFilter3Norm.tif
  -inv ${TEMP}/bfTvPCAImageFilter3InvNorm.tif
  -norm)

otb_add_test(NAME bfTuAngularProjectionSetImageFilterNew COMMAND otbDimensionalityReductionTestDriver
  otbAngularProjectionSetImageFilterNew)

otb_add_test(NAME bfTuNAPCAImageFilterNew COMMAND otbDimensionalityReductionTestDriver
  otbNAPCAImageFilterNewTest)

otb_add_test(NAME bfTvNAPCAImageFilter1 COMMAND otbDimensionalityReductionTestDriver
  --compare-image ${EPSILON_7}
  ${INPUTDATA}/cupriteSubHsi.tif
  ${TEMP}/bfTvNAPCAImageFilter1Inv.tif
  otbNAPCAImageFilterTest
  -in ${INPUTDATA}/cupriteSubHsi.tif
  -out ${TEMP}/bfTvNAPCAImageFilter1.tif
  -inv ${TEMP}/bfTvNAPCAImageFilter1Inv.tif)

otb_add_test(NAME bfTvNAPCAImageFilter3 COMMAND otbDimensionalityReductionTestDriver
  --compare-n-images ${EPSILON_7} 2
  ${BASELINE}/bfTvNAPCAImageFilter3.tif
  ${TEMP}/bfTvNAPCAImageFilter3.tif
  ${BASELINE}/bfTvNAPCAImageFilter3Inv.tif
  ${TEMP}/bfTvNAPCAImageFilter3Inv.tif
  otbNAPCAImageFilterTest
  -in ${INPUTDATA}/cupriteSubHsi.tif
  -out ${TEMP}/bfTvNAPCAImageFilter3.tif
  -inv ${TEMP}/bfTvNAPCAImageFilter3Inv.tif
  -norm)

otb_add_test(NAME bfTvNAPCAImageFilter2 COMMAND otbDimensionalityReductionTestDriver
  --compare-image ${EPSILON_7}
  ${INPUTDATA}/cupriteSubHsi.tif
  ${TEMP}/bfTvNAPCAImageFilter2Inv.tif
  otbNAPCAImageFilterTest
  -in ${INPUTDATA}/cupriteSubHsi.tif
  -out ${TEMP}/bfTvNAPCAImageFilter2.tif
  -inv ${TEMP}/bfTvNAPCAImageFilter2Inv.tif
  -norm)

otb_add_test(NAME bfTvNAPCAImageFilter4 COMMAND otbDimensionalityReductionTestDriver
  --compare-n-images ${EPSILON_7} 2
  ${BASELINE}/bfTvNAPCAImageFilter4.tif
  ${TEMP}/bfTvNAPCAImageFilter4.tif
  ${BASELINE}/bfTvNAPCAImageFilter4Inv.tif
  ${TEMP}/bfTvNAPCAImageFilter4Inv.tif
  otbNAPCAImageFilterTest
  -in ${INPUTDATA}/cupriteSubHsi.tif
  -out ${TEMP}/bfTvNAPCAImageFilter4.tif
  -inv ${TEMP}/bfTvNAPCAImageFilter4Inv.tif
  -norm
  -n 4)

otb_add_test(NAME bfTuInnerProductPCAImageFilterNew COMMAND otbDimensionalityReductionTestDriver
  otbInnerProductPCAImageFilterNew )

otb_add_test(NAME bfTuSparseWvltToAngleMapperListFilterNew COMMAND otbDimensionalityReductionTestDriver
  otbSparseWvltToAngleMapperListFilterNew)

otb_add_test(NAME bfTuAngularProjectionImageFilterNew COMMAND otbDimensionalityReductionTestDriver
  otbAngularProjectionImageFilterNew)

otb_add_test(NAME bfTuLocalActivityVectorImageFilterNew COMMAND otbImageManipulationTestDriver
  otbLocalActivityVectorImageFilterNewTest)
otb_add_test(NAME bfTvLocalActivityVectorImageFilter COMMAND otbImageManipulationTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvLocalActivityVectorImageFilter.tif
  ${TEMP}/bfTvLocalActivityVectorImageFilter.tif
  otbLocalActivityVectorImageFilterTest
  -in ${INPUTDATA}/cupriteSubHsi.tif
  -out ${TEMP}/bfTvLocalActivityVectorImageFilter.tif)
