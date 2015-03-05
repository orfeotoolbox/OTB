set(DOCUMENTATION "In some situation, it is desirable to represent a gray level
image in color for easier interpretation. This is particularly the case if pixel
values in the image are used to represent some data such as elevation,
deformation map, interferogram. In this case, it is important to ensure that similar
values will get similar colors. This module contains classes that can meet these
requirements.")

otb_module(OTBColorMap
  DEPENDS
    OTBITK

  TEST_DEPENDS
    OTBImageBase
    OTBImageIO
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
