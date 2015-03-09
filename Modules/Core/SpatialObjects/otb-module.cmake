set(DOCUMENTATION "Geometric shapes are represented in OTB using the ITK
spatial object hierarchy. These classes are intended to support modelling of anatomical
structures in ITK. OTB uses them in order to model cartographic elements. Using
a common basic interface, the spatial objects are capable of representing regions
of space in a variety of different ways. For example: mesh structures, image
masks, and implicit equations may be used as the underlying representation scheme.
Spatial objects are a natural data structure for communicating the results of
segmentation methods and for introducing geometrical priors in both segmentation and
registration methods.")

otb_module(OTBSpatialObjects
  DEPENDS
    OTBCommon
    OTBITK

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO
    OTBImageBase

  DESCRIPTION
    "${DOCUMENTATION}"
)
