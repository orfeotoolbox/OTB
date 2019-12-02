set(DOCUMENTATION "Large Scale OTB Application for image superpixel segmentation using SLIC algorithm.")

otb_module(OTBSLIC
  DEPENDS
  OTBCommon
  OTBApplicationEngine
  OTBMPI
  OTBGdalAdapters
  OTBGDAL
  
  DESCRIPTION "Superpixel segmentation - SLIC algorithm")
