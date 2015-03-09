set(DOCUMENTATION "This module contains useful tools for the task of labelling
images. It is possible to label with similar statistics using connectivity, to
include pixels in a growing region is to evaluate intensity value inside a specific
interval, or to label pixels that are connected to a seed and lie within a
neighbourhood. Remaping the labels is also possible, so that that the label numbers
are consecutive with no gaps between the label numbers used. Finally, it is also
possible to sort the labels based on the size of the object.")

otb_module(OTBLabelling
  DEPENDS
    OTBITK
    OTBImageManipulation
    OTBPointSet

  TEST_DEPENDS
    OTBImageBase
    OTBImageIO
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
