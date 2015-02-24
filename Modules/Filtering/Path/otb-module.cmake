set(DOCUMENTATION "This module is related to the handling of paths. For
instance, it contains the base class 'otbImageToPathFilter' for filters taking an image
as input, and producing an output of type Path. another interesting class is
'otbPathFunction', which is the base class for all objects that evaluate a function
of an image using a path list (computation of histograms for instance). And so on.")

otb_module(OTBPath
  DEPENDS
    OTBVectorDataBase
    OTBImageBase
    OTBObjectList
    OTBITK
    OTBCommon

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO
    OTBVectorDataIO

  DESCRIPTION
    "${DOCUMENTATION}"
)
