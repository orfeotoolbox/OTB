set(DOCUMENTATION "Cartography related module. For instance, users can retrieve
geographical information for longitude and latitude coordinates. compute the
extent of the input image and use it to fill the bbox for the OSM request. produce
Map file product (ie a file .map, the tiles to draw in a mapserver, and finally a
shapefile). request OSM Server using the OSM API 0.6 to get the xml file storing
the vector elements available. retrieve Longitude and Latitude coordinates from a
place name. And so on.")

otb_module(OTBCarto
  DEPENDS
    OTBCommon
    OTBCurlAdapters
    OTBITK
    OTBImageBase
    OTBImageIO
    OTBImageManipulation
    OTBProjection
    OTBTinyXML
    OTBTransform
    OTBVectorDataBase
    OTBVectorDataIO

  TEST_DEPENDS
    OTBCommandLineParser
    OTBOpenThreadsAdapters
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
