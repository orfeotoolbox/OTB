set(DOCUMENTATION "This module contains everything users may need handle images
: clamping, shifting, scaling, rescaling intensities, concatenating of images
into one vector image, splitting a vector image into many images, and so on. Among
nice other features, let's mention: applying a standard  (or a functor)  per
band, evaluating a ImageFunction onto a source image, generating an image from DEM
data.")

otb_module(OTBImageManipulation
  DEPENDS
    OTBCommon
    OTBITK
    OTBITKPendingPatches
    OTBImageBase
    OTBInterpolation
    OTBObjectList
    OTBStreaming
    OTBTransform

  TEST_DEPENDS
    OTBCommandLineParser
    OTBDensity
    OTBImageIO
    OTBTestKernel
    OTBTextures

  DESCRIPTION
    "${DOCUMENTATION}"
)
