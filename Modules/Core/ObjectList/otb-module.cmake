set(DOCUMENTATION "This module contains classes that allow manipulating an
ITK/OTB object list with the appropriate formalism. For instance, it is possible to
apply a given scalar image filter to a list of images. or to produce an output
corresponding to the elements selected from an image list. and so on.")

otb_module(OTBObjectList
  DEPENDS
    OTBCommon
    OTBITK

  TEST_DEPENDS
    OTBImageBase
    OTBImageIO
    OTBTestKernel
    OTBVectorDataBase

  DESCRIPTION
    "${DOCUMENTATION}"
)
