set(DOCUMENTATION "This module contains base classes that allow the
representation of mono- and multiband images.")

otb_module(OTBImageBase
  DEPENDS
    OTBCommon
    OTBITK
    OTBMetadata
    OTBOSSIMAdapters

  TEST_DEPENDS
    OTBBoost
    OTBDescriptors
    OTBImageIO
    OTBImageManipulation
    OTBMoments
    OTBStatistics
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
