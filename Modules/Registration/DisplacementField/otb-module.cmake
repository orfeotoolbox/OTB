set(DOCUMENTATION "This module contains filters that generate a deformation
field by using some interpolation criterion on the deformation values from the point
set. Several interpolation methods are available (Nearest neighbours, linear,
BSpline). The class otb::StereorectificationDisplacementFieldSource is specialized
in generating a deformation field between two images in epipolar geometry
(horizontal shifts).")

otb_module(OTBDisplacementField
  DEPENDS
    OTBITK
    OTBImageBase
    OTBTransform
    OTBCommon
    OTBOSSIMAdapters
    OTBObjectList

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO

  DESCRIPTION
    "${DOCUMENTATION}"
)
