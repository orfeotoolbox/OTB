set(DOCUMENTATION "Extended filenames provide a way to influence how data are
read and written by Orfeo ToolBox. They can be used to specify an external geom
file for sensor modeling, tell  that a TILED GeoTiff is to be written, read a
different subdataset or resolution (for multi-resolution data), or manage ortho-ready
product by skipping either geographic or sensor-model information.")

otb_module(OTBExtendedFilename 
  DEPENDS
    OTBIOGDAL
    OTBBoost
  TEST_DEPENDS
    OTBTestKernel
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
