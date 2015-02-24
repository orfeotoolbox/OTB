set(DOCUMENTATION "This module contains classes to estimate and detect clouds.
The implemented filters rely on the spectral angle principle to measure the
radiometric gap between a reference pixel and the other pixels of the image.")

otb_module(OTBCloudDetection
  DEPENDS
    OTBImageManipulation
    OTBITKPendingPatches

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO
    OTBITK
    OTBImageBase

  DESCRIPTION
    "${DOCUMENTATION}"
)
