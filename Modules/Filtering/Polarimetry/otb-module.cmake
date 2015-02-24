set(DOCUMENTATION "This module is concerned with Synthetic Aperture Radar
images. The ability of radar polarimetry to obtain more information about physical
properties of the surface than single-channel SAR data has led to a wide range of
applications. Here, the user can find (for instance) classes to handle different
representations of a polarisation state, to perform polarimetry synthesis, and so
on.")

otb_module(OTBPolarimetry
  DEPENDS
    OTBCommon
    OTBITKPendingPatches
    OTBITK

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO
    OTBImageBase

  DESCRIPTION
    "${DOCUMENTATION}"
)
