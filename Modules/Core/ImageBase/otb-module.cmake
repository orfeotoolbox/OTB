set(DOCUMENTATION "This module contains base classes that allow the
representation of mono- and multiband images.")

otb_module(OTBImageBase
  DEPENDS
    OTBMetadata
    OTBOSSIMAdapters
    OTBITK
    OTBITKPendingPatches
    OTBCommon

  TEST_DEPENDS
    OTBStatistics
    OTBImageIO
    OTBMoments
    OTBTestKernel
    OTBDescriptors
    OTBBoost
    OTBOssim
    OTBObjectList

  DESCRIPTION
    "${DOCUMENTATION}"
)
