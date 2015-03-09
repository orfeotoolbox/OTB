find_package ( GDAL REQUIRED )

mark_as_advanced(GDAL_INCLUDE_DIR)
mark_as_advanced(GDAL_LIBRARY)
mark_as_advanced(GDAL_CONFIG)

# TODO : backport work from ImportGDAL about tiff, geotiff, driver tests, etc...