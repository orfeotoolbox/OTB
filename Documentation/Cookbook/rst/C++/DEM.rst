DEM Handler
===========


The ``otb::DEMHandler`` class is a singleton, a reference to the singleton object can be acquired via the ``otb::DEMHandler::GetInstance()`` method. The DEMHandler is based on RasterIO from GDAL. A 2x2 window is extracted around the point of interest, and the height above ellipsoid or above mean sea level is interpolated on this window. The interpolation is bilinear. The following methods are provided:

- GetHeightAboveEllipsoid(lon, lat)
    * SRTM and geoid both available: dem_value + geoid_offset
    * No SRTM but geoid available: default height above ellipsoid + geoid_offset
    * SRTM available, but no geoid: dem_value
    * No SRTM and no geoid available: default height above ellipsoid

- GetHeightAboveMSL(lon, lat)
    * SRTM and geoid both available: dem_value
    * No SRTM but geoid available: 0
    * SRTM available, but no geoid: dem_value
    * No SRTM and no geoid available: 0

Several DEM tiles can be provided at the same time, using the ``OpenDEMDirectory`` method. All raster from the input directory will be opened by GDAL. Internally, a mosaic of all DEM tiles is then created as a virtual dataset (vrt).

All raster that can be opened by gdal can be used as a geoid. In previous OTB versions (using Ossim) it was common to use the egm96.grd file as geoid, this file cannot be opened by GDAL. However it is still possible to use it by using the following egm96.grd.hdr file::

  ENVI
  samples = 1441
  lines   = 721
  bands   = 1
  header offset = 24
  file type = ENVI Standard
  data type = 4
  interleave = bsq
  sensor type = Unknown
  byte order = 1
  wavelength units = Unknown
  map info = {Geographic Lat/Lon, 1, 1,-0.125, 90.125, 0.25, 0.25,WGS-84}
  coordinate system string = {GEOGCS["GCS_WGS_1984",DATUM["D_WGS_1984",SPHEROID["WGS_1984",6378137,298.257223563]],PRIMEM["Greenwich",0],UNIT["Degree",0.017453292519943295]]}
  band names = {
  Band 1}


With this file attached, GDAL will be able to read the egm96.grd file as a ENVI dataset