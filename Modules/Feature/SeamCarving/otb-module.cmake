set(DOCUMENTATION "This module contains necessary components to apply the seam
carving algorithm published by Shai Avidan, Ariel Shamir, Seam Carving for
Content-Aware  Resizing, ACM Transactions on Graphics, Volume 26, Number 3, SIGGRAPH
2007.")

otb_module(OTBSeamCarving
  DEPENDS
    OTBPath
    OTBCommon
    OTBITK

  TEST_DEPENDS
    OTBTestKernel
    OTBImageBase

  DESCRIPTION
    "${DOCUMENTATION}"
)
