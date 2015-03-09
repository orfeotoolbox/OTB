set(DOCUMENTATION "This module contains classes that implement classical corner
detection algorithms (Harris for instance). It also contains classes to detect
right angle by exploiting the output of a line detection algorithm.")

otb_module(OTBCorner
  DEPENDS
    OTBVectorDataBase
    OTBITK
    OTBDescriptors
    OTBSpatialObjects
    OTBVectorDataManipulation
    OTBPointSet
    OTBImageManipulation
    OTBCommon

  TEST_DEPENDS
    OTBVectorDataIO
    OTBTestKernel
    OTBImageIO
    OTBImageBase

  DESCRIPTION
    "${DOCUMENTATION}"
)
