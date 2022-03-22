Supported file formats
======================

OTB relies on `GDAL <https://gdal.org/>`_ for reading and writing data
(either raster or vector images). This means that all formats
supported by GDAL are theoretically also supported by OTB. That being
said, there are some limitations.

Raster images
-------------

Reading a raster image
~~~~~~~~~~~~~~~~~~~~~~

GDAL provides a very large panel of drivers to access metadata in
image files. OTB stores the metadata in a special dictionary called
otbImageMetadata. There is a metadata framework that uses interfaces
(ImageMetadataInterface) to link data read by GDAL to the
otbImageMetadata. Thus, to access metadata for a specific sensor, the
corresponding interface is required. As this time, available
interfaces are:

+------------------------+-------------------------+--------------------------------------------------------------------+
| Sensor                 | Format                  | Notes                                                              |
+========================+=========================+====================================================================+
| CosmoSkyMed            | HDF5 / TIFF             |  One Tiff file per polarization, or one HDF5 file containing all   |
|                        |                         |  the data. See HDF particularity below.                            |
+------------------------+-------------------------+--------------------------------------------------------------------+
| Formosat               | DIMAP / TIFF            | N/A                                                                |
+------------------------+-------------------------+--------------------------------------------------------------------+
| Ikonos                 | TIFF                    | N/A                                                                |
+------------------------+-------------------------+--------------------------------------------------------------------+
| Pleiades               | JPEG2000 / TIFF / DIMAP | We recommend the use of the tiff/jp2000 file. Indeed, when reading |
|                        |                         | the DIMAP file we noticed that the image is shifted by 0.5 pixel   |
|                        |                         | (this is a documented bug in GDAL).                                |
+------------------------+-------------------------+--------------------------------------------------------------------+
| QuickBird              | TIFF                    | N/A                                                                |
+------------------------+-------------------------+--------------------------------------------------------------------+
| Radarsat 2             | TIFF                    | N/A                                                                |
+------------------------+-------------------------+--------------------------------------------------------------------+
| Sentinel 1             | TIFF                    | Use the tiff file, not the manifest.                               |
+------------------------+-------------------------+--------------------------------------------------------------------+
| Spot 5                 | DIMAP / TIFF            | RPC not available.                                                 |
+------------------------+-------------------------+--------------------------------------------------------------------+
| Spot 6/7               | DIMAP / JPEG2000 / TIFF | N/A                                                                |
+------------------------+-------------------------+--------------------------------------------------------------------+
| TerraSarX              | COS                     | Use the cos file, not the main XML. MGD products are not supported.|
+------------------------+-------------------------+--------------------------------------------------------------------+
| WorldView 2            | TIFF                    | N/A                                                                |
+------------------------+-------------------------+--------------------------------------------------------------------+

Sensors not in this list can still be used with OTB, but the metadata
won't be accessible.

Full products generally contain multiple files (image files, manifest,
metadata files, etc). For SAR products, OTB takes necessarily an image
file. For optical sensor, OTB can also take the manifest file as
input. In that case, OTB will consider all the bands of the product.

Writing a raster image
~~~~~~~~~~~~~~~~~~~~~~

OTB uses a correspondence table to link a specific file extension to a
GDAL driver. This means that formats not present in this table can't
be written, even if the driver exists in GDAL. The formats available in
OTB for writing a raster are:

- GTiff (.tif / .tiff)

- ENVI (.hdr)

- HFA (.img)

- NITF (.ntf)

- PNG (.png)

- JPEG (.jpg / .jpeg)

- PCIDSK (.pix)

- ISIS2 (.lbl / .pds)

- JP2OpenJPEG / JP2KAK / JP2ECW (.j2k / .jp2 / .jpx)

A particularity of HDF datasets
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When reading a HDF dataset, one needs to select the right subdataset
using the :ref:`Extended Filename <extended-filename-sdataidx>`
``&sdataidx=<(int)idx>``.  For example, in some CosmoSkyMed products,
the first subDataset is a quicklook, and the actual product is the
second subdataset.


Vector data
-----------

OTB can read all vector formats supported by OGR. But the writing
process is a little tricky. OTB implements two ways of dealing with
writing vector data. The first one uses OGR::Datasource. The second
one uses otb::VectorData. Depending on which class is used by the
application, different formats are available. This is confusing, and
we plan on fixing this. For now, formats fully supported
(IE. supported by all applications) are:

- ESRI Shapefile (.shp)

- MapInfo File (.tab)

- Geographical Markup Language (.gml)

- GPS Exchange Format (.gpx)

- SQLite (.sqlite)

- Keyhole Markup Language (.kml)

- GeoPackage (.gpkg)

Digital Elevation Model
-----------------------

Many OTB application use an elevation model as input, usually with the
parameter “-elev.dem”. This parameter accepts any raster file
supported by GDAL, or a directory containing such files. In this
second case, all rasters from the input directory will be opened by
GDAL, so it could be a good idea to use a VRT. Beware that the DEM
folder should contain only DEM files. It is the same for the geoid
with the parameter "-elev.geoid". One can usually set a default
elevation with the parameter "-elev.default".

Depending on the provided parameters, the application will:

- compute the DEM + the value of the geoid if both the DEM and the
  geoid are provided.

- use the default value if none is provided.

- use the value read in the DEM if only the DEM is provided.

- compute the default elevation + the value of the geoid if only the
  geoid is provided.
  
A note on the egm96.grd file
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In previous OTB versions (using Ossim) it was common to use the
egm96.grd file as geoid. This file cannot be opened by GDAL. However
it is still possible to use it by using the following egm96.grd.hdr
file::

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


With this file attached, GDAL will be able to read the egm96.grd file
as a ENVI dataset.
