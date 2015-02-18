set(DOCUMENTATION "Cartography related module. For instance, users can retrieve
geographical information for longitude and latitude coordinates. compute the
extent of the input image and use it to fill the bbox for the OSM request. produce
Map file product (ie a file .map, the tiles to draw in a mapserver, and finally a
shapefile). request OSM Server using the OSM API 0.6 to get the xml file storing
the vector elements available. retrieve Longitude and Latitude coordinates from a
place name. And so on.")

otb_module(OTBCarto 
  DEPENDS
    OTBVectorDataBase
    OTBImageIO
    OTBProjection
    OTBTinyXML
    OTBImageBase
    OTBTransform
    OTBCommon
    OTBCurlAdapters
    OTBVectorDataIO
    OTBImageManipulation
  TEST_DEPENDS
    OTBOpenThreadsAdapters
    OTBTestKernel
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
