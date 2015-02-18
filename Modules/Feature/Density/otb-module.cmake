set(DOCUMENTATION "This module contains tools to computes a local density of
keypoints (SIFT or SURF, for instance). Many estimators are implemented
(Epanechnikov / simple cut off / gaussian function, ratio of the number of keypoints over
the total number of pixels within a region). Note that the estimated density can be
represented as an image or quiered at particular points (function).")

otb_module(OTBDensity 
  DEPENDS
    OTBPointSet
    OTBCommon
  TEST_DEPENDS
    OTBDescriptors
    OTBTestKernel
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
