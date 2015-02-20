set(DOCUMENTATION "This module is related to the handling of point sets. For
instance, it contains the base class 'otb::ImageToPointSetFilter' from which
inherit detectors such as Harris (note that contrary to the 'corner' module, here the
harris detector outputs a point set instead of an image.). With this module, users
will be able to apply a transform to all the points of a PointSet, extract all
the points of a PointSet that are in a given region, generate a random point set,
and so on.")

otb_module(OTBPointSet 
  DEPENDS
    OTBCommon
    OTBInterpolation
    OTBStreaming
  TEST_DEPENDS
    OTBTestKernel
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
