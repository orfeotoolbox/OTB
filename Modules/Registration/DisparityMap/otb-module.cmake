set(DOCUMENTATION "This module contains tools related to geometric disparities
between images. Following functionalities are implemented: estimation,
regularization (median filter), 3D points projection, production of DEM at regular sampling
in the chosen map projection system, resampling of a disparity map computed in
epipolar geometry into a disparity map relative to sensor geometry, sub-pixel
precision.")

otb_module(OTBDisparityMap
  DEPENDS
    OTBStereo
    OTBITK
    OTBITKPendingPatches
    OTBImageBase
    OTBTransform
    OTBPointSet
    OTBCommon

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO
    OTBImageManipulation
    OTBObjectList

  DESCRIPTION
    "${DOCUMENTATION}"
)
