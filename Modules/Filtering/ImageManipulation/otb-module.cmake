set(DOCUMENTATION "This module contains everything users may need handle images
: clamping, shifting, scaling, rescaling intensities, concatenating of images
into one vector image, splitting a vector image into many images, and so on. Among
nice other features, let's mention: applying a standard  (or a functor)  per
band, evaluating a ImageFunction onto a source image, generating an image from DEM
data.")

otb_module(OTBImageManipulation 
  DEPENDS
    OTBITKPendingPatches
    OTBImageBase
    OTBTransform
    OTBStreaming
    OTBCommon
    OTBInterpolation
    OTBOSSIMAdapters
    OTBObjectList
  TEST_DEPENDS
    OTBTextures
    OTBTestKernel
    OTBDensity
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
