set(DOCUMENTATION "This module contains base classes that allow the
representation of mono- and multiband images.")

otb_module(OTBImageBase
  DEPENDS
    OTBCommon
    OTBITK
    OTBITKPendingPatches
    OTBMetadata
    OTBOSSIMAdapters

  TEST_DEPENDS
    OTBBoost
    OTBDescriptors
    OTBImageIO
    OTBImageManipulation
    OTBMoments
    OTBObjectList
    OTBOssim
    OTBStatistics
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
