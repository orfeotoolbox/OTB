

# NOTE TLA : to move later to upper level.
# Old behavior was controled by otb_module_activation_option in otb-module files
option(OTB_USE_6S "Enable 6S radiometric and atmospheric correction module" ON)
option(OTB_USE_GSL "Enable GSL dependent modules" OFF)
option(OTB_USE_LIBKML "Enable libkml dependent modules" OFF)
option(OTB_USE_SIFTFAST "Enable SiftFast dependent modules" ON)

# Add the subdirectory depending of cmake activation options
if (OTB_USE_6S)
  add_subdirectory(6S)
endif()
if (OTB_USE_GSL)
  add_subdirectory(GSL)
endif()
if (OTB_USE_LIBKML)
  add_subdirectory(libkml)
endif()
if (OTB_USE_SIFTFAST)
  add_subdirectory(SiftFast)
endif()

add_subdirectory(ITK)
add_subdirectory(Boost)
add_subdirectory(Curl)
add_subdirectory(Date)
add_subdirectory(GDAL)
add_subdirectory(GeoTIFF)
add_subdirectory(TIFF)
add_subdirectory(TinyXML)
