DEM Handler
===========


The ``otb::DEMHandler`` class is a singleton, a reference to the singleton object can be acquired via the ``otb::DEMHandler::GetInstance()`` method. The DEMHandler is based on RasterIO from GDAL. A 2x2 window is extracted around the point of interest, and the height above ellipsoid or above mean sea level is interpolated on this window. The interpolation is bilinear. The following methods are provided:

- GetHeightAboveEllipsoid(lon, lat)
    * SRTM and geoid both available: dem_value + geoid_offset
    * No SRTM but geoid available: default height above ellipsoid (0 by default) + geoid_offset
    * SRTM available, but no geoid: dem_value
    * No SRTM and no geoid available: default height above ellipsoid (0 by default)

- GetHeightAboveMSL(lon, lat)
    * SRTM and geoid both available: dem_value
    * No SRTM but geoid available: 0
    * SRTM available, but no geoid: dem_value
    * No SRTM and no geoid available: 0

Several DEM tiles can be provided at the same time, using the ``OpenDEMDirectory`` method. All raster from the input directory will be opened by GDAL. Internally, a mosaic of all DEM tiles is then created as a virtual dataset (vrt).

A geoid file can be opened using the ``OpenGeoidFile`` method, and the default height above ellipsoid can be set using ``OpenGeoidFile`` method.

The ``ClearElevationParameters`` method can be used to clear the DEMs, the geoid and the default height above ellipsoid. All associated GDAL datasets will be closed.