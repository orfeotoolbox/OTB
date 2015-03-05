set(DOCUMENTATION "The Self Organizing Map, SOM, introduced by Kohonen is a
non-supervised neural learning algorithm. The map is composed of neighboring cells
which are in competition by means of mutual interactions and they adapt in order
to match characteristic patterns of the examples given during the learning. The
SOM is usually on a plane (2D).")

otb_module(OTBSOM
  DEPENDS
    OTBCommon
    OTBITK
    OTBImageBase

  TEST_DEPENDS
    OTBImageIO
    OTBImageManipulation
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
