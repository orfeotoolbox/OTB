otb_module_test()

set(OTBDescriptorsTests
otbDescriptorsTestDriver.cxx
otbImageToSURFKeyPointSetFilterOutputDescriptorAscii.cxx
otbKeyPointSetsMatchingFilterNew.cxx
otbImageToSIFTKeyPointSetFilterOutputInterestPointAscii.cxx
otbHistogramOfOrientedGradientCovariantImageFunction.cxx
otbImageToSURFKeyPointSetFilterOutputInterestPointAscii.cxx
otbImageToSURFKeyPointSetFilterNew.cxx
otbLandmarkNew.cxx
otbImageToSIFTKeyPointSetFilterNew.cxx
otbKeyPointSetsMatchingFilter.cxx
otbImageToSIFTKeyPointSetFilterOutputDescriptorAscii.cxx
otbImageToSIFTKeyPointSetFilterOutputAscii.cxx
otbFourierMellinImageFilter.cxx
otbImageToHessianDeterminantImageFilter.cxx
otbImageToSIFTKeyPointSetFilterOutputImage.cxx
otbFourierMellinDescriptors.cxx
otbImageToSIFTKeyPointSetFilterDistanceMap.cxx
otbImageToHessianDeterminantImageFilterNew.cxx
otbFourierMellinImageFilterNew.cxx
)

if(OTB_USE_SIFTFAST)
  list(APPEND OTBDescriptorsTests
       otbImageToFastSIFTKeyPointSetFilterNew.cxx
       otbImageToFastSIFTKeyPointSetFilterOutputInterestPointAscii.cxx
       ##>otbImageToFastSIFTKeyPointSetFilterOutputDescriptorAscii.cxx
       )
endif()

add_executable(otbDescriptorsTestDriver ${OTBDescriptorsTests})
target_link_libraries(otbDescriptorsTestDriver ${OTBDescriptors-Test_LIBRARIES})
otb_module_target_label(otbDescriptorsTestDriver)

# Tests Declaration

otb_add_test(NAME feTvImageToSURFKeyPointSetFilterSceneOutputDescriptorAscii COMMAND otbDescriptorsTestDriver
  --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToSURFKeyPointSetFilterSceneKeysOutputDescriptor.txt
  ${TEMP}/feTvImageToSURFKeyPointSetFilterSceneKeysOutputDescriptor.txt
  otbImageToSURFKeyPointSetFilterOutputDescriptorAscii
  ${INPUTDATA}/scene.png
  ${TEMP}/feTvImageToSURFKeyPointSetFilterSceneKeysOutputDescriptor.txt
  3 3
  )


otb_add_test(NAME feTuKeyPointSetsMatchingFilterNew COMMAND otbDescriptorsTestDriver
  otbKeyPointSetsMatchingFilterNew)

otb_add_test(NAME feTvImageToSIFTKeyPointSetFilterSceneOutputInterestPointAscii COMMAND otbDescriptorsTestDriver
  --ignore-order --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToSIFTKeyPointSetFilterSceneKeysOutputInterestPoint.txt
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSceneKeysOutputInterestPoint.txt
  otbImageToSIFTKeyPointSetFilterOutputInterestPointAscii
  ${INPUTDATA}/scene.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSceneKeysOutputInterestPoint.txt
  6 3 0.08 10.0
  )

otb_add_test(NAME feTuHistogramOfOrientedGradientCovariantImageFunction COMMAND otbDescriptorsTestDriver
  otbHistogramOfOrientedGradientCovariantImageFunctionNew)

otb_add_test(NAME feTvHistogramOfOrientedGradientCovariantImageFunction COMMAND otbDescriptorsTestDriver
  --compare-ascii ${EPSILON_8}
  ${BASELINE_FILES}/feTvHistogramOfOrientedGradientCovariantImageFunction.txt
  ${TEMP}/feTvHistogramOfOrientedGradientCovariantImageFunction.txt
  otbHistogramOfOrientedGradientCovariantImageFunction
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/feTvHistogramOfOrientedGradientCovariantImageFunction.txt
  5 273 64
  )

otb_add_test(NAME feTvImageToSURFKeyPointSetFilterSceneOutputInterestPointAscii COMMAND otbDescriptorsTestDriver
  --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToSURFKeyPointSetFilterSceneKeysOutputInterestPoint.txt
  ${TEMP}/feTvImageToSURFKeyPointSetFilterSceneKeysOutputInterestPoint.txt
  otbImageToSURFKeyPointSetFilterOutputInterestPointAscii
  ${INPUTDATA}/scene.png
  ${TEMP}/feTvImageToSURFKeyPointSetFilterSceneKeysOutputInterestPoint.txt
  3 3
  )

otb_add_test(NAME feTuImageToSURFKeyPointSetFilterNew COMMAND otbDescriptorsTestDriver
  otbImageToSURFKeyPointSetFilterNew)

otb_add_test(NAME feTuLandmarkNew COMMAND otbDescriptorsTestDriver
  otbLandmarkNew)

otb_add_test(NAME feTuImageToSIFTKeyPointSetFilterNew COMMAND otbDescriptorsTestDriver
  otbImageToSIFTKeyPointSetFilterNew)

otb_add_test(NAME feTvKeyPointSetsMatchingFilter COMMAND otbDescriptorsTestDriver
  --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvKeyPointSetsMatchingFilterOutputAscii.txt
  ${TEMP}/feTvKeyPointSetsMatchingFilterOutputAscii.txt
  otbKeyPointSetsMatchingFilter
  ${TEMP}/feTvKeyPointSetsMatchingFilterOutputAscii.txt
  0.6 0
  )

otb_add_test(NAME feTvImageToSIFTKeyPointSetFilterSceneDescriptorAscii COMMAND otbDescriptorsTestDriver
  --ignore-order --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToSIFTKeyPointSetFilterSceneKeysOutputDescriptor.txt
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSceneKeysOutputDescriptor.txt
  otbImageToSIFTKeyPointSetFilterOutputDescriptorAscii
  ${INPUTDATA}/scene.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSceneKeysOutputDescriptor.txt
  6 3 0.08 10.0
  )

otb_add_test(NAME feTvImageToSIFTKeyPointSetFilterGridOutputAscii COMMAND otbDescriptorsTestDriver
  --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToSIFTKeyPointSetFilterGridKeysOutput.txt
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterGridKeysOutput.txt
  otbImageToSIFTKeyPointSetFilterOutputAscii
  ${INPUTDATA}/damier.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterGridKeysOutput.txt
  7 3 0.01 10.0
  )

otb_add_test(NAME feTvImageToSIFTKeyPointSetFilterSquareRotatedOutputAscii COMMAND otbDescriptorsTestDriver
  --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToSIFTKeyPointSetFilterSquareRotatedKeysOutput.txt
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareRotatedKeysOutput.txt
  otbImageToSIFTKeyPointSetFilterOutputAscii
  ${INPUTDATA}/carre_ori.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareRotatedKeysOutput.txt
  7 3 0.01 10.0
  )

otb_add_test(NAME feTvImageToSIFTKeyPointSetFilterSquare2OutputAscii COMMAND otbDescriptorsTestDriver
  --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToSIFTKeyPointSetFilterSquareKeysOutput2.txt
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareKeysOutput2.txt
  otbImageToSIFTKeyPointSetFilterOutputAscii
  ${INPUTDATA}/carre.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareKeysOutput2.txt
  1 3 0.2 0.9
  )

otb_add_test(NAME feTvImageToSIFTKeyPointSetFilterSquareRotated2OutputAscii COMMAND otbDescriptorsTestDriver
  --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToSIFTKeyPointSetFilterSquareRotatedKeysOutput2.txt
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareRotatedKeysOutput2.txt
  otbImageToSIFTKeyPointSetFilterOutputAscii
  ${INPUTDATA}/carre_ori.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareRotatedKeysOutput2.txt
  1 3 0.215 10.0
  )

otb_add_test(NAME feTvImageToSIFTKeyPointSetFilterSquareOutputAscii COMMAND otbDescriptorsTestDriver
  --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToSIFTKeyPointSetFilterSquareKeysOutput.txt
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareKeysOutput.txt
  otbImageToSIFTKeyPointSetFilterOutputAscii
  ${INPUTDATA}/carre.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareKeysOutput.txt
  7 3 0.01 10.0
  )

otb_add_test(NAME feTvImageToSIFTKeyPointSetFilterQB_SuburbOutputAscii COMMAND otbDescriptorsTestDriver
  --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToSIFTKeyPointSetFilterQB_SuburbOutputAscii.txt
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterQB_SuburbOutputAscii.txt
  otbImageToSIFTKeyPointSetFilterOutputAscii
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterQB_SuburbOutputAscii.txt
  2 3 5.0 0.0
  )

otb_add_test(NAME feTvForwardFourierMellinImageFilter COMMAND otbDescriptorsTestDriver
  --compare-n-images ${EPSILON_6} 2
  ${BASELINE}/feForwardFourierMellinImageFilterReal.hdr
  ${TEMP}/feForwardFourierMellinImageFilterReal.hdr
  ${BASELINE}/feForwardFourierMellinImageFilterImag.hdr
  ${TEMP}/feForwardFourierMellinImageFilterImag.hdr
  otbFourierMellinImageFilter
  ${INPUTDATA}/circle.hdr
  ${TEMP}/feForwardFourierMellinImageFilterReal.hdr
  ${TEMP}/feForwardFourierMellinImageFilterImag.hdr)

otb_add_test(NAME feTvImageToHessianDeterminantImageFilter COMMAND otbDescriptorsTestDriver
  --compare-image ${EPSILON_3}
  ${BASELINE}/feTvImageToHessianDeterminantImageFilterOutput.tif
  ${TEMP}/feTvImageToHessianDeterminantImageFilterOutput.tif
  otbImageToHessianDeterminantImageFilter
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub.tif
  ${TEMP}/feTvImageToHessianDeterminantImageFilterOutput.tif
  1.5
  )

otb_add_test(NAME feTvImageToSIFTKeyPointSetFilterSquareOutputImage COMMAND otbDescriptorsTestDriver
  --compare-image ${EPSILON_8}
  ${BASELINE}/feTvImageToSIFTKeyPointSetFilterSquareImageOutput.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareImageOutput.png
  otbImageToSIFTKeyPointSetFilterOutputImage
  ${INPUTDATA}/carre.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareImageOutput.png
  7 3 0.01 10.0
  )

otb_add_test(NAME feTvImageToSIFTKeyPointSetFilterGridOutputImage COMMAND otbDescriptorsTestDriver
  --compare-image ${EPSILON_8}
  ${BASELINE}/feTvImageToSIFTKeyPointSetFilterGridImageOutput.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterGridImageOutput.png
  otbImageToSIFTKeyPointSetFilterOutputImage
  ${INPUTDATA}/damier.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterGridImageOutput.png
  7 3 0.01 10.0
  )

otb_add_test(NAME feTvFourierMellinDescriptorsRotationInvariant COMMAND otbDescriptorsTestDriver
  otbFourierMellinDescriptorsRotationInvariant
  ${INPUTDATA}/poupees.png
  4
  4
  90
  )

otb_add_test(NAME feTuFourierMellinDescriptorsNew COMMAND otbDescriptorsTestDriver
  otbFourierMellinDescriptorsNew
  )

otb_add_test(NAME feTvFourierMellinDescriptorsScaleInvariant COMMAND otbDescriptorsTestDriver
  otbFourierMellinDescriptorsScaleInvariant
  ${INPUTDATA}/poupees.png
  4
  4
  )

otb_add_test(NAME feTvFourierMellinDescriptors COMMAND otbDescriptorsTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/feTvFourierMellinDescriptors.txt
  ${TEMP}/feTvFourierMellinDescriptors.txt
  otbFourierMellinDescriptors
  ${INPUTDATA}/poupees.png
  4
  4
  ${TEMP}/feTvFourierMellinDescriptors.txt
  )

otb_add_test(NAME feTvImageToSIFTKeyPointSetFilterDistanceMap COMMAND otbDescriptorsTestDriver
  --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToSIFTKeyPointSetFilterDistanceMap.txt
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterDistanceMap.txt
  otbImageToSIFTKeyPointSetFilterDistanceMap
  ${INPUTDATA}/scene.png
  6 3 0.08 10.0
  15.0 # rotation
  1.2 # zoom factor
  10 255
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterDistanceMap.txt
  )

otb_add_test(NAME feTuImageToHessianDeterminantImageFilterNew COMMAND otbDescriptorsTestDriver
  otbImageToHessianDeterminantImageFilterNew)

otb_add_test(NAME feTuForwardFourierMellinImageFilterNew COMMAND otbDescriptorsTestDriver
  otbFourierMellinImageFilterNew)

if(OTB_USE_SIFTFAST)
otb_add_test(NAME feTuImageToFastSIFTKeyPointSetFilterNew COMMAND otbDescriptorsTestDriver
  otbImageToFastSIFTKeyPointSetFilterNew)

otb_add_test(NAME feTvImageToFastSIFTKeyPointSetFilterSceneOutputInterestPointAscii COMMAND otbDescriptorsTestDriver
  --ignore-order --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToFastSIFTKeyPointSetFilterSceneKeysOutputInterestPoint.txt
  ${TEMP}/feTvImageToFastSIFTKeyPointSetFilterSceneKeysOutputInterestPoint.txt
  otbImageToFastSIFTKeyPointSetFilterOutputInterestPointAscii
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub.tif
  ${TEMP}/feTvImageToFastSIFTKeyPointSetFilterSceneKeysOutputInterestPoint.txt
  6
  )

##>otb_add_test(NAME feTvImageToFastSIFTKeyPointSetFilterSceneOutputDescriptorAscii COMMAND otbDescriptorsTestDriver
##>  --ignore-order --epsilon-boundary 0.01 --compare-ascii ${EPSILON_2}
##>  ${BASELINE_FILES}/feTvImageToFastSIFTKeyPointSetFilterSceneKeysOutputDescriptor.txt
##>  ${TEMP}/feTvImageToFastSIFTKeyPointSetFilterSceneKeysOutputDescriptor.txt
##>  otbImageToFastSIFTKeyPointSetFilterOutputDescriptorAscii
##>  ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub.tif
##>  ${TEMP}/feTvImageToFastSIFTKeyPointSetFilterSceneKeysOutputDescriptor.txt
##>  6
##>  )
endif()
